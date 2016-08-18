#pragma once
#include "stdafx.h"
#include "view_base.h"

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

class ResFileControl : public CListContainerElementUI, public ViewBase
{
public:
	ResFileControl() {}
	~ResFileControl() {}

	BEGIN_DUIMSG_MAP(ResFileControl)
		DUIMSG_HANDLER(WM_LBUTTONUP)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(ResFileControl)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_ITEMCLICK, OnClickCtrl)
	END_DUINOTIFY_MAP()

private:
	void OnClickCtrl(TNotifyUI &msg, bool &handled)
	{
		int a = 0;
	}

	LRESULT OnCursorLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		int a = 0; 
		return LRESULT();
	}
};

class CtrlManager
{
public:
	CtrlManager(LPCTSTR file_path);
	~CtrlManager();

public:
	void Reset(LPCTSTR file_path);
	ResFileControl * CreateCtrlIter(LPCTSTR file_name);

private:
	void InitCtrlIter(PDUI_LISTCONTAINERELEM ctrl, LPCTSTR file_name);
	LPCTSTR GetProperIcon(LPCTSTR file_name);

private:
	LPCTSTR file_path_;
	CDialogBuilder builder_;
	vector<ResFileControl*> res_vector_;
	FILE_TYPE file_type_;
};

