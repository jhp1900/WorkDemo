#include "res_file_scan_wnd.h"
#include <ShlObj.h>
#include "traversing_resource.h"
#include "res_file_control.h"

ResFileScanWnd::ResFileScanWnd(HWND hwnd_parent)
  : hwnd_parent_(hwnd_parent)
{
}

ResFileScanWnd::~ResFileScanWnd()
{
}

void ResFileScanWnd::Init()
{
  Create(hwnd_parent_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
  ::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void ResFileScanWnd::OnClickOpen(TNotifyUI & msg, bool & handled)
{
  TCHAR folder_path[MAX_PATH] = { 0 };      // 文件夹路径
  BROWSEINFO bi = { 0 };
  bi.hwndOwner = 0;
  bi.pszDisplayName = folder_path;
  bi.lpszTitle = _T("文件夹目录：");
  bi.ulFlags = BIF_RETURNFSANCESTORS;         // BIF_RETURNONLYFSDIRS 只用于选中目录， BIF_BROWSEINCLUDEFILES 可用于目录和文件
  LPITEMIDLIST idl = SHBrowseForFolder(&bi);  // 打开文件对话框

  if (!idl)
    return;

  SHGetPathFromIDList(idl, folder_path);      // 获取文件的路径, 如：D:\Dow\360Yun\video
  folder_path_ = folder_path;

  PDUI_EDIT edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("res_path")));
  if (edit)
    edit->SetText(folder_path);

  TraversingResource search_file;
  vector<LPCTSTR> ex_names;
  search_file.GetResFiles(folder_path, ex_names, files_);
  FillFiles();
}

void ResFileScanWnd::FillFiles()
{
  PDUI_TILELAYOUT file_panel;
  file_panel = static_cast<PDUI_TILELAYOUT>(m_PaintManager.FindControl(_T("res_file_panel")));
  if (file_panel == 0)
    return;

  file_panel->RemoveAll();
  ResFileControl file_ctrl(folder_path_.GetData());
  for (auto iter : files_) {
    file_panel->Add(file_ctrl.CreateCtrlIter(iter));
  }

  file_ctrl.Reset(_T("45hfdsan"));
}

void ResFileScanWnd::OnClickTestBtn(TNotifyUI & msg, bool & handled)
{

}
