#include "test_ctrl.h"

TestCtrl::TestCtrl()
	: ctrl_(nullptr)
{
}

TestCtrl::~TestCtrl()
{
}

PDUI_CONTROL TestCtrl::GetCtol(CDialogBuilder &builder)
{
	if (!ctrl_) {
		ctrl_ = builder.Create();
	}

	return ctrl_;
}
