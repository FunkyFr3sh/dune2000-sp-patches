#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "hotkeys.h"
#include "ini.h"

#ifdef WWDEBUG
CLEAR(0x004A3BD3, 0xEB, 0x004A3BD3+1); // Allow multiple instances
CLEAR(0x004A3C61, 0xEB, 0x004A3C61+1); // Allow multiple instances
CLEAR(0x004A3D8E, 0xEB, 0x004A3D8E+1); // Allow multiple instances


void DebugLog(char *a)
{
    FILE *fp = fopen("debug.log", "a");
    if (fp)
    {
        //fprintf(fp, "t=%d, s=%x, f=%s", GameTicks, RandSeed, a);
        
        fclose(fp);
    }
}
#endif
