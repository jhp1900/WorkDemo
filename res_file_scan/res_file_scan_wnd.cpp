#include "res_file_scan_wnd.h"
#include <ShlObj.h>

#include "traversing_resource.h"

ResFileScanWnd::ResFileScanWnd(HWND hwnd_parent)
  :hwnd_parent_(hwnd_parent)
{
}

ResFileScanWnd::~ResFileScanWnd()
{
}

void ResFileScanWnd::OnClickOpen(TNotifyUI & msg, bool & handled)
{
  TCHAR folder_path[MAX_PATH] = { 0 };      // �ļ���·��
  BROWSEINFO bi = { 0 };
  bi.hwndOwner = 0;
  bi.pszDisplayName = folder_path;
  bi.lpszTitle = _T("�ļ���Ŀ¼��");
  bi.ulFlags = BIF_RETURNFSANCESTORS;         // BIF_RETURNONLYFSDIRS ֻ����ѡ��Ŀ¼�� BIF_BROWSEINCLUDEFILES ������Ŀ¼���ļ�
  LPITEMIDLIST idl = SHBrowseForFolder(&bi);  // ���ļ��Ի���

  if (!idl)
    return;

  SHGetPathFromIDList(idl, folder_path);      // ��ȡ�ļ���·��, �磺D:\Dow\360Yun\video

  TraversingResource search_file;
  //vector<LPCTSTR> ex_name = {// _T(".txt"),
  //  _T(".png"),
  //  _T(".jpg"),
  //  _T(".mp3"),
  //  _T(".mp4") };
  vector<LPCTSTR> ex_names;
  search_file.GetResFiles(folder_path, ex_names, files_);
}

void ResFileScanWnd::FillFiles()
{
}

void ResFileScanWnd::Init()
{
  Create(hwnd_parent_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
  ::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}