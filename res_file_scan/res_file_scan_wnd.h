#pragma once
#include "view_base.h"

class ResFileScanWnd : public ViewBase
{
public:
  ResFileScanWnd(HWND hwnd_parent);
  ~ResFileScanWnd();

  DECLARE_DUIWND_INFO(_T("ResFileScanWnd"), CS_DBLCLKS, _T("res_file_scan_wnd.xml"))

  BEGIN_DUINOTIFY_MAP(ResFileScanWnd)
    DUINOTIFY_HANDLER(_T("open_path"), DUINOTIFY_CLICK, OnClickOpen)
    DUINOTIFY_HANDLER(_T("test_btn"), DUINOTIFY_CLICK, OnClickTestBtn)
  END_DUINOTIFY_MAP()


public:
  void Init();

private:
  void OnClickOpen(TNotifyUI &msg, bool &handled);
  void FillFiles();

  void OnClickTestBtn(TNotifyUI &msg, bool &handled);

private:
  HWND hwnd_parent_;
  vector<TCHAR*> files_;    // 加载的文件集合。
  CDuiString folder_path_;
};

