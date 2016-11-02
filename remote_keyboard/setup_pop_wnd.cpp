#include "setup_pop_wnd.h"

SetupPopWnd::SetupPopWnd()
	: pa_hwnd_(NULL)
{
}

SetupPopWnd::~SetupPopWnd()
{
}

LRESULT SetupPopWnd::OnInit()
{
	return LRESULT();
}

HWND SetupPopWnd::CreateWithStyle(HWND pa_hwnd, DWORD style)
{
	pa_hwnd_ = pa_hwnd;
	return Create(pa_hwnd_, nullptr, style, 0);
}

HWND SetupPopWnd::CreateWithDefaltStyle(HWND pa_hwnd)
{
	pa_hwnd_ = pa_hwnd;
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

void SetupPopWnd::OnClickBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("quit_full")) {
		::SendMessage(pa_hwnd_, kAM_ChildEscMsg, 0, 0);
	}
}

void SetupPopWnd::PopupWindow(PPOINT point, bool left_bottom)
{
	int srceen_width = ::GetSystemMetrics(SM_CXSCREEN);
	int srceen_height = ::GetSystemMetrics(SM_CYSCREEN);
	int x = point->x;
	int y = point->y;

	if (x + 125 > srceen_width)
		x -= 125;
	if (y + 150 > srceen_height)
		y -= 150;

	::SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	ShowWindow();
}
