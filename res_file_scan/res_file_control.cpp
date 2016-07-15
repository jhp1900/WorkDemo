#include "res_file_control.h"
#include <Shlwapi.h>


ResFileControl::ResFileControl(LPCTSTR file_path)
  : file_path_(file_path)
{
  builder_.Create(_T("res_file_control.xml"));
}


ResFileControl::~ResFileControl()
{
}

void ResFileControl::Reset(LPCTSTR file_path)
{
  file_path_ = file_path;

}

PDUI_CONTAINER ResFileControl::CreateCtrlIter(LPCTSTR file_name)
{
  PDUI_CONTAINER ret = static_cast<PDUI_CONTAINER>(builder_.Create());
  if(ret)
    InitCtrlIter(ret, file_name);
  return ret;
}

void ResFileControl::InitCtrlIter(PDUI_CONTAINER ctrl, LPCTSTR file_name)
{
  ctrl->SetMaxHeight(90);
  ctrl->SetMaxWidth(100);
  ctrl->SetMinHeight(90);
  ctrl->SetMinWidth(100);
  ctrl->FindSubControl(_T("file_name"))->SetText(file_name);
  ctrl->FindSubControl(_T("file_view"))->SetBkImage(GetProperIcon(file_name));
  ctrl->SetToolTip(file_name);
}

LPCTSTR ResFileControl::GetProperIcon(LPCTSTR file_name)
{
  vector<LPCTSTR> picture_type = {
    _T(".png"),
    _T(".jpg"),
    _T(".jpeg"),
    _T(".bmp"),
    _T(".ico")
  };

  vector<LPCTSTR> video_type = {
    _T(".avi"),
    _T(".wmv"),
    _T(".mp4"),
    _T(".mov"),
    _T(".mkv"),
    _T(".flv"),
    _T(".f4v"),
    _T(".m4v"),
    _T(".rmvb"),
    _T(".rm"),
    _T(".3gp"),
    _T(".dat"),
    _T(".ts"),
    _T(".mts"),
    _T(".vob")
  };

  vector<LPCTSTR> music_type = {
    _T(".mp3"),
    _T(".wav"),
    _T(".wma"),
    _T(".ogg"),
    _T(".ape"),
    _T(".acc")
  };

  LPCTSTR ex_name = ::PathFindExtension(file_name);

  for (auto iter : picture_type) {
    if (!lstrcmpi(iter, ex_name))
      return _T("res\\picture_ico.png");
  }

  for (auto iter : video_type) {
    if (!lstrcmpi(iter, ex_name))
      return _T("res\\video_ico.png");
  }

  for (auto iter : music_type) {
    if (!lstrcmpi(iter, ex_name))
      return _T("res\\music_ico.png");
  }

  return _T("res\\default_ico.png");
}
