#pragma once
#include "view_base.h"
#include "msg_header.h"
#include "native_control_ui.h"
#include "res_file_scan_wnd.h"

class Manager : public ViewBase
{
public:
  Manager();
	~Manager() {}

  BEGIN_DUICONTROL_CREATE(Manager)
    DUICONTROL_CREATE(_T("Panel"), NativeControlUI)
    END_DUICONTROL_CREATE()

  BEGIN_DUIMSG_MAP(Manager)
    DUIMSG_HANDLER(kAM_InitOtherWndMsg, OnInitOtherWndMsg)
  END_DUIMSG_MAP()

  static Manager* GetInstance();
  static void DestroyInstance();

  virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
  virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

public:

private:
	virtual LRESULT OnInit() override;

private:
  LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
  static Manager *instance_;    // 单例模式的实例  
  ResFileScanWnd *resfile_wnd_;
};