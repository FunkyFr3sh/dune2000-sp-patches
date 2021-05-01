#include <windows.h>
#include <stdbool.h>
#include "dune2000.h"

typedef int (__stdcall *SetProcessAffinityMask_)(HANDLE hProcess, DWORD_PTR dwProcessAffinityMask);

bool SingleProcessorAffinity = true;

void SetSingleProcessorAffinity()
{
    if (SingleProcessorAffinity)
    {
        HMODULE hModule = LoadLibraryA("kernel32.dll");
        if (hModule)
        {
            SetProcessAffinityMask_ setProcessAffinityMask = (SetProcessAffinityMask_)GetProcAddress(hModule, "SetProcessAffinityMask");
            if (setProcessAffinityMask) setProcessAffinityMask(GetCurrentProcess(), 1);
        }
    }
}
