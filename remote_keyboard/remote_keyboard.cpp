#include "remote_keyboard.h"
#include "RemoteKeyBoardCtrl/RemoteKeyBoardCtrl_h.h"
#include "RemoteKeyBoardCtrl/RemoteKeyBoardCtrl_c.c"
//#include "KeyBoardCtrl\KeyBoardCtrl.h"
#include <bitset>

#pragma comment(lib,"Rpcrt4.lib")

static const struct {
	LPCTSTR name;
	LPCTSTR attr;
} g_LessionMappingTable[] = {
	{ _T("grade_edit"), _T("grade") },
	{ _T("teacher_id_edit"), _T("teacher_id") },
	{ _T("teacher_name_edit"), _T("teacher_name") },
	{ _T("subject_edit"), _T("subject") },
	{ _T("chapter_edit"), _T("chapter") },
	{ _T("chapter_name_edit"), _T("chapter_name") }
};

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len) {
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR *ptr) {
	free(ptr);
}

RemoteKeyboard::RemoteKeyboard()
	: pa_hwnd_(NULL)
	, current_channel_(-1)
	, in_channel_(false)
	, old_point_({ 0, 0 })
	, is_move_(false)
	, setup_wnd_(nullptr)
	, m_hwBinding(0)
	, m_szStringBinding(0)
	, m_lession_info_checksum(0)
{
	InterlockedExchange(&m_check_running, TRUE);	// 原子操作，给 m_check_running 赋值为 TRUE
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

LRESULT RemoteKeyboard::OnUpdateStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandle)
{
	static bool _last_enabled = true;
	bool _enable = (wParam != 0);
	unsigned int _check_value = (unsigned int)(lParam);
	TCHAR name[32];

	if (_last_enabled != _enable) {
		_last_enabled = _enable;
		for (int i = 1;; ++i) {
			_stprintf_s(name, sizeof(name) / sizeof(TCHAR), _T("rkbc_%02d"), i);
			if (!EnableControl(name, _last_enabled))
				break;
		}
	}

	if (_last_enabled) {
		unsigned int _recode_status = _check_value & 0x3;
		unsigned int _ch_status = (_check_value >> 2) & 0xf;
		unsigned int _director_status = (_check_value >> 6) & 0x3;

		for (int i = 1; i < 14; ++i) {
			_stprintf_s(name, sizeof(name) / sizeof(TCHAR), _T("rkbc_%02d"), i);

			if (i < 4) {
				EnableControl(name, _recode_status != i);
			} else if (i < 12) {
				EnableControl(name, _ch_status != i - 3);
			} else {
				EnableControl(name, _director_status != i - 11);
			}
		}
	}

	//auto _GetExtendInfoWrap = [this](byte * &pBuf, long & nSize)->boolean {
	//	boolean ret = false;

	//	RpcTryExcept
	//		ret = rkbc_GetExtendInfo(m_hwBinding, kExtendType_LessionInfo, __int64(m_lession_info_checksum), &pBuf, &nSize);
	//	RpcExcept(1)
	//		ret = false;
	//	RpcEndExcept

	//	return ret;
	//};

	//byte * pBuf = 0;
	//long nSize = 0;
	//if (_enable && _GetExtendInfoWrap(pBuf, nSize) && pBuf) {
	//	auto const & items = g_LessionMappingTable;

	//	pugi::xml_document doc;
	//	if (doc.load(LPCTSTR(pBuf))) {
	//		pugi::xml_node elem = doc.child(_T("lesson"));
	//		if (elem) {
	//			for (int i = 0; i < _countof(items); ++i) {
	//				DuiLib::CControlUI *ctrl;
	//				if (FindControl(items[i].name, ctrl)) {
	//					ctrl->SetText(elem.attribute(items[i].attr).as_string());
	//				}
	//			}

	//			m_lession_info = LPCTSTR(pBuf);
	//			boost::crc_32_type result;
	//			result.process_bytes(reinterpret_cast<const byte *>(m_lession_info.data()), (m_lession_info.size() + 1) * sizeof(wchar_t));
	//			m_lession_info_checksum = result.checksum();
	//		}
	//	}
	//	midl_user_free(pBuf);
	//}

	return LRESULT();
}

void RemoteKeyboard::OnClickSteupBtn(TNotifyUI & msg, bool & handled)
{
	LPPOINT lpoint = new tagPOINT;
	::GetCursorPos(lpoint);
	setup_wnd_->PopupWindow(lpoint);
}

void RemoteKeyboard::OnClick(TNotifyUI & msg, bool & handled)
{
	int key = _tstoi(msg.pSender->GetUserData());

	unsigned __int64 _check_value = 0;
	bool _enable = true;

	RpcTryExcept
		_check_value = rkbc_KeyBoardCtrl(m_hwBinding, key);
	RpcExcept(1)
		_enable = false;
	RpcEndExcept

	PostMessage(WM_UPDATE_STATUS, WPARAM(_enable), LPARAM(_check_value));
}

LRESULT RemoteKeyboard::ResponseDefaultKeyEvent(WPARAM wParam)
{
	::SendMessage(GetParent(pa_hwnd_), kAM_ChildEscMsg, 0, 0);
	
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
	setup_wnd_.reset(new SetupPopWnd);
	setup_wnd_->CreateWithDefaltStyle(GetParent(pa_hwnd_));

	/*  刷新控件状态  */
	SendMessage(WM_UPDATE_STATUS, WPARAM(false), LPARAM(0));
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	/*  确定远程地址  */
	PDUI_CONTROL ctrl = m_PaintManager.FindControl(_T("addr"));
	if (!ctrl) {
		::MessageBox(m_hWnd, _T("Address control not find!"), _T("OnInit"), MB_OK);
		return;
	}
	const DuiLib::CDuiString &str = ctrl->GetUserData();
	int pos = str.Find(_T(":"));
	const DuiLib::CDuiString ip = str.Left(pos);
	const DuiLib::CDuiString port = str.Right(str.GetLength() - pos - 1);
	if (ip.GetLength() < 7 || port.GetLength() < 1) {
		::MessageBox(m_hWnd, _T("Address is empty or incorrectly set!"), _T("OnInit"), MB_OK);
		return;
	}

	/*  绑定操作  */
	if (RpcStringBindingCompose(NULL, (RPC_WSTR)_T("ncacn_ip_tcp"), (RPC_WSTR)(LPTSTR)(LPCTSTR)(ip), 
		(RPC_WSTR)(LPTSTR)(LPCTSTR)(port), NULL, &m_szStringBinding) != RPC_S_OK
		|| RpcBindingFromStringBinding(m_szStringBinding, &m_hwBinding) != RPC_S_OK)
	{
		TCHAR buf[512];
		_stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _T("Connect the controlled terminal failed : %d"), GetLastError());
		::MessageBox(m_hWnd, buf, _T("OnInit"), MB_OK);
		return;
	}

	m_check_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&RemoteKeyboard::OnCheck, this)));
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

void RemoteKeyboard::SetSetupBtnVisible(bool visible)
{
	m_PaintManager.FindControl(_T("setupbtn"))->SetVisible(visible);
}

void RemoteKeyboard::ResetKeyPos()
{
	RECT pos[8] = {};
	RECT win_rect = {};
	GetWindowRect(m_hWnd, &win_rect);
	UINT width_pitch = (win_rect.right - win_rect.left - 50) / 5;
	UINT height_pitch = (win_rect.bottom - win_rect.top - 30 - 27) / 3;

	for (int i = 0; i < 4; ++i) {
		pos[i].left = (i + 1) * width_pitch;
		pos[i].right = pos[i].left + 50;
		pos[i].top = height_pitch;
		pos[i].bottom = pos[i].top + 30;
	}
	for (int i = 4; i < 8; ++i) {
		pos[i].left = (i - 3) * width_pitch;
		pos[i].right = pos[i].left + 50;
		pos[i].top = height_pitch * 2;
		pos[i].bottom = pos[i].top + 30;
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

bool RemoteKeyboard::EnableControl(LPCTSTR name, bool enable)
{
	PDUI_CONTROL ctrl = m_PaintManager.FindControl(name);
	if (!ctrl)
		return false;

	if (ctrl->IsEnabled() != enable)
		ctrl->SetEnabled(enable);

	return true;
}

void RemoteKeyboard::OnCheck()
{
	while (m_check_running) {
		unsigned __int64 _check_value = 0;
		bool _enable = true;

		RpcTryExcept
			_check_value = rkbc_GetStatus(m_hwBinding);
		RpcExcept(1)
			_enable = false;
		RpcEndExcept

		if (!m_check_running) break;

		PostMessage(WM_UPDATE_STATUS, WPARAM(_enable), LPARAM(_check_value));
		Sleep(1000);
	}
}

