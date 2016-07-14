#pragma once
#include "stdafx.h"

using std::vector;

class TraversingResource
{
public:
	TraversingResource();
	~TraversingResource();

public:
  void GetResFiles(LPCTSTR folder_path, const vector<LPCTSTR> &ex_names, vector<TCHAR *> &files);

private:
	void FirstFile(LPCTSTR file_name);		// ��һ���ļ� 
	void NextFile();		// ��һ���ļ�
	BOOL IsOK() const;		// �Ƿ���Բ�����һ��
	const WIN32_FIND_DATA &GetCurFile();	// �����ļ�����

protected:
	WIN32_FIND_DATA find_file_data_; // ��ǰ�ļ�����
	HANDLE find_handle_;	// ��ǰ����λ�þ��
	BOOL find_found_;		// ��ǰ�����Ƿ�ɹ�
};

