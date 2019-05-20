#include "Browser.h"

AdInPlaceFrame::AdInPlaceFrame(AdBrowser* browser) : AdBrowserBase(browser)
{
	
}


HRESULT STDMETHODCALLTYPE AdInPlaceFrame::GetWindow(HWND* lphwnd)
{
	if (lphwnd == nullptr)
		return E_POINTER;

	*lphwnd = m_browser->GetWindowHandle();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::ContextSensitiveHelp(BOOL fEnterMode)
{
	UNREFERENCED_PARAMETER(fEnterMode);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::GetBorder(LPRECT lprectBorder)
{
	UNREFERENCED_PARAMETER(lprectBorder);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::RequestBorderSpace(LPCBORDERWIDTHS pborderwidths)
{
	UNREFERENCED_PARAMETER(pborderwidths);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::SetBorderSpace(LPCBORDERWIDTHS pborderwidths)
{
	UNREFERENCED_PARAMETER(pborderwidths);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::SetActiveObject(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR pszObjName)
{
	UNREFERENCED_PARAMETER(pActiveObject);
	UNREFERENCED_PARAMETER(pszObjName);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	UNREFERENCED_PARAMETER(hmenuShared);
	UNREFERENCED_PARAMETER(lpMenuWidths);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{
	UNREFERENCED_PARAMETER(hmenuShared);
	UNREFERENCED_PARAMETER(holemenu);
	UNREFERENCED_PARAMETER(hwndActiveObject);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::RemoveMenus(HMENU hmenuShared)
{
	UNREFERENCED_PARAMETER(hmenuShared);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::SetStatusText(LPCOLESTR pszStatusText)
{
	UNREFERENCED_PARAMETER(pszStatusText);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::EnableModeless(BOOL fEnable)
{
	UNREFERENCED_PARAMETER(fEnable);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceFrame::TranslateAccelerator(LPMSG lpmsg, WORD wID)
{
	UNREFERENCED_PARAMETER(lpmsg);
	UNREFERENCED_PARAMETER(wID);

	return E_NOTIMPL;
}
