#include "traversing_resource.h"
#include <Shlwapi.h>

TraversingResource::TraversingResource()
{
	find_found_ = FALSE;
	find_handle_ = INVALID_HANDLE_VALUE;
	memset(&find_file_data_, 0, sizeof(find_file_data_));
}


TraversingResource::~TraversingResource()
{
	if (find_handle_ != INVALID_HANDLE_VALUE)
		::FindClose(find_handle_);
}

void TraversingResource::GetResFiles(LPCTSTR folder_path, const vector<LPCTSTR> *ex_names, vector<TCHAR *> &files, bool clean /* = true */)
{
  if (clean)
    files.clear();

  /* lambda 表达式： 如果是需求的文件类型，返回true */
  auto IsExName = [ex_names](LPCTSTR ex_name) -> bool {
    if (ex_names->empty())
      return true;
    for (auto iter : *ex_names) {
      if (!lstrcmp(ex_name, iter))
        return true;
    }
    return false;
  };

  for (FirstFile(folder_path); IsOK(); NextFile()) {
    LPCTSTR ex_name = ::PathFindExtension(GetCurFile().cFileName);
    if ( !(GetCurFile().dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  // 不是文件夹
      && IsExName(ex_name)) {     // 扩展名符合
      TCHAR *file_name = new TCHAR[MAX_PATH];
      lstrcpy(file_name, GetCurFile().cFileName);
      files.push_back(file_name);
    }
  }
}

void TraversingResource::FirstFile(LPCTSTR file_name)
{
  TCHAR file[MAX_PATH] = { 0 };
  lstrcpy(file, file_name);
  lstrcat(file, _T("\\*.*"));
	find_handle_ = ::FindFirstFile(file, &find_file_data_);
	if (find_handle_ != INVALID_HANDLE_VALUE)
		find_found_ = TRUE;
	else
		find_found_ = FALSE;
}

void TraversingResource::NextFile()
{
	find_found_ = ::FindNextFile(find_handle_, &find_file_data_);
}

BOOL TraversingResource::IsOK() const
{
	return find_found_;
}

const WIN32_FIND_DATA &TraversingResource::GetCurFile()
{
	return find_file_data_;
}