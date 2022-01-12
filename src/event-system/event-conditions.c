#include "dune2000.h"
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
    if ( !gGameMap.map[xpos + _CellNumbersWidthSpan[ypos]].__shroud_flags )
    {
      condition->val3 = run_count - 1;
      return true;
    }
  }
  return false;
}

bool Cond_Harvested(int side_id, bool specific_side, int credits)
{
  CSide *side = GetSide(specific_side?side_id:gSideId);
  return (side->CashReal + side->SpiceReal) >= credits;
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
      if (CheckIfUnitMatchesFilter((ObjectFilterStruct *)condition, unit))
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
  ObjectFilterStruct *filter = (ObjectFilterStruct *)condition;
  bool check_pos = (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && !(filter->pos_flags & OBJFILTERPOSFLAG_NEGATE);
  int min_x = check_pos?filter->pos_min_x:0;
  int min_y = check_pos?filter->pos_min_y:0;
  int max_x = check_pos?filter->pos_max_x:gGameMapWidth-1;
  int max_y = check_pos?filter->pos_max_y:gGameMapHeight-1;
  for (int y = min_y; y <= max_y; y++)
    for (int x = min_x; x <= max_x; x++)
    {
      if (CheckIfTileMatchesFilter((ObjectFilterStruct *)condition, &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y, (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && (filter->pos_flags & OBJFILTERPOSFLAG_NEGATE)))
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
