#pragma once
#include "window_impl_base.h"
#include "native_control_ui.h"
#include <memory>
#include "video_wnd.h"
#include "remote_keyboard.h"
#include "msg_head.h"

class FrameWnd : public WindowImplBase
{
public:
	FrameWnd();
	~FrameWnd();

	DECLARE_DUIWND_INFO(_T("FrameWnd"), CS_DBLCLKS, _T("frame_wnd.xml"))

	BEGIN_DUICONTROL_CREATE(FrameWnd)
		DUICONTROL_CREATE(_T("Panel"), NativeControlUI)
	END_DUICONTROL_CREATE()

	BEGIN_DUINOTIFY_MAP(FrameWnd)
		DUINOTIFY_HANDLER(_T("minbtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("fullbtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("closebtn"), DUINOTIFY_CLICK, OnClickSysBtn)

		DUINOTIFY_HANDLER(_T("test_btn"), DUINOTIFY_CLICK, OnClickTestBtn)
		DUINOTIFY_HANDLER(_T("show_btn"), DUINOTIFY_CLICK, OnClickTestBtn)
		DUINOTIFY_HANDLER(_T("noshow_btn"), DUINOTIFY_CLICK, OnClickTestBtn)
	END_DUINOTIFY_MAP()

	BEGIN_DUIMSG_MAP(FrameWnd)
		DUIMSG_HANDLER(kAM_InitOtherWndMsg, OnInitOtherWndMsg)
		DUIMSG_HANDLER(kAM_WndSizeChangeMsg, OnWndSizeChangeMsg)
		DUIMSG_HANDLER(kAM_ChildEscMsg, OnChildEscMsg)
	END_DUIMSG_MAP()

public:
	virtual LRESULT OnInit() override;

private:
	void OnClickSysBtn(TNotifyUI& msg, bool& handled);

	void OnClickTestBtn(TNotifyUI& msg, bool& handled);

private:
	LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnWndSizeChangeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnChildEscMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

private:
	void OnFullWnd();		// 窗口全屏
	void OnQuitFullWnd();	// 退出全屏

private:
	std::shared_ptr<VideoWnd> video_wnd_;
	std::shared_ptr<RemoteKeyboard> keyboard_;
};

