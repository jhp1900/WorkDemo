#pragma once
#include <Windows.h>

enum AppMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// ������ʼ����������
	kAM_WndSizeChangeMsg,		// Video ���ڴ�С�����仯
	kAM_ChildEscMsg,			// �Ӵ��� ESC ������Ϣ
	WM_UPDATE_STATUS,			// ˢ�¿ؼ�
};