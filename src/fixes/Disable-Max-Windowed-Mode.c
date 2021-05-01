#include <windows.h>
#include <stdbool.h>
#include "dune2000.h"
#include "ini.h"

typedef int (__stdcall *SetAppCompatData_)(int index, int data);

bool DisableMaxWindowedMode = false;

void SetDisableMaxWindowedMode()
{
    DisableMaxWindowedMode = D2kIniGetBool("Options", "DisableMaxWindowedMode", true);
    if (DisableMaxWindowedMode)
    {
        HMODULE hModule = LoadLibraryA("system32\\ddraw.dll");
        if (hModule)
        {
            SetAppCompatData_ setAppCompatData = (SetAppCompatData_)GetProcAddress(hModule, "SetAppCompatData");
            if (setAppCompatData) setAppCompatData(12, 0);
        }
    }
}
