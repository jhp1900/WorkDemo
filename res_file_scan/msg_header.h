#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// ������ʼ����������
	kAM_LoginMsg,	// ��¼��Ϣ 
	kAM_LogonMsg,	// ע����Ϣ
  kAM_AddObjectMsg,   // ��Ӽ��������Ϣ 
  kAM_AddCardMsg,     // ��Ӽ��俨Ƭ��Ϣ
};