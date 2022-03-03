#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

// Fix missing check for unit type availability when ordered to build such unit type

// Replace call to CanUnitBeBuilt with custom function with additional logic
CALL(0x00455555, _CanUnitBeBuiltExt); // ModelBuildUnitPick

bool CanUnitBeBuiltExt(unsigned __int8 side_id, unsigned __int8 unitType, char bool1)
{
  CSide *side = GetSide(side_id);
  for (int i = 0; i < (int)side->__UnitIconCount; i++)
  {
    if (side->__UnitIcons[i] == unitType)
      return w_CanUnitBeBuilt(side_id, unitType, bool1);
  }
  return false;
}
