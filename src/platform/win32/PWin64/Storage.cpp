#include "Browser.h"

AdBrowserStorage::AdBrowserStorage(AdBrowser* browser) : AdBrowserBase(browser){}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::CreateStream(const WCHAR* pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream** ppstm)
{
	UNREFERENCED_PARAMETER(pwcsName);
	UNREFERENCED_PARAMETER(grfMode);
	UNREFERENCED_PARAMETER(reserved1);
	UNREFERENCED_PARAMETER(reserved2);
	UNREFERENCED_PARAMETER(ppstm);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::OpenStream(const WCHAR* pwcsName, void* reserved1, DWORD grfMode, DWORD reserved2, IStream** ppstm)
{
	UNREFERENCED_PARAMETER(pwcsName);
	UNREFERENCED_PARAMETER(grfMode);
	UNREFERENCED_PARAMETER(reserved1);
	UNREFERENCED_PARAMETER(reserved2);
	UNREFERENCED_PARAMETER(ppstm);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::CreateStorage(const WCHAR* pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage** ppstg)
{
	UNREFERENCED_PARAMETER(pwcsName);
	UNREFERENCED_PARAMETER(grfMode);
	UNREFERENCED_PARAMETER(reserved1);
	UNREFERENCED_PARAMETER(reserved2);
	UNREFERENCED_PARAMETER(ppstg);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::OpenStorage(const WCHAR* pwcsName, IStorage* pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage** ppstg)
{
	UNREFERENCED_PARAMETER(pwcsName);
	UNREFERENCED_PARAMETER(grfMode);
	UNREFERENCED_PARAMETER(snbExclude);
	UNREFERENCED_PARAMETER(pstgPriority);
	UNREFERENCED_PARAMETER(reserved);
	UNREFERENCED_PARAMETER(ppstg);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::CopyTo(DWORD ciidExclude, IID const* rgiidExclude, SNB snbExclude, IStorage* pstgDest)
{
	UNREFERENCED_PARAMETER(ciidExclude);
	UNREFERENCED_PARAMETER(rgiidExclude);
	UNREFERENCED_PARAMETER(snbExclude);
	UNREFERENCED_PARAMETER(pstgDest);
	

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::MoveElementTo(const OLECHAR* pwcsName, IStorage* pstgDest, const OLECHAR* pwcsNewName, DWORD grfFlags)
{
	UNREFERENCED_PARAMETER(pwcsName);
	UNREFERENCED_PARAMETER(pstgDest);
	UNREFERENCED_PARAMETER(pwcsNewName);
	UNREFERENCED_PARAMETER(grfFlags);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::Commit(DWORD grfCommitFlags)
{
	UNREFERENCED_PARAMETER(grfCommitFlags);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::Revert()
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::EnumElements(DWORD reserved1, void* reserved2, DWORD reserved3, IEnumSTATSTG** ppenum)
{
	UNREFERENCED_PARAMETER(reserved1);
	UNREFERENCED_PARAMETER(reserved2);
	UNREFERENCED_PARAMETER(reserved3);
	UNREFERENCED_PARAMETER(ppenum);


	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::DestroyElement(const OLECHAR* pwcsName)
{
	UNREFERENCED_PARAMETER(pwcsName);


	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::RenameElement(const WCHAR* pwcsOldName, const WCHAR* pwcsNewName)
{
	UNREFERENCED_PARAMETER(pwcsOldName);
	UNREFERENCED_PARAMETER(pwcsNewName);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::SetElementTimes(const WCHAR* pwcsName, FILETIME const* pctime, FILETIME const* patime, FILETIME const* pmtime)
{
	UNREFERENCED_PARAMETER(pwcsName);
	UNREFERENCED_PARAMETER(pctime);
	UNREFERENCED_PARAMETER(patime);
	UNREFERENCED_PARAMETER(pmtime);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::SetClass(REFCLSID clsid)
{
	UNREFERENCED_PARAMETER(clsid);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::SetStateBits(DWORD grfStateBits, DWORD grfMask)
{
	UNREFERENCED_PARAMETER(grfStateBits);
	UNREFERENCED_PARAMETER(grfMask);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE AdBrowserStorage::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
	UNREFERENCED_PARAMETER(pstatstg);
	UNREFERENCED_PARAMETER(grfStatFlag);

	return E_NOTIMPL;
}
