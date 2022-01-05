#include "dune2000.h"

bool CompareValue(int val, int comp_val, bool comparison)
{
  if (comparison)
    return val >= comp_val;
  else
    return val == comp_val;
}

void RestoreUnitSelection(int side_id, bool restore_selection)
{
  if (!restore_selection)
    return;
  CSide *side = GetSide(side_id);
  for (Unit *unit = side->_Units_8; unit; unit = unit->Next)
  {
    unit->__ClearUnits_SelectedGroup_c_field_19_state = unit->PrevWasSelected;
    unit->PrevWasSelected = 0;
  }
}

void RestoreBuildingSelection(int side_id, bool restore_selection)
{
  if (!restore_selection)
    return;
  CSide *side = GetSide(side_id);
  for (Building *building = side->_Buildings_10; building; building = building->Next)
  {
    building->c_field_35_bool = building->PrevWasSelected;
    building->PrevWasSelected = 0;
  }
}
