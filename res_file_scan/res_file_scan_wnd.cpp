#include "res_file_scan_wnd.h"
#include "traversing_resource.h"
#include <ShlObj.h>

ResFileScanWnd::ResFileScanWnd(HWND hwnd_parent)
  : hwnd_parent_(hwnd_parent)
  , ex_names_(&file_type_.all_type)
{
  folder_path_ = _T("D:\\Dow\\360Yun");
  file_ctrl_ = new ResFileCtrl(folder_path_.GetData());
}

ResFileScanWnd::~ResFileScanWnd()
{
}

void ResFileScanWnd::Init()
{
  Create(hwnd_parent_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
  ::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  switch (uMsg)
  {
    case BFFM_INITIALIZED:
    {
      ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)folder_path_.GetData());
      break;
    }
    case BFFM_SELCHANGED:
    {
      TCHAR curr[MAX_PATH];
      SHGetPathFromIDList((LPCITEMIDLIST)lParam, curr);
      ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)curr);
      break;
    }
    default:
      break;
  }
  return 0;
}

void ResFileScanWnd::OnClickOpen(TNotifyUI & msg, bool & handled)
{
  TCHAR folder_path[MAX_PATH] = { 0 };        // 文件夹路径
  BROWSEINFO bi = { 0 };
  bi.hwndOwner = *this;
  bi.pszDisplayName = folder_path;
  bi.lpszTitle = _T("文件夹目录：");
  bi.ulFlags = BIF_RETURNFSANCESTORS;         // BIF_RETURNONLYFSDIRS 只用于选中目录， BIF_BROWSEINCLUDEFILES 可用于目录和文件
  bi.lpfn = BrowseCallbackProc;
  LPITEMIDLIST idl = SHBrowseForFolder(&bi);  // 打开文件对话框

  if (!idl)
    return;

  SHGetPathFromIDList(idl, folder_path);      // 获取文件的路径, 如：D:\Dow\360Yun\video
  folder_path_ = folder_path;

  PDUI_EDIT edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("res_path")));
  if (edit)
    edit->SetText(folder_path);

  FillFiles();
}

void ResFileScanWnd::OnChangeFileType(TNotifyUI & msg, bool & handled)
{
  LPCTSTR box_name = msg.pSender->GetName();
  if (!lstrcmp(box_name, _T("all_file"))) {
    ex_names_ = &file_type_.all_type;
  } else if (!lstrcmp(box_name, _T("video_file"))) {
    ex_names_ = &file_type_.video_type;
  } else if (!lstrcmp(box_name, _T("audio_file"))) {
    ex_names_ = &file_type_.audio_type;
  } else if (!lstrcmp(box_name, _T("picture_file"))) {
    ex_names_ = &file_type_.picture_type;
  }

  FillFiles();
}

void ResFileScanWnd::OnClickCtrl(TNotifyUI & msg, bool & handled)
{
	file_ctrl_->SelectChange(static_cast<PDUI_LISTCONTAINERELEM>(msg.pSender->GetParent()));
}

void ResFileScanWnd::FillFiles()
{
  TraversingResource search_file;
  search_file.GetResFiles(folder_path_, ex_names_, files_);

  PDUI_TILELAYOUT file_panel;
  file_panel = static_cast<PDUI_TILELAYOUT>(m_PaintManager.FindControl(_T("res_file_panel")));
  if (file_panel == 0)
    return;

  file_panel->RemoveAll();
  file_ctrl_->Reset(folder_path_.GetData());
  for (auto iter : files_) {
    file_panel->Add(file_ctrl_->CreateCtrlIter(iter));
  }
}

void ResFileScanWnd::OnClickTestBtn(TNotifyUI & msg, bool & handled)
{
	int test = 0;	// 添加测试语句
}
