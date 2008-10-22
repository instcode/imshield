/********************************************************************
* IMShield
*
* Copyright(C) Aug, 2006 - Khoa Nguyen
*
* Protect you & your Instant Messenger from unexpected messages
*********************************************************************/
#define USING_DLL

#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include "IMShield.h"
#include "System.h"
#include "IPC.h"
#include "YMSGService.h"
#include "YMSGMessage.h"
#include "HookingAPI.h"

TCHAR lpszLibrary[MAX_PATH];
BOOL g_isAttached = FALSE;
BOOL g_isRunning = TRUE;
DWORD ipc_descriptor = 0;

const LPCTSTR YAHOO_MESSENGER_APPLICATION = "YahooMessenger.exe";

#define PROCESS_ATTACH_TIME_WAIT	1000
#define MAX_ENUM_ENTRY_COUNT		1024

typedef BOOL (WINAPI *PFN_ENUM_PROCESSES_EX_CALLBACK) (LPCTSTR pszProcessBaseName, DWORD dwProcessID);
BOOL WINAPI EnumProcessesEx(PFN_ENUM_PROCESSES_EX_CALLBACK pfnCallback);

BOOL WINAPI FindProcessCallback(LPCTSTR pszProcessName, DWORD dwProcessID)
{
	if (_tcsicmp(pszProcessName, YAHOO_MESSENGER_APPLICATION) == 0)
	{
		if (InjectLibrary(dwProcessID, lpszLibrary))
		{
			g_isAttached = TRUE;
			SysTrace("Injected into %s (pid %u)\n", pszProcessName, dwProcessID);
		}
		// Return FALSE indicates we want to break the enum loop
		return FALSE;
	}
	// Continue with next process...
	return TRUE;
}


BOOL WINAPI EnumProcessesEx(PFN_ENUM_PROCESSES_EX_CALLBACK pfnCallback)
{
	DWORD processIDs[MAX_ENUM_ENTRY_COUNT], cbNeeded;
	if (!EnumProcesses(processIDs, sizeof(DWORD)*MAX_ENUM_ENTRY_COUNT, &cbNeeded))
		return FALSE;

	DWORD dwCount = cbNeeded / sizeof(DWORD);
	for (INT i = dwCount-1; i >= 0; --i)
	{
		DWORD dwProcessID = processIDs[i];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
		if (hProcess != NULL)
		{
			HMODULE hModule;
			if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
			{
				TCHAR szModulePath[MAX_PATH] = "Unknown";
				GetModuleFileNameEx(hProcess, hModule, szModulePath, sizeof(szModulePath));
				GetLongPathName(szModulePath, szModulePath, MAX_PATH);
				LPTSTR pszProcessName = _tcsrchr(szModulePath, _T('\\'));
				if (pszProcessName == NULL)
					pszProcessName = szModulePath;
				else
					pszProcessName++;
				if (!(pfnCallback)(pszProcessName, dwProcessID))
					break;
			}
			CloseHandle(hProcess);
		}
	}
	return TRUE;
}

DWORD WINAPI AttachProcessServiceRoutine(LPVOID lpParam)
{
	const LPCTSTR szEvents[] = {PING_EVENT, PROCESS_DETACH_EVENT, STOP_MONITORING_EVENT};
	DWORD dwLastPingTime = GetTickCount();
	g_isAttached = (WaitForEvent(PING_EVENT, 0) == WAIT_OBJECT_0);
	while (g_isRunning)
	{
		if (g_isAttached)
		{
			DWORD dwResult = WaitForEvents(3, (LPCTSTR *)szEvents, FALSE, REQUEST_TIME_WAIT);
			switch (dwResult)
			{
			case WAIT_OBJECT_0:
				//SysTrace("Receive PING from remote process...\n");
				dwLastPingTime = GetTickCount();
				ResetEvent(PING_EVENT);
				SignalEvent(ALIVE_EVENT);
				break;
			case WAIT_OBJECT_0 + 1:
				g_isAttached = FALSE;
				WaitForEvent(STOP_MONITORING_EVENT, PROCESS_ATTACH_TIME_WAIT);
				SysTrace("The remote process is detached...\n");
				ResetEvent(IM_SERVICE_EVENT);
				break;
			case WAIT_OBJECT_0 + 2:
			case WAIT_TIMEOUT:
				{
					DWORD dwCurrentTime = GetTickCount();
					if (dwCurrentTime - dwLastPingTime > 2*PING_TIME_WAIT)
					{
						g_isAttached = FALSE;
						SysTrace("No PING request! Remote thread is dead!...\n");
						ResetEvent(IM_SERVICE_EVENT);
					}
				}
				continue;
			}
		}
		else
		{
			//SysTrace("Find appropriate process to attach...\n");
			EnumProcessesEx((PFN_ENUM_PROCESSES_EX_CALLBACK)FindProcessCallback);
			WaitForEvent(STOP_MONITORING_EVENT, PROCESS_ATTACH_TIME_WAIT);
		}
	}
	SysTrace("AttachProcessServiceRoutine is exiting!...\n");
	return TRUE;
}

DWORD WINAPI MonitorServiceRoutine(LPVOID lpParam)
{
	YMSGService ymsgService;
	IPC_MESSAGE ipc_message;
	const LPCTSTR szEvents[] = {INCOMMING_BUFFER_AVAILABLE_EVENT, STOP_MONITORING_EVENT};

	while (g_isRunning)
	{
		// Wait for buffer available
		DWORD dwReturn = WaitForEvents(2, (LPCTSTR *)szEvents, FALSE, REQUEST_TIME_WAIT);
		if (dwReturn == WAIT_OBJECT_0)
		{
			ResetEvent(INCOMMING_BUFFER_AVAILABLE_EVENT);
			ReadIPC(ipc_descriptor, &ipc_message);
			ymsgService.HandleBuffer(ipc_message.lpBuffer, ipc_message.dwSize);
			SignalEvent(OUTGOING_BUFFER_AVAILABLE_EVENT);
		}
	}
	SysTrace("MonitorServiceRoutine is exiting!...\n");
	return TRUE;
}

BOOL WINAPI ConsoleHandler(DWORD dwEvent)
{
	switch (dwEvent)
	{
	case CTRL_CLOSE_EVENT:
		SysTrace("Program being closed!\n");
		SignalEvent(STOP_MONITORING_EVENT);
		break;
	case CTRL_LOGOFF_EVENT:
		SysTrace("User is logging off!\n");
		SignalEvent(STOP_MONITORING_EVENT);
		break;
	case CTRL_SHUTDOWN_EVENT:
		SysTrace("The machine is shutting down!\n");
		SignalEvent(STOP_MONITORING_EVENT);
		break;
	}
	return TRUE;
}

int main(INT argc, TCHAR *argv[])
{
	ResetEvent(STOP_MONITORING_EVENT);
	ResetEvent(OUTGOING_BUFFER_AVAILABLE_EVENT);
	ResetEvent(IM_SERVICE_EVENT);
	if (!GetModuleFileName(GetModuleHandle(NULL), lpszLibrary, MAX_PATH))
		return 0;
	_tcscpy(_tcsstr(lpszLibrary, ".exe"), ".dll");

	ipc_descriptor = OpenIPC(MAPPING_FILE_NAME, SHARE_MEM_SIZE);
	if (ipc_descriptor < 0)
		return 0;
	g_isRunning = TRUE;
	BeginThread(0, AttachProcessServiceRoutine);
	BeginThread(1, MonitorServiceRoutine);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE);
	WaitForEvent(STOP_MONITORING_EVENT, INFINITE);
	g_isRunning = FALSE;
	WaitForThreadsToExit();
	CloseIPC(ipc_descriptor);
	ResetEvent(STOP_MONITORING_EVENT);
	SysTrace("IMShield is exiting!...\n");
	return 0;
}