#pragma once
#include "view_base.h"

class TestCtrl : public CListContainerElementUI
{
public:
	TestCtrl();
	~TestCtrl();

public:
	PDUI_CONTROL GetCtol(CDialogBuilder &builder);

private:
	PDUI_CONTROL ctrl_;
};

