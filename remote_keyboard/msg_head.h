#pragma once
#include <Windows.h>

enum AppMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// ������ʼ����������
	kAM_WndSizeChangeMsg,		// Video ���ڴ�С�����仯
	kAM_ChildEscMsg,			// �Ӵ��� ESC ������Ϣ
	kAM_Update_Status,			// ˢ�¿ؼ�
	kAM_PopVKMsg,				// ���õ������У��������ⰴť����Ϣ
};