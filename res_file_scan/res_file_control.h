#pragma once
#include "stdafx.h"
class ResFileControl : public CContainerUI
{
public:
  ResFileControl(LPCTSTR file_path);
  ~ResFileControl();

public:
  void Reset(LPCTSTR file_path);
  PDUI_CONTAINER CreateCtrlIter(LPCTSTR file_name);
  void InitCtrlIter(PDUI_CONTAINER ctrl, LPCTSTR file_name);
  LPCTSTR GetProperIcon(LPCTSTR file_name);

private:
  LPCTSTR file_path_;
  CDialogBuilder builder_;
};

