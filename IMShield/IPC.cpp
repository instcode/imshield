#include <windows.h>
#include "System.h"
#include "IPC.h"

LPBYTE g_lpMapAddress = NULL;
HANDLE hMapFile = NULL;

HANDLE GetEvent(LPCTSTR lpszEvent)
{
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, lpszEvent);
	if (hEvent == NULL)
		hEvent = CreateEvent(NULL, TRUE, FALSE, lpszEvent);
	return hEvent;
}

BOOL CloseEvent(LPCTSTR lpszEvent)
{
	HANDLE hEvent = GetEvent(lpszEvent);
	return ::CloseHandle(hEvent);
}

BOOL ResetEvent(LPCTSTR lpszEvent)
{
	HANDLE hEvent = GetEvent(lpszEvent);
	return ::ResetEvent(hEvent);
}

BOOL SignalEvent(LPCTSTR lpszEvent)
{
	HANDLE hEvent = GetEvent(lpszEvent);
	return ::SetEvent(hEvent);
}

DWORD WaitForEvents(UINT nCount, LPCTSTR* lpszEvents, BOOL isWaitAll, DWORD dwMiliseconds)
{
	HANDLE* phEvents = new HANDLE[nCount];
	for (UINT i = 0; i < nCount; i++)
	{
		phEvents[i] = GetEvent(lpszEvents[i]);
	}
	HRESULT hResult = WaitForMultipleObjects(nCount, phEvents, isWaitAll, dwMiliseconds);
	delete [] phEvents;
	return hResult;
}

DWORD WaitForEvent(LPCTSTR lpszEvent, DWORD dwMiliseconds)
{
	HANDLE hEvent = GetEvent(lpszEvent);
	return WaitForSingleObject(hEvent, dwMiliseconds);
}

DWORD OpenIPC(LPCSTR lpszMappingFile, DWORD dwMappingSize)
{
	DWORD dwDescriptor = 0;
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,    // read/write permission
		FALSE,                  // Do not inherit the name
		MAPPING_FILE_NAME       // of the mapping object.
	);
	if (hMapFile == NULL)
	{
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // current file handle
			NULL,                    // default security
			PAGE_READWRITE,          // read/write permission
			0,                       // max. object size
			dwMappingSize,           // size of hFile
			lpszMappingFile          // name of mapping object
		);
	}
	if (hMapFile == NULL)
	{
		SysTrace("Could not create file mapping object.\n");
	}

	g_lpMapAddress = (LPBYTE)MapViewOfFile(
		hMapFile,                          // handle to mapping object 
		FILE_MAP_ALL_ACCESS,               // read/write permission 
		0,                                 // max. object size 
		0,                                 // size of hFile 
		0                                  // map entire file 
	);
	if (g_lpMapAddress == NULL) 
	{ 
		SysTrace("Could not map view of file...\n");
		return -1;
	}
	else
	{
		SysTrace("Create IPC successfully.\n");
	}

	return dwDescriptor;
}

DWORD ReadIPC(DWORD dwDescriptor, LPIPC_MESSAGE lpMessage)
{
	DWORD dwSize = ntohl(*((long*)(g_lpMapAddress)));
	if (lpMessage != NULL) {
		lpMessage->dwSize = dwSize;
		lpMessage->lpBuffer = (LPBYTE)(g_lpMapAddress + 4);
	}
	return dwSize;
}

DWORD ReadIPC2(DWORD dwDescriptor, LPIPC_MESSAGE lpMessage)
{
	DWORD dwSize = ntohl(*((long*)(g_lpMapAddress)));
	if (lpMessage != NULL) {
		lpMessage->dwSize = dwSize;
		memcpy(lpMessage->lpBuffer, (LPBYTE)(g_lpMapAddress + 4), lpMessage->dwSize);
	}
	return dwSize;
}

DWORD WriteIPC(DWORD dwDescriptor, LPIPC_MESSAGE lpMessage)
{
	GetByteArrayFromInt(lpMessage->dwSize, g_lpMapAddress);
	memcpy((LPBYTE)(g_lpMapAddress + 4), lpMessage->lpBuffer, lpMessage->dwSize);
	return TRUE;
}

BOOL CloseIPC(DWORD dwDescriptor)
{
	SysTrace("Clean up the IPC...\n");
	if (g_lpMapAddress != NULL)
		UnmapViewOfFile(g_lpMapAddress);
	if (hMapFile != NULL)
		CloseHandle(hMapFile);
	return TRUE;
}
