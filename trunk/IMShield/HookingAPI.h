#ifndef _HOOKING_API_H_
#define _HOOKING_API_H_

#ifndef USING_DLL
	#define HOOKING_API __declspec (dllexport)
#else
	#define HOOKING_API __declspec (dllimport)
#endif

BOOL WINAPI HookAPI(LPCSTR pszModule, LPCSTR pszFuncName, PROC pfnHookProc, PROC *pfnNextHookProc, DWORD dwFlags = 0);
BOOL WINAPI HookAPI(HMODULE hFromModule, LPCSTR pszFunctionModule, LPCSTR pszFunctionName, PROC pfnNewProc);
BOOL WINAPI UnHookAPI(HMODULE hFromModule, LPCSTR pszFunctionModule, LPCSTR pszFunctionName, PROC pfnOldProc);
BOOL WINAPI InjectLibrary(DWORD dwPID, LPCSTR lpszLibrary);

#endif