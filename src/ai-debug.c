#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"

#ifdef AI_DEBUG_ENABLED
// Bypass check for DebugPrintAllowed variable
CLEAR(0x00423837, 0x90, 0x00423839);
// Replace call by custom function for writing log into file
CALL(0x00423840, _AIDebugLogToFile);

void AIDebugLogToFile(char *s)
{
  static FILE *f = NULL;
  if (!f)
    f = fopen("C:\\modding\\ai_debug.txt", "w");
  fprintf(f, "%d: %s", gGameTicks, s);
}
#endif
