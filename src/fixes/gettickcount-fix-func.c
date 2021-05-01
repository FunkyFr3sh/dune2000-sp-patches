#include <windows.h>
#include "macros/patch.h"

CLEAR(0x00402863, 0x90, 0x00402869);
CALL(0x00402863, _fake_timeGetTime);

CLEAR(0x00402873, 0x90, 0x00402879);
CALL(0x00402873, _fake_timeGetTime);

CLEAR(0x00409C56, 0x90, 0x00409C5C);
CALL(0x00409C56, _fake_GetTickCount);

CLEAR(0x0040B24A, 0x90, 0x0040B250);
CALL(0x0040B24A, _fake_GetTickCount);

CLEAR(0x0040CBAA, 0x90, 0x0040CBB0);
CALL(0x0040CBAA, _fake_GetTickCount);

CLEAR(0x0040CBD7, 0x90, 0x0040CBDD);
CALL(0x0040CBD7, _fake_GetTickCount);

CLEAR(0x004139A8, 0x90, 0x004139AE);
CALL(0x004139A8, _fake_GetTickCount);

CLEAR(0x004145E3, 0x90, 0x004145E9);
CALL(0x004145E3, _fake_GetTickCount);

CLEAR(0x0041601E, 0x90, 0x00416024);
CALL(0x0041601E, _fake_GetTickCount);

CLEAR(0x004359B1, 0x90, 0x004359B7);
CALL(0x004359B1, _fake_GetTickCount);

CLEAR(0x0043D4AE, 0x90, 0x0043D4B4);
CALL(0x0043D4AE, _fake_GetTickCount);

CLEAR(0x0043D536, 0x90, 0x0043D53C);
CALL(0x0043D536, _fake_GetTickCount);

CLEAR(0x0043F984, 0x90, 0x0043F98A);
CALL(0x0043F984, _fake_GetTickCount);

CLEAR(0x0044782A, 0x90, 0x00447830);
CALL(0x0044782A, _fake_GetTickCount);

CLEAR(0x00447FF6, 0x90, 0x00447FFC);
CALL(0x00447FF6, _fake_GetTickCount);

CLEAR(0x00448E73, 0x90, 0x00448E79);
CALL(0x00448E73, _fake_GetTickCount);

CLEAR(0x00449C38, 0x90, 0x00449C3E);
CALL(0x00449C38, _fake_GetTickCount);

CLEAR(0x00449E63, 0x90, 0x00449E69);
CALL(0x00449E63, _fake_GetTickCount);

CLEAR(0x00449E9A, 0x90, 0x00449EA0);
CALL(0x00449E9A, _fake_GetTickCount);

CLEAR(0x0044A300, 0x90, 0x0044A306);
CALL(0x0044A300, _fake_GetTickCount);

CLEAR(0x00458D44, 0x90, 0x00458D4A);
CALL(0x00458D44, _fake_GetTickCount);

CLEAR(0x0045CD5F, 0x90, 0x0045CD65);
CALL(0x0045CD5F, _fake_GetTickCount);

CLEAR(0x0045CEC5, 0x90, 0x0045CECB);
CALL(0x0045CEC5, _fake_GetTickCount);

CLEAR(0x00470AE1, 0x90, 0x00470AE7);
CALL(0x00470AE1, _fake_GetTickCount);

CLEAR(0x0047109E, 0x90, 0x004710A4);
CALL(0x0047109E, _fake_GetTickCount);

CLEAR(0x00471DDB, 0x90, 0x00471DE1);
CALL(0x00471DDB, _fake_GetTickCount);

CLEAR(0x00471E09, 0x90, 0x00471E0F);
CALL(0x00471E09, _fake_GetTickCount);

CLEAR(0x004731DE, 0x90, 0x004731E4);
CALL(0x004731DE, _fake_GetTickCount);

CLEAR(0x004733BD, 0x90, 0x004733C3);
CALL(0x004733BD, _fake_GetTickCount);

CLEAR(0x0047374F, 0x90, 0x00473755);
CALL(0x0047374F, _fake_GetTickCount);

CLEAR(0x0047BFEF, 0x90, 0x0047BFF5);
CALL(0x0047BFEF, _fake_GetTickCount);

CLEAR(0x004852F4, 0x90, 0x004852FA);
CALL(0x004852F4, _fake_GetTickCount);

CLEAR(0x00485639, 0x90, 0x0048563F);
CALL(0x00485639, _fake_GetTickCount);

CLEAR(0x0048C998, 0x90, 0x0048C99E);
CALL(0x0048C998, _fake_GetTickCount);

CLEAR(0x0048D101, 0x90, 0x0048D107);
CALL(0x0048D101, _fake_GetTickCount);

CLEAR(0x0048E721, 0x90, 0x0048E727);
CALL(0x0048E721, _fake_GetTickCount);

CLEAR(0x00492E84, 0x90, 0x00492E8A);
CALL(0x00492E84, _fake_GetTickCount);

CLEAR(0x00492F14, 0x90, 0x00492F1A);
CALL(0x00492F14, _fake_GetTickCount);

CLEAR(0x0045ABA0, 0x90, 0x0045ABA7);
LJMP(0x0045ABA0, _fake_GetTickCount);

DWORD WINAPI fake_timeGetTime()
{
    static DWORD last;
    DWORD current = timeGetTime();
    
    if (current > last || last > 0xFFFFE380)
    {
        last = current;
        return current;
    }

    return last;
}

DWORD WINAPI fake_GetTickCount()
{
    static DWORD last;
    DWORD current = GetTickCount();
    
    if (current > last || last > 0xFFFFE380)
    {
        last = current;
        return current;
    }

    return last;
}
