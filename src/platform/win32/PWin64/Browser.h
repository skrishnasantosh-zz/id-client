#pragma once

#pragma once

#include <windows.h>
#include <exdisp.h>
#include <mshtml.h>
#include <mshtmhst.h>
#include <oaidl.h>
#include <string>


#ifndef GWL_USERDATA
#define GWL_USERDATA -21
#endif

#define BROWSER_FRAME_CLASS TEXT("AdBrowserFrame") 
#define BROWSER_FRAME_TITLE TEXT("Login")

class AdBrowser;

class AdBrowserBase : public IUnknown
{
protected:
	AdBrowser* m_browser;
	volatile ULONG m_refCount;

protected:
	AdBrowserBase(AdBrowser* browser);
	virtual ~AdBrowserBase();

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
};

class AdBrowserStorage : public IStorage, AdBrowserBase
{
public:
	AdBrowserStorage(AdBrowser* browser);

public://COM
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj) { return AdBrowserBase::QueryInterface(riid, ppvObj); }
	ULONG STDMETHODCALLTYPE AddRef() { return AdBrowserBase::AddRef(); }
	ULONG STDMETHODCALLTYPE Release() { return AdBrowserBase::Release(); }

	HRESULT STDMETHODCALLTYPE CreateStream(const WCHAR* pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream** ppstm);
	HRESULT STDMETHODCALLTYPE OpenStream(const WCHAR* pwcsName, void* reserved1, DWORD grfMode, DWORD reserved2, IStream** ppstm);
	HRESULT STDMETHODCALLTYPE CreateStorage(const WCHAR* pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage** ppstg);
	HRESULT STDMETHODCALLTYPE OpenStorage(const WCHAR* pwcsName, IStorage* pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage** ppstg);
	HRESULT STDMETHODCALLTYPE CopyTo(DWORD ciidExclude, IID const* rgiidExclude, SNB snbExclude, IStorage* pstgDest);
	HRESULT STDMETHODCALLTYPE MoveElementTo(const OLECHAR* pwcsName, IStorage* pstgDest, const OLECHAR* pwcsNewName, DWORD grfFlags);
	HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);
	HRESULT STDMETHODCALLTYPE Revert();
	HRESULT STDMETHODCALLTYPE EnumElements(DWORD reserved1, void* reserved2, DWORD reserved3, IEnumSTATSTG** ppenum);
	HRESULT STDMETHODCALLTYPE DestroyElement(const OLECHAR* pwcsName);
	HRESULT STDMETHODCALLTYPE RenameElement(const WCHAR* pwcsOldName, const WCHAR* pwcsNewName);
	HRESULT STDMETHODCALLTYPE SetElementTimes(const WCHAR* pwcsName, FILETIME const* pctime, FILETIME const* patime, FILETIME const* pmtime);
	HRESULT STDMETHODCALLTYPE SetClass(REFCLSID clsid);
	HRESULT STDMETHODCALLTYPE SetStateBits(DWORD grfStateBits, DWORD grfMask);
	HRESULT STDMETHODCALLTYPE Stat(STATSTG* pstatstg, DWORD grfStatFlag);
};

class AdInPlaceFrame : public IOleInPlaceFrame, AdBrowserBase
{
public:
	AdInPlaceFrame(AdBrowser* frame);

public:	//COM
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj) { return AdBrowserBase::QueryInterface(riid, ppvObj); }
	ULONG STDMETHODCALLTYPE AddRef() { return AdBrowserBase::AddRef(); }
	ULONG STDMETHODCALLTYPE Release() { return AdBrowserBase::Release(); }

	HRESULT STDMETHODCALLTYPE GetWindow(HWND FAR* lphwnd);
	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
	HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder);
	HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths);
	HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths);
	HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR pszObjName);
	HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
	HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
	HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared);
	HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText);
	HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
	HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID);
};

class AdInPlaceSite : public IOleInPlaceSite, AdBrowserBase
{
public:
	AdInPlaceSite(AdBrowser* browser);

public: //COM
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj) { return AdBrowserBase::QueryInterface(riid, ppvObj); }
	ULONG STDMETHODCALLTYPE AddRef() { return AdBrowserBase::AddRef(); }
	ULONG STDMETHODCALLTYPE Release() { return AdBrowserBase::Release(); }

	HRESULT STDMETHODCALLTYPE GetWindow(HWND FAR* lphwnd);
	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
	HRESULT STDMETHODCALLTYPE CanInPlaceActivate();
	HRESULT STDMETHODCALLTYPE OnInPlaceActivate();
	HRESULT STDMETHODCALLTYPE OnUIActivate();
	HRESULT STDMETHODCALLTYPE GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);
	HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtent);
	HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable);
	HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate();
	HRESULT STDMETHODCALLTYPE DiscardUndoState();
	HRESULT STDMETHODCALLTYPE DeactivateAndUndo();
	HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect);
};

class AdClientSite : public IOleClientSite, AdBrowserBase
{
public:
	AdClientSite(AdBrowser* browser);

public://COM
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj) { return AdBrowserBase::QueryInterface(riid, ppvObj); }
	ULONG STDMETHODCALLTYPE AddRef() { return AdBrowserBase::AddRef(); }
	ULONG STDMETHODCALLTYPE Release() { return AdBrowserBase::Release(); }

	HRESULT STDMETHODCALLTYPE SaveObject();
	HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk);
	HRESULT STDMETHODCALLTYPE GetContainer(LPOLECONTAINER FAR* ppContainer);
	HRESULT STDMETHODCALLTYPE ShowObject();
	HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow);
	HRESULT STDMETHODCALLTYPE RequestNewObjectLayout();
};

class AdDocHostUiHandler : public IDocHostUIHandler2, AdBrowserBase
{
public:
	AdDocHostUiHandler(AdBrowser* browser);

public://COM

	HRESULT __stdcall ShowContextMenu(DWORD dwID, POINT* ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved);
	HRESULT __stdcall GetHostInfo(DOCHOSTUIINFO* pInfo);
	HRESULT __stdcall ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);
	HRESULT __stdcall HideUI(void);
	HRESULT __stdcall UpdateUI(void);
	HRESULT __stdcall EnableModeless(BOOL fEnable);
	HRESULT __stdcall OnDocWindowActivate(BOOL fActivate);
	HRESULT __stdcall OnFrameWindowActivate(BOOL fActivate);
	HRESULT __stdcall ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fRameWindow);
	HRESULT __stdcall GetOptionKeyPath(LPOLESTR* pchKey, DWORD dw);
	HRESULT __stdcall GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
	HRESULT __stdcall GetExternal(IDispatch** ppDispatch);
	HRESULT __stdcall TranslateUrl(DWORD dwTranslate, LPWSTR pchURLIn, LPWSTR* ppchURLOut);
	HRESULT __stdcall FilterDataObject(IDataObject* pDO, IDataObject** ppDORet);
	HRESULT __stdcall GetOverrideKeyPath(LPOLESTR* pchKey, DWORD dw);
	HRESULT TranslateAcceleratorW(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID);
};


class AdBrowser : public virtual DWebBrowserEvents2
{
private:
	HWND m_hWnd;
	IOleObject* m_webBrowserOle;


private:
	HINSTANCE m_hInstance;
	std::wstring m_title;

public:
	AdBrowserStorage m_storage;
	AdInPlaceFrame m_inPlaceFrame;
	AdInPlaceSite m_inPlaceSite;
	AdClientSite m_clientSite;

	volatile ULONG m_refCount;

public:
	AdBrowser(HINSTANCE hInstance, const wchar_t* title);
	~AdBrowser();	

public:
	HWND GetWindowHandle();
	void ShowWindow(HWND parent, BOOL modal, const wchar_t* urlStr);	

private: //COM
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObject);
	ULONG STDMETHODCALLTYPE  AddRef();
	ULONG STDMETHODCALLTYPE Release();

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo);
	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);

private:
	LRESULT CALLBACK WindowProcThis(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void InitWebBrowser();
	void UninitWebBrowser();

	BOOL SetupEvents();

	void Navigate(const wchar_t* url);

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
};

