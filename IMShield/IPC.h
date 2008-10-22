#ifndef IPC_H
#define IPC_H

#ifndef USING_DLL
	#define IPC_API __declspec (dllexport)
#else
	#define IPC_API __declspec (dllimport)
#endif

#define MAPPING_FILE_NAME	"IMShield.map"
#define SHARE_MEM_SIZE		10*1024

typedef struct
{
	DWORD dwSize;
	LPBYTE lpBuffer;
}
IPC_MESSAGE, *LPIPC_MESSAGE;

DWORD WaitForEvents(UINT nCount, LPCTSTR* lpszEvents, BOOL isWaitAll, DWORD dwMiliseconds);
DWORD WaitForEvent(LPCTSTR lpszEvent, DWORD dwMiliseconds);
BOOL CloseEvent(LPCTSTR lpszEvent);
BOOL ResetEvent(LPCTSTR lpszEvent);
BOOL SignalEvent(LPCTSTR lpszEvent);

DWORD OpenIPC(LPCSTR lpszMappingFile = MAPPING_FILE_NAME, DWORD dwMappingSize = SHARE_MEM_SIZE);
DWORD ReadIPC(DWORD dwDescriptor, LPIPC_MESSAGE lpMessage);
DWORD ReadIPC2(DWORD dwDescriptor, LPIPC_MESSAGE lpMessage);
DWORD WriteIPC(DWORD dwDescriptor, LPIPC_MESSAGE lpMessage);
BOOL CloseIPC(DWORD dwDescriptor);

#endif