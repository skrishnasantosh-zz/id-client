#include "Browser.h"

AdBrowserBase::AdBrowserBase(AdBrowser* browserWindow) : m_refCount(0)
{
	m_browser = browserWindow;
}

AdBrowserBase::~AdBrowserBase()
{
	if (m_refCount != 0)
	{
		//This is a leak
	}
}

HRESULT STDMETHODCALLTYPE AdBrowserBase::QueryInterface(REFIID riid, LPVOID FAR* ppvObject)
{
	if (ppvObject == nullptr)
		return E_POINTER;

	*ppvObject = nullptr;

	if (!memcmp(&riid, &IID_IUnknown, sizeof(GUID)) || !memcmp(&riid, &IID_IOleClientSite, sizeof(GUID)))
		* ppvObject = &(m_browser->m_clientSite);

	else if (!memcmp(&riid, &IID_IOleInPlaceSite, sizeof(GUID)))
		* ppvObject = &(m_browser->m_inPlaceSite);

	else if (!memcmp(&riid, &DIID_DWebBrowserEvents2, sizeof(GUID)))
		* ppvObject = &(m_browser);

	if (*ppvObject != nullptr)
		return S_OK;

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE AdBrowserBase::AddRef()
{
	return ::InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE AdBrowserBase::Release()
{
	return ::InterlockedDecrement(&m_refCount);
	//Do not delete on zero
}