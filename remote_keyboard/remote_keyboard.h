#pragma once
#include "window_impl_base.h"
#include "msg_head.h"
#include <memory>
#include "setup_pop_wnd.h"

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
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(FrameWnd)
		DUINOTIFY_HANDLER(_T("setupbtn"), DUINOTIFY_CLICK, OnClickSteupBtn)
	END_DUINOTIFY_MAP()

private:
	LRESULT OnCursorLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCursorLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCursorMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnTime(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);

private:
	void OnClickSteupBtn(TNotifyUI& msg, bool& handled);

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

private:
	HWND pa_hwnd_;
	UINT current_channel_;
	bool in_channel_;
	POINT old_point_;
	bool is_move_;
	std::shared_ptr<SetupPopWnd> setup_wnd_;
};

