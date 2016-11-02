#pragma once
#include "window_impl_base.h"
#include "msg_head.h"

class SetupPopWnd : public WindowImplBase
{
public:
	SetupPopWnd();
	~SetupPopWnd();

	DECLARE_DUIWND_INFO(_T("SetupPopWnd"), CS_DBLCLKS, _T("setup_pop_wnd.xml"))

	BEGIN_DUIMSG_MAP(SetupPopWnd)
		DUIMSG_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		DUIMSG_HANDLER(WM_KEYDOWN, OnKeyDown)
		DUIMSG_HANDLER(WM_KILLFOCUS, OnKillFocus)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(FrameWnd)
		DUINOTIFY_HANDLER(_T("quit_full"), DUINOTIFY_CLICK, OnClickBtn)
	END_DUINOTIFY_MAP()

public:
	LRESULT OnInit() override;
	HWND CreateWithStyle(HWND pa_hwnd, DWORD style);
	HWND CreateWithDefaltStyle(HWND pa_hwnd);

private:
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) override;

private:
	void OnClickBtn(TNotifyUI& msg, bool& handled);

public:
	void PopupWindow(PPOINT point, bool left_bottom = false);

private:
	HWND pa_hwnd_;
};

