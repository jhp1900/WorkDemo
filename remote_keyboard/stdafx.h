// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <objbase.h>
#include "COMUTIL.H"
#include "duilib\UIlib.h"
#include "duilib\Utils\UIWnd.h"
#include "duilib\Utils\UICrack.h"

#define PUGIXML_WCHAR_MODE
#include "pugixml\pugixml.hpp"

using namespace DuiLib;

#ifdef NDEBUG
#	pragma comment(lib, "duilib-sl.lib")
#else
#	pragma comment(lib, "duilib_sl-d.lib")
#endif

#define MAX max
#define MIN min