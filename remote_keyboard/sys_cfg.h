#pragma once
#include "stdafx.h"

enum IPSourceType
{
	ServerIP,
	BackStreamsIP
};

class SysCfg
{
public:
	SysCfg();
	~SysCfg();

	bool LoadFile(const WCHAR * path = nullptr);
	bool SetPaht(const WCHAR * path = nullptr);
	bool GetIsLoad() { return is_load_; }

	char * WideToMulti(CDuiString wide, char * multi);
	CDuiString MultiToWide(string multi);

public:
	bool GetIpInfo(IPSourceType ip_source_type, CDuiString &ip, CDuiString &port);
	bool SetIpInfo(IPSourceType ip_source_type, LPCTSTR ip, LPCTSTR port);

private:
	bool SaveFile(const WCHAR * path = nullptr);

	pugi::xml_node GetRootNode();
	pugi::xml_node GetNode(LPCTSTR name);

private:
	pugi::xml_document doc_;
	std::wstring path_;
	bool is_load_;
};

