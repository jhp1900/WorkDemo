#include "res_file_control.h"
#include <Shlwapi.h>


ResFileCtrl::ResFileCtrl(LPCTSTR file_path)
  : file_path_(file_path)
{
  builder_.Create(_T("res_file_control.xml"));
}


ResFileCtrl::~ResFileCtrl()
{
}

void ResFileCtrl::Reset(LPCTSTR file_path)
{
  file_path_ = file_path;
  ctrl_vector_.clear();
}

PDUI_LISTCONTAINERELEM ResFileCtrl::CreateCtrlIter(LPCTSTR file_name)
{
	PDUI_LISTCONTAINERELEM ret = static_cast<PDUI_LISTCONTAINERELEM>(builder_.Create());
	if (ret) {
		InitCtrlIter(ret, file_name);
		ctrl_vector_.push_back(ret);
	}

	return ret;
}

void ResFileCtrl::InitCtrlIter(PDUI_LISTCONTAINERELEM ctrl, LPCTSTR file_name)
{
  ctrl->SetMaxHeight(90);
  ctrl->SetMaxWidth(100);
  ctrl->SetMinHeight(90);
  ctrl->SetMinWidth(100);
  ctrl->FindSubControl(_T("file_name"))->SetText(file_name);
  ctrl->FindSubControl(_T("file_view"))->SetBkImage(GetProperIcon(file_name));
  ctrl->SetToolTip(file_name);
}

LPCTSTR ResFileCtrl::GetProperIcon(LPCTSTR file_name)
{
  LPCTSTR ex_name = ::PathFindExtension(file_name);

  for (auto iter : file_type.picture_type) {
    if (!lstrcmpi(iter, ex_name))
      return _T("res\\picture_ico.png");
  }

  for (auto iter : file_type.video_type) {
    if (!lstrcmpi(iter, ex_name))
      return _T("res\\video_ico.png");
  }

  for (auto iter : file_type.audio_type) {
    if (!lstrcmpi(iter, ex_name))
      return _T("res\\music_ico.png");
  }

  return _T("res\\default_ico.png");
}

void ResFileCtrl::SelectChange(PDUI_LISTCONTAINERELEM ctrl)
{
	for (auto iter : ctrl_vector_)
		iter->SetBkColor(0);
	ctrl->SetBkColor(0xffcce8ff);
	::MessageBox(nullptr, ctrl->FindSubControl(_T("file_name"))->GetText(), _T("ÎÄ¼þÃû"), MB_OK);
}
