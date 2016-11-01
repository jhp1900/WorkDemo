#pragma once
#include "window_impl_base.h"

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

private:
	LRESULT OnCursorLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCursorLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCursorMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnTime(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);

public:
	void Init(HWND pa_hwnd);
	COLORREF GetWindowBkColor(LPCTSTR control_name);
	void ResetWndSize();

private:
	void ResetKeyPos();
	bool InChKeyRect(UINT channel, POINT point);

private:
	HWND pa_hwnd_;
	UINT current_channel_;
	bool in_channel_;
	POINT old_point_;
	bool is_move_;
};

