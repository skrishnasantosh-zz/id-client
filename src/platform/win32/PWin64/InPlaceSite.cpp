#include "Browser.h"

AdInPlaceSite::AdInPlaceSite(AdBrowser* browser) : AdBrowserBase(browser)
{
	
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::GetWindow(HWND FAR* lphwnd)
{
	if (lphwnd == nullptr)
		return E_POINTER;

	*lphwnd = m_browser->GetWindowHandle();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::ContextSensitiveHelp(BOOL fEnterMode)
{
	UNREFERENCED_PARAMETER(fEnterMode);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::CanInPlaceActivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::OnInPlaceActivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::OnUIActivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	if (lplpFrame == nullptr)
		return E_POINTER;

	*lplpFrame = (LPOLEINPLACEFRAME)&(m_browser->m_inPlaceFrame);

	if (lplpDoc != nullptr)
		*lplpDoc = 0;

	if (lpFrameInfo != nullptr)
	{
		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->hwndFrame = m_browser->GetWindowHandle();
		lpFrameInfo->haccel = 0;
		lpFrameInfo->cAccelEntries = 0;

		if (lprcPosRect != nullptr)
			GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
		if (lprcClipRect != nullptr)
			GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);
	}	

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::Scroll(SIZE scrollExtent)
{
	UNREFERENCED_PARAMETER(scrollExtent);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::OnUIDeactivate(BOOL fUndoable)
{
	UNREFERENCED_PARAMETER(fUndoable);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::OnInPlaceDeactivate()
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::DiscardUndoState()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::DeactivateAndUndo()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdInPlaceSite::OnPosRectChange(LPCRECT lprcPosRect)
{
	UNREFERENCED_PARAMETER(lprcPosRect);

	return S_OK;
}
