#pragma once
#include <Windows.h>

enum AppMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// ������ʼ����������
	kAM_WndSizeChangeMsg,		// Video ���ڴ�С�����仯
	kAM_ChildEscMsg,			// �Ӵ��� ESC ������Ϣ
	kAM_Update_Status,			// ˢ�¿ؼ�
	kAM_PopClickMsg,			// ���õ������а�ť�ĵ����Ϣ
	kAM_PTZ,					// ptz_wnd ����ĵ����Ϣ
	kAM_ResetServeIP,			// ���÷����Ip
	kAM_ResetBackStreamsIP,		// ���ñ�����IP
};