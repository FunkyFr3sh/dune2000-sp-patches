#include "dune2000.h"
#include "event-core.h"
#include "event-conditions.h"

bool Cond_BuildingExists(int side_id, int building_type)
{
  for ( Building *building = GetSide(side_id)->_Buildings_10; building; building = building->Next )
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
  for ( Unit *unit = GetSide(side_id)->_Units_8; unit; unit = unit->Next )
  {
    if ( unit->Type == unit_type )
    {
      return true;
    }
  }
  return false;
}

bool Cond_Interval(int start_delay, int time_amount, int run_count, ConditionData *condition)
{
  if ( start_delay )
  {
    condition->arg3 = start_delay - 1;
  }
  else
  {
    if ( run_count )
    {
      condition->value = run_count - 1;
      condition->arg3 = time_amount;
      return true;
    }
  }
  return false;
}

bool Cond_Timer(int comp_func, int time_amount, int time_shift)
{
  switch ( comp_func )
  {
    case 0: return gGameTicks > (unsigned) time_amount;
    case 1: return gGameTicks < (unsigned) time_amount;
    case 2: return gGameTicks ==  (unsigned) time_amount;
    case 3: return ((gGameTicks + time_shift) % time_amount) == 0;
  }
  return false;
}

bool Cond_Casualties(int side_id, int threshold, float proportion)
{
  CSide *side = GetSide(side_id);
  int units_lost = side->__units_lost;
  if ( units_lost > threshold )
  {
    unsigned int units_killed = side->__units_killed;
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
      condition->value = run_count - 1;
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
