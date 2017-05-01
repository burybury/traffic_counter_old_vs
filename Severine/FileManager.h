//stl
#include <iostream>
#include <vector>
//winapi
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
//local
#include "File.h"

#pragma once
class FileManager
{
private:
	RecordingTime getLastAccessTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize);
	bool setLastAccessTime(File file);
public:
	FileManager();
	std::vector<File> mFilesCamN;
	std::vector<File> mFilesCamS;
	enum CamPosition {
		NORTH = 1,
		SOUTH = 2,
		UNKNOWN = 0
	};
	bool addPathsToCam(CamPosition camPos,std::vector<std::string> paths);
	~FileManager();
};

