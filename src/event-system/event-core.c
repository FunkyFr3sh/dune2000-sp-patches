#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
#include "event-utils.h"
#include "event-core.h"
#include "event-conditions.h"
#include "event-actions.h"
#include "event-filters.h"

// New extended arrays for event and condition data

EventData _gEventArray[MAX_EVENTS];
ConditionData _gConditionArray[MAX_CONDITIONS];

// Other variables

int tick_random_value;

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
  if ( _canQueue_IsMultiplayer || gGameType == GAME_SKIRMISH )
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
    char condition_results[MAX_CONDITIONS];
    for (int condition_index = 0; condition_index < _gConditionCount; condition_index++)
    {
      condition_results[condition_index] = EvaluateCondition(condition_index);
    }
    // Process events
    for (int event_index = 0; event_index < _gEventCount; event_index++)
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
      }
    }
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
  int args[7];
  for (int i = 0; i < 2; i++)
  {
    coord_x[i] = condition->coord_x[i];
    coord_y[i] = condition->coord_y[i];
  }
  args[0] = condition->side_id;
  args[1] = condition->arg1;
  args[2] = condition->arg2;
  args[3] = condition->val1;
  args[4] = condition->val2;
  args[5] = condition->val3;
  args[6] = condition->val4;
  // Run condition
  switch ( condition->condition_type )
  {
    // Vanilla / General
    case CT_BUILDINGEXISTS:     return Cond_BuildingExists(A_SIDE, A_ARG1);
    case CT_UNITEXISTS:         return Cond_UnitExists    (A_SIDE, A_ARG2);
    case CT_INTERVAL:           return Cond_Interval      (A_ARG1, A_VAL1, A_VAL2, A_VAL3, condition);
    case CT_TIMER:              return Cond_Timer         (A_ARG1, A_ARG2, A_VAL2, A_VAL3, condition);
    case CT_CASUALTIES:         return Cond_Casualties    (A_SIDE, A_VAL3, A_FLOAT);
    case CT_BASEDESTROYED:      return !_gBuildingsExist[A_SIDE];
    case CT_UNITSDESTROYED:     return !_gUnitsExist[A_SIDE];
    case CT_REVEALED:           return Cond_Revealed      (COORD0, A_VAL3, condition);
    case CT_CREDITS:            return Cond_Credits       (A_SIDE, A_ARG1, A_ARG2, A_VAL3);
    case CT_FLAG:               return A_VAL3 != 0;
    case CT_RANDOMCHANCE:       return Cond_RandomChance  (A_VAL1, A_VAL2, A_VAL3, A_VAL4, condition);
    case CT_RANDOMINTERVAL:     return Cond_RandomInterval(A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3, condition);
    case CT_DIPLOMACY:          return _gDiplomacy[A_SIDE][A_ARG1] == A_ARG2;
    case CT_DIFFICULTY:         return gDifficultyLevel == A_VAL3;
    // Environment checking
    case CT_CHECKUNITS:         return Cond_CheckUnits    (condition);
    case CT_CHECKBUILDINGS:     return Cond_CheckBuildings(condition);
    case CT_CHECKCRATES:        return Cond_CheckCrates   (condition);
    case CT_CHECKTILES:         return Cond_CheckTiles    (condition);
    case CT_SPICE_IN_AREA:      return Cond_SpiceInArea   (COORD0, COORD1, A_VAL3);
    case CT_DAMAGE_IN_AREA:     return Cond_DamageInArea  (COORD0, COORD1, A_ARG1, A_ARG2, A_VAL3);
    // Side related
    case CT_POWER:              return Cond_Power         (A_SIDE, A_ARG1, A_VAL1, A_VAL3);
    case CT_BUILDING_UPGRADES:  return CompareValue(GetSide(A_SIDE)->__BuildingGroupUpgradeCount[A_ARG1], A_VAL3, !A_VAL1);
    case CT_STARPORT_STOCK:     return Cond_StarportStock (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_STARPORT_COST:      return Cond_StarportCost  (A_SIDE, A_ARG1, A_VAL1, A_VAL3);
    case CT_STARPORT_PICK:      return Cond_StarportPick  (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_STARPORT_DELIVERY:  return GetSide(A_SIDE)->__StarportDeliveryInProgress;
    case CT_BUILDING_ICON:      return Cond_BuildingIcon  (A_SIDE, A_ARG1, A_ARG2, A_VAL2);
    case CT_UNIT_ICON:          return Cond_UnitIcon      (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL2);
    case CT_UPGRADE_ICON:       return Cond_UpgradeIcon   (A_SIDE, A_ARG2, A_VAL2);
    case CT_SPICE_HARVESTED:    return CompareValue(GetSide(A_SIDE)->__SpiceHarvested, A_VAL3, !A_VAL1);
    case CT_UNITS_BUILT:        return Cond_UnitsBuilt    (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_BUILDINGS_BUILT:    return Cond_BuildingsBuilt(A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_UNITS_LOST:         return Cond_UnitsLost     (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL3);
    case CT_BUILDINGS_LOST:     return Cond_BuildingsLost (A_SIDE, A_VAL1, A_VAL3);
    case CT_UNITS_KILLED:       return Cond_UnitsKilled   (A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3);
    case CT_BUILDINGS_KILLED:   return Cond_BuildingsKilled(A_SIDE, A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3);
    case CT_SIDE_PROPERTY:      return CompareDataValue   ((char *)GetSide(A_SIDE),    A_ARG1, A_VAL1, A_VAL3, !A_VAL2);
    // AI related
    case CT_AI_PROPERTY:        return CompareDataValue   ((char *)&_gAIArray[A_SIDE], A_ARG1, A_VAL1, A_VAL3, !A_VAL2);
    // Memory related
    case CT_MEMORY_VALUE:       return CompareDataValue   (NULL,                       A_ARG1, A_VAL1, A_VAL3, !A_VAL2);
    default:
      DebugFatal("event-core.c", "Unknown condition type %d", condition->condition_type);
  }
  return false;  
}

#undef COORD0
#undef COORD1
#undef A_SIDE
#undef A_ITEM
#undef A_ENUM
#undef A_VALUE

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
  for (int i = 0; i < 4; i++)
  {
    e.coord_x[i] = event->coord_x[i];
    e.coord_y[i] = event->coord_y[i];
  }
  for (int i = 0; i < 5; i++)
    e.args[i] = event->args[i];
  e.args[5] = event->value;
  e.data = event->data;
  // Unit manipulation events: process all side's units
  if ((et >= ET_DESTROY_UNIT && et <= ET_SHOW_UNIT_DATA)
      || et == ET_ORDER_REPAIR_SINGLE_UNIT
      || et == ET_ORDER_UNIT_ATTACK_UNIT
      || et == ET_ORDER_UNIT_DEPLOY
      || et == ET_ORDER_BUILDING_ATTACK_UNIT)
  {
    int skip = event->filter_skip;
    int limit = event->data[0];
    if (et == ET_ORDER_UNIT_ATTACK_UNIT
        || et == ET_ORDER_BUILDING_ATTACK_UNIT)
      limit = 1;
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
            e.index = unit->MyIndex;
            ExecuteEventAction(et, &e);
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
      || et == ET_ORDER_BUILDING_SELL)
  {
    int skip = event->filter_skip;
    int limit = event->data[0];
    if (et == ET_ORDER_DOCK_WITH_REFINERY
        || et == ET_ORDER_REPAIR_SELECTED_UNITS
        || et == ET_ORDER_UNIT_ATTACK_BUILDING
        || et == ET_ORDER_BUILDING_ATTACK_BUILDING)
      limit = 1;
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
            e.index = building->MyIndex;
            ExecuteEventAction(et, &e);
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
  if (et >= ET_REMOVE_CRATE && et <= ET_SHOW_CRATE_DATA)
  {
    int skip = event->filter_skip;
    int limit = event->data[0];
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
          e.index = i;
          ExecuteEventAction(et, &e);
          affected++;
        }
      }
      if (limit && (affected == limit))
        return;
    }
    return;
  }
  // Tile manipulation events: process all tiles
  if (et >= ET_CHANGE_TILE && et <= ET_SHOW_TILE_DATA)
  {
    int skip = event->filter_skip;
    int limit = event->data[0];
    int affected = 0;
    ObjectFilterStruct *filter = (ObjectFilterStruct *)&e.data[1];
    bool check_pos = (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && !(filter->pos_flags & OBJFILTERPOSFLAG_NEGATE);
    int min_x = check_pos?filter->pos_min_x:0;
    int min_y = check_pos?filter->pos_min_y:0;
    int max_x = check_pos?filter->pos_max_x:gGameMapWidth-1;
    int max_y = check_pos?filter->pos_max_y:gGameMapHeight-1;
    // Process all tiles
    for (int y = min_y; y <= max_y; y++)
      for (int x = min_x; x <= max_x; x++)
      {
        if (CheckIfTileMatchesFilter((ObjectFilterStruct *)&e.data[1], &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y, (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && (filter->pos_flags & OBJFILTERPOSFLAG_NEGATE)))
        {
          if (skip)
            skip--;
          else
          {
            e.index = x + (y << 8);
            ExecuteEventAction(et, &e);
            affected++;
          }
        }
        if (limit && (affected == limit))
          return;
      }
    return;
  }
  // Normal events: just execute action
  ExecuteEventAction(et, &e);
}

#define COORD0  e->coord_x[0], e->coord_y[0]
#define COORD1  e->coord_x[1], e->coord_y[1]
#define COORD2  e->coord_x[2], e->coord_y[2]
#define COORD3  e->coord_x[3], e->coord_y[3]
#define A_SIDE  e->args[1]
#define A_AMNT  e->args[0]
#define A_ITEM  e->args[2]
#define A_ENUM  e->args[3]
#define A_BOOL  e->args[4]
#define A_VALUE e->args[5]
#define OBJ_IDX e->index

#define VALUEOPERATION(data) data = ValueOperation(data, A_VALUE, A_ENUM)

void ExecuteEventAction(int event_type, EventContext *e)
{
  switch ( event_type )
  {
  // Vanilla / General
  case ET_REINFORCEMENT:          EvAct_AddDelivery         (COORD0, A_SIDE, A_AMNT, A_BOOL, 0,       DELIVERYTYPE_REINFORCE, e->data); break;
  case ET_STARPORT_DELIVERY:      EvAct_AddDelivery         (COORD0, A_SIDE, A_AMNT, A_BOOL, A_VALUE, DELIVERYTYPE_STARPORT,  e->data); break;
  case ET_ALLEGIANCE:             EvAct_SetDiplomacy        (A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_LEAVE:                  CSide__BlowupAll_surrender(GetSide(A_SIDE)); break;
  case ET_BESERK:                 _gAIArray[A_SIDE].__GoBeserk_OtherStates = 1; break;
  case ET_PLAYSOUND:              EvAct_PlaySound           (A_VALUE, A_BOOL, COORD0); break;
  case ET_SETBUILDRATE:           VALUEOPERATION(_gAIArray[A_SIDE].UnitBuildRate); break;
  case ET_SETATTACKBUILDINGRATE:  VALUEOPERATION(_gAIArray[A_SIDE].TimeBetweenBuildingAttacks); break;
  case ET_SETCASH:                EvAct_SetCash             (A_SIDE, A_ENUM, A_VALUE); break;
  case ET_SETTECH:                EvAct_SetTech             (A_SIDE, A_ENUM, A_BOOL, A_VALUE); break;
  case ET_WIN:                    if ( !gLose ) gWin = 1; break;
  case ET_LOSE:                   if ( !gWin ) gLose = 1; break;
  case ET_SWITCH_MY_SIDE:         EvAct_SwitchMySide        (A_SIDE, A_ENUM, A_BOOL); break;
  case ET_HIDE_MAP:               EvAct_HideMap             (); break;
  case ET_REVEAL:                 EvAct_RevealMap           (COORD0, A_AMNT); break;
  case ET_SETTIMER:               VALUEOPERATION(_gTimerValue); break;
  case ET_HIDETIMER:              _gTimerValue = -1; break;
  case ET_SHOWMESSAGE:            EvAct_ShowMessage         (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VALUE, (ShowMessageEventData *)&e->data[1]); break;
  case ET_UNIT_SPAWN:             EvAct_UnitSpawn           (COORD0, A_SIDE, A_AMNT, A_ENUM, A_VALUE, e->data); break;
  case ET_SET_FLAG:               _gConditionArray[A_SIDE].val3 = A_VALUE; break;
  case ET_UN_BLOCK_EVENT:         EvAct_UnBlockEvent        (A_BOOL, A_VALUE); break;
  case ET_PLAY_MUSIC:             EvAct_PlayMusic           (e->data); break;
  case ET_DAMAGE_TILES:           EvAct_DamageTiles         (COORD0, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_UNIT:               EvAct_AddUnit             (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VALUE); break;
  case ET_ADD_BUILDING:           EvAct_AddBuilding         (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL, A_VALUE); break;
  case ET_ADD_PROJECTILE:         EvAct_AddProjectile       (COORD0, COORD1, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_EXPLOSION:          EvAct_AddExplosion        (COORD0, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_CRATE:              EvAct_AddCrate            (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VALUE); break;
  case ET_ADD_CONCRETE:           EvAct_AddConcrete         (COORD0, COORD1, A_SIDE, A_VALUE); break;
  case ET_SPICE_BLOOM:            EvAct_SpiceBloom          (COORD0, A_AMNT, A_ENUM, A_BOOL); break;
  case ET_SHAKE_SCREEN:           _ScreenShakes = A_VALUE; break;
  case ET_CENTER_VIEWPORT:        EvAct_CenterViewport      (COORD0); break;
  case ET_CHANGE_MAP_BLOCK:       EvAct_ChangeMapBlock      (COORD0, COORD1, A_ENUM, (uint16_t *)&e->data[1]); break;
  case ET_TRANSFORM_TILES:        EvAct_TransformTiles      (A_AMNT, A_ENUM, (uint16_t *)&e->data[1]); break;
  case ET_ADD_BUILDING_DESTRUCT:  EvAct_AddBuildingDestruct (COORD0, A_SIDE, A_ITEM); break;
  case ET_ACTIVATE_TIMER:         EvAct_ActivateTimer       (A_VALUE); break;
  case ET_REMOVE_MESSAGE:         EvAct_RemoveMessage       (A_SIDE, A_AMNT, A_ITEM); break;
  case ET_SET_MESSAGE_COLOR:      EvAct_SetMessageColor     (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VALUE, *((int *)&e->data[1])); break;
  // Side manipulation
  case ET_TRANSFER_CREDITS:       EvAct_TransferCredits     (A_SIDE, A_ENUM, A_VALUE); break;
  case ET_SET_BUILDING_UPGRADES:  EvAct_SetBuildingUpgrades (A_SIDE, A_ITEM, A_ENUM, A_VALUE); break;
  case ET_SET_STARPORT_STOCK:     VALUEOPERATION(GetSide(A_SIDE)->__StarportUnitTypeStock[A_ITEM]); break;
  case ET_SET_STARPORT_COST:      EvAct_SetStarportCost     (A_SIDE, A_ITEM, A_ENUM, A_BOOL, A_VALUE); break;
  case ET_CHANGE_STARPORT_UNIT:   GetSide(A_SIDE)->__StarportIcons[A_AMNT] = A_ITEM; break;
  case ET_SHOW_SIDE_DATA:         EvAct_ShowSideData        (A_SIDE, A_VALUE); break;
  // AI manipulation
  case ET_SET_AI_PROPERTY:        EvAct_SetAIProperty       (A_SIDE, A_AMNT, A_ENUM, A_VALUE, *(int *)&e->data[1]); break;
  case ET_SHOW_AI_DATA:           EvAct_ShowAIData          (A_SIDE, A_VALUE); break;
  // Memory manipulation
  case ET_SET_MEMORY_DATA:        EvAct_SetMemoryData       (A_AMNT, A_ENUM, A_VALUE, *(int *)&e->data[1]); break;
  case ET_SHOW_MEMORY_DATA:       EvAct_ShowMemoryData      (A_VALUE); break;
  // Unit manipulation
  case ET_DESTROY_UNIT:           EvAct_DestroyUnit         (A_SIDE, A_BOOL, OBJ_IDX); break;
  case ET_DAMAGE_HEAL_UNIT:       EvAct_DamageHealUnit      (A_SIDE, A_ENUM, A_BOOL, A_VALUE, OBJ_IDX); break;
  case ET_CHANGE_UNIT_OWNER:      ChangeUnitOwner           (A_SIDE, A_ITEM, OBJ_IDX, 0); break;
  case ET_CHANGE_UNIT_TYPE:       EvAct_ChangeUnitType      (A_SIDE, A_ITEM, A_BOOL, OBJ_IDX); break;
  case ET_SET_UNIT_FLAG:          EvAct_SetUnitFlag         (A_SIDE, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_UNIT_PROPERTY:      EvAct_SetUnitProperty     (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SELECT_UNIT:            EvAct_SelectUnit          (A_SIDE, A_BOOL, OBJ_IDX); break;
  case ET_AIRLIFT_UNIT:           EvAct_AirliftUnit         (A_SIDE, COORD0, A_BOOL, OBJ_IDX); break;
  case ET_SHOW_UNIT_DATA:         EvAct_ShowUnitData        (A_SIDE, OBJ_IDX); break;
  // Building manipulation
  case ET_DESTROY_BUILDING:       EvAct_DestroyBuilding     (A_SIDE, A_BOOL, OBJ_IDX); break;
  case ET_DAMAGE_HEAL_BUILDING:   EvAct_DamageHealBuilding  (A_SIDE, A_ENUM, A_BOOL, A_VALUE, OBJ_IDX); break;
  case ET_CHANGE_BUILDING_OWNER:  EvAct_ChangeBuildingOwner (A_SIDE, A_ITEM, OBJ_IDX); break;
  case ET_CHANGE_BUILDING_TYPE:   EvAct_ChangeBuildingType  (A_SIDE, A_ITEM, OBJ_IDX); break;
  case ET_SET_BUILDING_FLAG:      EvAct_SetBuildingFlag     (A_SIDE, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_BUILDING_PROPERTY:  EvAct_SetBuildingProperty (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SELECT_BUILDING:        EvAct_SelectBuilding      (A_SIDE, A_BOOL, OBJ_IDX); break;
  case ET_SHOW_BUILDING_DATA:     EvAct_ShowBuildingData    (A_SIDE, OBJ_IDX); break;
  // Crate manipulation
  case ET_REMOVE_CRATE:           EvAct_RemoveCrate         (OBJ_IDX); break;
  case ET_PICKUP_CRATE:           EvAct_PickupCrate         (A_SIDE, OBJ_IDX); break;
  case ET_SET_CRATE_PROPERTY:     EvAct_SetCrateProperty    (A_AMNT, A_ITEM, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SHOW_CRATE_DATA:        EvAct_ShowCrateData       (OBJ_IDX); break;
  // Tile manipulation
  case ET_CHANGE_TILE:            EvAct_ChangeTile          (A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_TILE_ATTRIBUTE:     EvAct_SetTileAttribute    (A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_TILE_DAMAGE:        EvAct_SetTileDamage       (A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_REVEAL_TILE:            EvAct_RevealTile          (A_AMNT, OBJ_IDX); break;
  case ET_HIDE_TILE:              EvAct_HideTile            (OBJ_IDX); break;
  case ET_SHOW_TILE_DATA:         EvAct_ShowTileData        (OBJ_IDX); break;
  // Orders
  case ET_ORDER_UNIT_MOVE:                GenerateUnitMoveOrder               (A_SIDE, COORD0);           RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_DOCK_WITH_REFINERY:       GenerateDockWithRefineryOrder       (A_SIDE, OBJ_IDX);          RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_REPAIR_SELECTED_UNITS:    GenerateRepairSelectedUnitsOrder    (A_SIDE, OBJ_IDX);          RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_REPAIR_SINGLE_UNIT:       GenerateRepairSingleUnitOrder       (A_SIDE, OBJ_IDX);          break;
  case ET_ORDER_UNIT_ATTACK_UNIT:         GenerateUnitAttackUnitOrder         (A_ITEM, A_SIDE, OBJ_IDX);  RestoreUnitSelection(A_ITEM, A_BOOL); break;
  case ET_ORDER_UNIT_ATTACK_BUILDING:     GenerateUnitAttackBuildingOrder     (A_ITEM, A_SIDE, OBJ_IDX);  RestoreUnitSelection(A_ITEM, A_BOOL); break;
  case ET_ORDER_UNIT_ATTACK_TILE:         GenerateUnitAttackTileOrder         (A_SIDE, COORD0);           RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_GUARD:               GenerateUnitGuardOrder              (A_SIDE);                   RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_SCATTER:             GenerateUnitScatterOrder            (A_SIDE);                   RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_RETREAT:             EvAct_OrderUnitRetreat              (A_SIDE);                   RestoreUnitSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_UNIT_DEPLOY:              GenerateUnitDeployOrder             (A_SIDE, OBJ_IDX);          break;
  case ET_ORDER_BUILDING_ATTACK_UNIT:     GenerateBuildingAttackUnitOrder     (A_ITEM, A_SIDE, OBJ_IDX);  RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILDING_ATTACK_BUILDING: GenerateBuildingAttackBuildingOrder (A_ITEM, A_SIDE, OBJ_IDX);  RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILDING_SET_PRIMARY:     GenerateBuildingSetPrimaryOrder     (A_SIDE);                   RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILDING_REPAIR:          GenerateBuildingRepairOrder         (A_SIDE, OBJ_IDX);          break;
  case ET_ORDER_BUILDING_SELL:            GenerateBuildingSellOrder           (A_SIDE, OBJ_IDX);          break;
  case ET_ORDER_STOP:                     GenerateStopOrder                   (A_SIDE);                   RestoreUnitSelection(A_SIDE, A_BOOL); RestoreBuildingSelection(A_SIDE, A_BOOL); break;
  case ET_ORDER_BUILD_BUILDING_PICK:      GenerateBuildBuildingPickOrder      (A_SIDE, A_ITEM);           break;
  case ET_ORDER_BUILD_BUILDING_CANCEL:    EvAct_OrderBuildBuildingCancel      (A_SIDE, A_BOOL);           break;
  case ET_ORDER_BUILD_PLACE_BUILDING:     EvAct_OrderBuildPlaceBuilding       (A_SIDE, COORD0);           break;
  case ET_ORDER_BUILD_UNIT_PICK:          GenerateBuildUnitPickOrder          (A_SIDE, A_ITEM);           break;
  case ET_ORDER_BUILD_UNIT_CANCEL:        EvAct_OrderBuildUnitCancel          (A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_BOOL);   break;
  case ET_ORDER_STARPORT_PICK:            EvAct_OrderStarportPick             (A_SIDE, A_ITEM);           break;
  case ET_ORDER_STARPORT_UNPICK:          GenerateStarportUnpickOrder         (A_SIDE, A_ITEM);           break;
  case ET_ORDER_STARPORT_PURCHASE:        GenerateStarportPurchaseOrder       (A_SIDE);                   break;
  case ET_ORDER_STARPORT_CANCEL:          GenerateStarportCancelOrder         (A_SIDE);                   break;
  case ET_ORDER_UPGRADE_PICK:             GenerateUpgradePickOrder            (A_SIDE, A_ITEM);           break;
  case ET_ORDER_UPGRADE_CANCEL:           EvAct_OrderUpgradeCancel            (A_SIDE, A_BOOL);           break;
  case ET_ORDER_SPECIAL_WEAPON:           GenerateSpecialWeaponOrder          (A_SIDE, A_ITEM, COORD0);   break;
  default:
    DebugFatal("event-core.c", "Unknown event type %d", event_type);
  }
}
