//////////////////////////////////////////////////////////////////////////
// ���������ExeMgr.exe V1.0
// ���ܣ�������������򣬱�֤����������ڶ���û�����ֻ������һ�Σ��رձ��������
// author: cgw
// 2013-05-04
//////////////////////////////////////////////////////////////////////////
// ExeMgr.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "windows.h"
#include "Psapi.h"
#include <fstream>
#include <iostream>
using namespace std;
#pragma comment(lib, "Psapi.lib")

const static std::string g_strExeName = "Analyse.exe";		// ��������������
const static std::string g_strExePath = "E:\\VCWorks\\Analyse\\Debug\\Analyse.exe";		// ���������·��

BOOL findProcess(const string &strProcessName, HANDLE &hProcess);
VOID OpenExe(const string &strPath);
VOID CloseExe(const HANDLE &hProcess);
VOID WriteLog(LPCSTR format, ...);

int _tmain(int argc, _TCHAR* argv[])
{
	// �������������в���start or close���Ʊ���������״̬
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	if (argc > 2)
	{
		// ����
		WriteLog("�����в�������");
		return 0;
	}
	else if (argc == 1)
	{
		// �������в�����Ĭ��Ϊstart
		// ɨ����̣�������������Ƿ��Ѿ�����
		// Get the list of process identifiers.
		if (findProcess(g_strExeName, hProcess))
		{
			// �ҵ����̣�����������Ѿ�����
			WriteLog("%s�Ѿ�������", g_strExeName.c_str());
			return 0;
		} 
		else
		{
			// δ�ҵ����̣����������δ����
			OpenExe(g_strExePath);
			return 0;
		}
	}
	else if (argc == 2)
	{
		// ����������argv[0]Ϊ������exe��argv[1]Ϊstart or close
		std::string strArgv(argv[1]);
		if (stricmp(strArgv.c_str(), "start") == 0)
		{
			// start
			// ɨ����̣�������������Ƿ��Ѿ�����
			if (findProcess(g_strExeName, hProcess))
			{
				// �ҵ����̣�����������Ѿ�����
				WriteLog("%s�Ѿ�������", g_strExeName.c_str());
				return 0;
			} 
			else
			{
				// δ�ҵ����̣����������δ����
				OpenExe(g_strExePath);
				return 0;
			}
		} 
		else if (stricmp(strArgv.c_str(), "close") == 0)
		{
			// close
			if (findProcess(g_strExeName, hProcess))
			{
				// �ҵ����̣�����������Ѿ�����
				CloseExe(hProcess);
				return 0;
			} 
			else
			{
				// δ�ҵ����̣����������δ����
				WriteLog("δ�ҵ����̣����ùرա�");
				return 0;
			}
		}
		else
		{
			// �����в�������
			WriteLog("�����в�������");
			return 0;
		}
		
	}

	system("pause");
	return 0;
}

// ɨ����̣����ұ������������Ƿ����
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
				// �ҵ�����
				return TRUE;
			}
		}
		
	}	

	return FALSE;
}
// ��������
VOID OpenExe(const string &strPath)
{
	int nRet = (int)ShellExecute(NULL, "open", strPath.c_str(), NULL, NULL, SW_SHOW);
	if (nRet > 32)
	{
		WriteLog("%s�����ɹ�", strPath.c_str());
	} 
	else
	{
		WriteLog("%s����ʧ�ܣ� �����룺%d", strPath.c_str(), nRet);
	}
}
// �رճ���
VOID CloseExe(const HANDLE &hProcess)
{
	if (TerminateProcess(hProcess, 0))
	{
		// �رճɹ�
		WriteLog("�رճɹ���");
	} 
	else
	{
		// �ر�ʧ��
		int nErr = GetLastError();
		WriteLog("�ر�ʧ�ܣ� �����룺%d", nErr);
	}
	
}

// дlog
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