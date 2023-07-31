#include "dune2000.h"
#include "event-utils.h"
#include "event-core.h"
#include "event-conditions.h"
#include "event-filters.h"
#include "rules.h"
#include "utils.h"

bool Cond_BuildingExists(int side_id, int building_type)
{
  for ( Building *building = GetSide(side_id)->__FirstBuildingPtr; building; building = building->Next )
  {
    if ( building->Type == building_type )
    {
      return true;
    }
  }
  return false;
}

bool Cond_UnitExists(int side_id, int unit_type)
{
  for ( Unit *unit = GetSide(side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( unit->Type == unit_type )
    {
      return true;
    }
  }
  return false;
}

bool Cond_Interval(bool inactive, int start_delay, int next_delay, int run_count, ConditionData *condition)
{
  if (inactive)
    return false;
  int off_ticks = rulesExt__intervalsAreOffByOneTick?1:0;
  int ticks = gGameTicks - condition->val4 - (start_delay + off_ticks);
  if (ticks < 0)
    return false;
  return ((ticks % (next_delay + off_ticks) == 0) &&
          ((run_count == 0) || (ticks / (next_delay + off_ticks) < run_count)));
}

bool Cond_Timer(bool inactive, int comp_func, int time_amount, int time_shift, ConditionData *condition)
{
  if (inactive)
    return false;
  int ticks = gGameTicks - condition->val4;
  if (ticks < 0)
    return false;
  switch ( comp_func )
  {
    case 0: return ticks > time_amount;
    case 1: return ticks < time_amount;
    case 2: return ticks == time_amount;
    case 3: return ((ticks + time_shift) % time_amount) == 0;
  }
  return false;
}

bool Cond_Casualties(int side_id, int threshold, float proportion)
{
  CSide *side = GetSide(side_id);
  int units_lost = side->__UnitsLost;
  if ( units_lost > threshold )
  {
    unsigned int units_killed = side->__UnitsKilled;
    double v12;
    if ( units_killed )
    {
      v12 = (double)(100 * units_lost / units_killed) * 0.0099999998;
    }
    else
    {
      v12 = (double)units_lost;
    }
    if ( v12 > proportion )
    {
      return true;
    }
  }
  return false;
}

bool Cond_Revealed(int xpos, int ypos, int run_count, ConditionData *condition)
{
  if ( run_count )
  {
    if ( !gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__shroud )
    {
      condition->val3 = run_count - 1;
      return true;
    }
  }
  return false;
}

bool Cond_Credits(int side_id, eCreditsCheck check_for, bool specific_side, int credits)
{
  CSide *side = GetSide(specific_side?side_id:gSideId);
  switch (check_for)
  {
    case CREDITSCHECK_TOTAL:        return (side->CashReal + side->SpiceReal) >= credits;
    case CREDITSCHECK_SPICE:        return (side->SpiceReal) >= credits;
    case CREDITSCHECK_CASH:         return (side->CashReal) >= credits;
    case CREDITSCHECK_MAX_STORAGE:  return (side->__MaxStorage) >= credits;
  }
  return false;
}

bool Cond_RandomChance(int range, int min_value, int max_value, int fixed_result, ConditionData *condition)
{
  if (fixed_result && (gGameTicks < (unsigned)fixed_result))
    return false;
  if (fixed_result && (gGameTicks > (unsigned)fixed_result))
    return condition->arg1;
  int value = tick_random_value % range + 1;
  bool result = (value >= min_value) && (value <= max_value);
  condition->arg1 = result;
  return result;
}

bool Cond_RandomInterval(int status, int run_count, int start_delay, int min_delay, int max_delay, ConditionData *condition)
{
  if (status == 1)
    return false;
  // Initial state of condition - initialize internal data
  if (status == 0)
  {
    condition->side_id = run_count; // Remaining runs
    condition->val4 = start_delay + (rand() % (max_delay - min_delay + 1) + min_delay); // Countdown counter
    condition->arg1 = 2; // Set condition status - initialized and running
  }
  // Countdown counter expired
  if (condition->val4 == 0)
  {
    // decrease remaining runs
    condition->side_id--;
    // No more remaining runs and interval should not repeat infinitely, inactivate condition
    if (condition->side_id == 0 && run_count)
      condition->arg1 = 1;
    // Set countdown counter to new random value
    else
      condition->val4 = rand() % (max_delay - min_delay + 1) + min_delay;
    return true;
  }
  else
    // Decrease countdown counter
    condition->val4--;
  return false;
}

bool Cond_CheckUnits(ConditionData *condition)
{
  bool strict_equal = condition->arg1 & CONDITIONFILTERFLAG_STRICT_EQUAL;
  int amount = LLIMIT(condition->arg2, 1);
  int matched = 0;
  for (int side_id = 0; side_id < 8; side_id++)
  {
    if ((condition->side_id != 8) && (condition->side_id != side_id))
      continue;
    CSide *side = GetSide(side_id);
    for (Unit *unit = side->__FirstUnitPtr; unit; unit = unit->Next)
    {
      if (CheckIfUnitMatchesFilter((ObjectFilterStruct *)condition, unit, side_id))
        matched++;
      // Already found enough matches, no need to search further
      if (!strict_equal && matched == amount)
        return true;
      // Found more matches than allowed
      if (strict_equal && matched > amount)
        return false;
    }
  }
  if (strict_equal && matched == amount)
    return true;
  return false;
}

bool Cond_CheckBuildings(ConditionData *condition)
{
  bool strict_equal = condition->arg1 & CONDITIONFILTERFLAG_STRICT_EQUAL;
  int amount = LLIMIT(condition->arg2, 1);
  int matched = 0;
  for (int side_id = 0; side_id < 8; side_id++)
  {
    if ((condition->side_id != 8) && (condition->side_id != side_id))
      continue;
    CSide *side = GetSide(side_id);
    for (Building *bld = side->__FirstBuildingPtr; bld; bld = bld->Next)
    {
      if (CheckIfBuildingMatchesFilter((ObjectFilterStruct *)condition, bld, side_id))
        matched++;
      // Already found enough matches, no need to search further
      if (!strict_equal && matched == amount)
        return true;
      // Found more matches than allowed
      if (strict_equal && matched > amount)
        return false;
    }
  }
  if (strict_equal && matched == amount)
    return true;
  return false;
}

bool Cond_CheckCrates(ConditionData *condition)
{
  bool strict_equal = condition->arg1 & CONDITIONFILTERFLAG_STRICT_EQUAL;
  int amount = LLIMIT(condition->arg2, 1);
  int matched = 0;
  for (int i = 0; i < MAX_CRATES; i++)
  {
    if (!gCrates[i].__is_active)
      continue;
    if (CheckIfCrateMatchesFilter((ObjectFilterStruct *)condition, &gCrates[i]))
      matched++;
    // Already found enough matches, no need to search further
    if (!strict_equal && matched == amount)
      return true;
    // Found more matches than allowed
    if (strict_equal && matched > amount)
      return false;
  }
  if (strict_equal && matched == amount)
    return true;
  return false;
}

bool Cond_CheckTiles(ConditionData *condition)
{
  bool strict_equal = condition->arg1 & CONDITIONFILTERFLAG_STRICT_EQUAL;
  int amount = LLIMIT(condition->arg2, 1);
  int matched = 0;
  int min_x;
  int min_y;
  int max_x;
  int max_y;
  GetBoundsForPosFilter((ObjectFilterStruct *)condition, &min_x, &min_y, &max_x, &max_y);
  for (int y = min_y; y <= max_y; y++)
    for (int x = min_x; x <= max_x; x++)
    {
      if (CheckIfTileMatchesFilter((ObjectFilterStruct *)condition, &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y))
        matched++;
      // Already found enough matches, no need to search further
      if (!strict_equal && matched == amount)
        return true;
      // Found more matches than allowed
      if (strict_equal && matched > amount)
        return false;
    }
  if (strict_equal && matched == amount)
    return true;
  return false;
}

bool Cond_SpiceInArea(int min_x, int min_y, int max_x, int max_y, int amount)
{
  int total_amount = 0;
  for (int y = min_y; y <= max_y; y++)
  {
    for (int x = min_x; x <= max_x; x++)
    {
      total_amount += (gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags >> 20) & 7;
    }
  }
  return total_amount >= amount;
}

bool Cond_DamageInArea(int min_x, int min_y, int max_x, int max_y, bool specific_terrain, int terrain_type, int damage)
{
  int total_damage = 0;
  for (int y = min_y; y <= max_y; y++)
  {
    for (int x = min_x; x <= max_x; x++)
    {
      int terr = (gGameMap.map[x + _CellNumbersWidthSpan[y]].__tile_bitflags >> 29) & 7;
      if (terr == terrain_type || !specific_terrain)
        total_damage += gGameMap.map[x + _CellNumbersWidthSpan[y]].__damage;
    }
  }
  return total_damage >= damage;
}

bool Cond_Power(int side_id, ePowerCheck check_for, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  switch (check_for)
  {
    case POWERCHECK_PERCENT:      return CompareValue(side->__PowerPercent1, value, !equal);
    case POWERCHECK_TOTAL_OUTPUT: return CompareValue(side->__PowerOutput, value, !equal);
    case POWERCHECK_TOTAL_DRAIN:  return CompareValue(side->__PowerDrained, value, !equal);
    case POWERCHECK_EXTRA_OUTPUT: return CompareValue(side->__PowerOutput - side->__PowerDrained, value, !equal);
    case POWERCHECK_EXTRA_DRAIN:  return CompareValue(side->__PowerDrained - side->__PowerOutput, value, !equal);
  }
  return false;
}

bool Cond_StarportStock(int side_id, int unit_type, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  int amount = 0;
  if (total)
  {
    for (int i = 0; i < 8; i++)
      amount += side->__StarportUnitTypeStock[side->__StarportIcons[i]];
  }
  else
    amount = side->__StarportUnitTypeStock[unit_type];
  return CompareValue(amount, value, !equal);
}

bool Cond_StarportCost(int side_id, int unit_type, bool equal, int value)
{
  return CompareValue(GetSide(side_id)->__StarportUnitTypeCost[unit_type], value, !equal);
}

bool Cond_StarportPick(int side_id, int unit_type, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  int amount = 0;
  if (total)
  {
    for (int i = 0; i < 8; i++)
      amount += side->__StarportUnitTypePicked[side->__StarportIcons[i]];
  }
  else
    amount = side->__StarportUnitTypePicked[unit_type];
  return CompareValue(amount, value, !equal);
}

bool Cond_BuildingIcon(int side_id, bool status, bool any_building, int building_group)
{
  CSide *side = GetSide(side_id);
  int building_group_built = (side->__BuildingBuildQueue.__type != -1)?_templates_buildattribs[side->__BuildingBuildQueue.__type].GroupType:-1;
  bool building_group_match = (building_group_built == building_group) || (any_building && building_group_built != -1);
  bool status_match = (side->__BuildingBuildQueue.w_field_2_buildprogress == 0x5A00) || (!status);
  return building_group_match && status_match;
}

bool Cond_UnitIcon(int side_id, bool status, bool any_unit, int queue, int unit_group)
{
  CSide *side = GetSide(side_id);
  for (int i = 0; i < 10; i++)
  {
    int unit_type_built = side->__UnitBuildQueue[i].__type;
    if (unit_type_built == -1)
      continue;
    int unit_group_built = _templates_unitattribs[unit_type_built].__UnitType;
    bool status_match = (side->__UnitBuildQueue[i].w_field_2_buildprogress == 0x5A00) || (!status);
    if (!any_unit)
    {
      if (unit_group_built == unit_group)
        return status_match;
    }
    else if (!queue)
    {
      return status_match;
    }
    else
    {
      int prereq1_group = _templates_unitattribs[unit_type_built].__PreReq1;
      char *queue_groups = (char *)&_templates_GroupIDs;
      if (prereq1_group == queue_groups[(queue <= 6)?queue:queue+3])
        return status_match;
    }
  }
  return false;
}

bool Cond_UpgradeIcon(int side_id,bool any_building, int building_group)
{
  CSide *side = GetSide(side_id);
  int building_group_built = (side->__BuildingUpgradeQueue.__type != -1)?_templates_buildattribs[side->__BuildingUpgradeQueue.__type].GroupType:-1;
  return (building_group_built == building_group) || (any_building && building_group_built != -1);
}

bool Cond_UnitsBuilt(int side_id, int unit_type, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  if (total)
    return CompareValue(side->__UnitsBuilt, value, !equal);
  else
    return CompareValue(side->__UnitsBuiltPerType[unit_type], value, !equal);
}

bool Cond_BuildingsBuilt(int side_id, int building_type, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  if (total)
    return CompareValue(side->__BuildingsBuilt, value, !equal);
  else
    return CompareValue(side->__BuildingsBuiltPerType[building_type], value, !equal);
}

bool Cond_UnitsLost(int side_id, int unit_type, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  if (total)
    return CompareValue(side->__UnitsLost, value, !equal);
  else
    return CompareValue(side->__UnitsLostPerType[unit_type], value, !equal);
}

bool Cond_BuildingsLost(int side_id, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  return CompareValue(side->__BuildingsLost, value, !equal);
}

bool Cond_UnitsKilled(int side_id, int unit_type, int enemy, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  if (total)
    return CompareValue(side->__UnitsKilled, value, !equal);
  else
    return CompareValue(side->__UnitsKilledPerTypeAndSide[unit_type].__kills_per_side[enemy], value, !equal);
}

bool Cond_BuildingsKilled(int side_id, int building_type, int enemy, bool total, bool equal, int value)
{
  CSide *side = GetSide(side_id);
  if (total)
    return CompareValue(side->__BuildingsKilled, value, !equal);
  else
    return CompareValue(side->__BuildingsKilledPerTypeAndSide[building_type].__kills_per_side[enemy], value, !equal);
}
