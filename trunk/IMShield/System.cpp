#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "System.h"

#define MAX_THREAD_COUNT 3

HANDLE hThread[MAX_THREAD_COUNT];

BOOL BeginThread(DWORD dwID, LPTHREAD_START_ROUTINE lpStartAddress)
{
	if (dwID < MAX_THREAD_COUNT && hThread[dwID] == NULL)
	{
		DWORD dwThreadID;
		DWORD dwIM = 0;

		hThread[dwID] = CreateThread(
			NULL,			// default security attributes
			NULL,			// use default stack size
			lpStartAddress,	// thread function
			&dwIM,			// argument to thread function
			NULL,			// use default creation flags
			&dwThreadID		// returns the thread identifier
		);
		return TRUE;
	}
	return FALSE;
}

BOOL WaitForThreadsToExit()
{
	for (UINT i = 0; i < MAX_THREAD_COUNT; i++)
	{
		if (hThread[i] != NULL)
		{
			SysTrace("Wait for thread %d to exit...\n", hThread[i]);
			// Wait until the service thread has terminated.
			WaitForSingleObject(hThread[i], INFINITE);
			// Close all thread handles upon completion.
			CloseHandle(hThread[i]);
			SysTrace("Thread %d exited", hThread[i]);
		}
	}
	return TRUE;
}

void GetByteArrayFromShort(int inputShort, byte* outputBuffer)
{
	outputBuffer[0] = (byte)((inputShort >> 8) & 0xFF);
	outputBuffer[1] = (byte)(inputShort & 0xFF);
}

void GetByteArrayFromInt(int inputShort, byte* outputBuffer)
{
	outputBuffer[0] = (byte)((inputShort >> 24) & 0xFF);
	outputBuffer[1] = (byte)((inputShort >> 16) & 0xFF);
	outputBuffer[2] = (byte)((inputShort >> 8) & 0xFF);
	outputBuffer[3] = (byte)((inputShort & 0xFF));
}

void SysTrace(const char *szFormat, ...)
{
	char szMessage[1024];
	va_list vl;
	va_start(vl, szFormat);
	vsprintf(szMessage, szFormat, vl);
	va_end(vl);
#ifdef _DEBUG
	OutputDebugString(szMessage);
#else
	printf(szMessage);
#endif
}