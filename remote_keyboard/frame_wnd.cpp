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

	} else if (name == _T("show_btn")) {
		
	} else if (name == _T("noshow_btn")) {
		
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

	OnFullWnd();
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
	keyboard_->SetSetupBtnVisible(true);
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);	
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
