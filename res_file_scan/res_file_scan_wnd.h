#pragma once
#include "view_base.h"
#include "res_file_control.h"

static CDuiString folder_path_;

class ResFileScanWnd : public ViewBase
{
public:
  ResFileScanWnd(HWND hwnd_parent);
  ~ResFileScanWnd();

  DECLARE_DUIWND_INFO(_T("ResFileScanWnd"), CS_DBLCLKS, _T("res_file_scan_wnd.xml"))

  BEGIN_DUINOTIFY_MAP(ResFileScanWnd)
    DUINOTIFY_HANDLER(_T("open_path"), DUINOTIFY_CLICK, OnClickOpen)
    DUINOTIFY_HANDLER(_T("test_btn"), DUINOTIFY_CLICK, OnClickTestBtn)
    DUINOTIFY_HANDLER(_T("all_file"), DUINOTIFY_SELECTCHANGED, OnChangeFileType)
    DUINOTIFY_HANDLER(_T("video_file"), DUINOTIFY_SELECTCHANGED, OnChangeFileType)
    DUINOTIFY_HANDLER(_T("audio_file"), DUINOTIFY_SELECTCHANGED, OnChangeFileType)
    DUINOTIFY_HANDLER(_T("picture_file"), DUINOTIFY_SELECTCHANGED, OnChangeFileType)
    DUINOTIFY_TYPE_HANDLER(DUINOTIFY_CLICK, OnClickCtrl)
  END_DUINOTIFY_MAP()

public:
  void Init();

private:
  void OnClickOpen(TNotifyUI &msg, bool &handled);
  void OnChangeFileType(TNotifyUI &msg, bool &handled);
  void OnClickCtrl(TNotifyUI &msg, bool &handled);
  void FillFiles();

  void OnClickTestBtn(TNotifyUI &msg, bool &handled);

private:
  HWND hwnd_parent_;
  vector<TCHAR*> files_;    // 加载的文件集合。
  FILE_TYPE file_type_;
  ResFileCtrl * file_ctrl_;
  const vector<LPCTSTR> *ex_names_;
};

