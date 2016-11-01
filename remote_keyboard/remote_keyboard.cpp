#include "remote_keyboard.h"

RemoteKeyboard::RemoteKeyboard()
	: pa_hwnd_(NULL)
	, current_channel_(-1)
	, in_channel_(false)
	, old_point_({ 0, 0 })
	, is_move_(false)
{
}

RemoteKeyboard::~RemoteKeyboard()
{
}

LRESULT RemoteKeyboard::OnCursorLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = false;
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);
	for (int i = 4; i < 12; ++i) {
		if (InChKeyRect(i, point)) {
			SetTimer(m_hWnd, 1, 200, nullptr);
			in_channel_ = true;
			is_move_ = false;
			current_channel_ = i;
			old_point_ = point;
			return 0;
		}
	}

	return LRESULT();
}

LRESULT RemoteKeyboard::OnCursorLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = false;
	current_channel_ = -1;
	if (in_channel_) {
		if (is_move_)
			bHandled = true;
		KillTimer(m_hWnd, 1);
		in_channel_ = false;
	}

	return LRESULT();
}

LRESULT RemoteKeyboard::OnCursorMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (in_channel_) {
		TCHAR name[32];
		_stprintf_s(name, sizeof(name) / sizeof(TCHAR), _T("rkbc_%02d"), current_channel_);
		PDUI_CONTROL ctrl = m_PaintManager.FindControl(name);
		RECT rect = ctrl->GetPos();
		int add_x = GET_X_LPARAM(lParam) - old_point_.x;
		int add_y = GET_Y_LPARAM(lParam) - old_point_.y;
		old_point_.x = GET_X_LPARAM(lParam);
		old_point_.y = GET_Y_LPARAM(lParam);
		rect.left += add_x;
		rect.right += add_x;
		rect.top += add_y;
		rect.bottom += add_y;
		ctrl->SetPos(rect);
	}

	bHandled = false;
	return LRESULT();
}

LRESULT RemoteKeyboard::OnTime(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandle)
{
	if (in_channel_)
		return is_move_ = true;

	is_move_ = false;
	return LRESULT();
}

void RemoteKeyboard::Init(HWND pa_hwnd)
{
	pa_hwnd_ = pa_hwnd;
	Create(pa_hwnd, _T(""), WS_POPUP & ~WS_VISIBLE | WS_MAXIMIZE, WS_EX_LAYERED, 0, 0, 0, 0);
	SetParent(m_hWnd, pa_hwnd_);
	COLORREF cr_key = GetWindowBkColor(_T("body_layout"));
	SetLayeredWindowAttributes(m_hWnd, cr_key, 255, LWA_COLORKEY);

	ResetKeyPos();
}

COLORREF RemoteKeyboard::GetWindowBkColor(LPCTSTR control_name)
{
	DWORD bkcolor = m_PaintManager.FindControl(control_name)->GetBkColor();
	return RGB(BYTE(bkcolor >> 16), BYTE(bkcolor >> 8), BYTE(bkcolor));
}

void RemoteKeyboard::ResetWndSize()
{
	RECT pa_rect = { 0 };
	GetWindowRect(pa_hwnd_, &pa_rect);
	int width = pa_rect.right - pa_rect.left;
	int height = pa_rect.bottom - pa_rect.top;
	MoveWindow(m_hWnd, 0, 0, width, height, true);
	ResetKeyPos();
}

void RemoteKeyboard::ResetKeyPos()
{
	RECT pos[8] = {};
	RECT win_rect = {};
	GetWindowRect(m_hWnd, &win_rect);
	UINT width_pitch = (win_rect.right - win_rect.left - 50) / 5;
	UINT height_pitch = (win_rect.bottom - win_rect.top - 50 - 27) / 3;

	for (int i = 0; i < 4; ++i) {
		pos[i].left = (i + 1) * width_pitch;
		pos[i].right = pos[i].left + 50;
		pos[i].top = height_pitch;
		pos[i].bottom = pos[i].top + 50;
	}
	for (int i = 4; i < 8; ++i) {
		pos[i].left = (i - 3) * width_pitch;
		pos[i].right = pos[i].left + 50;
		pos[i].top = height_pitch * 2;
		pos[i].bottom = pos[i].top + 50;
	}

	TCHAR name[32];
	PDUI_CONTROL ctrl;
	for (int i = 0; i < 8; ++i) {
		_stprintf_s(name, sizeof(name) / sizeof(TCHAR), _T("rkbc_%02d"), i + 4);
		ctrl = m_PaintManager.FindControl(name);
		ctrl->SetPos(pos[i]);
	}
}

bool RemoteKeyboard::InChKeyRect(UINT channel, POINT point)
{
	TCHAR name[32];
	_stprintf_s(name, sizeof(name) / sizeof(TCHAR), _T("rkbc_%02d"), channel);
	const RECT rect = m_PaintManager.FindControl(name)->GetPos();
	return PtInRect(&rect, point);
}

