#include "FileManager.h"


FileManager::FileManager()
{
	
}


RecordingTime FileManager::getLastAccessTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return RecordingTime{ 0,0,0,0,false};
	
	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftAccess, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	return RecordingTime{
		stLocal.wHour,
		stLocal.wMinute,
		stLocal.wSecond,
		stLocal.wMilliseconds,
		true
	};

}

bool FileManager::setLastAccessTime(File file)
{
	HANDLE hFile;
	TCHAR szBuf[MAX_PATH];

	hFile = CreateFile(file.getPath().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with %d\n", GetLastError());
		return false;
	
	}
	else
	{
		std::cout << "CreateHandle success " << std::endl;
		RecordingTime rt = getLastAccessTime(hFile, szBuf, MAX_PATH);
		if (rt.init)
		{
			file.setTime(rt);
			std::cout << "Get time success \n" << std::endl;
		}
		

		std::cout << "CreateHandle success " << std::endl;
		return true;
	}
}


bool FileManager::addPathsToCam(CamPosition camPos, std::vector<std::string> paths)
{
	if (camPos == CamPosition::NORTH)
	{
		for (auto path : paths)
		{
			File f = File(path);
		}
		return true;
	}
	else if(camPos == CamPosition::SOUTH)
	{

		return true;
	}
	return false;
}

FileManager::~FileManager()
{
}
