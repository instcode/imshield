#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <stdlib.h>

#include "HookingAPI.h"

BOOL WINAPI HookAPI(HANDLE hModule, PROC pfnHookProc, PROC pfnRealProc)
{
	#define MakePtr(cast, ptr, addValue) (cast)((DWORD)(ptr)+(DWORD)(addValue))

	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;

	if (IsBadReadPtr(hModule, sizeof(PIMAGE_NT_HEADERS)))
		return FALSE;

	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return FALSE;

	pNTHeader = MakePtr(PIMAGE_NT_HEADERS, pDOSHeader, pDOSHeader->e_lfanew);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE )
		return FALSE;

	pImportDesc = MakePtr(PIMAGE_IMPORT_DESCRIPTOR, hModule, 
		pNTHeader->OptionalHeader.
		DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].
		VirtualAddress);

	// Bail out if the RVA of the imports section is 0 (it doesn't exist)
	if (pImportDesc == (PIMAGE_IMPORT_DESCRIPTOR)pNTHeader)
		return FALSE;

	while (pImportDesc->Name)
	{
		PIMAGE_THUNK_DATA pThunk;
		pThunk = MakePtr(PIMAGE_THUNK_DATA, hModule, pImportDesc->FirstThunk);
		while (pThunk->u1.Function)
		{
			PROC* ppfn = (PROC*) &pThunk->u1.Function;
			if (*ppfn == pfnRealProc) 
			{
				MEMORY_BASIC_INFORMATION mbi;
				::VirtualQuery(ppfn, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
				// In order to provide writable access to this part of the 
				// memory we need to change the memory protection
				if (!::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect))
					break;
				// Hook the function.
				*ppfn = pfnHookProc;
				// Restore the protection back
				DWORD dwOldProtect;
				::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
				//SysTrace("\t\tHook successfully!\n", 0);
				return TRUE;
			}
			pThunk++;
		}

		pImportDesc++;
	}
	return FALSE;
}

BOOL WINAPI HookAPI(LPCSTR pszModuleName, LPCSTR pszFuncName, PROC pfnHookProc, PROC *pfnNextHookProc, DWORD dwFlags)
{
	// Get the function to be hook
	// The library may not be loaded, so we load it...
	HMODULE hModule = ::LoadLibrary(pszModuleName);
	*pfnNextHookProc = NULL;
	if (hModule != NULL)
		*pfnNextHookProc = GetProcAddress(hModule, pszFuncName);
	if (*pfnNextHookProc == NULL)
		return FALSE;

	//SysTrace("Address of '%s' function in module '%s' is at (0x%08X)\n", pszFuncName, pszModuleName, pfnNextHookProc);

	// Now replace in all the module of the current process
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return FALSE;
	}
	do
	{
		/*
		TCHAR szModName[_MAX_PATH];
		if (GetModuleFileName(me32.hModule, szModName, sizeof(szModName)/sizeof(TCHAR)))
			SysTrace(TEXT("%s (0x%08X)\n"), szModName, me32.hModule);
		//*/
		HookAPI(me32.hModule, pfnHookProc, *pfnNextHookProc);
	}
	while (Module32Next(hModuleSnap, &me32));
	CloseHandle(hModuleSnap);
	return TRUE;
}

BOOL WINAPI UnHookAPI(HMODULE hFromModule, LPCSTR pszFunctionModule, LPCSTR pszFunctionName, PROC pfnOldProc)
{
	return NULL;
}

BOOL WINAPI InjectLibrary(DWORD dwPID, LPCSTR lpszLibrary)
{
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwPID);	

	if (hProcess == NULL)
		return FALSE;
	
	HANDLE	hThread;
	LPVOID	lpLibraryParam = NULL;
	HMODULE hKernel32 = ::GetModuleHandle("kernel32.dll");
	DWORD	dwSize = (DWORD)_tcslen(lpszLibrary);

	lpLibraryParam = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (lpLibraryParam == NULL)
		return CloseHandle(hProcess);

	::WriteProcessMemory(hProcess, lpLibraryParam, (LPCVOID)lpszLibrary, dwSize, NULL);
	hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)::GetProcAddress(hKernel32, "LoadLibraryA"), lpLibraryParam, 0, NULL);
	if (hThread != NULL)
	{
		::WaitForSingleObject(hThread, INFINITE);
		::CloseHandle(hThread);
	}
	return CloseHandle(hProcess);
}
