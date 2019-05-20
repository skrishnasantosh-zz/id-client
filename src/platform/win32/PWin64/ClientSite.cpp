#include "Browser.h"

AdClientSite::AdClientSite(AdBrowser* browser) : AdBrowserBase(browser)
{

}

HRESULT STDMETHODCALLTYPE AdClientSite::SaveObject()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
{
	UNREFERENCED_PARAMETER(dwAssign);
	UNREFERENCED_PARAMETER(dwWhichMoniker);
	UNREFERENCED_PARAMETER(ppmk);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdClientSite::GetContainer(LPOLECONTAINER FAR* ppContainer)
{
	if (ppContainer != nullptr)
		*ppContainer = nullptr;

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdClientSite::ShowObject()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdClientSite::OnShowWindow(BOOL fShow)
{
	UNREFERENCED_PARAMETER(fShow);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdClientSite::RequestNewObjectLayout()
{
	return E_NOTIMPL;
}
