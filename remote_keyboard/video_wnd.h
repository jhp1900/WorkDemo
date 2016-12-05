#pragma once
#include "window_impl_base.h"
#include "vlc_tool.h"
#include <memory>
#include "msg_head.h"

class VideoWnd : public WindowImplBase
{
public:
	VideoWnd();
	~VideoWnd();

	DECLARE_DUIWND_INFO(_T("VideoWnd"), CS_DBLCLKS, _T("video_wnd.xml"))

	BEGIN_DUIMSG_MAP(FrameWnd)
		DUIMSG_HANDLER(WM_SIZE, OnWndSizeMsg)
		DUIMSG_HANDLER(kAM_ResetBackStreamsIP, ResetIPInfo)
	END_DUIMSG_MAP()

public:
	void Init(HWND pa_hwnd_);
	virtual LRESULT OnInit() override;
	void RunVideo();
	HWND GetHwnd() { return hwnd_; }

private:
	LRESULT OnWndSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT ResetIPInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
	HWND hwnd_;
	HWND pa_hwnd_;
	std::shared_ptr<VLCTool> vlc_tool_;
};
