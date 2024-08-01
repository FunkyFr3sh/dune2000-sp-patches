#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

//Hardcode the error messages to avoid problems with missing failure.txt file


DETOUR(0x004A3120, 0x004A31C6, _FakeFailureTxtGetLine);
char *FakeFailureTxtGetLine(int lineNumber)
{
    switch(lineNumber)
    {
        case 0: return "Dune 2000 requires a 256 colour desktop to run.";
        case 1: return "This video card does not support the required modes. Please open the Dune 2000 config program and change your resolution. Windows 8/10: Please enable the 'Reduced Color Mode: 16bit' Compatibility setting for dune2000.exe and dune2000-spawn.exe.";
        case 2: return "Insufficient RAM and/or Virtual Memory. Check Virtual Memory settings and disk space to ensure at least 12Mb is available.";
        case 3: return "File not found";
        case 4: return "Please reinstall";
        default: return "Unknown Error";
    }
}
