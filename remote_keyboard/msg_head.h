#pragma once
#include <Windows.h>

enum AppMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_VideoWndSizeMsg,		// Video 窗口大小发生变化
};