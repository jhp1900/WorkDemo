#pragma once
#include "window_impl_base.h"
#include "res_singleton.h"

class IpSetWnd : public WindowImplBase
{
public:
	IpSetWnd(HWND pa_hwnd, IPSourceType type);
	~IpSetWnd();

	DECLARE_DUIWND_INFO(_T("IpSetWnd"), CS_DBLCLKS, _T("ipset_wnd.xml"))

	BEGIN_DUINOTIFY_MAP(IpSetWnd)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_CLICK, OnClickBtn)
	END_DUINOTIFY_MAP()

public:
	bool DoModal();
	virtual LRESULT OnInit() override;

private:
	void OnLocalInit();

private:
	void OnClickBtn(TNotifyUI& msg, bool& handled);

private:
	HWND pa_hwnd_;
	IPSourceType ip_source_type_;
};

