#include "setup_pop_wnd.h"

std::pair<CDuiString, CDuiString> syn_name[] = {
	{ _T("begin"), _T("begin_check") },
	{ _T("pause"), _T("pause_check") },
	{ _T("stop"), _T("stop_check") },
	{ _T("auto_direct"), _T("auto_direct_check") },
	{_T("manual_direct"), _T("manual_direct_check")}
};

SetupPopWnd::SetupPopWnd(HWND pa_hwnd, HWND top_hwnd)
	: pa_hwnd_(pa_hwnd)
	, top_hwnd_(top_hwnd)
{
}

SetupPopWnd::~SetupPopWnd()
{
}

LRESULT SetupPopWnd::OnInit()
{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// ����͸��ɫ  		
	COLORREF cr_key = RGB(0, 0, 0);
	SetLayeredWindowAttributes(m_hWnd, cr_key, 0, LWA_COLORKEY);
	SetLayeredWindowAttributes(m_hWnd, 0, 170, LWA_ALPHA);
	return LRESULT();
}

HWND SetupPopWnd::CreateWithStyle(DWORD style)
{
	return Create(pa_hwnd_, nullptr, style, 0);
}

HWND SetupPopWnd::CreateWithDefaltStyle()
{
	return Create(pa_hwnd_, nullptr,
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
}

LRESULT SetupPopWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT SetupPopWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT SetupPopWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	ShowWindow(SW_HIDE);
	bHandled = FALSE;
	return LRESULT();
}

LRESULT SetupPopWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	ShowWindow(SW_HIDE);
	return LRESULT();
}

void SetupPopWnd::OnClickBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("sync_lession"))
		::SendMessage(pa_hwnd_, kAM_PopClickMsg, ClassSchedule, 0);
	else if (name == _T("ptz"))
		::SendMessage(pa_hwnd_, kAM_PopClickMsg, PTZ, 0);
	else if (name == _T("control_panel"))
		::SendMessage(pa_hwnd_, kAM_PopClickMsg, ControlPanel, 0);
	else if (name == _T("link_serve"))
		::SendMessage(pa_hwnd_, kAM_PopClickMsg, LinkServe, 0);
	else if (name == _T("back_streams"))
		::SendMessage(pa_hwnd_, kAM_PopClickMsg, BackStreams, 0);
	else if (name == _T("quit_full"))
		::SendMessage(top_hwnd_, kAM_ChildEscMsg, 0, 0);
	else if (name == _T("closebtn"))
		::exit(0);

	ShowWindow(SW_HIDE);
}

void SetupPopWnd::PopupWindow(PPOINT point, bool left_bottom)
{
	int srceen_width = ::GetSystemMetrics(SM_CXSCREEN);
	int srceen_height = ::GetSystemMetrics(SM_CYSCREEN);
	int x = point->x;
	int y = point->y;

	if (x + 100 > srceen_width)
		x -= 100;
	if (y + 130 > srceen_height)
		y -= 130;

	::SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	ShowWindow();
}

void SetupPopWnd::DataSync(int index, bool enable)
{
	//m_PaintManager.FindControl(syn_name[index].first)->SetEnabled(enable);
}
