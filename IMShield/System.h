#ifndef _THREAD_H_
#define _THREAD_H_

BOOL BeginThread(DWORD dwID, LPTHREAD_START_ROUTINE lpStartAddress);
BOOL WaitForThreadsToExit();

void SysTrace(const char *szFormat, ...);
void GetByteArrayFromShort(int inputShort, byte* outputBuffer);
void GetByteArrayFromInt(int inputShort, byte* outputBuffer);

#endif