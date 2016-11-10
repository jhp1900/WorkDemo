#include "remote_keyboard.h"
#include "RemoteKeyBoardCtrl/RemoteKeyBoardCtrl_h.h"
#include "RemoteKeyBoardCtrl/RemoteKeyBoardCtrl_c.c"
#include <bitset>
#include "debug_tools.h"
#include "pugixml\pugixml.cpp"

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
	, setup_wnd_(nullptr)
	, ptz_wnd_(nullptr)
	, m_hwBinding(0)
	, m_szStringBinding(0)
	, m_lession_info_checksum(0)
{
	InterlockedExchange(&m_check_running, TRUE);	// 原子操作，给 m_check_running 赋值为 TRUE
}

RemoteKeyboard::~RemoteKeyboard()
{
}

LRESULT RemoteKeyboard::OnCursorRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = false;
	POINT point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);
	for (int i = 4; i < 12; ++i) {
		if (InChKeyRect(i, point)) {
			in_channel_ = true;
			current_channel_ = i;
			old_point_ = point;
			return 0;
		}
	}

	return LRESULT();
}

LRESULT RemoteKeyboard::OnCursorRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = false;
	current_channel_ = -1;
	in_channel_ = false;

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

	auto _GetExtendInfoWrap = [this](byte* &pBuf, long &nSize) -> boolean {
		boolean ret = false;

		RpcTryExcept
			ret = rkbc_GetExtendInfo(m_hwBinding, kExtendType_LessionInfo, __int64(m_lession_info_checksum), &pBuf, &nSize);
		RpcExcept(1)
			ret = false;
		RpcEndExcept

			return ret;
	};

	/* 如果同步排课界面不处于显示状态，则不刷新 */
	if (!m_PaintManager.FindControl(_T("sync_panel"))->IsVisible())	 
		return 0;
	byte *pBuf = 0;
	long nSize = 0;
	if (_enable && _GetExtendInfoWrap(pBuf, nSize) && pBuf) {
		auto const &items = g_LessionMappingTable;

		pugi::xml_document doc;
		if (doc.load(LPCTSTR(pBuf))) {
			pugi::xml_node elem = doc.child(_T("lesson"));
			if (elem) {
				for (int i = 0; i < _countof(items); ++i) {
					DuiLib::CControlUI *ctrl;
					if (ctrl = m_PaintManager.FindControl(items[i].name))
						ctrl->SetText(elem.attribute(items[i].attr).as_string());
				}

				m_lession_info = LPCTSTR(pBuf);
				boost::crc_32_type result;
				result.process_bytes(reinterpret_cast<const byte *>(m_lession_info.data()), (m_lession_info.size() + 1) * sizeof(wchar_t));
				m_lession_info_checksum = result.checksum();
			}
		}
		midl_user_free(pBuf);
	}

	return LRESULT();
}

LRESULT RemoteKeyboard::OnPopClickMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandle)
{
	switch (wParam)
	{
		case ClassSchedule: {
			PDUI_CONTROL panel = m_PaintManager.FindControl(_T("sync_panel"));
			panel->SetVisible(!panel->IsVisible());
			break;
		}

		case PTZ: {
			ptz_wnd_.reset(new PtzWnd(m_hWnd));
			ptz_wnd_->DoModal();
			break;
		}

		case ControlPanel: {
			PDUI_CONTROL panel = m_PaintManager.FindControl(_T("control_panel"));
			panel->SetVisible(!panel->IsVisible());
			break;
		}

		default:
			break;
	}
	return LRESULT();
}

void RemoteKeyboard::OnClickSteupBtn(TNotifyUI & msg, bool & handled)
{
	LPPOINT lpoint = new tagPOINT;
	::GetCursorPos(lpoint);
	setup_wnd_->PopupWindow(lpoint);
}

void RemoteKeyboard::OnClosePanel(TNotifyUI & msg, bool & handled)
{
	msg.pSender->GetParent()->SetVisible(false);
}

void RemoteKeyboard::OnClickSyncBtn(TNotifyUI & msg, bool & handled)
{
	auto const &items = g_LessionMappingTable;
	DuiLib::CControlUI *ctrl;

	if (msg.pSender->GetName() == _T("sync_ok")) {
		pugi::xml_document xmldoc;
		pugi::xml_node root = xmldoc.append_child(PUGIXML_TEXT("lesson"));

		for (int i = 0; i < _countof(items); ++i) {
			pugi::xml_attribute attr = root.append_attribute(items[i].attr);
			if (ctrl = m_PaintManager.FindControl(items[i].name))
				attr = ctrl->GetText();
		}
		std::wostringstream oss;
		root.print(oss, L"", pugi::format_raw);

		auto _ExecuteExtendCmdWrap = [this](const std::wstring &buf) -> boolean {
			boolean ret = false;
			RpcTryExcept
				ret = rkbc_ExecuteExtendCmd(m_hwBinding, kExtendType_LessionInfo, reinterpret_cast<const byte *>(buf.data()), (buf.size() + 1) * sizeof(wchar_t));
			RpcExcept(1)
				ret = false;
			RpcEndExcept

				return ret;
		};

		if (_ExecuteExtendCmdWrap(oss.str())) {
			m_lession_info = oss.str();

			boost::crc_32_type result;
			result.process_bytes(reinterpret_cast<const byte *>(m_lession_info.data()), (m_lession_info.size() + 1) * sizeof(wchar_t));
			m_lession_info_checksum = result.checksum();
		}
	}

	if (ctrl = m_PaintManager.FindControl(_T("sync_panel")))
		ctrl->SetVisible(false);
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

	PostMessage(kAM_Update_Status, WPARAM(_enable), LPARAM(_check_value));
}

LRESULT RemoteKeyboard::ResponseDefaultKeyEvent(WPARAM wParam)
{
	::SendMessage(GetParent(pa_hwnd_), kAM_ChildEscMsg, 0, 0);
	return LRESULT();
}

void RemoteKeyboard::Init(HWND pa_hwnd)
{
	pa_hwnd_ = pa_hwnd;
	Create(pa_hwnd, _T(""), WS_POPUP & ~WS_VISIBLE | WS_MAXIMIZE, WS_EX_LAYERED);
	SetParent(m_hWnd, pa_hwnd_);
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// 设置透明色  		
	COLORREF cr_key = GetWindowBkColor(_T("body_layout"));
	SetLayeredWindowAttributes(m_hWnd, cr_key, 0, LWA_COLORKEY);
	//SetLayeredWindowAttributes(m_hWnd, 0, 50, LWA_ALPHA);

	ResetKeyPos();
	setup_wnd_.reset(new SetupPopWnd(m_hWnd, GetParent(pa_hwnd_)));
	setup_wnd_->CreateWithDefaltStyle();

	/*  刷新控件状态  */
	SendMessage(kAM_Update_Status, WPARAM(false), LPARAM(0));
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

LRESULT RemoteKeyboard::OnInit()
{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// 设置透明色  
	COLORREF clTransparent = RGB(0, 0, 0);
	SetLayeredWindowAttributes(m_hWnd, clTransparent, 0, LWA_COLORKEY); // 设置异形
	SetLayeredWindowAttributes(m_hWnd, 0, 50, LWA_ALPHA);    // 设置半透明

	return LRESULT();
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
	UINT width_pitch = (win_rect.right - win_rect.left - 70) / 5;
	UINT height_pitch = (win_rect.bottom - win_rect.top - 50 - 27) / 3;

	for (int i = 0; i < 4; ++i) {
		pos[i].left = (i + 1) * width_pitch;
		pos[i].right = pos[i].left + 70;
		pos[i].top = height_pitch;
		pos[i].bottom = pos[i].top + 50;
	}
	for (int i = 4; i < 8; ++i) {
		pos[i].left = (i - 3) * width_pitch;
		pos[i].right = pos[i].left + 70;
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

		PostMessage(kAM_Update_Status, WPARAM(_enable), LPARAM(_check_value));
		Sleep(1000);
	}
}

