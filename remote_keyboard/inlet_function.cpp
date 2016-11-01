#include "frame_wnd.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	::CoInitialize(NULL);
	UNREFERENCED_PARAMETER(hPrevInstance);		// 用来消除编译警告,
	UNREFERENCED_PARAMETER(lpCmdLine);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\skin"));

	FrameWnd frame_wnd;
	frame_wnd.Create(NULL, _T("远程键盘"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	frame_wnd.CenterWindow();
	frame_wnd.ShowWindow(true);

	CPaintManagerUI::MessageLoop();
	::CoUninitialize();

	return 0;
}