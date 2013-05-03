//////////////////////////////////////////////////////////////////////////
// 程序管理器ExeMgr.exe V1.0
// 功能：启动被管理程序，保证被管理程序在多个用户级下只能启动一次；关闭被管理程序；
// author: cgw
// 2013-05-04
//////////////////////////////////////////////////////////////////////////
// ExeMgr.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include "Psapi.h"
#include <fstream>
#include <iostream>
using namespace std;
#pragma comment(lib, "Psapi.lib")

const static std::string g_strExeName = "Analyse.exe";		// 被管理程序进程名
const static std::string g_strExePath = "E:\\VCWorks\\Analyse\\Debug\\Analyse.exe";		// 被管理程序路径

BOOL findProcess(const string &strProcessName, HANDLE &hProcess);
VOID OpenExe(const string &strPath);
VOID CloseExe(const HANDLE &hProcess);
VOID WriteLog(LPCSTR format, ...);

int _tmain(int argc, _TCHAR* argv[])
{
	// 根据输入命令行参数start or close控制被管理程序的状态
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	if (argc > 2)
	{
		// 出错
		WriteLog("命令行参数出错。");
		return 0;
	}
	else if (argc == 1)
	{
		// 无命令行参数，默认为start
		// 扫描进程，看被管理程序是否已经存在
		// Get the list of process identifiers.
		if (findProcess(g_strExeName, hProcess))
		{
			// 找到进程，被管理程序已经开启
			WriteLog("%s已经被开启", g_strExeName.c_str());
			return 0;
		} 
		else
		{
			// 未找到进程，被管理程序未开启
			OpenExe(g_strExePath);
			return 0;
		}
	}
	else if (argc == 2)
	{
		// 两个参数：argv[0]为被管理exe，argv[1]为start or close
		std::string strArgv(argv[1]);
		if (stricmp(strArgv.c_str(), "start") == 0)
		{
			// start
			// 扫描进程，看被管理程序是否已经存在
			if (findProcess(g_strExeName, hProcess))
			{
				// 找到进程，被管理程序已经开启
				WriteLog("%s已经被开启", g_strExeName.c_str());
				return 0;
			} 
			else
			{
				// 未找到进程，被管理程序未开启
				OpenExe(g_strExePath);
				return 0;
			}
		} 
		else if (stricmp(strArgv.c_str(), "close") == 0)
		{
			// close
			if (findProcess(g_strExeName, hProcess))
			{
				// 找到进程，被管理程序已经开启
				CloseExe(hProcess);
				return 0;
			} 
			else
			{
				// 未找到进程，被管理程序未开启
				WriteLog("未找到进程，不用关闭。");
				return 0;
			}
		}
		else
		{
			// 命令行参数错误
			WriteLog("命令行参数出错。");
			return 0;
		}
		
	}

	system("pause");
	return 0;
}

// 扫描进程，查找被管理程序进程是否存在
BOOL findProcess(const string &strProcessName, HANDLE &hProcess)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return FALSE;

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.
	for ( i = 0; i < cProcesses; i++ )
	{
		if( aProcesses[i] != 0 )
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

			// Get a handle to the process.
			hProcess = OpenProcess( PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ,
				FALSE, aProcesses[i] );

			// Get the process name.
			if (NULL != hProcess )
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
					&cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, 
						sizeof(szProcessName)/sizeof(TCHAR) );
				}
			}

			// Print the process name and identifier.
			_tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, aProcesses[i] );

			std::string strProcess(szProcessName);
			int nRet = stricmp(strProcess.c_str(), strProcessName.c_str());
			if (nRet == 0)
			{
				// 找到进程
				return TRUE;
			}
		}
		
	}	

	return FALSE;
}
// 开启程序
VOID OpenExe(const string &strPath)
{
	int nRet = (int)ShellExecute(NULL, "open", strPath.c_str(), NULL, NULL, SW_SHOW);
	if (nRet > 32)
	{
		WriteLog("%s开启成功", strPath.c_str());
	} 
	else
	{
		WriteLog("%s开启失败， 错误码：%d", strPath.c_str(), nRet);
	}
}
// 关闭程序
VOID CloseExe(const HANDLE &hProcess)
{
	if (TerminateProcess(hProcess, 0))
	{
		// 关闭成功
		WriteLog("关闭成功。");
	} 
	else
	{
		// 关闭失败
		int nErr = GetLastError();
		WriteLog("关闭失败， 错误码：%d", nErr);
	}
	
}

// 写log
void WriteLog(LPCSTR format, ...)
{
	char text[1024];
	SYSTEMTIME s;
	GetLocalTime(&s);
	sprintf(text, "%d-%d-%d %d:%d:%d\t", s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wSecond);
	va_list header;
	va_start(header, format);
	_vsnprintf(text + strlen(text), 1024, format, header);
	va_end(header);
	std::ofstream ofs("log.txt", std::ios_base::app);
	ofs << text << std::endl;
}