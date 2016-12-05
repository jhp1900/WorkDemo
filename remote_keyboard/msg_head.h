#pragma once
#include <Windows.h>

enum AppMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_WndSizeChangeMsg,		// Video 窗口大小发生变化
	kAM_ChildEscMsg,			// 子窗口 ESC 按键消息
	kAM_Update_Status,			// 刷新控件
	kAM_PopClickMsg,			// 设置弹出框中按钮的点击消息
	kAM_PTZ,					// ptz_wnd 界面的点击消息
	kAM_ResetServeIP,			// 重置服务端Ip
	kAM_ResetBackStreamsIP,		// 重置背景流IP
};