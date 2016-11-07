#pragma once
#include "window_impl_base.h"
#include "vlc_tool.h"
#include <memory>
#include "msg_head.h"
#include "ch_ctrl.h"

class VideoWnd : public WindowImplBase
{
public:
	VideoWnd();
	~VideoWnd();

	DECLARE_DUIWND_INFO(_T("VideoWnd"), CS_DBLCLKS, _T("video_wnd.xml"))

	BEGIN_DUIMSG_MAP(FrameWnd)
		DUIMSG_HANDLER(kAM_InitOtherWndMsg, OnInitOtherWndMsg)
		DUIMSG_HANDLER(WM_SIZE, OnWndSizeMsg)
	END_DUIMSG_MAP()

public:
	void Init(HWND pa_hwnd_);
	virtual LRESULT OnInit() override;
	void RunVideo();
	HWND GetHwnd() { return hwnd_; }

private:
	LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnWndSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
	HWND hwnd_;
	HWND pa_hwnd_;
	std::shared_ptr<VLCTool> vlc_tool_;
	std::shared_ptr<ChCtrl> ch_ctrl_;
};
