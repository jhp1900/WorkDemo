#pragma once
#include <Windows.h>

enum AppMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// ������ʼ����������
	kAM_VideoWndSizeMsg,		// Video ���ڴ�С�����仯
};