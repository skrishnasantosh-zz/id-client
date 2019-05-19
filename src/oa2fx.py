from requests_oauthlib import OAuth2Session
import requests
from requests_oauthlib import OAuth2
import sys, time
import logging
import json
import os.path
import os
import platform
from sys import platform as _platform
from urllib.parse import urlparse, parse_qs
from selenium import webdriver
from selenium.webdriver.ie.options import Options
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC 


#from selenium.common.exceptions import TheNameOfTheExceptionClass

platformApp = ""
driver = None

logFormat = "[%(name)s][%(levelname)s][%(asctime)-15s] %(message)s"
logging.basicConfig(format = logFormat, filename='idclient.log', filemode='w', level=logging.INFO)
logging.Formatter.converter = time.gmtime 

logger = logging.getLogger('oa2fx')

appPath = os.path.dirname(os.path.abspath(sys.argv[1]))

if _platform == "linux" or _platform == "linux2":
    logger.info("starting application on linux - " + str(platform.release()))    
    platformApp = "plinux"
elif _platform == "darwin":
    logger.info("starting application on macos - " + str(platform.release()))
    platformApp = "pmacos"
elif _platform == "win32" or _platform == "win64":
    logger.info("starting application on windows - " + str(platform.release()))
    platformApp = "pwin64"

    ieOptions = Options()

    ieOptions.ignore_protected_mode_settings = True
    ieOptions.ignore_zoom_level = True
    ieOptions.force_create_process_api = True
    ieOptions.native_events = True
    ieOptions.require_window_focus = True
    
    ieOptions.set_capability("version", "10")
    ieOptions.set_capability("browserName", "internet explorer")
    
    driver = webdriver.ie.webdriver.WebDriver(appPath + '\\IEDriverServer.exe', options = ieOptions)    
else:
    logger.info("unsupported platform %s. application cannot continue", _platform)
    sys.exit(-3)

logger.info("application root folder would be %s", appPath)
logger.info("application will use %s for native invocations", platformApp)

logger.info("loading config.json")

config = None

if (os.path.isfile('config.json')):
    with open('config.json') as configFile:
        config = json.load(configFile)

if (config == None):
    #TODO: load default config
    logger.error("config file is not found")
    sys.exit(-2)

logger.info("configuration is " + str(config))

if (len(sys.argv) < 5):
    logger.error("insufficient commandline arguments")    
    sys.exit (-1)

def getOptions():    
    logger.info("commandline args %s", str(sys.argv))    
    environ = ""
    clientKey = ""
    clientSecret = ""
    callback = ""
    realm = ""
    auth=""
    tempFile = ""

    for arg in sys.argv:
        if (arg.startswith("-")):
            argument = arg[1:len(arg)]
            argSplit = argument.split(':')
            if (len(argSplit) < 2):
                logger.error("invalid value for commandline arg '%s'", argument)
                sys.exit(-1)
            argName = argSplit[0]
            argValue = argSplit[1]

            if (argName.lower() == 'key'):
                clientKey = argValue            
            if (argName.lower() == 'secret'):
                clientSecret = argValue
            if (argName.lower() == 'auth'):
                auth = argValue
            if (argName.lower() == 'environ'):
                environ = argValue
            if (argName.lower() == 'realm'):
                realm = argValue
            if (argName.lower() == 'callback'):
                callback = argValue
            if (argName.lower() == 'temp'):
                tempFile = argValue

    if (auth == ""):
        auth = "oauth2" #default to oAuth2

    if (clientKey == ""):
        logger.error("client key is not specified in commandline")
        sys.exit(-1)

    if (clientSecret == ""):
        logger.error("client secret is not specified in commandline")
        sys.exit(-1)
    
    if (environ == ""):
        logger.error("environ is not specified in commandline")
        sys.exit(-1)

    if (tempFile == ""):
        logger.error("temp file is not specified in commandline")
        sys.exit(-1)

    if (auth == "oauth2" and callback == ""):
        logger.error("oauth2 chosen but callback is not specified in commandline")
        sys.exit(-1)

    if (environ.lower() != "dev" and environ.lower() != "stg" and environ.lower() != "prd"):
        logger.error("commandline argument invalid for 'environ' - %s", environ)
        sys.exit(-1)

    logger.info("setting command line options")
    logger.info("environ = %s, auth = %s, clientKey = %s, clientSecret = %s, callback = %s, realm = %s", 
                environ, auth, clientKey, clientSecret, callback, realm) #TODO: Remove client secret


    return (environ, auth, clientKey, clientSecret, callback, realm, tempFile)
    
(environ, auth, clientKey, clientSecret, callback, realm, tempFile) = getOptions()

callback = "https://accounts.autodesk.com/Health" #TODO: Hardcoded callback

logger.info("configured environment is %s", environ)

if (environ == "prd"):
    environ = ""
else:
    environ = "-" + environ

oauth1RequestTokenUrl = config['urls']['oauth1']['requestToken'].replace("{0}", environ)
oauth1AuthorizeUrl = config['urls']['oauth1']['authorize'].replace("{0}", environ)
oauth1AccessTokenUrl = config['urls']['oauth1']['accessToken'].replace("{0}", environ)

logger.info("oauth1 request token url is %s", oauth1RequestTokenUrl)
logger.info("oauth1 authorize url is %s", oauth1AuthorizeUrl)
logger.info("oauth1 access token url is %s", oauth1AccessTokenUrl)

oauth2AuthorizeUrl = config['urls']['oauth2']['authorize'].replace("{0}", environ)
oauth2GetTokenUrl = config['urls']['oauth2']['getToken'].replace("{0}", environ)
oauth2Callback = callback

logger.info("oauth2 authorize url is %s", oauth2AuthorizeUrl)
logger.info("oauth2 get token url is %s", oauth2GetTokenUrl)
logger.info("oauth2 callback url is %s", oauth2Callback)

oauth2 = OAuth2Session(clientKey, redirect_uri=oauth2Callback)

def oAuth2GetAuthorizeCodeUrl():
    (authUrl, state) = oauth2.authorization_url(oauth2AuthorizeUrl)
    logger.info("authorization url is %s", authUrl)
    
    driver.get(authUrl)

    wait = WebDriverWait(driver, 300).until(EC.url_contains(callback))

    codeUrl = driver.current_url
    driver.quit()
    
    print (codeUrl)
            
    return codeUrl

def oAuth2GetTokenByAuthorizeCode(code, authResponse):

    post_data = "grant_type=authorization_code&code=" + code + "&redirect_uri=" + callback + "&client_id=" + clientKey + "&client_secret=" + clientSecret
    response = requests.post(oauth2GetTokenUrl, data=post_data, headers={'Content-Type' : 'application/x-www-form-urlencoded'})

    logger.info("oauth2 get token response - " + response.text)

    print (response.text)

    responseJson = json.loads(response.text)

    accessToken = responseJson['access_token']
    refreshToken = responseJson['refresh_token']
    tokenType = responseJson['token_type']
    expiresIn = responseJson['expires_in']

    return (accessToken, refreshToken, tokenType, expiresIn)

authorizeUrl = oAuth2GetAuthorizeCodeUrl()

urlComps = urlparse(authorizeUrl)
queries = parse_qs(urlComps.query)

authorizeCode = queries['code'][0]

print ("Retrieved Token is")
print ("---------------------------------------------------------------------------")
token = oAuth2GetTokenByAuthorizeCode(authorizeCode, authorizeUrl)
print ("---------------------------------------------------------------------------")

