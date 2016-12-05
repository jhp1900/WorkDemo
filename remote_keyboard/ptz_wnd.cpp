#include "ptz_wnd.h"
#include "msg_head.h"

PtzWnd::PtzWnd(HWND pa_hwnd)
	: pa_hwnd_(pa_hwnd)
{
}

PtzWnd::~PtzWnd()
{
}

bool PtzWnd::DoModal()
{
	if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(pa_hwnd_);
	return ShowModal() != 0;
}

LRESULT PtzWnd::OnInit()
{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// ÉèÖÃÍ¸Ã÷É«  		
	COLORREF cr_key = RGB(0, 0, 0);
	SetLayeredWindowAttributes(m_hWnd, cr_key, 0, LWA_COLORKEY);
	SetLayeredWindowAttributes(m_hWnd, 0, 170, LWA_ALPHA);

	btn_name_.push_back(_T("ptz_up"));
	btn_name_.push_back(_T("ptz_down"));
	btn_name_.push_back(_T("ptz_left"));
	btn_name_.push_back(_T("ptz_right"));
	btn_name_.push_back(_T("ptz_home"));
	btn_name_.push_back(_T("ptz_zoom_inc"));
	btn_name_.push_back(_T("ptz_zoom_dec"));

	return LRESULT();
}

void PtzWnd::OnClickBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.pSender->GetName() == _T("close_ptz")) {
		Close(0);
	} else {
		::PostMessage(pa_hwnd_, kAM_PTZ, (WPARAM)name.GetData(), 0);
		Sleep(100);
		::PostMessage(pa_hwnd_, kAM_PTZ, (WPARAM)_T("ptz_stop"), 0);
	}
}

LRESULT PtzWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	if (!InBtnRect(point, _T("close_ptz"))) {
		for (auto name : btn_name_) {
			if (InBtnRect(point, name)) {
				::PostMessage(pa_hwnd_, kAM_PTZ, (WPARAM)name, 0);
				OutputDebugString(_T("LButtonDown -- debugview"));
				break;
			}
		}
	}
	return 0;
}

LRESULT PtzWnd::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	if (InBtnRect(point, _T("close_ptz")))
		Close(0);
	else 
		::PostMessage(pa_hwnd_, kAM_PTZ, (WPARAM)_T("ptz_stop"), 0);

	OutputDebugString(_T("LButtonUp -- debugview"));
	return LRESULT();
}

bool PtzWnd::InBtnRect(POINT point, LPCTSTR name)
{
	const RECT rect = m_PaintManager.FindControl(name)->GetPos();
	return PtInRect(&rect, point);
}

