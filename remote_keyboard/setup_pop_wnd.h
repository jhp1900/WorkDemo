#pragma once
#include "window_impl_base.h"
#include "msg_head.h"

class SetupPopWnd : public WindowImplBase
{
public:
	SetupPopWnd(HWND pa_hwnd, HWND top_hwnd);
	~SetupPopWnd();

	DECLARE_DUIWND_INFO(_T("SetupPopWnd"), CS_DBLCLKS, _T("setup_pop_wnd.xml"))

	BEGIN_DUIMSG_MAP(SetupPopWnd)
		DUIMSG_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		DUIMSG_HANDLER(WM_KEYDOWN, OnKeyDown)
		DUIMSG_HANDLER(WM_KILLFOCUS, OnKillFocus)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(FrameWnd)
		DUINOTIFY_HANDLER(_T("quit_full"), DUINOTIFY_CLICK, OnClickBtn)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_SELECTCHANGED, OnSelectChanged)
	END_DUINOTIFY_MAP()

public:
	LRESULT OnInit() override;
	HWND CreateWithStyle(DWORD style);
	HWND CreateWithDefaltStyle();

private:
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) override;
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

private:
	void OnClickBtn(TNotifyUI& msg, bool& handled);
	void OnSelectChanged(TNotifyUI& msg, bool& handled);

public:
	void PopupWindow(PPOINT point, bool left_bottom = false);
	void DataSync(int index, bool enable);		// 数据同步

private:
	HWND pa_hwnd_;
	HWND top_hwnd_;
};

