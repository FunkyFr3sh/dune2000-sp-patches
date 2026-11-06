#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function CanSideUpgradeBuildingGroup
DETOUR(0x004431D0, 0x0044324D, _Mod__CanSideUpgradeBuildingGroup);

bool Mod__CanSideUpgradeBuildingGroup(eSideType side_id, eBuildingGroupType building_group)
{
  CSide *side; // eax
  unsigned int current_upgrade_count; // edi

  side = GetSide(side_id);
  if ( (signed int)building_group < 0 )
  {
    return 0;
  }
  if ( !side->__BuildingsExistPerGroup[(int)building_group] )
  {
    return 0;
  }
  current_upgrade_count = side->__BuildingGroupUpgradeCount[(int)building_group];
  if ( current_upgrade_count < 3 )
  {
    // New logic start
    // Building upgrade becomes available if side owns any upgradable building from same group
    for (Building *building = side->__FirstBuildingPtr; building; building = building->Next)
    {
      BuildingAtrbStruct *building_template = &_templates_buildattribs[building->Type];
      if (building_template->GroupType == building_group && building_template->_____TechLevelUpgrade[current_upgrade_count] != -1 && building_template->_____TechLevelUpgrade[current_upgrade_count] <= _gMiscData.Tech[(int)side_id])
        return 1;
    }
    // New logic end
  }
  return 0;
}
