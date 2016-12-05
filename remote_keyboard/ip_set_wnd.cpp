#include "ip_set_wnd.h"
#include "msg_head.h"

IpSetWnd::IpSetWnd(HWND pa_hwnd, IPSourceType type)
	: pa_hwnd_(pa_hwnd)
	, ip_source_type_(type)
{
}

IpSetWnd::~IpSetWnd()
{
}

bool IpSetWnd::DoModal()
{
	if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(pa_hwnd_);
	OnLocalInit();
	return ShowModal() != 0;
}

LRESULT IpSetWnd::OnInit()
{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// ÉèÖÃÍ¸Ã÷É«  		
	COLORREF cr_key = RGB(0, 0, 0);
	SetLayeredWindowAttributes(m_hWnd, cr_key, 0, LWA_COLORKEY);
	SetLayeredWindowAttributes(m_hWnd, 0, 170, LWA_ALPHA);
	return LRESULT();
}

void IpSetWnd::OnLocalInit()
{
	CDuiString ip, port;
	SysCfg *sys_cfg = ResSingleton::GetInstance()->GetSysCfg();
	sys_cfg->GetIpInfo(ip_source_type_, ip, port);
	m_PaintManager.FindControl(_T("ip_edit"))->SetText(ip);
	m_PaintManager.FindControl(_T("port_edit"))->SetText(port);
}

void IpSetWnd::OnClickBtn(TNotifyUI & msg, bool & handled)
{
	if (msg.pSender->GetName() == _T("ok")) {
		WPARAM ip = (WPARAM)m_PaintManager.FindControl(_T("ip_edit"))->GetText().GetData();
		LPARAM port = (LPARAM)m_PaintManager.FindControl(_T("port_edit"))->GetText().GetData();
		ResSingleton::GetInstance()->GetSysCfg()->SetIpInfo(ip_source_type_, (LPCTSTR)ip, (LPCTSTR)port);

		if(ip_source_type_ == ServerIP)
			::PostMessage(pa_hwnd_, kAM_ResetServeIP, ip, port);
		else if(ip_source_type_ == BackStreamsIP)
			::PostMessage(pa_hwnd_, kAM_ResetBackStreamsIP, 0, 0);
	}

	Close(0);
}
