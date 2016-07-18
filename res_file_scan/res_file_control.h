#pragma once
#include "stdafx.h"

typedef struct _FILE_TYPE {
  const vector<LPCTSTR> all_type;

  const vector<LPCTSTR> picture_type = {
    _T(".png"),
    _T(".jpg"),
    _T(".jpeg"),
    _T(".bmp"),
    _T(".ico")
  };

  const vector<LPCTSTR> video_type = {
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

  const vector<LPCTSTR> audio_type = {
    _T(".mp3"),
    _T(".wav"),
    _T(".wma"),
    _T(".ogg"),
    _T(".ape"),
    _T(".acc")
  };
} FILE_TYPE;

class ResFileControl : public CListContainerElementUI
{
public:
  ResFileControl(LPCTSTR file_path);
  ~ResFileControl();

public:
  void Reset(LPCTSTR file_path);
  PDUI_LISTCONTAINERELEM CreateCtrlIter(LPCTSTR file_name);
  void InitCtrlIter(PDUI_LISTCONTAINERELEM ctrl, LPCTSTR file_name);
  LPCTSTR GetProperIcon(LPCTSTR file_name);

public:
  FILE_TYPE file_type;

private:
  LPCTSTR file_path_;
  CDialogBuilder builder_;
};

