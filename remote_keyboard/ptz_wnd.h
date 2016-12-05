#pragma once
#include "window_impl_base.h"

class PtzWnd : public WindowImplBase
{
public:
	PtzWnd(HWND pa_hwnd);
	~PtzWnd();

	DECLARE_DUIWND_INFO(_T("PtzWnd"), CS_DBLCLKS, _T("ptz_wnd.xml"))

	BEGIN_DUINOTIFY_MAP(PtzWnd)
		DUINOTIFY_HANDLER(_T("close_ptz"), DUINOTIFY_CLICK, OnClickBtn)
	END_DUINOTIFY_MAP()

	//BEGIN_DUIMSG_MAP(PtzWnd)
	//	DUIMSG_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	//	DUIMSG_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	//END_DUIMSG_MAP()

public:
	bool DoModal();
	virtual LRESULT OnInit() override;

private:
	void OnClickBtn(TNotifyUI& msg, bool& handled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

private:
	bool InBtnRect(POINT point, LPCTSTR name);

private:
	HWND pa_hwnd_;
	vector<LPCTSTR> btn_name_;
};

