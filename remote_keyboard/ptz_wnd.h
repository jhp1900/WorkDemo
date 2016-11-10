#pragma once
#include "window_impl_base.h"

class PtzWnd : public WindowImplBase
{
public:
	PtzWnd(HWND pa_hwnd);
	~PtzWnd();

	DECLARE_DUIWND_INFO(_T("PtzWnd"), CS_DBLCLKS, _T("ptz_wnd.xml"))

	BEGIN_DUINOTIFY_MAP(FrameWnd)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_CLICK, OnClickBtn)
	END_DUINOTIFY_MAP()

public:
	bool DoModal();
	virtual LRESULT OnInit() override;

private:
	void OnClickBtn(TNotifyUI& msg, bool& handled);

private:
	HWND pa_hwnd_;
};

