#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

// Fix cursor over enemy building when only armed engineer/saboteur is selected

// Replace call to IsAnyArmedUnitSelected with custom function with additional logic
CALL(0x004437D8, _IsAnyArmedUnitSelectedExceptEngineerAndSaboteur); // HandleGameLoopEvents

bool IsAnyArmedUnitSelectedExceptEngineerAndSaboteur()
{
  for (Unit *unit = GetSide(gSideId)->__FirstUnitPtr; unit; unit = unit->Next)
  {
    UnitAtribStruct *unit_template = &_templates_unitattribs[unit->Type];
    if (unit->__IsSelected && 
        unit_template->__PrimaryWeapon != -1 && 
        unit_template->__Behavior != UnitBehavior_SABOTEUR && 
        unit_template->__Behavior != UnitBehavior_ENGINEER)
      return 1;
  }
  return 0;
}
