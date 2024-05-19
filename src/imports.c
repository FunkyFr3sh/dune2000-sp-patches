#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "rules.h"
#include "utils.h"
#include "ini.h"


ENUMDISPLAYSETTINGSAPROC EnumDisplaySettingsAImp;
LOADCURSORFROMFILEAPROC LoadCursorFromFileAImp;
SETCURSORPROC SetCursorImp;
BOOL *GameHandlesClose;

void InitImports()
{
    HMODULE hModule = LoadLibraryA("ddraw.dll");
    if (hModule)
    {
        GameHandlesClose = (BOOL*)GetProcAddress(hModule, "GameHandlesClose");
        if (GameHandlesClose)
        {
            *GameHandlesClose = TRUE;
        }
    }
    
    hModule = LoadLibraryA("User32.dll");
    if (hModule)
    {
        EnumDisplaySettingsAImp = (void*)GetProcAddress(hModule, "EnumDisplaySettingsA");
        SetCursorImp = (void*)GetProcAddress(hModule, "SetCursor");
        LoadCursorFromFileAImp = (void*)GetProcAddress(hModule, "LoadCursorFromFileA");
    }
}
