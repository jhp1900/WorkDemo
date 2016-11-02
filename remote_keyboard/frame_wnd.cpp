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

LRESULT FrameWnd::OnWndSizeChangeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	keyboard_->ResetWndSize();
	return LRESULT();
}

LRESULT FrameWnd::OnChildEscMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	OnQuitFullWnd();
	return LRESULT();
}

LRESULT FrameWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if(wParam == VK_ESCAPE)
		OnQuitFullWnd();
	return LRESULT();
}

void FrameWnd::OnFullWnd()
{
	m_PaintManager.FindControl(_T("title_layout"))->SetVisible(false);
	PDUI_CONTAINER frame_body;
	frame_body = static_cast<PDUI_CONTAINER>(m_PaintManager.FindControl(_T("frame_body")));
	frame_body->SetInset({ 0,0,0,0 });
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(m_hWnd, 0, 0, cx, cy, true);
	keyboard_->SetSetupBtnVisible(true);
}

void FrameWnd::OnQuitFullWnd()
{
	keyboard_->SetSetupBtnVisible(false);
	m_PaintManager.FindControl(_T("title_layout"))->SetVisible(true);
	PDUI_CONTAINER frame_body;
	frame_body = static_cast<PDUI_CONTAINER>(m_PaintManager.FindControl(_T("frame_body")));
	frame_body->SetInset({ 5,0,5,5 });
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}
