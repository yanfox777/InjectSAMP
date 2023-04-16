#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

BOOL Inject(DWORD pId, LPSTR dllName) {
    HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
    if (h)
    {
        LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        LPVOID dereercomp = VirtualAllocEx(h, NULL, strlen(dllName), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(h, dereercomp, dllName, strlen(dllName), NULL);
        HANDLE asdc = CreateRemoteThread(h, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
        WaitForSingleObject(asdc, INFINITE);
        VirtualFreeEx(h, dereercomp, strlen(dllName), MEM_RELEASE);
        CloseHandle(asdc);
        CloseHandle(h);
        return TRUE;
    }
    return FALSE;
}
int main()
{
    STARTUPINFOA cif; ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    if (CreateProcessA("E:\\Games\\GTA Kalinovka\\kalinovka.exe", (LPSTR) "-c -n NickName -h 127.0.0.1 -p 7777", NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED, NULL, "E:\\Games\\GTA Kalinovka\\", &cif, &pi))
    {
        if (pi.hProcess != NULL)
        {
            if (!Inject(pi.dwProcessId, (LPSTR) "E:\\Games\\GTA Kalinovka\\samp.dll"))
            {
                TerminateProcess(pi.hProcess, 0);
                ExitProcess(0);
            }
            ResumeThread(pi.hThread);
        }
    }
    else MessageBoxA(NULL, "Failed to Create Process", "Error", MB_ICONERROR);
    return 0;
}

