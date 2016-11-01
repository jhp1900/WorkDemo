#include "frame_wnd.h"
#include "debug_tools.h"

FrameWnd::FrameWnd()
	: video_wnd_(nullptr)
	, keyboard_(nullptr)
{
}

FrameWnd::~FrameWnd()
{
}

LRESULT FrameWnd::OnInit()
{
	::PostMessage(m_hWnd, kAM_InitOtherWndMsg, 0, 0);	// 发送一个自定义消息到消息队列中
	return LRESULT();
}

void FrameWnd::OnClickSysBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("minbtn")) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	} else if (name == _T("fullbtn")) {
		OnFullWnd();
	} else if (name == _T("closebtn")) {
		exit(0);
	}
}

void FrameWnd::OnClickTestBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("test_btn")) {
		RECT rect = { 0 };
		GetWindowRect(keyboard_->GetHWND(), &rect);
		debugPrintf("\n\n------ RemoteKeyboard Size width=%d height%d ------------\n\n",
			rect.right - rect.left, rect.bottom - rect.top);
	} else if (name == _T("show_btn")) {
		keyboard_->ShowWindow(true);
	} else if (name == _T("noshow_btn")) {
		keyboard_->ShowWindow(false);
	}
}

LRESULT FrameWnd::OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	video_wnd_.reset(new VideoWnd());
	video_wnd_->Init(m_hWnd);
	video_wnd_->RunVideo();

	keyboard_.reset(new RemoteKeyboard);
	keyboard_->Init(video_wnd_->GetHwnd());
	keyboard_->ShowWindow(true);

	return LRESULT();
}

LRESULT FrameWnd::OnVideoWndSizeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	keyboard_->ResetWndSize();
	return LRESULT();
}

void FrameWnd::OnFullWnd()
{
	m_PaintManager.FindControl(_T("title_layout"))->SetVisible(false);
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	MoveWindow(m_hWnd, 0, 0, cx, cy, true);
}

void FrameWnd::OnQuitFullWnd()
{
}
