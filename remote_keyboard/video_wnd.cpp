#include "video_wnd.h"
#include "res_singleton.h"

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

LRESULT VideoWnd::OnInit()
{
	return LRESULT();
}

void VideoWnd::RunVideo()
{
	char url[MAX_PATH] = { 0 };
	CDuiString str_url = _T("rtsp://");
	CDuiString ip, port;
	SysCfg *sys_cfg = ResSingleton::GetInstance()->GetSysCfg();
	sys_cfg->GetIpInfo(BackStreamsIP, ip, port);
	sys_cfg->WideToMulti(str_url + ip + _T("/") + port, url);

	vlc_tool_.reset(new VLCTool);
	if (!vlc_tool_->Initial())
		return;
	vlc_tool_->SetMediaSource(url);
	vlc_tool_->SetPlayerWindows(hwnd_);
	vlc_tool_->StartPlay();
	ShowWindow(true);
}

LRESULT VideoWnd::OnWndSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::PostMessage(pa_hwnd_, kAM_WndSizeChangeMsg, 0, 0);
	return LRESULT();
}

LRESULT VideoWnd::ResetIPInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	vlc_tool_->Destory();
	RunVideo();
	return LRESULT();
}
