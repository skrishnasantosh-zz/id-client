from requests_oauthlib import OAuth1Session
import requests
from requests_oauthlib import OAuth1
import sys, time
import logging

logFormat = '%(asctime)-15s %(message)s'
logging.basicConfig(format = logFormat, filename='idclient.log', filemode='w', level=logging.INFO)
logging.Formatter.converter = time.gmtime 

logger = logging.getLogger('tknfx')

logger.info("starting application")

if (len(sys.argv) < 4):    
    logger.error("insufficient commandline arguments")    
    exit (-1)

environ = sys.argv[1]
givenConsumerKey = sys.argv[2]
givenConsumerSecret = sys.argv[3]
realm = None

logger.info("configured environment is %s", environ)

if (len(sys.argv) >= 5):
    realm = sys.argv[4]
    
logger.info("consumer key is %s", givenConsumerKey)
logger.info("consumer secret is %s", givenConsumerSecret)
logger.info("realm is %s", realm)

requestTokenUrl = "https://accounts-" + environ + ".autodesk.com/oauth/requesttoken"
authorizeUrl = "https://accounts-" + environ + ".autodesk.com/oauth/authorize"
accessTokenUrl = "https://accounts-" + environ + ".autodesk.com/oauth/accesstoken"

logger.info("request token url is %s", requestTokenUrl)
logger.info("authorize url is %s", authorizeUrl)
logger.info("access token url is %s", accessTokenUrl)

oauthVerifier = "tknfx_verifier"

oauth1 = OAuth1Session(givenConsumerKey, givenConsumerSecret, verifier=oauthVerifier)

def oAuth1GetRequestToken(consumerKey, consumerSecret):    
    requestTokenResponse = oauth1.fetch_request_token(requestTokenUrl, realm)

    logger.info("request token response is %s", requestTokenResponse)

    requestToken = requestTokenResponse.get("oauth_token")
    requestTokenSecret = requestTokenResponse.get("oauth_token_secret")    
    
    return (requestToken, requestTokenSecret)

def oAuth1Authorize(authorizeUrl, requestToken):
    fullAuthUrl = oauth1.authorization_url(authorizeUrl, requestToken)

    logger.info("full authorize url is %s", fullAuthUrl)
    return True
    
def oAuth1GetAccessToken(consumerKey, consumerSecret, requestToken, requestTokenSecret):
    oauth1 = OAuth1Session(consumerKey, consumerSecret, requestToken, requestTokenSecret, verifier=oauthVerifier)
    accessTokenResponse = oauth1.fetch_access_token(accessTokenUrl, verifier="IBrCkx5L6BbBev4a35A5TgqC3WI%253D")

    logger.info("access token response is %s", accessTokenResponse)

    accessToken = accessTokenResponse.get('oauth_token')
    accessTokenSecret = accessTokenResponse.get('oauth_token_secret')

    return (accessToken, accessTokenSecret)


def main(consumerKey, consumerSecret):
    #(requestToken, requestTokenSecret) = oAuth1GetRequestToken(consumerKey, consumerSecret)
    #oAuth1Authorize(authorizeUrl, requestToken)
    oAuth1GetAccessToken(consumerKey, consumerSecret, "IBrCkx5L6BbBev4a35A5TgqC3WI=", "5zVyt9pKQzK7vvpFAZlKFPw6+H4=")

main(givenConsumerKey, givenConsumerSecret)

