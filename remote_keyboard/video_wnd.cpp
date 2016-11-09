#include "video_wnd.h"

VideoWnd::VideoWnd()
	: hwnd_(NULL)
	, pa_hwnd_(NULL)
	, vlc_tool_(nullptr)
	, ch_ctrl_(nullptr)
{}

VideoWnd::~VideoWnd()
{}

void VideoWnd::Init(HWND pa_hwnd) 
{
	pa_hwnd_ = pa_hwnd;
	hwnd_ = Create(pa_hwnd_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

LRESULT VideoWnd::OnInit()
{
	PostMessage(kAM_InitOtherWndMsg, 0, 0);
	return LRESULT();
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

LRESULT VideoWnd::OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	//ch_ctrl_.reset(new ChCtrl(m_hWnd));
	//ch_ctrl_->InitCtrl();
	//ch_ctrl_->ShowWindow(true);
	//ch_ctrl_->SetCtrlPos(100, 100, 70, 70);
	return LRESULT();
}

LRESULT VideoWnd::OnWndSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::PostMessage(pa_hwnd_, kAM_WndSizeChangeMsg, 0, 0);
	return LRESULT();
}

LRESULT VideoWnd::OnRBtnDowMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	GET_X_LPARAM(lParam);
	GET_Y_LPARAM(lParam);
	return LRESULT();
}
