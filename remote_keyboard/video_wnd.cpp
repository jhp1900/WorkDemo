#include "video_wnd.h"

VideoWnd::VideoWnd()
	: hwnd_(NULL)
	, pa_hwnd_(NULL)
	, vlc_tool_(nullptr)
{}

VideoWnd::~VideoWnd()
{}

void VideoWnd::Init(HWND pa_hwnd) 
{
	pa_hwnd_ = pa_hwnd;
	hwnd_ = Create(pa_hwnd_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void VideoWnd::RunVideo() 
{
	vlc_tool_.reset(new VLCTool);
	if (!vlc_tool_->Initial())
		return;
	vlc_tool_->SetMediaSource("rtsp://192.0.0.122/11");
	vlc_tool_->SetPlayerWindows(hwnd_);
	vlc_tool_->StartPlay();
	ShowWindow(true);
}

LRESULT VideoWnd::OnWndSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::PostMessage(pa_hwnd_, kAM_WndSizeChangeMsg, 0, 0);
	return LRESULT();
}