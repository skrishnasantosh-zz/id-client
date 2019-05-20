#include <ExDisp.h>
#include "Browser.h"

#define MAIN_APP_TIMER 0x010001
#define MAIN_APP_TIMER_DURATION 10000

AdBrowser::AdBrowser(HINSTANCE hInstance, const wchar_t* title) : 
	m_hInstance(hInstance), m_title(title), m_hWnd(nullptr),
	m_storage(this), m_inPlaceFrame(this), m_inPlaceSite(this), 
	m_clientSite(this), m_webBrowserOle(nullptr)
{ }

AdBrowser::~AdBrowser()
{ }

HWND AdBrowser::GetWindowHandle()
{
	return m_hWnd;
}

LRESULT CALLBACK AdBrowser::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	AdBrowser* frame = nullptr;

	switch (uMsg) 
	{
	case WM_CREATE:
		frame = (AdBrowser*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		if (frame != nullptr)	
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)frame);		
		break;

	
	default:
		frame = (AdBrowser*)GetWindowLongPtr(hWnd, GWLP_USERDATA);		
		break;
	}

	if (frame == nullptr)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	return frame->WindowProcThis(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK AdBrowser::WindowProcThis(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		this->m_hWnd = hWnd;
		InitWebBrowser();
		break;
			
	case WM_DESTROY:
		KillTimer(hWnd, MAIN_APP_TIMER);
		UninitWebBrowser();
		PostQuitMessage(0);
		
		return 0;

	case WM_TIMER:

		if (wParam == MAIN_APP_TIMER)
		{
			HRESULT hr = E_FAIL;
			IWebBrowser2* browser = nullptr;

			hr = m_webBrowserOle->QueryInterface(IID_IWebBrowser2, (void**)&browser);

			if (SUCCEEDED(hr))
			{
				READYSTATE readyState;
				BSTR urlBStr = nullptr;

				hr = browser->get_LocationURL(&urlBStr);

				if (SUCCEEDED(hr))
				{
					std::wstring ws(urlBStr, SysStringLen(urlBStr));

					if (ws.find(L"https://accounts.autodesk.com/Health") != std::wstring::npos)
					{
						KillTimer(hWnd, MAIN_APP_TIMER);
						MessageBox(NULL, L"Callback done", L"Hi", MB_OK);

						SendMessage(hWnd, WM_DESTROY, 0, 0);
					}

					SysFreeString(urlBStr);
				}
			}
		}

		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void AdBrowser::InitWebBrowser()
{
	HRESULT hr = E_FAIL;

	hr = OleCreate(CLSID_WebBrowser, IID_IOleObject, OLERENDER_DRAW, nullptr, &m_clientSite, &m_storage, (void**)&m_webBrowserOle);

	if (SUCCEEDED(hr))
	{
		RECT rect = { 0 };

		m_webBrowserOle->SetHostNames(L"My Host Name", 0);
		GetClientRect(m_hWnd, &rect);

		hr = OleSetContainedObject(m_webBrowserOle, TRUE);

		if (SUCCEEDED(hr))
		{
			hr = m_webBrowserOle->DoVerb(OLEIVERB_SHOW, NULL, &m_clientSite, -1, m_hWnd, &rect);

			if (SUCCEEDED(hr))
			{
				IWebBrowser2* browser;

				hr = m_webBrowserOle->QueryInterface(IID_IWebBrowser2, (void**)& browser);

				if (SUCCEEDED(hr))
				{
					browser->put_Left(0);
					browser->put_Top(0);
					browser->put_Width(rect.right);
					browser->put_Height(rect.bottom);
					
					browser->Release();
				}
			}
		}
	}
}

void AdBrowser::UninitWebBrowser()
{

	if (m_webBrowserOle != nullptr)
	{
		if (!IsBadReadPtr(m_webBrowserOle, sizeof(IOleObject*)))
		{
			m_webBrowserOle->Close(OLECLOSE_NOSAVE);
			m_webBrowserOle->Release();
		}
	}
}

BOOL AdBrowser::SetupEvents() 
{
	IConnectionPointContainer* cpc = nullptr;
	IConnectionPoint* cp = nullptr;
	IWebBrowser2* browser = nullptr;

	HRESULT hr = E_FAIL;

	hr = m_webBrowserOle->QueryInterface(IID_IWebBrowser2, (void**)&browser);

	if (SUCCEEDED(hr))
	{
		hr = browser->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&cpc));

		if (SUCCEEDED(hr))
		{
			hr = cpc->FindConnectionPoint(DIID_DWebBrowserEvents2, &cp);
			
			if (SUCCEEDED(hr))
			{
				unsigned long cookie = 0x01;
				auto dispatch = static_cast<IDispatch*>(this);
				hr = cp->Advise(dispatch, &cookie);
			}
		}
	}

	cp->Release();
	cpc->Release();
	browser->Release();

	return SUCCEEDED(hr);
}

void AdBrowser::Navigate(const wchar_t* urlStr)
{
	IWebBrowser2* browser;
	VARIANT			url;	
	HRESULT hr = E_FAIL;

	hr = m_webBrowserOle->QueryInterface(IID_IWebBrowser2, (void**)&browser);

	if (SUCCEEDED(hr))
	{		
		VariantInit(&url);
		url.vt = VT_BSTR;

		url.bstrVal = SysAllocString(urlStr);

		if (url.bstrVal == NULL)
		{
			browser->Release();
			VariantClear(&url);
			MessageBox(m_hWnd, TEXT("Error"), TEXT("Unable to navigate WebBrowser. Cannot allocate string value"), MB_ICONERROR | MB_OK);
			return;
		}
				
		browser->put_Silent(VARIANT_TRUE);		

		hr = browser->Navigate2(&url, 0, 0, 0, 0);

		if (FAILED(hr))
		{
			MessageBox(m_hWnd, TEXT("Error"), TEXT("Unable to navigate WebBrowser. Navigate Failed"), MB_ICONERROR | MB_OK);
		}

		SysFreeString(url.bstrVal);
		url.bstrVal = NULL;

		VariantClear(&url);
				
		browser->Release();
	}
}

void AdBrowser::ShowWindow(HWND parent, BOOL modal, const wchar_t* urlStr)
{
	MSG	msg;
	std::wstring className(BROWSER_FRAME_CLASS);	

	if (OleInitialize(NULL) == S_OK)
	{
		WNDCLASSEX		wc;		
		RECT rect;

		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = m_hInstance;
		wc.lpfnWndProc = WindowProc;
		wc.lpszClassName = &className[0];
		RegisterClassEx(&wc);

		GetClientRect(GetDesktopWindow(), &rect);
		rect.left = (rect.right / 2) - (1024 / 2);
		rect.top = (rect.bottom / 2) - (512 / 2);
		
		m_hWnd = CreateWindowEx(0, &className[0], BROWSER_FRAME_TITLE, WS_SYSMENU | WS_CAPTION | WS_DLGFRAME,
			rect.left, rect.top, 1024, 512, HWND_DESKTOP, NULL, m_hInstance, this);

		if (m_hWnd != NULL && m_hWnd != INVALID_HANDLE_VALUE)
		{
			msg.hwnd = m_hWnd;

			SetTimer(m_hWnd, MAIN_APP_TIMER, MAIN_APP_TIMER_DURATION, NULL);

			if (modal && (parent != NULL && parent != INVALID_HANDLE_VALUE && parent != HWND_DESKTOP))			
				EnableWindow(parent, FALSE);			

			::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
			UpdateWindow(m_hWnd);			

			SetupEvents();

			Navigate(urlStr);

			while (GetMessage(&msg, 0, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (modal && (parent != NULL && parent != INVALID_HANDLE_VALUE && parent != HWND_DESKTOP))
				EnableWindow(parent, TRUE);				
		}	

		OleUninitialize();
	}
}

//Implement DWebBrowser2


HRESULT STDMETHODCALLTYPE AdBrowser::QueryInterface(REFIID riid, LPVOID FAR* ppvObject)
{
	if (ppvObject == nullptr)
		return E_POINTER;

	*ppvObject = nullptr;

	//if (!memcmp(&riid, &IID_IUnknown, sizeof(GUID)) ||  !memcmp(&riid, &IID_IDispatch, sizeof(GUID)) || !memcmp(&riid, &IID_IDocHostUIHandler, sizeof(GUID))) 	
		//.*ppvObject = this->m_;	
	

	if (!memcmp(&riid, &IID_IUnknown, sizeof(GUID)) || !memcmp(&riid, &IID_IOleClientSite, sizeof(GUID)))
		* ppvObject = &(this->m_clientSite);

	else if (!memcmp(&riid, &IID_IOleInPlaceSite, sizeof(GUID)))
		* ppvObject = &(this->m_inPlaceSite);

	if (*ppvObject != nullptr)
		return S_OK;

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE AdBrowser::AddRef()
{
	return ::InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE AdBrowser::Release()
{
	return ::InterlockedDecrement(&m_refCount);
	//Do not delete on zero
}


HRESULT STDMETHODCALLTYPE AdBrowser::GetTypeInfoCount(UINT* pctinfo)
{
	if (pctinfo != nullptr)
		*pctinfo = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdBrowser::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
	UNREFERENCED_PARAMETER(iTInfo);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(ppTInfo);
	
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowser::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(rgszNames);
	UNREFERENCED_PARAMETER(cNames);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(rgDispId);

	return E_NOINTERFACE;
}

#define DISPID_NAVIGATECOMPLETE2    252   // UIActivate new document
#define DISPID_NAVIGATEERROR        271   // Fired to indicate the a binding error has occured

HRESULT STDMETHODCALLTYPE AdBrowser::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
	switch (dispIdMember)
	{
	case DISPID_NAVIGATECOMPLETE2:
		return S_OK;

	case DISPID_NAVIGATEERROR:
		return S_OK;
	
	}

	return S_OK;
}
