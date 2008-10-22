#define INCL_WINSOCK_API_TYPEDEFS	1

#include <winsock2.h>
#include "HookingAPI.h"
#include "System.h"
#include "IMShield.h"
#include "IPC.h"

LPFN_WSARECV		fpWSARecv = NULL;
LPFN_WSASEND		fpWSASend = NULL;
LPFN_SEND			fpSend = NULL;
LPFN_CONNECT		fpConnect = NULL;
LPFN_CLOSESOCKET	fpCloseSocket = NULL;

SOCKET				g_socket = INVALID_SOCKET;
LPCRITICAL_SECTION	g_lpCriticalSection = NULL;
BOOL				g_isBeingMonitored = FALSE;

BOOL				g_isRunning = TRUE;
DWORD				ipc_descriptor = 0;

int WSAAPI CloseSocketHookProc(SOCKET s)
{
	SysTrace("CloseSocketHookProc - Global socket: %d - Closing socket: %d\n", g_socket, s);
	if (g_socket == s)
		g_socket = INVALID_SOCKET;
	return (fpCloseSocket)(s);
}

int WSAAPI ConnectHookProc(SOCKET s, const struct sockaddr* name, int namelen)
{
	SOCKADDR_IN* pSock = (SOCKADDR_IN*)name;
	char sIP[15];
	int nRemoteIP = pSock->sin_addr.s_addr;
	SysTrace("ConnectHookProc - Connected socket: %d - Remote Host: %d.%d.%d.%d\n", s,
		nRemoteIP & 0xFF, (nRemoteIP >> 8) & 0xFF, (nRemoteIP >> 16) & 0xFF, (nRemoteIP >> 24) & 0xFF);

	g_socket = s;
	return (fpConnect)(s, name, namelen);
}

int WSAAPI SendHookProc(SOCKET s, const char FAR* buf, int len, int flags)
{
	if (g_isBeingMonitored && len > 0) {
		EnterCriticalSection(g_lpCriticalSection);
		SysTrace("SendHookProc - Using socket: %d\n", s);

		IPC_MESSAGE ipc_message;
		ipc_message.lpBuffer = (LPBYTE)buf;
		ipc_message.dwSize = len;
		
		WriteIPC(ipc_descriptor, &ipc_message);
		SignalEvent(INCOMMING_BUFFER_AVAILABLE_EVENT);

		// Wait for buffer available
		const LPCTSTR szEvents[] = {OUTGOING_BUFFER_AVAILABLE_EVENT, STOP_MONITORING_EVENT};
		DWORD dwReturn = WaitForEvents(
			2, (LPCTSTR *)szEvents, FALSE, REQUEST_TIME_WAIT);
		if (dwReturn == WAIT_OBJECT_0)
		{
			ResetEvent(OUTGOING_BUFFER_AVAILABLE_EVENT);
			ReadIPC(ipc_descriptor, &ipc_message);
			buf = (const char*)ipc_message.lpBuffer;
			len = ipc_message.dwSize;
		}
		// We must complete send data within critical section,
		// otherwise, it may be intercepted by another thread
		// and therefore, data will be corrupted.
		int nRet = (fpSend)(s, buf, len, flags);
		LeaveCriticalSection(g_lpCriticalSection);
		return nRet;
	}
	return (fpSend)(s, buf, len, flags);
}

int WSAAPI WSARecvHookProc(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int nRetCode = (fpWSARecv)(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	if (nRetCode != 0)
		return nRetCode;

	if (g_isBeingMonitored && *lpNumberOfBytesRecvd > 0) {
		EnterCriticalSection(g_lpCriticalSection);
		SysTrace("WSARecvHookProc - Receive count: %d\n", *lpNumberOfBytesRecvd);
		
		IPC_MESSAGE ipc_message;
		ipc_message.lpBuffer = (LPBYTE)lpBuffers->buf;
		ipc_message.dwSize = *lpNumberOfBytesRecvd;

		WriteIPC(ipc_descriptor, &ipc_message);
		SignalEvent(INCOMMING_BUFFER_AVAILABLE_EVENT);
		const LPCTSTR szEvents[] = {OUTGOING_BUFFER_AVAILABLE_EVENT, STOP_MONITORING_EVENT};
		DWORD dwReturn = WaitForEvents(
			2, (LPCTSTR *)szEvents, FALSE, REQUEST_TIME_WAIT);
		if (dwReturn == WAIT_OBJECT_0)
		{
			ResetEvent(OUTGOING_BUFFER_AVAILABLE_EVENT);
			// Take the modified buffer effect
			ReadIPC(ipc_descriptor, &ipc_message);
			lpBuffers->buf = (char *)ipc_message.lpBuffer;
			*lpNumberOfBytesRecvd = ipc_message.dwSize;
		}
		LeaveCriticalSection(g_lpCriticalSection);
	}
	return nRetCode;
}

DWORD WINAPI IMServiceRoutine(LPVOID lpParam) 
{
	while (g_isRunning)
	{
		const LPCTSTR szEvents[] = {IM_SERVICE_EVENT, PROCESS_DETACH_EVENT};
		DWORD dwReturn = WaitForEvents(
			2, (LPCTSTR *)szEvents, FALSE, REQUEST_TIME_WAIT);

		if (dwReturn != WAIT_OBJECT_0)
			continue;
		ResetEvent(IM_SERVICE_EVENT);

		// Make sure no one can modify the IPC buffer concurrently
		EnterCriticalSection(g_lpCriticalSection);

		// Get the data & serve
		IPC_MESSAGE ipc_message;
		ReadIPC(ipc_descriptor, &ipc_message);

		// Check whether we have a valid socket, and send the buffer
		// Note: Use the buffer of the IPC directly for fast processing
		if (g_socket != INVALID_SOCKET) {
			SysTrace("Service: Send request...\n");
			(fpSend)(g_socket, (const char*)ipc_message.lpBuffer, ipc_message.dwSize, 0);
		}
		// It's time to leave ;)
		LeaveCriticalSection(g_lpCriticalSection);
	}
	SysTrace("IMServiceRoutine is exiting!...\n");
	return 0; 
} 

DWORD WINAPI PingServiceRoutine(LPVOID lpParam) 
{
	const LPCTSTR PING[]	= {ALIVE_EVENT, STOP_MONITORING_EVENT, PROCESS_DETACH_EVENT};
	const LPCTSTR SLEEP[]	= {PROCESS_DETACH_EVENT, STOP_MONITORING_EVENT};
	const LPCTSTR WAIT[]	= {ALIVE_EVENT, PROCESS_DETACH_EVENT};

	DWORD		dwTimeWait = REQUEST_TIME_WAIT;
	BOOL		isSignalPingEvent = TRUE;
	LPCTSTR*	lpszEvents = (LPCTSTR *)PING;
	
	while (g_isRunning)
	{
		if (isSignalPingEvent)
		{
			//SysTrace("Send PING signal!...\n");
			SignalEvent(PING_EVENT);
		}
		
		DWORD dwReturn = WaitForEvents(3, lpszEvents, FALSE, dwTimeWait);
		switch (dwReturn)
		{
		case WAIT_OBJECT_0:
			ResetEvent(ALIVE_EVENT);
			g_isBeingMonitored = TRUE;
			//SysTrace("Supervisor is monitoring!...\n");
			dwTimeWait = PING_TIME_WAIT;
			lpszEvents = (LPCTSTR *)SLEEP;
			break;
		case WAIT_TIMEOUT:
			if (!isSignalPingEvent)
			{
				dwTimeWait = REQUEST_TIME_WAIT;
				lpszEvents = (LPCTSTR *)PING;
				break;
			}
			// Cross over!!
		case WAIT_OBJECT_0 + 1:
			g_isBeingMonitored = FALSE;
			SysTrace("Supervisor is no longer existed!...\n");
			dwTimeWait = INFINITE;
			lpszEvents = (LPCTSTR *)WAIT;
			break;
		}
		isSignalPingEvent = (dwReturn == WAIT_TIMEOUT) || (dwReturn == WAIT_OBJECT_0 + 1);
	}
	SysTrace("PingServiceRoutine is exiting!...\n");
	return TRUE;
}

BOOL BeginMonitorIM(HINSTANCE hInstance)
{
	// Initialize IM Shield
	g_lpCriticalSection = new CRITICAL_SECTION;
	InitializeCriticalSection(g_lpCriticalSection);

	ipc_descriptor = OpenIPC(MAPPING_FILE_NAME, SHARE_MEM_SIZE);
	if (ipc_descriptor < 0)
		return FALSE;
	
	ResetEvent(PROCESS_DETACH_EVENT);
	ResetEvent(INCOMMING_BUFFER_AVAILABLE_EVENT);
	g_isRunning = TRUE;

	BeginThread(0, PingServiceRoutine);
	BeginThread(1, IMServiceRoutine);

	// Hook APIs
	HookAPI("ws2_32.dll", "WSARecv", (PROC)WSARecvHookProc, (PROC*)&fpWSARecv);
	HookAPI("ws2_32.dll", "connect", (PROC)ConnectHookProc, (PROC*)&fpConnect);
	HookAPI("ws2_32.dll", "closesocket", (PROC)CloseSocketHookProc, (PROC*)&fpCloseSocket);
	HookAPI("ws2_32.dll", "send", (PROC)SendHookProc, (PROC*)&fpSend);

	SysTrace("Begin monitoring IM...\n");
	return TRUE;
}

BOOL EndMonitorIM()
{
	g_isRunning = FALSE;
	SignalEvent(PROCESS_DETACH_EVENT);
	WaitForThreadsToExit();
	CloseIPC(ipc_descriptor);
	DeleteCriticalSection(g_lpCriticalSection);

	delete g_lpCriticalSection;
	SysTrace("Stop monitoring IM...\n");

	ResetEvent(PROCESS_DETACH_EVENT);
	ResetEvent(INCOMMING_BUFFER_AVAILABLE_EVENT);
	return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) 
	{
	case DLL_PROCESS_ATTACH:
		BeginMonitorIM(hInstance);
		break;
	case DLL_PROCESS_DETACH:
		EndMonitorIM();
		break;
	}
	return TRUE;
}
