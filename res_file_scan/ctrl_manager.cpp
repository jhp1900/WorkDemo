#include "ctrl_manager.h"
#include <Shlwapi.h>

CtrlManager::CtrlManager(LPCTSTR file_path)
	: file_path_(file_path)
{
	builder_.Create(_T("res_file_control.xml"));
}

CtrlManager::~CtrlManager()
{
}

void CtrlManager::Reset(LPCTSTR file_path)
{
	file_path_ = file_path;
}

ResFileControl * CtrlManager::CreateCtrlIter(LPCTSTR file_name)
{
	ResFileControl * ret = static_cast<ResFileControl *>(builder_.Create());
	if (ret)
		InitCtrlIter(ret, file_name);
	res_vector_.push_back(ret);
	return ret;
}

void CtrlManager::InitCtrlIter(PDUI_LISTCONTAINERELEM ctrl, LPCTSTR file_name)
{
	ctrl->SetMaxHeight(90);
	ctrl->SetMaxWidth(100);
	ctrl->SetMinHeight(90);
	ctrl->SetMinWidth(100);
	ctrl->FindSubControl(_T("file_name"))->SetText(file_name);
	ctrl->FindSubControl(_T("file_view"))->SetBkImage(GetProperIcon(file_name));
	ctrl->SetToolTip(file_name);
}

LPCTSTR CtrlManager::GetProperIcon(LPCTSTR file_name)
{
	LPCTSTR ex_name = ::PathFindExtension(file_name);

	for (auto iter : file_type_.picture_type) {
		if (!lstrcmpi(iter, ex_name))
			return _T("res\\picture_ico.png");
	}

	for (auto iter : file_type_.video_type) {
		if (!lstrcmpi(iter, ex_name))
			return _T("res\\video_ico.png");
	}

	for (auto iter : file_type_.audio_type) {
		if (!lstrcmpi(iter, ex_name))
			return _T("res\\music_ico.png");
	}

	return _T("res\\default_ico.png");
}
