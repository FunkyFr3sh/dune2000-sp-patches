#include "dune2000.h"
#include "macros/patch.h"
#include "ini.h"

CLEAR(0x004035EE, 0xCC, 0x00403601);
LJMP(0x004035EE, 0x00403601);
CALL(0x0042B7EA, _LoadNoCDSetting);

void LoadNoCDSetting(char *arg)
{
    ForceNoCD = D2kIniGetBool("Options", "ForceNoCD", true);
    CUIManager__GetCD(arg);
}
