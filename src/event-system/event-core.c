#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
#include "event-utils.h"
#include "event-core.h"
#include "event-actions.h"
#include "event-conditions.h"
#include "../spawner/short-game.h"

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

// Profiling variables

int profiler_executed_events_cur_tick;
int profiler_executed_events[MAX_EVENTS + 1];
int profiler_filter_check_cur_tick;
int profiler_filter_check[MAX_EVENTS + 1];

// Custom implementation of function HandleConditionsAndEvents
DETOUR(0x00453B90, 0x004540DC, _Mod__HandleConditionsAndEvents);
void Mod__HandleConditionsAndEvents()
{
  CheckBuildingsLeft();
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
  unsigned char a1; // [esp+14h] [ebp-44h]
  signed int v34; // [esp+18h] [ebp-40h]

  a1 = (unsigned char)(gGameTicks & 0xF) >> 1;
  v30 = 0;
  v29 = -1;
  EvaluateIfBuildingsOrUnitsExistForSide(a1, gGameTicks & 1);

  // Debug features
  if (DebugFeatures & DEBUGFEATURE_INSTANT_BUILD)
  {
    CSide *side = GetSide(gSideId);
    if (side->__BuildingBuildQueue.__type != -1)
      side->__BuildingBuildQueue.__build_progress = 0x5A00;
    if (side->__BuildingUpgradeQueue.__type != -1)
      side->__BuildingUpgradeQueue.__build_progress = 0x59FF;
    for (int i = 0; i < MAX_UNIT_BUILD_QUEUES; i++)
      if (side->__UnitBuildQueue[i].__type != -1 && side->__UnitBuildQueue[i].__build_progress < 0x5A00)
        side->__UnitBuildQueue[i].__build_progress = 0x5A00;
  }
  if (DebugFeatures & DEBUGFEATURE_SHOW_SELECTED_UNIT_DATA)
  {
    for (int i = 0; i < MAX_SIDES; i++)
      for (Unit *u = GetSide(i)->__FirstUnitPtr; u; u = u->Next)
        if (u->__IsSelected)
        {
          EvAct_ShowUnitData(-1, i, u->MyIndex);
          break;
        }
  }
  if (DebugFeatures & DEBUGFEATURE_SHOW_SELECTED_BUILDING_DATA)
  {
    for (int i = 0; i < MAX_SIDES; i++)
      for (Building *b = GetSide(i)->__FirstBuildingPtr; b; b = b->Next)
        if (b->__IsSelected)
        {
          EvAct_ShowBuildingData(-1, i, b->MyIndex);
          break;
        }
  }

  if (!MapScriptExists && (_IsMultiplayer || gGameType == GAME_SKIRMISH))
  {
DEFAULT_WIN_LOSE_EVENTS:
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
    if ( _gUnitsExist[(unsigned char)gSideId] || _gBuildingsExist[(unsigned char)gSideId] )
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

    if (SpawnerActive)
    {
        if (UseDefaultWinLoseEvents || (!_gEventCount && gGameType != GAME_CAMPAIGN))
        {
            goto DEFAULT_WIN_LOSE_EVENTS;
        }
    }
    
    ExecuteEventsInRange(0, _gEventCount, EBT_GLOBAL);

    // Show number of executed events in current tick
    if (DebugFeatures & DEBUGFEATURE_ENABLE_EVENT_PROFILING)
    {
      char s[200];
      sprintf(s, "Tick %d executed events %d filter checks %d", gGameTicks, profiler_executed_events_cur_tick, profiler_filter_check_cur_tick);
      QueueMessage(s, 0);
    }

    // Reset number of executed events
    profiler_executed_events_cur_tick = 0;
    profiler_filter_check_cur_tick = 0;
  }
}

#define C0  coord_x[0], coord_y[0]
#define C1  coord_x[1], coord_y[1]
#define A0  args[0]
#define A1  args[1]
#define A2  args[2]
#define A3  args[3]
#define A4  args[4]
#define A5  args[5]
#define A6  args[6]
#define FL condition->float_val

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
    case CT_BUILDINGEXISTS:     return Cond_BuildingExists  (A0, A1);
    case CT_UNITEXISTS:         return Cond_UnitExists      (A0, A2);
    case CT_INTERVAL:           return Cond_Interval        (A1, A3, A4, A5, condition);
    case CT_TIMER:              return Cond_Timer           (A1, A2, A4, A5, condition);
    case CT_CASUALTIES:         return Cond_Casualties      (A0, A5, FL);
    case CT_BASEDESTROYED:      return !_gBuildingsExist[A0];
    case CT_UNITSDESTROYED:     return !_gUnitsExist[A0];
    case CT_REVEALED:           return Cond_Revealed        (C0, A5, condition);
    case CT_CREDITS:            return Cond_Credits         (A0, A1, A2, A5);
    case CT_FLAG:               return A5 != 0;
    case CT_RANDOMCHANCE:       return Cond_RandomChance    (A3, A4, A5, A6, condition);
    case CT_RANDOMINTERVAL:     return Cond_RandomInterval  (A1, A2, A3, A4, A5, condition);
    case CT_DIPLOMACY:          return _gDiplomacy[A0][A1] == A2;
    case CT_DIFFICULTY:         return gDifficultyLevel == A5;
    // Environment checking
    case CT_CHECKUNITS:         return Cond_CheckUnits      (condition);
    case CT_CHECKBUILDINGS:     return Cond_CheckBuildings  (condition);
    case CT_CHECKCRATES:        return Cond_CheckCrates     (condition);
    case CT_CHECKTILES:         return Cond_CheckTiles      (condition);
    case CT_SPICE_IN_AREA:      return Cond_SpiceInArea     (C0, C1, A5);
    case CT_DAMAGE_IN_AREA:     return Cond_DamageInArea    (C0, C1, A1, A2, A5);
    // Side related
    case CT_POWER:              return Cond_Power           (A0, A1, A3, A5);
    case CT_BUILDING_UPGRADES:  return CompareValue         (GetSide(A0)->__BuildingGroupUpgradeCount[A1], A5, !A3);
    case CT_STARPORT_STOCK:     return Cond_StarportStock   (A0, A1, A2, A3, A5);
    case CT_STARPORT_COST:      return Cond_StarportCost    (A0, A1, A3, A5);
    case CT_STARPORT_PICK:      return Cond_StarportPick    (A0, A1, A2, A3, A5);
    case CT_STARPORT_DELIVERY:  return GetSide(A0)->__StarportDeliveryInProgress;
    case CT_BUILDING_ICON:      return Cond_BuildingIcon    (A0, A1, A2, A4);
    case CT_UNIT_ICON:          return Cond_UnitIcon        (A0, A1, A2, A3, A4);
    case CT_UPGRADE_ICON:       return Cond_UpgradeIcon     (A0, A2, A4);
    case CT_SPICE_HARVESTED:    return CompareValue         (GetSide(A0)->__SpiceHarvested, A5, !A3);
    case CT_UNITS_BUILT:        return Cond_UnitsBuilt      (A0, A1, A2, A3, A5);
    case CT_BUILDINGS_BUILT:    return Cond_BuildingsBuilt  (A0, A1, A2, A3, A5);
    case CT_UNITS_LOST:         return Cond_UnitsLost       (A0, A1, A2, A3, A5);
    case CT_BUILDINGS_LOST:     return Cond_BuildingsLost   (A0, A3, A5);
    case CT_UNITS_KILLED:       return Cond_UnitsKilled     (A0, A1, A2, A3, A4, A5);
    case CT_BUILDINGS_KILLED:   return Cond_BuildingsKilled (A0, A1, A2, A3, A4, A5);
    case CT_SIDE_PROPERTY:      return CompareDataValue     ((char *)GetSide(A0),    A1, A3, A5, !A4);
    // AI related
    case CT_AI_PROPERTY:        return CompareDataValue     ((char *)&_gAIArray[A0], A1, A3, A5, !A4);
    // Memory related
    case CT_MEMORY_VALUE:       return CompareDataValue     (NULL,                   A1, A3, A5, !A4);
    // Variable related
    case CT_VARIABLE_VALUE:     return Cond_VariableValue   (A1, A4, A5);
    case CT_VARIABLE_CHANGED:   return Cond_VariableChanged (A1);
    default:
      DebugFatal(EVENT_ERROR, "Unknown condition type %d (condition %d)", condition->condition_type, condition_index);
  }
  return false;  
}

#undef C0
#undef C1
#undef A0
#undef A1
#undef A2
#undef A3
#undef A4
#undef A5
#undef A6
#undef FL

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
  DebugFatal(EVENT_ERROR, "Missing END event for block-start event %d", event_index);
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
          DebugFatal(EVENT_ERROR, "Exit event cannot be used outside of a Block (event %d)", event_index);
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
          DebugFatal(EVENT_ERROR, "Break event cannot be used outside of a Loop (event %d)", event_index);
        else
          break;
      }
      if (continue_count)
      {
        if (block_type == EBT_GLOBAL || block_type == EBT_BLOCK)
          DebugFatal(EVENT_ERROR, "Continue event cannot be used outside of a Loop (event %d)", event_index);
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

// clear original Mission::ExecuteEvent to make sure we get warnings about conflicting patches
CLEAR_INT(0x00454120, 0x0045448B);

void ExecuteEvent(int event_index)
{
  EventData *event = &_gEventArray[event_index];
  int et = event->event_type;
  // Check if event is blocked and block it if auto-block is set
  if (event->event_flags & EVENTFLAG_BLOCKED)
    return;
  if (event->event_flags & EVENTFLAG_AUTO_BLOCK)
    event->event_flags |= EVENTFLAG_BLOCKED;
  // Count this execution in profiler
  profiler_executed_events_cur_tick++;
  profiler_executed_events[event_index]++;
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
      e.coord_x[0] = GetVariableValue(event_index, event->filter_skip);
      e.coord_y[0] = GetVariableValue(event_index, event->filter_skip + 1);
    }
    else
    {
      e.object_index = GetVariableValue(event_index, event->filter_skip);
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
      SetVariableValue(event_index, e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < MAX_SIDES; side_id++)
    {
      if ((arg_side_id != MAX_SIDES) && (arg_side_id != side_id))
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
        if (CheckIfUnitMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], unit, side_id))
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
      SetVariableValue(event_index, e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < MAX_SIDES; side_id++)
    {
      if ((arg_side_id != MAX_SIDES) && (arg_side_id != side_id))
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
        if (CheckIfBuildingMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], building, side_id))
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
      SetVariableValue(event_index, e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < MAX_SIDES; side_id++)
    {
      if ((arg_side_id != MAX_SIDES) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      // Process all bullets
      for (Bullet *bullet = side->__FirstBulletPtr; bullet; bullet = bullet->Next)
      {
        if (CheckIfBulletMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], bullet))
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
      SetVariableValue(event_index, e.args[0], 0);
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < MAX_SIDES; side_id++)
    {
      if ((arg_side_id != MAX_SIDES) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      // Process all explosions
      for (Explosion *explosion = side->__FirstExplosionPtr; explosion; explosion = explosion->Next)
      {
        if (CheckIfExplosionMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], explosion))
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
      SetVariableValue(event_index, e.args[0], 0);
    int affected = 0;
    // Process all crates
    for (int i = 0; i < MAX_CRATES; i++)
    {
      if (CheckIfCrateMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], &gCrates[i]))
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
      SetVariableValue(event_index, e.args[0], 0);
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
        if (CheckIfTileMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y))
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
      SetVariableValue(event_index, e.args[0], 0);
    int affected = 0;
    // Process all sides
    for (int i = 0; i < MAX_SIDES; i++)
    {
      if (CheckIfSideMatchesFilter(event_index, (ObjectFilterStruct *)&e.data[1], i))
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

#define ID  e->event_index
#define C0  e->coord_x[0], e->coord_y[0]
#define C1  e->coord_x[1], e->coord_y[1]
#define C2  e->coord_x[2], e->coord_y[2]
#define C3  e->coord_x[3], e->coord_y[3]
#define A0  e->args[1]
#define A1  e->args[0]
#define A2  e->args[2]
#define A3  e->args[3]
#define A4  e->args[4]
#define A5  e->args[5]
#define A6  *(int *)&e->data[1]
#define OBJ e->object_index

#define VALUEOPERATION(data) data = ValueOperation(ID, data, A5, A3)

void ExecuteEventAction(EventContext *e)
{
  switch ( e->event_type )
  {
  // Vanilla / General
  case ET_REINFORCEMENT:                  EvAct_AddDelivery                   (ID, C0, A0, A1, A2, A4, A5, DELIVERYTYPE_REINFORCE, e->data); break;
  case ET_STARPORT_DELIVERY:              EvAct_AddDelivery                   (ID, C0, A0, A1, A2, A4, A5, DELIVERYTYPE_STARPORT,  e->data); break;
  case ET_ALLEGIANCE:                     EvAct_SetDiplomacy                  (ID, A0, A2, A3, A4);                         break;
  case ET_LEAVE:                          CSide__BlowupAll_surrender(GetSide(A0));                                          break;
  case ET_BESERK:                         _gAIArray[A0].__GoBeserk_OtherStates = 1;                                         break;
  case ET_PLAYSOUND:                      EvAct_PlaySound                     (ID, C0, A5, A3, A4);                         break;
  case ET_SETBUILDRATE:                   VALUEOPERATION(_gAIArray[A0].UnitBuildRate);                                      break;
  case ET_SETATTACKBUILDINGRATE:          VALUEOPERATION(_gAIArray[A0].TimeBetweenBuildingAttacks);                         break;
  case ET_SETCASH:                        EvAct_SetCash                       (ID, A0, A3, A5);                             break;
  case ET_SETTECH:                        EvAct_SetTech                       (ID, A0, A3, A4, A5);                         break;
  case ET_WIN:                            if (SpawnerActive && _IsMultiplayer && MeIsSpectator) _GameOver = 1; if ( !gLose ) gWin = 1; break;
  case ET_LOSE:                           if ( !gWin ) { gLose = 1; if (SpawnerActive && _IsMultiplayer) _GameOver = 1; }   break;
  case ET_SWITCH_MY_SIDE:                 EvAct_SwitchMySide                  (ID, A0, A3, A4);                             break;
  case ET_HIDE_MAP:                       EvAct_HideMap                       ();                                           break;
  case ET_REVEAL:                         EvAct_RevealMap                     (ID, C0, A1);                                 break;
  case ET_SETTIMER:                       VALUEOPERATION(_gTimerValue);                                                     break;
  case ET_HIDETIMER:                      _gTimerValue = -1;                                                                break;
  case ET_SHOWMESSAGE:                    EvAct_ShowMessage                   (ID, C0, A0, A1, A2, A3, A4, A5, (ShowMessageEventData *)&e->data[1]); break;
  case ET_UNIT_SPAWN:                     EvAct_UnitSpawn                     (ID, C0, A0, A1, A3, A5, e->data);            break;
  case ET_SET_FLAG:                       _gConditionArray[A0].val3 = A5;                                                   break;
  case ET_UN_BLOCK_EVENT:                 EvAct_UnBlockEvent                  (ID, A4, A5);                                 break;
  case ET_PLAY_MUSIC:                     EvAct_PlayMusic                     (e->data);                                    break;
  case ET_DAMAGE_TILES:                   EvAct_DamageTiles                   (ID, C0, C2, C3, A0, A2, A3, A4);             break;
  case ET_ADD_UNIT:                       EvAct_AddUnit                       (ID, C0, A0, A1, A2, A3, A4, A5, A6);         break;
  case ET_ADD_BUILDING:                   EvAct_AddBuilding                   (ID, C0, A0, A1, A2, A3, A4, A5, A6);         break;
  case ET_ADD_BULLET:                     EvAct_AddBullet                     (ID, C0, C1, C2, C3, A0, A2, A3, A4, A5, A6); break;
  case ET_ADD_EXPLOSION:                  EvAct_AddExplosion                  (ID, C0, C2, C3, A0, A2, A3, A4, A5, A6);     break;
  case ET_ADD_CRATE:                      EvAct_AddCrate                      (ID, C0, A0, A1, A2, A3, A5);                 break;
  case ET_ADD_CONCRETE:                   EvAct_AddConcrete                   (ID, C0, C1, A0, A5);                         break;
  case ET_SPICE_BLOOM:                    EvAct_SpiceBloom                    (ID, C0, A1, A3, A4);                         break;
  case ET_SHAKE_SCREEN:                   _ScreenShakes = A5;                                                               break;
  case ET_CHANGE_VIEWPORT:                EvAct_ChangeViewport                (C0, A3, A4);                                 break;
  case ET_CHANGE_MAP_BLOCK:               EvAct_ChangeMapBlock                (ID, C0, C1, A3, (uint16_t *)&e->data[1]);    break;
  case ET_TRANSFORM_TILES:                EvAct_TransformTiles                (ID, A1, A3, (uint16_t *)&e->data[1]);        break;
  case ET_ADD_BUILDING_DESTRUCT:          EvAct_AddBuildingDestruct           (ID, C0, A0, A2);                             break;
  case ET_ADD_HOMING_BULLET:              EvAct_AddHomingBullet               (ID, C0, C1, A0, A1, A2, A3, A4, A5, A6);     break;
  case ET_ACTIVATE_TIMER:                 EvAct_ActivateTimer                 (ID, A5);                                     break;
  case ET_REMOVE_MESSAGE:                 EvAct_RemoveMessage                 (A0, A1, A2);                                 break;
  case ET_SET_MESSAGE_COLOR:              EvAct_SetMessageColor               (ID, A0, A1, A2, A3, A5, A6);                 break;
  case ET_SET_TOOLTIP:                    EvAct_SetTooltip                    (ID, A2, A3, A5, (ShowMessageEventData *)&e->data[1]); break;
  // Side manipulation
  case ET_TRANSFER_CREDITS:               EvAct_TransferCredits               (ID, A0, A3, A5);                             break;
  case ET_SET_BUILDING_UPGRADES:          EvAct_SetBuildingUpgrades           (ID, A0, A2, A3, A5);                         break;
  case ET_SET_STARPORT_STOCK:             VALUEOPERATION(GetSide(A0)->__StarportUnitTypeStock[A2]);                         break;
  case ET_SET_STARPORT_COST:              EvAct_SetStarportCost               (ID, A0, A2, A3, A4, A5);                     break;
  case ET_CHANGE_STARPORT_UNIT:           GetSide(A0)->__StarportIcons[A1] = A2;                                            break;
  case ET_SHOW_SIDE_DATA:                 EvAct_ShowSideData                  (ID, A0, A5);                                 break;
  // AI manipulation
  case ET_SET_AI_PROPERTY:                EvAct_SetAIProperty                 (ID, A0, A1, A3, A5, A6);                     break;
  case ET_SHOW_AI_DATA:                   EvAct_ShowAIData                    (ID, A0, A5);                                 break;
  // Memory manipulation
  case ET_SET_MEMORY_DATA:                EvAct_SetMemoryData                 (ID, A1, A3, A5, A6);                         break;
  case ET_SHOW_MEMORY_DATA:               EvAct_ShowMemoryData                (ID, A5);                                     break;
  // Unit manipulation
  case ET_DESTROY_UNIT:                   EvAct_DestroyUnit                   (ID, A0, A4, OBJ);                            break;
  case ET_DAMAGE_HEAL_UNIT:               EvAct_DamageHealUnit                (ID, A0, A3, A4, A5, OBJ);                    break;
  case ET_CHANGE_UNIT_OWNER:              ChangeUnitOwner                     (A0, A2, OBJ, 0);                             break;
  case ET_CHANGE_UNIT_TYPE:               EvAct_ChangeUnitType                (ID, A0, A2, A4, OBJ);                        break;
  case ET_SET_UNIT_FLAG:                  EvAct_SetUnitFlag                   (ID, A0, A3, A5, OBJ);                        break;
  case ET_SET_UNIT_PROPERTY:              EvAct_SetUnitProperty               (ID, A0, A1, A2, A3, A5, OBJ);                break;
  case ET_SELECT_UNIT:                    EvAct_SelectUnit                    (ID, A0, A4, OBJ);                            break;
  case ET_AIRLIFT_UNIT:                   EvAct_AirliftUnit                   (ID, A0, C0, A4, OBJ);                        break;
  case ET_SHOW_UNIT_DATA:                 EvAct_ShowUnitData                  (ID, A0, OBJ);                                break;
  // Building manipulation
  case ET_DESTROY_BUILDING:               EvAct_DestroyBuilding               (ID, A0, A4, OBJ);                            break;
  case ET_DAMAGE_HEAL_BUILDING:           EvAct_DamageHealBuilding            (ID, A0, A3, A4, A5, OBJ);                    break;
  case ET_CHANGE_BUILDING_OWNER:          EvAct_ChangeBuildingOwner           (ID, A0, A2, OBJ);                            break;
  case ET_CHANGE_BUILDING_TYPE:           EvAct_ChangeBuildingType            (ID, A0, A2, OBJ);                            break;
  case ET_SET_BUILDING_FLAG:              EvAct_SetBuildingFlag               (ID, A0, A3, A5, OBJ);                        break;
  case ET_SET_BUILDING_PROPERTY:          EvAct_SetBuildingProperty           (ID, A0, A1, A2, A3, A5, OBJ);                break;
  case ET_SELECT_BUILDING:                EvAct_SelectBuilding                (ID, A0, A4, OBJ);                            break;
  case ET_SHOW_BUILDING_DATA:             EvAct_ShowBuildingData              (ID, A0, OBJ);                                break;
  // Bullet manipulation
  case ET_SET_BULLET_PROPERTY:            EvAct_SetBulletProperty             (ID, A0, A1, A2, A3, A5, OBJ);                break;
  // Explosion manipulation
  case ET_SET_EXPLOSION_PROPERTY:         EvAct_SetExplosionProperty          (ID, A0, A1, A2, A3, A5, OBJ);                break;
  // Crate manipulation
  case ET_REMOVE_CRATE:                   EvAct_RemoveCrate                   (ID, OBJ);                                    break;
  case ET_PICKUP_CRATE:                   EvAct_PickupCrate                   (ID, A0, OBJ);                                break;
  case ET_SET_CRATE_PROPERTY:             EvAct_SetCrateProperty              (ID, A1, A2, A3, A5, OBJ);                    break;
  case ET_SHOW_CRATE_DATA:                EvAct_ShowCrateData                 (ID, OBJ);                                    break;
  // Tile manipulation
  case ET_CHANGE_TILE:                    EvAct_ChangeTile                    (ID, A3, A5, C0);                             break;
  case ET_SET_TILE_ATTRIBUTE:             EvAct_SetTileAttribute              (ID, A3, A5, C0);                             break;
  case ET_SET_TILE_PROPERTY:              EvAct_SetTileProperty               (ID, A1, A2, A3, A5, C0);                     break;
  case ET_REVEAL_TILE:                    EvAct_RevealTile                    (ID, A1, C0);                                 break;
  case ET_HIDE_TILE:                      EvAct_HideTile                      (ID, C0);                                     break;
  case ET_SHOW_TILE_DATA:                 EvAct_ShowTileData                  (ID, C0);                                     break;
  // Orders
  case ET_ORDER_UNIT_MOVE:                GenerateUnitMoveOrder               (A0, C0);       RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_DOCK_WITH_REFINERY:       GenerateDockWithRefineryOrder       (A0, OBJ);      RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_REPAIR_SELECTED_UNITS:    GenerateRepairSelectedUnitsOrder    (A0, OBJ);      RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_REPAIR_SINGLE_UNIT:       GenerateRepairSingleUnitOrder       (A0, OBJ);                                    break;
  case ET_ORDER_UNIT_ATTACK_UNIT:         GenerateUnitAttackUnitOrder         (A2, A0, OBJ);  RestoreUnitSelection(A2, A4); break;
  case ET_ORDER_UNIT_ATTACK_BUILDING:     GenerateUnitAttackBuildingOrder     (A2, A0, OBJ);  RestoreUnitSelection(A2, A4); break;
  case ET_ORDER_UNIT_ATTACK_TILE:         GenerateUnitAttackTileOrder         (A0, C0);       RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_UNIT_GUARD:               GenerateUnitGuardOrder              (A0);           RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_UNIT_SCATTER:             GenerateUnitScatterOrder            (A0);           RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_UNIT_RETREAT:             EvAct_OrderUnitRetreat              (ID, A0);       RestoreUnitSelection(A0, A4); break;
  case ET_ORDER_UNIT_DEPLOY:              GenerateUnitDeployOrder             (A0, OBJ);                                    break;
  case ET_ORDER_BUILDING_ATTACK_UNIT:     GenerateBuildingAttackUnitOrder     (A2, A0, OBJ);  RestoreBuildingSelection(A0, A4); break;
  case ET_ORDER_BUILDING_ATTACK_BUILDING: GenerateBuildingAttackBuildingOrder (A2, A0, OBJ);  RestoreBuildingSelection(A0, A4); break;
  case ET_ORDER_BUILDING_SET_PRIMARY:     GenerateBuildingSetPrimaryOrder     (A0);           RestoreBuildingSelection(A0, A4); break;
  case ET_ORDER_BUILDING_REPAIR:          GenerateBuildingRepairOrder         (A0, OBJ);                                    break;
  case ET_ORDER_BUILDING_SELL:            GenerateBuildingSellOrder           (A0, OBJ);                                    break;
  case ET_ORDER_STOP:                     GenerateStopOrder                   (A0);           RestoreUnitSelection(A0, A4); RestoreBuildingSelection(A0, A4); break;
  case ET_ORDER_BUILD_BUILDING_PICK:      EvAct_OrderBuildBuildingPick        (ID, A0, A2);                                 break;
  case ET_ORDER_BUILD_BUILDING_CANCEL:    EvAct_OrderBuildBuildingCancel      (ID, A0, A4);                                 break;
  case ET_ORDER_BUILD_PLACE_BUILDING:     EvAct_OrderBuildPlaceBuilding       (ID, A0, C0);                                 break;
  case ET_ORDER_BUILD_UNIT_PICK:          GenerateBuildUnitPickOrder          (A0, A2);                                     break;
  case ET_ORDER_BUILD_UNIT_CANCEL:        EvAct_OrderBuildUnitCancel          (ID, A0, A1, A2, A3, A4);                     break;
  case ET_ORDER_STARPORT_PICK:            EvAct_OrderStarportPick             (ID, A0, A2);                                 break;
  case ET_ORDER_STARPORT_UNPICK:          GenerateStarportUnpickOrder         (A0, A2);                                     break;
  case ET_ORDER_STARPORT_PURCHASE:        GenerateStarportPurchaseOrder       (A0);                                         break;
  case ET_ORDER_STARPORT_CANCEL:          GenerateStarportCancelOrder         (A0);                                         break;
  case ET_ORDER_UPGRADE_PICK:             GenerateUpgradePickOrder            (A0, A2);                                     break;
  case ET_ORDER_UPGRADE_CANCEL:           EvAct_OrderUpgradeCancel            (ID, A0, A4);                                 break;
  case ET_ORDER_SPECIAL_WEAPON:           GenerateSpecialWeaponOrder          (A0, A2, C0);                                 break;
  // Game modifications
  case ET_SET_UNIT_TEMPLATE_PROPERTY:     EvAct_SetUnitTemplateProperty       (ID, A1, A2, A3, A4, A5);                     break;
  case ET_SET_BUILDING_TEMPLATE_PROPERTY: EvAct_SetBuildingTemplateProperty   (ID, A1, A2, A3, A4, A5);                     break;
  case ET_SET_WEAPON_TEMPLATE_PROPERTY:   EvAct_SetWeaponTemplateProperty     (ID, A1, A2, A3, A4, A5);                     break;
  case ET_SET_EXPLOSION_TEMPLATE_PROPERTY:EvAct_SetExplosionTemplateProperty  (ID, A1, A2, A3, A4, A5);                     break;
  case ET_SET_ARMOUR_VALUE:               EvAct_SetArmourValue                (ID, A1, A2, A3, A4, A5, A6);                 break;
  case ET_SET_SPEED_VALUE:                EvAct_SetSpeedValue                 (ID, A1, A2, A4, A5);                         break;
  case ET_SET_RULE:                       EvAct_SetRule                       (ID, A2, A4, A5);                             break;
  // Miscellaneous
  case ET_ADD_RADAR_MARKER:               EvAct_AddRadarMarker                (ID, C0, A0, A1, A3, A4, A5, A6);             break;
  // Variable operations
  case ET_SET_VARIABLE:                   EvAct_SetVariable                   (ID, A1, A2, A3, A4, A5);                     break;
  case ET_GET_VARIABLE:                   EvAct_GetVariable                   (ID, A2, A3, A4);                             break;
  case ET_SET_FLOAT_VARIABLE:             EvAct_SetFloatVariable              (ID, A1, A2, A3, A4, A5);                     break;
  case ET_CONVERT_VARIABLE:               EvAct_ConvertVariable               (ID, A2, A3, A4);                             break;
  case ET_DEBUG_VARIABLES:                EvAct_DebugVariables                (ID, A2, A3, A4);                             break;
  case ET_GET_RANDOM_VALUE:               EvAct_GetRandomValue                (ID, A2, A5, A6);                             break;
  case ET_GET_RANDOM_COORDS:              EvAct_GetRandomCoords               (ID, C0, C1, A2);                             break;
  case ET_GET_VALUE_FROM_LIST:            EvAct_GetValueFromList              (ID, A1, A2, A3, A4, (uint8_t *)e->data);     break;
  case ET_GET_COORDS_FROM_LIST:           EvAct_GetCoordsFromList             (ID, A1, A2, A3, A4, (uint8_t *)e->data);     break;
  case ET_GET_AREA_FROM_LIST:             EvAct_GetAreaFromList               (ID, A1, A2, A3, A4, (uint8_t *)e->data);     break;
  case ET_GET_UNIT_COUNT:                 EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_BUILDING_COUNT:             EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_BULLET_COUNT:               EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_EXPLOSION_COUNT:            EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_CRATE_COUNT:                EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_TILE_COUNT:                 EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_SIDE_COUNT:                 EvAct_GetCount                      (ID, A1);                                     break;
  case ET_GET_SPICE_COUNT:                EvAct_GetSpiceCount                 (ID, A1, C0);                                 break;
  case ET_GET_DAMAGE_COUNT:               EvAct_GetDamageCount                (ID, A1, C0);                                 break;
  case ET_GET_UNIT_PROPERTY:              EvAct_GetObjectProperty             (ID, A0, A1, A2, A3, A4, OBJECT_UNIT);        break;
  case ET_GET_BUILDING_PROPERTY:          EvAct_GetObjectProperty             (ID, A0, A1, A2, A3, A4, OBJECT_BUILDING);    break;
  case ET_GET_BULLET_PROPERTY:            EvAct_GetObjectProperty             (ID, A0, A1, A2, A3, A4, OBJECT_BULLET);      break;
  case ET_GET_EXPLOSION_PROPERTY:         EvAct_GetObjectProperty             (ID, A0, A1, A2, A3, A4, OBJECT_EXPLOSION);   break;
  case ET_GET_CRATE_PROPERTY:             EvAct_GetCrateProperty              (ID, A1, A2, A3, A4);                         break;
  case ET_GET_TILE_PROPERTY:              EvAct_GetTileProperty               (ID, A1, A2, A3, A4);                         break;
  case ET_GET_SIDE_PROPERTY:              EvAct_GetSideProperty               (ID, A0, A1, A4, A5);                         break;
  case ET_GET_AI_PROPERTY:                EvAct_GetAIProperty                 (ID, A0, A1, A4, A5);                         break;
  case ET_GET_MEMORY_DATA:                EvAct_GetMemoryData                 (ID, A1, A4, A5);                             break;
  case ET_GET_UNIT_TEMPLATE_PROPERTY:     EvAct_GetUnitTemplateProperty       (ID, A1, A2, A3, A4);                         break;
  case ET_GET_BUILDING_TEMPLATE_PROPERTY: EvAct_GetBuildingTemplateProperty   (ID, A1, A2, A3, A4);                         break;
  case ET_GET_WEAPON_TEMPLATE_PROPERTY:   EvAct_GetWeaponTemplateProperty     (ID, A1, A2, A3, A4);                         break;
  case ET_GET_EXPLOSION_TEMPLATE_PROPERTY:EvAct_GetExplosionTemplateProperty  (ID, A1, A2, A3, A4);                         break;
  case ET_GET_ARMOUR_VALUE:               EvAct_GetArmourValue                (ID, A1, A2, A3, A4, A5);                     break;
  case ET_GET_SPEED_VALUE:                EvAct_GetSpeedValue                 (ID, A1, A2, A3);                             break;
  case ET_GET_GROUP_ID_VALUE:             EvAct_GetGroupIDValue               (ID, A1, A2);                                 break;
  case ET_GET_UNIT_TYPE:                  EvAct_GetUnitType                   (ID, A0, A1, A3, A4, (ObjectFilterStruct *)&e->data[1]);break;
  case ET_GET_BUILDING_TYPE:              EvAct_GetBuildingType               (ID, A0, A1, A3, A4, (ObjectFilterStruct *)&e->data[1]);break;
  case ET_GET_GAME_TICKS:                 EvAct_GetGameTicks                  (ID, A4);                                     break;
  case ET_GET_MY_SIDE_ID:                 EvAct_GetMySideId                   (ID, A4);                                     break;
  case ET_GET_DIFFICULTY:                 EvAct_GetDifficulty                 (ID, A4);                                     break;
  case ET_GET_RULE:                       EvAct_GetRule                       (ID, A2, A4);                                 break;
  case ET_GET_DIPLOMACY:                  EvAct_GetDiplomacy                  (ID, A0, A2, A4);                             break;
  case ET_GET_TECH:                       EvAct_GetTech                       (ID, A0, A4);                                 break;
  case ET_GET_HOUSE_ID:                   EvAct_GetHouseId                    (ID, A0, A4);                                 break;
  case ET_GET_CREDITS:                    EvAct_GetCredits                    (ID, A0, A3, A4);                             break;
  case ET_GET_POWER:                      EvAct_GetPower                      (ID, A0, A3, A4);                             break;
  case ET_GET_BUILDING_UPGRADES:          EvAct_GetBuildingUpgrades           (ID, A0, A2, A4);                             break;
  case ET_GET_STARPORT_STOCK:             EvAct_GetStarportStock              (ID, A0, A2, A4);                             break;
  case ET_GET_STARPORT_COST:              EvAct_GetStarportCost               (ID, A0, A2, A4);                             break;
  case ET_GET_STARPORT_PICK:              EvAct_GetStarportPick               (ID, A0, A2, A4);                             break;
  case ET_GET_BUILDING_QUEUE_STATE:       EvAct_GetBuildingQueueState         (ID, A0, A1, A2, A4);                         break;
  case ET_GET_UNIT_QUEUE_STATE:           EvAct_GetUnitQueueState             (ID, A0, A1, A2, A3, A4, A5);                 break;
  case ET_GET_UPGRADE_QUEUE_STATE:        EvAct_GetUpgradeQueueState          (ID, A0, A1, A2, A4);                         break;
  case ET_GET_SPICE_HARVESTED:            EvAct_GetSpiceHarvested             (ID, A0, A4);                                 break;
  case ET_GET_UNITS_BUILT:                EvAct_GetUnitsBuilt                 (ID, A0, A2, A3, A4);                         break;
  case ET_GET_BUILDINGS_BUILT:            EvAct_GetBuildingsBuilt             (ID, A0, A2, A3, A4);                         break;
  case ET_GET_UNITS_LOST:                 EvAct_GetUnitsLost                  (ID, A0, A2, A3, A4);                         break;
  case ET_GET_BUILDINGS_LOST:             EvAct_GetBuildingsLost              (ID, A0, A4);                                 break;
  case ET_GET_UNITS_KILLED:               EvAct_GetUnitsKilled                (ID, A0, A1, A2, A3, A4);                     break;
  case ET_GET_BUILDINGS_KILLED:           EvAct_GetBuildingsKilled            (ID, A0, A1, A2, A3, A4);                     break;
  case ET_GET_MOUSE_POSITION:             EvAct_GetMousePosition              (ID, A2, A3);                                 break;
  case ET_GET_KEYBOARD_MOUSE_STATE:       EvAct_GetKeyboardMouseState         (ID, A2, A3);                                 break;
  case ET_GET_UNIT_UNDER_CURSOR:          EvAct_GetUnitUnderCursor            (ID, A2, A3, A4, A5);                         break;
  case ET_GET_BUILDING_UNDER_CURSOR:      EvAct_GetBuildingUnderCursor        (ID, A2, A3, A4);                             break;
  case ET_GET_SIDEBAR_BUTTON_UNDER_CURSOR:EvAct_GetSidebarButtonUnderCursor   (ID, A2, A3, A4);                             break;
  case ET_GET_GAME_INTERFACE_DATA:        EvAct_GetGameInterfaceData          (ID, A1, A2, A3);                             break;
  case ET_GET_OBJECT_POSITION:            EvAct_GetObjectPosition             (ID, A0, A2, A3, A4);                         break;
  case ET_GET_DIRECTION:                  EvAct_GetDirection                  (ID, A2, A3, A4);                             break;
  case ET_GET_POSITION_ON_CIRCLE:         EvAct_GetPositionOnCircle           (ID, A1, A4, A5, A6);                         break;
  case ET_GET_NEAREST_BUILDING_TILE:      EvAct_GetNearestBuildingTile        (ID, A0, A1, A2, A3, A4);                     break;
  case ET_GET_DISTANCE:                   EvAct_GetDistance                   (ID, A1, A2, A3, A4);                         break;
  case ET_CHECK_DISTANCE:                 EvAct_CheckDistance                 (ID, A1, A2, A5, A6);                         break;
  // Blocks
  case ET_CALLABLE_BLOCK_START:                                                                                             break;
  case ET_HOOK_BLOCK_START:                                                                                                 break;
  case ET_EXECUTE_BLOCK:                  EvAct_ExecuteBlock                  (ID, A5);                                     break;
  case ET_EXIT_FROM_BLOCK:                exit_count = 1;                                                                   break;
  // Conditional expression
  case ET_IF:                             EvAct_If                            (ID, A1, A2, A3, (CondExprData *)&e->data[1]);break;
  case ET_ELSE_IF:                        DebugFatal(EVENT_ERROR, "Invalid ELSE IF event (event %d)", ID);                  break;
  case ET_ELSE:                           DebugFatal(EVENT_ERROR, "Invalid ELSE event (event %d)", ID);                     break;
  // Loops
  case ET_LOOP_WHILE:                     EvAct_LoopWhile                     (ID, (CondExprData *)&e->data[1]);            break;
  case ET_LOOP_VALUES_FROM_RANGE:         EvAct_LoopValuesFromRange           (ID, A2, A5, A6);                             break;
  case ET_LOOP_COORDS_FROM_AREA:          EvAct_LoopCoordsFromArea            (ID, C0, C1, A2);                             break;
  case ET_LOOP_VALUES_FROM_LIST:          EvAct_LoopValuesFromList            (ID, A1, A2, (uint8_t *)e->data);             break;
  case ET_LOOP_COORDS_FROM_LIST:          EvAct_LoopCoordsFromList            (ID, A1, A2, (uint8_t *)e->data);             break;
  case ET_LOOP_AREAS_FROM_LIST:           EvAct_LoopAreasFromList             (ID, A1, A2, (uint8_t *)e->data);             break;
  case ET_LOOP_UNITS:                     EvAct_LoopObject                    (ID, A1, A2, A0, OBJ);                        break;
  case ET_LOOP_BUILDINGS:                 EvAct_LoopObject                    (ID, A1, A2, A0, OBJ);                        break;
  case ET_LOOP_BULLETS:                   EvAct_LoopObject                    (ID, A1, A2, A0, OBJ);                        break;
  case ET_LOOP_EXPLOSIONS:                EvAct_LoopObject                    (ID, A1, A2, A0, OBJ);                        break;
  case ET_LOOP_CRATES:                    EvAct_LoopItem                      (ID, A1, OBJ);                                break;
  case ET_LOOP_TILES:                     EvAct_LoopTiles                     (ID, A1, C0);                                 break;
  case ET_LOOP_SIDES:                     EvAct_LoopItem                      (ID, A1, OBJ);                                break;
  case ET_BREAK_LOOP:                     break_count = 1;                                                                  break;
  case ET_CONTINUE_LOOP:                  continue_count = 1;                                                               break;
  // End
  case ET_END:                            DebugFatal(EVENT_ERROR, "Invalid END event (event %d)", ID);                      break;
  default:                                DebugFatal(EVENT_ERROR, "Unknown event type %d (event %d)", e->event_type, ID);
  }
}

int ExecuteEventHook(int hook_type, int num_vars, int var0, int var1, int var2, int var3, int var4)
{
  if (event_hooks[hook_type] == -1)
    return var0;
  // Set variables
  if (num_vars >= 1)
    SetVariableValue(-1, 0, var0);
  if (num_vars >= 2)
    SetVariableValue(-1, 1, var1);
  if (num_vars >= 3)
    SetVariableValue(-1, 2, var2);
  if (num_vars >= 4)
    SetVariableValue(-1, 3, var3);
  if (num_vars >= 5)
    SetVariableValue(-1, 4, var4);
  // Execute hook
  ExecuteEventBlock(event_hooks[hook_type], EBT_BLOCK);
  return GetVariableValue(-1, 0);
}

int GetVariableValueOrConst(int flags, int flag_index, int var_index_or_const)
{
  if (flags & (1 << flag_index))
    return gEventVariableArray[var_index_or_const].value;
  else
    return var_index_or_const;
}

void SetVariableValue(int event_index, int var_index, int value)
{
  if (var_index < 0 || var_index >= MAX_EVENT_VARIABLES)
    DebugFatal(EVENT_ERROR, "Trying to write to variable %d (event %d)", var_index, event_index);
  EventVariable *v = &gEventVariableArray[var_index];
  if (v->ticks != gGameTicks)
  {
    v->old_value = v->value;
    v->ticks = gGameTicks;
  }
  v->value = value;
}

int GetVariableValue(int event_index, int var_index)
{
  if (var_index < 0 || var_index >= MAX_EVENT_VARIABLES)
    DebugFatal(EVENT_ERROR, "Trying to read from variable %d (event %d)", var_index, event_index);
  return gEventVariableArray[var_index].value;
}
