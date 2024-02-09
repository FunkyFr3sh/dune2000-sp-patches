#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
#include "event-utils.h"
#include "event-core.h"
#include "event-actions.h"
#include "event-conditions.h"

// New extended arrays for event and condition data

EventData _gEventArray[MAX_EVENTS];
EventExtraData gEventExtraData[MAX_EVENTS];
ConditionData _gConditionArray[MAX_CONDITIONS];
EventVariable gEventVariableArray[MAX_EVENT_VARIABLES];

// Other variables

int tick_random_value;
char condition_results[MAX_CONDITIONS];
int event_hooks[HOOK_TYPE_COUNT];
int exit_count;
int break_count;
int continue_count;

// Custom implementation of function HandleConditionsAndEvents
CALL(0x00449A8D, _Mod__HandleConditionsAndEvents);

void Mod__HandleConditionsAndEvents()
{
  char v20; // bl
  char (*v21)[8]; // ebp
  char *v22; // edx
  int v23; // esi
  char v24; // al
  int v25; // eax
  char (*v26)[8]; // ecx
  signed int v27; // edi
  char v28; // dl
  char v29; // [esp+12h] [ebp-46h]
  char v30; // [esp+13h] [ebp-45h]
  unsigned __int8 a1; // [esp+14h] [ebp-44h]
  signed int v34; // [esp+18h] [ebp-40h]

  a1 = (unsigned __int8)(gGameTicks & 0xF) >> 1;
  v30 = 0;
  v29 = -1;
  EvaluateIfBuildingsOrUnitsExistForSide(a1, gGameTicks & 1);
  if ( _IsMultiplayer || gGameType == GAME_SKIRMISH )
  {
    v20 = 0;
    v21 = _gDiplomacy;
    _somebool_6B7050 = 0;
    v22 = &_gAIArray[0].__IsAI;
    v23 = 0;
    v34 = 8;
    do
    {
      v24 = _persideflags_6B8258[v23];
      if ( !v24 || _gBuildingsExist[v23] )
      {
        v30 = 1;
      }
      else if ( _gUnitsExist[v23] )
      {
        v29 &= ~v24;
      }
      if ( _gUnitsExist[v23] || _gBuildingsExist[v23] )
      {
        v25 = 0;
        v26 = v21;
        v27 = 8;
        do
        {
          if ( (_gUnitsExist[v25] || _gBuildingsExist[v25]) && (_gDiplomacy[v23][v25] || *(_BYTE *)v26) )
          {
            v20 = 1;
          }
          ++v25;
          ++v26;
          --v27;
        }
        while ( v27 );
        if ( !*v22 )
        {
          _somebool_6B7050 = 1;
        }
      }
      ++v23;
      v21 = (char (*)[8])((char *)v21 + 1);
      v22 += 7608;
      --v34;
    }
    while ( v34 );
    if ( v20 )
    {
      v28 = _GameOver;
    }
    else
    {
      v28 = 1;
      _GameOver = 1;
    }
    if ( _gUnitsExist[(unsigned __int8)gSideId] || _gBuildingsExist[(unsigned __int8)gSideId] )
    {
      if ( v28 )
      {
        if ( !gLose )
        {
          gWin = 1;
        }
      }
    }
    else if ( !gWin )
    {
      gLose = 1;
    }
    if ( !v30 && !_NumCratesAllowed && !v28 )
    {
      if ( v29 == -3 )
      {
        _GameOver = 1;
        if ( gSideId == a1 )
        {
          gWin = 1;
        }
        else
        {
          gLose = 1;
        }
      }
      if ( v29 == -2 )
      {
        _GameOver = 1;
        if ( gSideId == a1 )
        {
          gWin = 1;
        }
        else
        {
          gLose = 1;
        }
      }
    }
  }
  else
  {
    // Generate random value for this tick
    tick_random_value = rand();

    // Evaluate conditions
    for (int condition_index = 0; condition_index < _gConditionCount; condition_index++)
    {
      condition_results[condition_index] = EvaluateCondition(condition_index);
    }
    // Process events
    exit_count = 0;
    break_count = 0;
    continue_count = 0;
    ExecuteEventsInRange(0, _gEventCount, EBT_GLOBAL);
  }
}

#define COORD0  coord_x[0], coord_y[0]
#define COORD1  coord_x[1], coord_y[1]
#define A_SIDE  args[0]
#define A_ARG1  args[1]
#define A_ARG2  args[2]
#define A_VAL1  args[3]
#define A_VAL2  args[4]
#define A_VAL3  args[5]
#define A_VAL4  args[6]
#define A_FLOAT condition->float_val

bool EvaluateCondition(int condition_index)
{
  ConditionData *condition = &_gConditionArray[condition_index];
  // Fill condition context
  int coord_x[2];
  int coord_y[2];
  int args[7] = {0};
  for (int i = 0; i < 2; i++)
  {
    coord_x[i] = GetVariableValueOrConst(condition->coord_var_flags, i*2, condition->coord_x[i]);
    coord_y[i] = GetVariableValueOrConst(condition->coord_var_flags, i*2+1, condition->coord_y[i]);
  }
  if ((condition->condition_type < CT_CHECKUNITS) || (condition->condition_type > CT_CHECKTILES))
  {
    args[0] = GetVariableValueOrConst(condition->arg_var_flags, 0, condition->side_id);
    args[1] = GetVariableValueOrConst(condition->arg_var_flags, 1, condition->arg1);
    args[2] = GetVariableValueOrConst(condition->arg_var_flags, 2, condition->arg2);
    args[3] = GetVariableValueOrConst(condition->arg_var_flags, 3, condition->val1);
    args[4] = GetVariableValueOrConst(condition->arg_var_flags, 4, condition->val2);
    args[5] = GetVariableValueOrConst(condition->arg_var_flags, 5, condition->val3);
    args[6] = GetVariableValueOrConst(condition->arg_var_flags, 6, condition->val4);
  }
  // Run condition
  switch ( condition->condition_type )
  {
    // Vanilla / General
    case CT_BUILDINGEXISTS:     return Cond_BuildingExists  (A_SIDE, A_ARG1);
    case CT_UNITEXISTS:         return Cond_UnitExists      (A_SIDE, A_ARG2);
    case CT_INTERVAL:           return Cond_Interval        (A_ARG1, A_VAL1, A_VAL2, A_VAL3, condition);
    case CT_TIMER:              return Cond_Timer           (A_ARG1, A_ARG2, A_VAL2, A_VAL3, condition);
    case CT_CASUALTIES:         return Cond_Casualties      (A_SIDE, A_VAL3, A_FLOAT);
    case CT_BASEDESTROYED:      return !_gBuildingsExist[A_SIDE];
    case CT_UNITSDESTROYED:     return !_gUnitsExist[A_SIDE];
    case CT_REVEALED:           return Cond_Revealed        (COORD0, A_VAL3, condition);
    case CT_CREDITS:            return Cond_Credits         (A_SIDE, A_ARG1, A_ARG2, A_VAL3);
    case CT_FLAG:               return A_VAL3 != 0;
    case CT_RANDOMCHANCE:       return Cond_RandomChance    (A_VAL1, A_VAL2, A_VAL3, A_VAL4, condition);
    case CT_RANDOMINTERVAL:     return Cond_RandomInterval  (A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3, condition);
    case CT_DIPLOMACY:          return _gDiplomacy[A_SIDE][A_ARG1] == A_ARG2;
    case CT_DIFFICULTY:         return gDifficultyLevel == A_VAL3;
    // Environment checking
    case CT_CHECKUNITS:         return Cond_CheckUnits      (condition);
    case CT_CHECKBUILDINGS:     return Cond_CheckBuildings  (condition);
    case CT_CHECKCRATES:        return Cond_CheckCrates     (condition);
    case CT_CHECKTILES:         return Cond_CheckTiles      (condition);
    case CT_SPICE_IN_AREA:      return Cond_SpiceInArea     (COORD0, COORD1, A_VAL3);
    case CT_DAMAGE_IN_AREA:     return Cond_DamageInArea    (COORD0, COORD1, A_ARG1, A_ARG2, A_VAL3);
    // Side related
    case CT_POWER:              return Cond_Power           (A_SIDE, A_ARG1, A_VAL1, A_VAL3);
    case CT_BUILDING_UPGRADES:  return CompareValue(GetSide(A_SIDE)->__BuildingGroupUpgradeCount[A_ARG1], A_VAL3, !A_VAL1);
    case CT_STARPORT_STOCK:     return Cond_StarportStock   (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_STARPORT_COST:      return Cond_StarportCost    (A_SIDE, A_ARG1, A_VAL1, A_VAL3);
    case CT_STARPORT_PICK:      return Cond_StarportPick    (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_STARPORT_DELIVERY:  return GetSide(A_SIDE)->__StarportDeliveryInProgress;
    case CT_BUILDING_ICON:      return Cond_BuildingIcon    (A_SIDE, A_ARG1, A_ARG2, A_VAL2);
    case CT_UNIT_ICON:          return Cond_UnitIcon        (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL2);
    case CT_UPGRADE_ICON:       return Cond_UpgradeIcon     (A_SIDE, A_ARG2, A_VAL2);
    case CT_SPICE_HARVESTED:    return CompareValue(GetSide(A_SIDE)->__SpiceHarvested, A_VAL3, !A_VAL1);
    case CT_UNITS_BUILT:        return Cond_UnitsBuilt      (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_BUILDINGS_BUILT:    return Cond_BuildingsBuilt  (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_UNITS_LOST:         return Cond_UnitsLost       (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_BUILDINGS_LOST:     return Cond_BuildingsLost   (A_SIDE, A_VAL1, A_VAL3);
    case CT_UNITS_KILLED:       return Cond_UnitsKilled     (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3);
    case CT_BUILDINGS_KILLED:   return Cond_BuildingsKilled (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3);
    case CT_SIDE_PROPERTY:      return CompareDataValue     ((char *)GetSide(A_SIDE),    A_ARG1, A_VAL1, A_VAL3, !A_VAL2);
    // AI related
    case CT_AI_PROPERTY:        return CompareDataValue     ((char *)&_gAIArray[A_SIDE], A_ARG1, A_VAL1, A_VAL3, !A_VAL2);
    // Memory related
    case CT_MEMORY_VALUE:       return CompareDataValue     (NULL,                       A_ARG1, A_VAL1, A_VAL3, !A_VAL2);
    // Variable related
    case CT_VARIABLE_VALUE:     return Cond_VariableValue   (A_ARG1, A_VAL2, A_VAL3);
    case CT_VARIABLE_CHANGED:   return Cond_VariableChanged (A_ARG1);
    default:
      DebugFatal("event-core.c", "Unknown condition type %d", condition->condition_type);
  }
  return false;  
}

#undef COORD0
#undef COORD1
#undef A_SIDE
#undef A_ARG1
#undef A_ARG2
#undef A_VAL1
#undef A_VAL2
#undef A_VAL3
#undef A_VAL4
#undef A_FLOAT

bool IsStartBlockEvent(int event_index)
{
  int t = _gEventArray[event_index].event_type;
  return (t == ET_CALLABLE_BLOCK_START || t == ET_HOOK_BLOCK_START || t == ET_IF || (t >= ET_LOOP_WHILE && t <= ET_LOOP_SIDES));
}

int FindEndMarkerForBlockEvent(int event_index)
{
  int i = event_index + 1;
  while (i < _gEventCount)
  {
    if (gEventExtraData[event_index].else_event_index == -1 &&
        (_gEventArray[event_index].event_type == ET_IF || _gEventArray[event_index].event_type == ET_ELSE_IF) &&
        (_gEventArray[i].event_type == ET_ELSE_IF || _gEventArray[i].event_type == ET_ELSE))
      gEventExtraData[event_index].else_event_index = i;
    if (_gEventArray[i].event_type == ET_END)
      return i;
    if (IsStartBlockEvent(i))
      i = FindEndMarkerForBlockEvent(i);
    i++;
  }
  DebugFatal("event-core.c", "Missing END event for block-start event %d", event_index);
  return 0;
}

void ExecuteEventsInRange(int min_event_index, int max_event_index, eEventBlockType block_type)
{
  int event_index = min_event_index;
  while (event_index < max_event_index)
  {
    bool event_can_happen;
    // OR-evaluation
    if (_gEventArray[event_index].event_flags & EVENTFLAG_CONDITIONS_OR)
    {
      event_can_happen = false;
      for (int i = 0; i < _gEventArray[event_index].num_conditions; i++)
      {
        if ( condition_results[(int)_gEventArray[event_index].condition_index[i]] != _gEventArray[event_index].condition_negation[i] )
        {
          event_can_happen = true;
          break;
        }
      }
    }
    // AND-evaluation
    else
    {
      event_can_happen = true;
      for (int i = 0; i < _gEventArray[event_index].num_conditions; i++)
      {
        if ( condition_results[(int)_gEventArray[event_index].condition_index[i]] == _gEventArray[event_index].condition_negation[i] )
        {
          event_can_happen = false;
          break;
        }
      }
    }
    // Execute event
    if ( event_can_happen )
    {
      ExecuteEvent(event_index);
      // Handle Exit, Break and Continue statements
      if (exit_count)
      {
        if (block_type == EBT_GLOBAL)
          DebugFatal("event-core.c", "Exit event cannot be used outside of a Block (event %d)", event_index);
        else
        {
          if (block_type == EBT_BLOCK)
            exit_count--;
          break;
        }
      }
      if (break_count)
      {
        if (block_type == EBT_GLOBAL || block_type == EBT_BLOCK)
          DebugFatal("event-core.c", "Break event cannot be used outside of a Loop (event %d)", event_index);
        else
          break;
      }
      if (continue_count)
      {
        if (block_type == EBT_GLOBAL || block_type == EBT_BLOCK)
          DebugFatal("event-core.c", "Continue event cannot be used outside of a Loop (event %d)", event_index);
        else
        {
          if (block_type == EBT_LOOP)
            continue_count--;
          break;
        }
      }
    }
    event_index = gEventExtraData[event_index].next_event_index;
  }
}

void ExecuteEventBlock(int event_index, eEventBlockType block_type)
{
  ExecuteEventsInRange(event_index + 1, gEventExtraData[event_index].next_event_index - 1, block_type);
}

void ExecuteEvent(int event_index)
{
  EventData *event = &_gEventArray[event_index];
  int et = event->event_type;
  // Check if event is blocked and block it if auto-block is set
  if (event->event_flags & EVENTFLAG_BLOCKED)
    return;
  if (event->event_flags & EVENTFLAG_AUTO_BLOCK)
    event->event_flags |= EVENTFLAG_BLOCKED;
  // Fill event context
  EventContext e;
  e.event_type = et;
  e.event_index = event_index;
  for (int i = 0; i < 4; i++)
  {
    e.coord_x[i] = GetVariableValueOrConst(event->coord_var_flags, i*2, event->coord_x[i]);
    e.coord_y[i] = GetVariableValueOrConst(event->coord_var_flags, i*2+1, event->coord_y[i]);
  }
  for (int i = 0; i < 5; i++)
  {
    int flag_num = i;
    if (i == 0)
      flag_num = 1;
    if (i == 1)
      flag_num = 0;
    e.args[i] = GetVariableValueOrConst(event->arg_var_flags, flag_num, event->args[i]);
  }
  e.args[5] = GetVariableValueOrConst(event->arg_var_flags, 5, event->value);
  e.data = event->data;
  int skip = GetVariableValueOrConst(event->event_flags, 4, event->filter_skip);
  int limit = GetVariableValueOrConst(event->event_flags, 5, event->data[0]);
  // Handle event if object index is used instead of filter
  if (event->event_flags & EVENTFLAG_OBJECT_INDEX)
  {
    if (et >= ET_CHANGE_TILE && et <= ET_SHOW_TILE_DATA)
    {
      e.coord_x[0] = GetVariableValue(event->filter_skip);
      e.coord_y[0] = GetVariableValue(event->filter_skip + 1);
    }
    else
    {
      e.object_index = GetVariableValue(event->filter_skip);
      // Clear and backup unit selection
      if (et == ET_SELECT_UNIT)
        for (Unit *unit = GetSide(e.args[1])->__FirstUnitPtr; unit; unit = unit->Next)
        {
          unit->PrevWasSelected = unit->__IsSelected;
          unit->__IsSelected = 0;
        }
      // Clear and backup building selection
      if (et == ET_SELECT_BUILDING)
        for (Building *building = GetSide(e.args[1])->__FirstBuildingPtr; building; building = building->Next)
        {
          building->PrevWasSelected = building->__IsSelected;
          building->__IsSelected = 0;
        }
    }
    ExecuteEventAction(&e);
    return;
  }
  // Unit manipulation events: process all side's units
  if ((et >= ET_DESTROY_UNIT && et <= ET_SHOW_UNIT_DATA)
      || et == ET_ORDER_REPAIR_SINGLE_UNIT
      || et == ET_ORDER_UNIT_ATTACK_UNIT
      || et == ET_ORDER_UNIT_DEPLOY
      || et == ET_ORDER_BUILDING_ATTACK_UNIT
      || et == ET_GET_UNIT_COUNT
      || et == ET_LOOP_UNITS)
  {
    if (et == ET_ORDER_UNIT_ATTACK_UNIT
        || et == ET_ORDER_BUILDING_ATTACK_UNIT)
      limit = 1;
    if (et == ET_GET_UNIT_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < 8; side_id++)
    {
      if ((arg_side_id != 8) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      // Clear and backup unit selection
      if (et == ET_SELECT_UNIT)
        for (Unit *unit = side->__FirstUnitPtr; unit; unit = unit->Next)
        {
          unit->PrevWasSelected = unit->__IsSelected;
          unit->__IsSelected = 0;
        }
      // Process all units
      for (Unit *unit = side->__FirstUnitPtr; unit; unit = unit->Next)
      {
        if (CheckIfUnitMatchesFilter((ObjectFilterStruct *)&e.data[1], unit, side_id))
        {
          if (skip)
            skip--;
          else
          {
            e.object_index = unit->MyIndex;
            ExecuteEventAction(&e);
            if (break_count)
            {
              break_count--;
              arg_side_id = -1;
              break;
            }
            affected++;
          }
        }
        if (limit && (affected == limit))
          return;
      }
    }
    return;
  }
  // Building manipulation events: process all side's buildings
  if ((et >= ET_DESTROY_BUILDING && et <= ET_SHOW_BUILDING_DATA)
      || et == ET_ORDER_DOCK_WITH_REFINERY
      || et == ET_ORDER_REPAIR_SELECTED_UNITS
      || et == ET_ORDER_UNIT_ATTACK_BUILDING
      || et == ET_ORDER_BUILDING_ATTACK_BUILDING
      || et == ET_ORDER_BUILDING_REPAIR
      || et == ET_ORDER_BUILDING_SELL
      || et == ET_GET_BUILDING_COUNT
      || et == ET_LOOP_BUILDINGS)
  {
    if (et == ET_ORDER_DOCK_WITH_REFINERY
        || et == ET_ORDER_REPAIR_SELECTED_UNITS
        || et == ET_ORDER_UNIT_ATTACK_BUILDING
        || et == ET_ORDER_BUILDING_ATTACK_BUILDING)
      limit = 1;
    if (et == ET_GET_BUILDING_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < 8; side_id++)
    {
      if ((arg_side_id != 8) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      // Clear and backup building selection
      if (et == ET_SELECT_BUILDING)
        for (Building *building = side->__FirstBuildingPtr; building; building = building->Next)
        {
          building->PrevWasSelected = building->__IsSelected;
          building->__IsSelected = 0;
        }
      // Process all buildings
      for (Building *building = side->__FirstBuildingPtr; building; building = building->Next)
      {
        if (CheckIfBuildingMatchesFilter((ObjectFilterStruct *)&e.data[1], building, side_id))
        {
          if (skip)
            skip--;
          else
          {
            e.object_index = building->MyIndex;
            ExecuteEventAction(&e);
            if (break_count)
            {
              break_count--;
              arg_side_id = -1;
              break;
            }
            affected++;
          }
        }
        if (limit && (affected == limit))
          return;
      }
    }
    return;
  }
  // Bullet manipulation events: process all side's bullets
  if (et == ET_SET_BULLET_PROPERTY || et == ET_GET_BULLET_COUNT || et == ET_LOOP_BULLETS)
  {
    if (et == ET_GET_BULLET_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < 8; side_id++)
    {
      if ((arg_side_id != 8) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      // Process all bullets
      for (Bullet *bullet = side->__FirstBulletPtr; bullet; bullet = bullet->Next)
      {
        if (CheckIfBulletMatchesFilter((ObjectFilterStruct *)&e.data[1], bullet))
        {
          if (skip)
            skip--;
          else
          {
            e.object_index = bullet->MyIndex;
            ExecuteEventAction(&e);
            if (break_count)
            {
              break_count--;
              arg_side_id = -1;
              break;
            }
            affected++;
          }
        }
        if (limit && (affected == limit))
          return;
      }
    }
    return;
  }
  // Explosion manipulation events: process all side's explosions
  if (et == ET_SET_EXPLOSION_PROPERTY || et == ET_GET_EXPLOSION_COUNT || et == ET_LOOP_EXPLOSIONS)
  {
    if (et == ET_GET_EXPLOSION_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < 8; side_id++)
    {
      if ((arg_side_id != 8) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      // Process all explosions
      for (Explosion *explosion = side->__FirstExplosionPtr; explosion; explosion = explosion->Next)
      {
        if (CheckIfExplosionMatchesFilter((ObjectFilterStruct *)&e.data[1], explosion))
        {
          if (skip)
            skip--;
          else
          {
            e.object_index = explosion->MyIndex;
            ExecuteEventAction(&e);
            if (break_count)
            {
              break_count--;
              arg_side_id = -1;
              break;
            }
            affected++;
          }
        }
        if (limit && (affected == limit))
          return;
      }
    }
    return;
  }
  // Crate manipulation events: process all crates
  if ((et >= ET_REMOVE_CRATE && et <= ET_SHOW_CRATE_DATA)
      || et == ET_GET_CRATE_COUNT
      || et == ET_LOOP_CRATES)
  {
    if (et == ET_GET_CRATE_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    // Process all crates
    for (int i = 0; i < MAX_CRATES; i++)
    {
      if (CheckIfCrateMatchesFilter((ObjectFilterStruct *)&e.data[1], &gCrates[i]))
      {
        if (skip)
          skip--;
        else
        {
          e.object_index = i;
          ExecuteEventAction(&e);
          if (break_count)
          {
            break_count--;
            break;
          }
          affected++;
        }
      }
      if (limit && (affected == limit))
        return;
    }
    return;
  }
  // Tile manipulation events: process all tiles
  if ((et >= ET_CHANGE_TILE && et <= ET_SHOW_TILE_DATA)
      || et == ET_GET_TILE_COUNT
      || et == ET_GET_SPICE_COUNT
      || et == ET_GET_DAMAGE_COUNT
      || et == ET_LOOP_TILES)
  {
    if (et == ET_GET_TILE_COUNT || et == ET_GET_SPICE_COUNT || et == ET_GET_DAMAGE_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    // Position optimization
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    GetBoundsForPosFilter((ObjectFilterStruct *)&e.data[1], &min_x, &min_y, &max_x, &max_y);
    // Process all tiles
    for (int y = min_y; y <= max_y; y++)
      for (int x = min_x; x <= max_x; x++)
      {
        if (CheckIfTileMatchesFilter((ObjectFilterStruct *)&e.data[1], &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y))
        {
          if (skip)
            skip--;
          else
          {
            e.coord_x[0] = x;
            e.coord_y[0] = y;
            ExecuteEventAction(&e);
            if (break_count)
            {
              break_count--;
              break;
            }
            affected++;
          }
        }
        if (limit && (affected == limit))
          return;
      }
    return;
  }
  // Side manipulation events: process all crates
  if (et == ET_GET_SIDE_COUNT || et == ET_LOOP_SIDES)
  {
    if (et == ET_GET_SIDE_COUNT)
      SetVariableValue(e.args[0], 0);
    int affected = 0;
    // Process all sides
    for (int i = 0; i < 8; i++)
    {
      if (CheckIfSideMatchesFilter((ObjectFilterStruct *)&e.data[1], i))
      {
        if (skip)
          skip--;
        else
        {
          e.object_index = i;
          ExecuteEventAction(&e);
          if (break_count)
          {
            break_count--;
            break;
          }
          affected++;
        }
      }
      if (limit && (affected == limit))
        return;
    }
    return;
  }
  // Normal events: just execute action
  ExecuteEventAction(&e);
}

#define EV_IDX  e->event_index
#define COORD0  e->coord_x[0], e->coord_y[0]
#define COORD1  e->coord_x[1], e->coord_y[1]
#define COORD2  e->coord_x[2], e->coord_y[2]
#define COORD3  e->coord_x[3], e->coord_y[3]
#define A_SIDE  e->args[1]
#define A_AMNT  e->args[0]
#define A_ITEM  e->args[2]
#define A_ENUM  e->args[3]
#define A_BOOL  e->args[4]
#define A_VAL1  e->args[5]
#define A_VAL2  *(int *)&e->data[1]
#define OBJ_ID e->object_index

#define VALUEOPERATION(data) data = ValueOperation(data, A_VAL1, A_ENUM)

void ExecuteEventAction(EventContext *e)
{
  switch ( e->event_type )
  {
  // Vanilla / General
  case ET_REINFORCEMENT:          EvAct_AddDelivery         (COORD0, A_SIDE, A_AMNT, A_ITEM, A_BOOL, A_VAL1, DELIVERYTYPE_REINFORCE, e->data); break;
  case ET_STARPORT_DELIVERY:      EvAct_AddDelivery         (COORD0, A_SIDE, A_AMNT, A_ITEM, A_BOOL, A_VAL1, DELIVERYTYPE_STARPORT,  e->data); break;
  case ET_ALLEGIANCE:             EvAct_SetDiplomacy        (A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_LEAVE:                  CSide__BlowupAll_surrender(GetSide(A_SIDE)); break;
  case ET_BESERK:                 _gAIArray[A_SIDE].__GoBeserk_OtherStates = 1; break;
  case ET_PLAYSOUND:              EvAct_PlaySound           (A_VAL1, A_ENUM, A_BOOL, COORD0); break;
  case ET_SETBUILDRATE:           VALUEOPERATION(_gAIArray[A_SIDE].UnitBuildRate); break;
  case ET_SETATTACKBUILDINGRATE:  VALUEOPERATION(_gAIArray[A_SIDE].TimeBetweenBuildingAttacks); break;
  case ET_SETCASH:                EvAct_SetCash             (A_SIDE, A_ENUM, A_VAL1); break;
  case ET_SETTECH:                EvAct_SetTech             (A_SIDE, A_ENUM, A_BOOL, A_VAL1); break;
  case ET_WIN:                    if ( !gLose ) gWin = 1; break;
  case ET_LOSE:                   if ( !gWin ) gLose = 1; break;
  case ET_SWITCH_MY_SIDE:         EvAct_SwitchMySide        (A_SIDE, A_ENUM, A_BOOL); break;
  case ET_HIDE_MAP:               EvAct_HideMap             (); break;
  case ET_REVEAL:                 EvAct_RevealMap           (COORD0, A_AMNT); break;
  case ET_SETTIMER:               VALUEOPERATION(_gTimerValue); break;
  case ET_HIDETIMER:              _gTimerValue = -1; break;
  case ET_SHOWMESSAGE:            EvAct_ShowMessage         (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1, (ShowMessageEventData *)&e->data[1]); break;
  case ET_UNIT_SPAWN:             EvAct_UnitSpawn           (COORD0, A_SIDE, A_AMNT, A_ENUM, A_VAL1, e->data); break;
  case ET_SET_FLAG:               _gConditionArray[A_SIDE].val3 = A_VAL1; break;
  case ET_UN_BLOCK_EVENT:         EvAct_UnBlockEvent        (A_BOOL, A_VAL1); break;
  case ET_PLAY_MUSIC:             EvAct_PlayMusic           (e->data); break;
  case ET_DAMAGE_TILES:           EvAct_DamageTiles         (COORD0, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_UNIT:               EvAct_AddUnit             (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1, A_VAL2); break;
  case ET_ADD_BUILDING:           EvAct_AddBuilding         (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1, A_VAL2); break;
  case ET_ADD_BULLET:             EvAct_AddBullet           (COORD0, COORD1, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL, A_VAL1, A_VAL2); break;
  case ET_ADD_EXPLOSION:          EvAct_AddExplosion        (COORD0, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL, A_VAL1, A_VAL2); break;
  case ET_ADD_CRATE:              EvAct_AddCrate            (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VAL1); break;
  case ET_ADD_CONCRETE:           EvAct_AddConcrete         (COORD0, COORD1, A_SIDE, A_VAL1); break;
  case ET_SPICE_BLOOM:            EvAct_SpiceBloom          (COORD0, A_AMNT, A_ENUM, A_BOOL); break;
  case ET_SHAKE_SCREEN:           _ScreenShakes = A_VAL1; break;
  case ET_CHANGE_VIEWPORT:        EvAct_ChangeViewport      (COORD0, A_ENUM, A_BOOL); break;
  case ET_CHANGE_MAP_BLOCK:       EvAct_ChangeMapBlock      (COORD0, COORD1, A_ENUM, (uint16_t *)&e->data[1]); break;
  case ET_TRANSFORM_TILES:        EvAct_TransformTiles      (A_AMNT, A_ENUM, (uint16_t *)&e->data[1]); break;
  case ET_ADD_BUILDING_DESTRUCT:  EvAct_AddBuildingDestruct (COORD0, A_SIDE, A_ITEM); break;
  case ET_ADD_HOMING_BULLET:      EvAct_AddHomingBullet     (COORD0, COORD1, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1, A_VAL2); break;
  case ET_ACTIVATE_TIMER:         EvAct_ActivateTimer       (A_VAL1); break;
  case ET_REMOVE_MESSAGE:         EvAct_RemoveMessage       (A_SIDE, A_AMNT, A_ITEM); break;
  case ET_SET_MESSAGE_COLOR:      EvAct_SetMessageColor     (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VAL1, A_VAL2); break;
  case ET_SET_TOOLTIP:            EvAct_SetTooltip          (A_ITEM, A_ENUM, A_VAL1, (ShowMessageEventData *)&e->data[1]);
  // Side manipulation
  case ET_TRANSFER_CREDITS:       EvAct_TransferCredits     (A_SIDE, A_ENUM, A_VAL1); break;
  case ET_SET_BUILDING_UPGRADES:  EvAct_SetBuildingUpgrades (A_SIDE, A_ITEM, A_ENUM, A_VAL1); break;
  case ET_SET_STARPORT_STOCK:     VALUEOPERATION(GetSide(A_SIDE)->__StarportUnitTypeStock[A_ITEM]); break;
  case ET_SET_STARPORT_COST:      EvAct_SetStarportCost     (A_SIDE, A_ITEM, A_ENUM, A_BOOL, A_VAL1); break;
  case ET_CHANGE_STARPORT_UNIT:   GetSide(A_SIDE)->__StarportIcons[A_AMNT] = A_ITEM; break;
  case ET_SHOW_SIDE_DATA:         EvAct_ShowSideData        (A_SIDE, A_VAL1); break;
  // AI manipulation
  case ET_SET_AI_PROPERTY:        EvAct_SetAIProperty       (A_SIDE, A_AMNT, A_ENUM, A_VAL1, A_VAL2); break;
  case ET_SHOW_AI_DATA:           EvAct_ShowAIData          (A_SIDE, A_VAL1); break;
  // Memory manipulation
  case ET_SET_MEMORY_DATA:        EvAct_SetMemoryData       (A_AMNT, A_ENUM, A_VAL1, A_VAL2); break;
  case ET_SHOW_MEMORY_DATA:       EvAct_ShowMemoryData      (A_VAL1); break;
  // Unit manipulation
  case ET_DESTROY_UNIT:           EvAct_DestroyUnit         (A_SIDE, A_BOOL, OBJ_ID); break;
  case ET_DAMAGE_HEAL_UNIT:       EvAct_DamageHealUnit      (A_SIDE, A_ENUM, A_BOOL, A_VAL1, OBJ_ID); break;
  case ET_CHANGE_UNIT_OWNER:      ChangeUnitOwner           (A_SIDE, A_ITEM, OBJ_ID, 0); break;
  case ET_CHANGE_UNIT_TYPE:       EvAct_ChangeUnitType      (A_SIDE, A_ITEM, A_BOOL, OBJ_ID); break;
  case ET_SET_UNIT_FLAG:          EvAct_SetUnitFlag         (A_SIDE, A_ENUM, A_VAL1, OBJ_ID); break;
  case ET_SET_UNIT_PROPERTY:      EvAct_SetUnitProperty     (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VAL1, OBJ_ID); break;
  case ET_SELECT_UNIT:            EvAct_SelectUnit          (A_SIDE, A_BOOL, OBJ_ID); break;
  case ET_AIRLIFT_UNIT:           EvAct_AirliftUnit         (A_SIDE, COORD0, A_BOOL, OBJ_ID); break;
  case ET_SHOW_UNIT_DATA:         EvAct_ShowUnitData        (A_SIDE, OBJ_ID); break;
  // Building manipulation
  case ET_DESTROY_BUILDING:       EvAct_DestroyBuilding     (A_SIDE, A_BOOL, OBJ_ID); break;
  case ET_DAMAGE_HEAL_BUILDING:   EvAct_DamageHealBuilding  (A_SIDE, A_ENUM, A_BOOL, A_VAL1, OBJ_ID); break;
  case ET_CHANGE_BUILDING_OWNER:  EvAct_ChangeBuildingOwner (A_SIDE, A_ITEM, OBJ_ID); break;
  case ET_CHANGE_BUILDING_TYPE:   EvAct_ChangeBuildingType  (A_SIDE, A_ITEM, OBJ_ID); break;
  case ET_SET_BUILDING_FLAG:      EvAct_SetBuildingFlag     (A_SIDE, A_ENUM, A_VAL1, OBJ_ID); break;
  case ET_SET_BUILDING_PROPERTY:  EvAct_SetBuildingProperty (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VAL1, OBJ_ID); break;
  case ET_SELECT_BUILDING:        EvAct_SelectBuilding      (A_SIDE, A_BOOL, OBJ_ID); break;
  case ET_SHOW_BUILDING_DATA:     EvAct_ShowBuildingData    (A_SIDE, OBJ_ID); break;
  // Bullet manipulation
  case ET_SET_BULLET_PROPERTY:    EvAct_SetBulletProperty   (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VAL1, OBJ_ID); break;
  // Explosion manipulation
  case ET_SET_EXPLOSION_PROPERTY: EvAct_SetExplosionProperty(A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VAL1, OBJ_ID); break;
  // Crate manipulation
  case ET_REMOVE_CRATE:           EvAct_RemoveCrate         (OBJ_ID); break;
  case ET_PICKUP_CRATE:           EvAct_PickupCrate         (A_SIDE, OBJ_ID); break;
  case ET_SET_CRATE_PROPERTY:     EvAct_SetCrateProperty    (A_AMNT, A_ITEM, A_ENUM, A_VAL1, OBJ_ID); break;
  case ET_SHOW_CRATE_DATA:        EvAct_ShowCrateData       (OBJ_ID); break;
  // Tile manipulation
  case ET_CHANGE_TILE:            EvAct_ChangeTile          (A_ENUM, A_VAL1, COORD0); break;
  case ET_SET_TILE_ATTRIBUTE:     EvAct_SetTileAttribute    (A_ENUM, A_VAL1, COORD0); break;
  case ET_SET_TILE_PROPERTY:      EvAct_SetTileProperty     (A_AMNT, A_ITEM, A_ENUM, A_VAL1, COORD0); break;
  case ET_REVEAL_TILE:            EvAct_RevealTile          (A_AMNT, COORD0); break;
  case ET_HIDE_TILE:              EvAct_HideTile            (COORD0); break;
  case ET_SHOW_TILE_DATA:         EvAct_ShowTileData        (COORD0); break;
  // Orders
  case ET_ORDER_UNIT_MOVE:                GenerateUnitMoveOrder               (A_SIDE, COORD0);                         RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_DOCK_WITH_REFINERY:       GenerateDockWithRefineryOrder       (A_SIDE, OBJ_ID);                         RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_REPAIR_SELECTED_UNITS:    GenerateRepairSelectedUnitsOrder    (A_SIDE, OBJ_ID);                         RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_REPAIR_SINGLE_UNIT:       GenerateRepairSingleUnitOrder       (A_SIDE, OBJ_ID);                         break;
  case ET_ORDER_UNIT_ATTACK_UNIT:         GenerateUnitAttackUnitOrder         (A_ITEM, A_SIDE, OBJ_ID);                 RestoreUnitSelection(A_ITEM, A_BOOL); break;
  case ET_ORDER_UNIT_ATTACK_BUILDING:     GenerateUnitAttackBuildingOrder     (A_ITEM, A_SIDE, OBJ_ID);                 RestoreUnitSelection(A_ITEM, A_BOOL); break;
  case ET_ORDER_UNIT_ATTACK_TILE:         GenerateUnitAttackTileOrder         (A_SIDE, COORD0);                         RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_GUARD:               GenerateUnitGuardOrder              (A_SIDE);                                 RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_SCATTER:             GenerateUnitScatterOrder            (A_SIDE);                                 RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_RETREAT:             EvAct_OrderUnitRetreat              (A_SIDE);                                 RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_DEPLOY:              GenerateUnitDeployOrder             (A_SIDE, OBJ_ID);                         break;
  case ET_ORDER_BUILDING_ATTACK_UNIT:     GenerateBuildingAttackUnitOrder     (A_ITEM, A_SIDE, OBJ_ID);                 RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILDING_ATTACK_BUILDING: GenerateBuildingAttackBuildingOrder (A_ITEM, A_SIDE, OBJ_ID);                 RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILDING_SET_PRIMARY:     GenerateBuildingSetPrimaryOrder     (A_SIDE);                                 RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILDING_REPAIR:          GenerateBuildingRepairOrder         (A_SIDE, OBJ_ID);                         break;
  case ET_ORDER_BUILDING_SELL:            GenerateBuildingSellOrder           (A_SIDE, OBJ_ID);                         break;
  case ET_ORDER_STOP:                     GenerateStopOrder                   (A_SIDE);                                 RestoreUnitSelection(A_SIDE, A_BOOL); RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILD_BUILDING_PICK:      GenerateBuildBuildingPickOrder      (A_SIDE, A_ITEM);                         break;
  case ET_ORDER_BUILD_BUILDING_CANCEL:    EvAct_OrderBuildBuildingCancel      (A_SIDE, A_BOOL);                         break;
  case ET_ORDER_BUILD_PLACE_BUILDING:     EvAct_OrderBuildPlaceBuilding       (A_SIDE, COORD0);                         break;
  case ET_ORDER_BUILD_UNIT_PICK:          GenerateBuildUnitPickOrder          (A_SIDE, A_ITEM);                         break;
  case ET_ORDER_BUILD_UNIT_CANCEL:        EvAct_OrderBuildUnitCancel          (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ORDER_STARPORT_PICK:            EvAct_OrderStarportPick             (A_SIDE, A_ITEM);                         break;
  case ET_ORDER_STARPORT_UNPICK:          GenerateStarportUnpickOrder         (A_SIDE, A_ITEM);                         break;
  case ET_ORDER_STARPORT_PURCHASE:        GenerateStarportPurchaseOrder       (A_SIDE);                                 break;
  case ET_ORDER_STARPORT_CANCEL:          GenerateStarportCancelOrder         (A_SIDE);                                 break;
  case ET_ORDER_UPGRADE_PICK:             GenerateUpgradePickOrder            (A_SIDE, A_ITEM);                         break;
  case ET_ORDER_UPGRADE_CANCEL:           EvAct_OrderUpgradeCancel            (A_SIDE, A_BOOL);                         break;
  case ET_ORDER_SPECIAL_WEAPON:           GenerateSpecialWeaponOrder          (A_SIDE, A_ITEM, COORD0);                 break;
  // Variable operations
  case ET_SET_VARIABLE:                   EvAct_SetVariable                   (A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1);                     break;
  case ET_GET_VARIABLE:                   EvAct_GetVariable                   (A_ITEM, A_ENUM, A_BOOL);                                     break;
  case ET_SET_FLOAT_VARIABLE:             EvAct_SetFloatVariable              (A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1);                     break;
  case ET_CONVERT_VARIABLE:               EvAct_ConvertVariable               (A_ITEM, A_ENUM, A_BOOL);                                     break;
  case ET_DEBUG_VARIABLES:                EvAct_DebugVariables                (A_ITEM, A_ENUM, A_BOOL);                                     break;
  case ET_GET_RANDOM_VALUE:               EvAct_GetRandomValue                (A_ITEM, A_VAL1, A_VAL2);                                     break;
  case ET_GET_RANDOM_COORDS:              EvAct_GetRandomCoords               (COORD0, COORD1, A_ITEM);                                     break;
  case ET_GET_VALUE_FROM_LIST:            EvAct_GetValueFromList              (EV_IDX, A_AMNT, A_ITEM, A_ENUM, A_BOOL, (uint8_t *)e->data); break;
  case ET_GET_COORDS_FROM_LIST:           EvAct_GetCoordsFromList             (EV_IDX, A_AMNT, A_ITEM, A_ENUM, A_BOOL, (uint8_t *)e->data); break;
  case ET_GET_AREA_FROM_LIST:             EvAct_GetAreaFromList               (EV_IDX, A_AMNT, A_ITEM, A_ENUM, A_BOOL, (uint8_t *)e->data); break;
  case ET_GET_UNIT_COUNT:                 EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_BUILDING_COUNT:             EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_BULLET_COUNT:               EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_EXPLOSION_COUNT:            EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_CRATE_COUNT:                EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_TILE_COUNT:                 EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_SIDE_COUNT:                 EvAct_GetCount                      (A_AMNT);                                                     break;
  case ET_GET_SPICE_COUNT:                EvAct_GetSpiceCount                 (A_AMNT, COORD0);                                             break;
  case ET_GET_DAMAGE_COUNT:               EvAct_GetDamageCount                (A_AMNT, COORD0);                                             break;
  case ET_GET_UNIT_PROPERTY:              EvAct_GetObjectProperty             (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_BUILDING_PROPERTY:          EvAct_GetObjectProperty             (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_BULLET_PROPERTY:            EvAct_GetObjectProperty             (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_EXPLOSION_PROPERTY:         EvAct_GetObjectProperty             (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_CRATE_PROPERTY:             EvAct_GetCrateProperty              (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_TILE_PROPERTY:              EvAct_GetTileProperty               (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_SIDE_PROPERTY:              EvAct_GetSideProperty               (A_SIDE, A_AMNT, A_BOOL, A_VAL1);                             break;
  case ET_GET_AI_PROPERTY:                EvAct_GetAIProperty                 (A_SIDE, A_AMNT, A_BOOL, A_VAL1);                             break;
  case ET_GET_MEMORY_DATA:                EvAct_GetMemoryData                 (A_AMNT, A_BOOL, A_VAL1);                                     break;
  case ET_GET_UNIT_TEMPLATE_PROPERTY:     EvAct_GetUnitTemplateProperty       (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_BUILDING_TEMPLATE_PROPERTY: EvAct_GetBuildingTemplateProperty   (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_WEAPON_TEMPLATE_PROPERTY:   EvAct_GetWeaponTemplateProperty     (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_EXPLOSION_TEMPLATE_PROPERTY:EvAct_GetExplosionTemplateProperty  (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_ARMOUR_VALUE:               EvAct_GetArmourValue                (A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1);                     break;
  case ET_GET_SPEED_VALUE:                EvAct_GetSpeedValue                 (A_AMNT, A_ITEM, A_ENUM);                                     break;
  case ET_GET_GROUP_ID_VALUE:             EvAct_GetGroupIDValue               (A_AMNT, A_ITEM);                                             break;
  case ET_GET_UNIT_TYPE:                  EvAct_GetUnitType                   (A_SIDE, A_AMNT, A_ENUM, A_BOOL, (ObjectFilterStruct *)&e->data[1]);break;
  case ET_GET_BUILDING_TYPE:              EvAct_GetBuildingType               (A_SIDE, A_AMNT, A_ENUM, A_BOOL, (ObjectFilterStruct *)&e->data[1]);break;
  case ET_GET_GAME_TICKS:                 EvAct_GetGameTicks                  (A_BOOL);                                                     break;
  case ET_GET_MY_SIDE_ID:                 EvAct_GetMySideId                   (A_BOOL);                                                     break;
  case ET_GET_DIFFICULTY:                 EvAct_GetDifficulty                 (A_BOOL);                                                     break;
  case ET_GET_RULE:                       EvAct_GetRule                       (A_ITEM, A_BOOL);                                             break;
  case ET_GET_DIPLOMACY:                  EvAct_GetDiplomacy                  (A_SIDE, A_ITEM, A_BOOL);                                     break;
  case ET_GET_TECH:                       EvAct_GetTech                       (A_SIDE, A_BOOL);                                             break;
  case ET_GET_HOUSE_ID:                   EvAct_GetHouseId                    (A_SIDE, A_BOOL);                                             break;
  case ET_GET_CREDITS:                    EvAct_GetCredits                    (A_SIDE, A_ENUM, A_BOOL);                                     break;
  case ET_GET_POWER:                      EvAct_GetPower                      (A_SIDE, A_ENUM, A_BOOL);                                     break;
  case ET_GET_BUILDING_UPGRADES:          EvAct_GetBuildingUpgrades           (A_SIDE, A_ITEM, A_BOOL);                                     break;
  case ET_GET_STARPORT_STOCK:             EvAct_GetStarportStock              (A_SIDE, A_ITEM, A_BOOL);                                     break;
  case ET_GET_STARPORT_COST:              EvAct_GetStarportCost               (A_SIDE, A_ITEM, A_BOOL);                                     break;
  case ET_GET_STARPORT_PICK:              EvAct_GetStarportPick               (A_SIDE, A_ITEM, A_BOOL);                                     break;
  case ET_GET_BUILDING_QUEUE_STATE:       EvAct_GetBuildingQueueState         (A_SIDE, A_AMNT, A_ITEM, A_BOOL);                             break;
  case ET_GET_UNIT_QUEUE_STATE:           EvAct_GetUnitQueueState             (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VAL1);             break;
  case ET_GET_UPGRADE_QUEUE_STATE:        EvAct_GetUpgradeQueueState          (A_SIDE, A_AMNT, A_ITEM, A_BOOL);                             break;
  case ET_GET_SPICE_HARVESTED:            EvAct_GetSpiceHarvested             (A_SIDE, A_BOOL);                                             break;
  case ET_GET_UNITS_BUILT:                EvAct_GetUnitsBuilt                 (A_SIDE, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_BUILDINGS_BUILT:            EvAct_GetBuildingsBuilt             (A_SIDE, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_UNITS_LOST:                 EvAct_GetUnitsLost                  (A_SIDE, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_BUILDINGS_LOST:             EvAct_GetBuildingsLost              (A_SIDE, A_BOOL);                                             break;
  case ET_GET_UNITS_KILLED:               EvAct_GetUnitsKilled                (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_BUILDINGS_KILLED:           EvAct_GetBuildingsKilled            (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_MOUSE_POSITION:             EvAct_GetMousePosition              (A_ITEM, A_ENUM);                                             break;
  case ET_GET_KEYBOARD_MOUSE_STATE:       EvAct_GetKeyboardMouseState         (A_ITEM, A_ENUM);                                             break;
  case ET_GET_UNIT_UNDER_CURSOR:          EvAct_GetUnitUnderCursor            (A_ITEM, A_ENUM, A_BOOL, A_VAL1);                             break;
  case ET_GET_BUILDING_UNDER_CURSOR:      EvAct_GetBuildingUnderCursor        (A_ITEM, A_ENUM, A_BOOL);                                     break;
  case ET_GET_SIDEBAR_BUTTON_UNDER_CURSOR:EvAct_GetSidebarButtonUnderCursor   (A_ITEM, A_ENUM, A_BOOL);                                     break;
  case ET_GET_GAME_INTERFACE_DATA:        EvAct_GetGameInterfaceData          (A_AMNT, A_ITEM, A_ENUM);                                     break;
  case ET_GET_OBJECT_POSITION:            EvAct_GetObjectPosition             (A_SIDE, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_GET_DIRECTION:                  EvAct_GetDirection                  (A_ITEM, A_ENUM, A_BOOL);                                     break;
  case ET_GET_POSITION_ON_CIRCLE:         EvAct_GetPositionOnCircle           (A_AMNT, A_BOOL, A_VAL1, A_VAL2);                             break;
  case ET_GET_NEAREST_BUILDING_TILE:      EvAct_GetNearestBuildingTile        (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);                     break;
  case ET_GET_DISTANCE:                   EvAct_GetDistance                   (A_AMNT, A_ITEM, A_ENUM, A_BOOL);                             break;
  case ET_CHECK_DISTANCE:                 EvAct_CheckDistance                 (A_AMNT, A_ITEM, A_VAL1, A_VAL2);                             break;
  // Blocks
  case ET_CALLABLE_BLOCK_START:                                                                                                     break;
  case ET_HOOK_BLOCK_START:                                                                                                         break;
  case ET_EXECUTE_BLOCK:                  EvAct_ExecuteBlock                  (EV_IDX, A_VAL1);                                     break;
  case ET_EXIT_FROM_BLOCK:                exit_count = 1;                                                                           break;
  // Conditional expression
  case ET_IF:                             EvAct_If                            (EV_IDX, A_AMNT, A_ITEM, A_ENUM, (CondExprData *)&e->data[1]);break;
  case ET_ELSE_IF:                        DebugFatal("event-core.c", "Invalid ELSE IF event (event %d)", e->event_index);           break;
  case ET_ELSE:                           DebugFatal("event-core.c", "Invalid ELSE event (event %d)", e->event_index);              break;
  // Loops
  case ET_LOOP_WHILE:                     EvAct_LoopWhile                     (EV_IDX, (CondExprData *)&e->data[1]);                break;
  case ET_LOOP_VALUES_FROM_RANGE:         EvAct_LoopValuesFromRange           (EV_IDX, A_ITEM, A_VAL1, A_VAL2);                     break;
  case ET_LOOP_COORDS_FROM_AREA:          EvAct_LoopCoordsFromArea            (EV_IDX, COORD0, COORD1, A_ITEM);                     break;
  case ET_LOOP_VALUES_FROM_LIST:          EvAct_LoopValuesFromList            (EV_IDX, A_AMNT, A_ITEM, (uint8_t *)e->data);         break;
  case ET_LOOP_COORDS_FROM_LIST:          EvAct_LoopCoordsFromList            (EV_IDX, A_AMNT, A_ITEM, (uint8_t *)e->data);         break;
  case ET_LOOP_AREAS_FROM_LIST:           EvAct_LoopAreasFromList             (EV_IDX, A_AMNT, A_ITEM, (uint8_t *)e->data);         break;
  case ET_LOOP_UNITS:                     EvAct_LoopObject                    (EV_IDX, A_AMNT, A_ITEM, A_SIDE, OBJ_ID);             break;
  case ET_LOOP_BUILDINGS:                 EvAct_LoopObject                    (EV_IDX, A_AMNT, A_ITEM, A_SIDE, OBJ_ID);             break;
  case ET_LOOP_BULLETS:                   EvAct_LoopObject                    (EV_IDX, A_AMNT, A_ITEM, A_SIDE, OBJ_ID);             break;
  case ET_LOOP_EXPLOSIONS:                EvAct_LoopObject                    (EV_IDX, A_AMNT, A_ITEM, A_SIDE, OBJ_ID);             break;
  case ET_LOOP_CRATES:                    EvAct_LoopItem                      (EV_IDX, A_AMNT, OBJ_ID);                             break;
  case ET_LOOP_TILES:                     EvAct_LoopTiles                     (EV_IDX, A_AMNT, COORD0);                             break;
  case ET_LOOP_SIDES:                     EvAct_LoopItem                      (EV_IDX, A_AMNT, OBJ_ID);                             break;
  case ET_BREAK_LOOP:                     break_count = 1;                                                                          break;
  case ET_CONTINUE_LOOP:                  continue_count = 1;                                                                       break;
  // End
  case ET_END:                            DebugFatal("event-core.c", "Invalid END event (event %d)", e->event_index);               break;
  default:
    DebugFatal("event-core.c", "Unknown event type %d (event %d)", e->event_type, e->event_index);
  }
}

int ExecuteEventHook(int hook_type, int num_vars, int var0, int var1, int var2, int var3, int var4)
{
  if (event_hooks[hook_type] == -1)
    return var0;
  // Set variables
  if (num_vars >= 1)
    SetVariableValue(0, var0);
  if (num_vars >= 2)
    SetVariableValue(1, var1);
  if (num_vars >= 3)
    SetVariableValue(2, var2);
  if (num_vars >= 4)
    SetVariableValue(3, var3);
  if (num_vars >= 5)
    SetVariableValue(4, var4);
  // Execute hook
  ExecuteEventBlock(event_hooks[hook_type], EBT_BLOCK);
  return GetVariableValue(0);
}

int GetVariableValueOrConst(int flags, int flag_index, int var_index_or_const)
{
  if (flags & (1 << flag_index))
    return gEventVariableArray[var_index_or_const].value;
  else
    return var_index_or_const;
}

void SetVariableValue(int var_index, int value)
{
  EventVariable *v = &gEventVariableArray[var_index];
  if (v->ticks != gGameTicks)
  {
    v->old_value = v->value;
    v->ticks = gGameTicks;
  }
  v->value = value;
}

int GetVariableValue(int var_index)
{
  return gEventVariableArray[var_index].value;
}
