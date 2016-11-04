#pragma once
#include "window_impl_base.h"
#include "msg_head.h"
#include <memory>
#include "setup_pop_wnd.h"
#include <boost\crc.hpp>
#include <boost\thread.hpp>
//#include <atlbase.h>
//#include <atlwin.h>
//#include <atltypes.h>

class RemoteKeyboard : public WindowImplBase
{
public:
	RemoteKeyboard();
	~RemoteKeyboard();

	DECLARE_DUIWND_INFO(_T("RemoteKeyboard"), CS_DBLCLKS, _T("RemoteKeyBoard.xml"))

	BEGIN_DUIMSG_MAP(RemoteKeyboard)
		DUIMSG_HANDLER(WM_LBUTTONDOWN, OnCursorLButtonDown)
		DUIMSG_HANDLER(WM_LBUTTONUP, OnCursorLButtonUp)
		DUIMSG_HANDLER(WM_MOUSEMOVE, OnCursorMove)
		DUIMSG_HANDLER(WM_TIMER, OnTime)
		DUIMSG_HANDLER(kAM_Update_Status, OnUpdateStatus)
		DUIMSG_HANDLER(kAM_PopVKMsg, OnPopVKMsg)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(FrameWnd)
		DUINOTIFY_HANDLER(_T("setupbtn"), DUINOTIFY_CLICK, OnClickSteupBtn)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_CLICK, OnClick)
	END_DUINOTIFY_MAP()

private:
	LRESULT OnCursorLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCursorLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCursorMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnTime(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);
	LRESULT OnUpdateStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);
	LRESULT OnPopVKMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);

private:
	void OnClickSteupBtn(TNotifyUI& msg, bool& handled);
	void OnClick(TNotifyUI& msg, bool& handled);

private:
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

public:
	void Init(HWND pa_hwnd);
	COLORREF GetWindowBkColor(LPCTSTR control_name);
	void ResetWndSize();
	void SetSetupBtnVisible(bool visible);				// 设置 设置按钮 的可见性

private:
	void ResetKeyPos();
	bool InChKeyRect(UINT channel, POINT point);
	bool EnableControl(LPCTSTR name, bool enable);
	void OnCheck();
	void OnVKClick(int key);

private:
	HWND pa_hwnd_;
	UINT current_channel_;
	bool in_channel_;
	POINT old_point_;
	bool is_move_;
	std::shared_ptr<SetupPopWnd> setup_wnd_;

	RPC_BINDING_HANDLE m_hwBinding;
	RPC_WSTR m_szStringBinding;
	boost::shared_ptr<boost::thread> m_check_thread;
	volatile LONG m_check_running;
	std::wstring m_lession_info;
	DWORD m_lession_info_checksum;
};

