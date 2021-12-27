#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
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
    case CT_BUILDINGEXISTS: return Cond_BuildingExists(A_SIDE, A_ARG1);
    case CT_UNITEXISTS:     return Cond_UnitExists    (A_SIDE, A_ARG2);
    case CT_INTERVAL:       return Cond_Interval      (A_ARG1, A_VAL1, A_VAL2, A_VAL3, condition);
    case CT_TIMER:          return Cond_Timer         (A_ARG1, A_ARG2, A_VAL2, A_VAL3, condition);
    case CT_CASUALTIES:     return Cond_Casualties    (A_SIDE, A_VAL3, A_FLOAT);
    case CT_BASEDESTROYED:  return !_gBuildingsExist[A_SIDE];
    case CT_UNITSDESTROYED: return !_gUnitsExist[A_SIDE];
    case CT_REVEALED:       return Cond_Revealed      (COORD0, A_VAL3, condition);
    case CT_HARVESTED:      return Cond_Harvested     (A_SIDE, A_ARG2, A_VAL3);
    case CT_FLAG:           return A_VAL3 != 0;
    case CT_RANDOMCHANCE:   return Cond_RandomChance  (A_VAL1, A_VAL2, A_VAL3, A_VAL4, condition);
    case CT_RANDOMINTERVAL: return Cond_RandomInterval(A_ARG1, A_ARG2, A_VAL1, A_VAL2, A_VAL3, condition);
    case CT_CHECKUNITS:     return Cond_CheckUnits    (condition);
    case CT_CHECKBUILDINGS: return Cond_CheckBuildings(condition);
    case CT_CHECKCRATES:    return Cond_CheckCrates   (condition);
    case CT_CHECKTILES:     return Cond_CheckTiles    (condition);
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
  if (event->event_type >= ET_DESTROY_UNIT && event->event_type <= ET_SHOW_UNIT_DATA)
  {
    int limit = event->data[0];
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < 8; side_id++)
    {
      if ((arg_side_id != 8) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      for (Unit *unit = side->_Units_8; unit; unit = unit->Next)
      {
        if (CheckIfUnitMatchesFilter((ObjectFilterStruct *)&e.data[1], unit))
        {
          e.index = unit->MyIndex;
          ExecuteEventAction(event->event_type, &e);
          affected++;
        }
        if (limit && (affected == limit))
          return;
      }
    }
    return;
  }
  // Building manipulation events: process all side's buildings
  if (event->event_type >= ET_DESTROY_BUILDING && event->event_type <= ET_SHOW_BUILDING_DATA)
  {
    int limit = event->data[0];
    int affected = 0;
    int arg_side_id = e.args[1];
    for (int side_id = 0; side_id < 8; side_id++)
    {
      if ((arg_side_id != 8) && (arg_side_id != side_id))
        continue;
      e.args[1] = side_id;
      CSide *side = GetSide(side_id);
      for (Building *building = side->_Buildings_10; building; building = building->Next)
      {
        if (CheckIfBuildingMatchesFilter((ObjectFilterStruct *)&e.data[1], building, side_id))
        {
          e.index = building->MyIndex;
          ExecuteEventAction(event->event_type, &e);
          affected++;
        }
        if (limit && (affected == limit))
          return;
      }
    }
    return;
  }
  // Crate manipulation events: process all crates
  if (event->event_type >= ET_REMOVE_CRATE && event->event_type <= ET_74)
  {
    int limit = event->data[0];
    int affected = 0;
    for (int i = 0; i < MAX_CRATES; i++)
    {
      if (CheckIfCrateMatchesFilter((ObjectFilterStruct *)&e.data[1], &gCrates[i]))
      {
        e.index = i;
        ExecuteEventAction(event->event_type, &e);
        affected++;
      }
      if (limit && (affected == limit))
        return;
    }
    return;
  }
  // Tile manipulation events: process all tiles
  if (event->event_type >= ET_SET_TILE_ATTRIBUTE && event->event_type <= ET_79)
  {
    int limit = event->data[0];
    int affected = 0;
    ObjectFilterStruct *filter = (ObjectFilterStruct *)&e.data[1];
    bool check_pos = (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && !(filter->pos_flags & OBJFILTERPOSFLAG_NEGATE);
    int min_x = check_pos?filter->pos_min_x:0;
    int min_y = check_pos?filter->pos_min_y:0;
    int max_x = check_pos?filter->pos_max_x:gGameMapWidth-1;
    int max_y = check_pos?filter->pos_max_y:gGameMapHeight-1;
    for (int y = min_y; y <= max_y; y++)
      for (int x = min_x; x <= max_x; x++)
      {
        if (CheckIfTileMatchesFilter((ObjectFilterStruct *)&e.data[1], &gGameMap.map[x + _CellNumbersWidthSpan[y]], x, y, (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && (filter->pos_flags & OBJFILTERPOSFLAG_NEGATE)))
        {
          e.index = x + _CellNumbersWidthSpan[y];
          ExecuteEventAction(event->event_type, &e);
          affected++;
        }
        if (limit && (affected == limit))
          return;
      }
    return;
  }
  // Normal events: just execute action
  ExecuteEventAction(event->event_type, &e);
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

void ExecuteEventAction(int event_type, EventContext *e)
{
  switch ( event_type )
  {
  case ET_REINFORCEMENT:          EvAct_AddDelivery         (COORD0, A_SIDE, A_AMNT, A_BOOL, DELIVERYTYPE_REINFORCE, e->data); break;
  case ET_STARPORT_DELIVERY:      EvAct_AddDelivery         (COORD0, A_SIDE, A_AMNT, A_BOOL, DELIVERYTYPE_STARPORT,  e->data); break;
  case ET_ALLEGIANCE:             EvAct_SetDiplomacy        (A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_LEAVE:                  CSide__BlowupAll_surrender(GetSide(A_SIDE)); break;
  case ET_BESERK:                 _gAIArray[A_SIDE].__GoBeserk_OtherStates = 1; break;
  case ET_PLAYSOUND:              EvAct_PlaySound           (A_VALUE, A_BOOL, COORD0); break;
  case ET_SETBUILDRATE:           _gAIArray[A_SIDE].UnitBuildRate = A_VALUE; break;
  case ET_SETATTACKBUILDINGRATE:  _gAIArray[A_SIDE].TimeBetweenBuildingAttacks = A_VALUE; break;
  case ET_SETCASH:                EvAct_SetCash             (A_SIDE, A_ENUM, A_VALUE); break;
  case ET_SETTECH:                EvAct_SetTech             (A_SIDE, A_BOOL, A_VALUE); break;
  case ET_WIN:                    if ( !gLose ) gWin = 1; break;
  case ET_LOSE:                   if ( !gWin ) gLose = 1; break;
  case ET_SWITCH_MY_SIDE:         EvAct_SwitchMySide        (A_SIDE, A_ENUM, A_BOOL); break;
  case ET_HIDE_MAP:               EvAct_HideMap             (); break;
  case ET_REVEAL:                 EvAct_RevealMap           (COORD0, A_AMNT); break;
  case ET_SHOWTIMER:              gTimerValue = A_VALUE; break;
  case ET_HIDETIMER:              gTimerValue = -1; break;
  case ET_SHOWMESSAGE:            EvAct_ShowMessage         (A_VALUE, (ShowMessageEventData *)&e->data[1]); break;
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
  case ET_SHAKE_SCREEN:           _ScreenShakes = A_VALUE;   break;
  case ET_CENTER_VIEWPORT:        EvAct_CenterViewport      (COORD0); break;
  case ET_CHANGE_MAP_BLOCK:       EvAct_ChangeMapBlock      (COORD0, COORD1, (uint16_t *)&e->data[1]); break;
  case ET_TRANSFORM_TILES:        EvAct_TransformTiles      (A_AMNT, (uint16_t *)&e->data[1]); break;
  case ET_ACTIVATE_TIMER:         EvAct_ActivateTimer       (A_VALUE); break;
  // Unit manipulation
  case ET_DESTROY_UNIT:           EvAct_DestroyUnit         (A_SIDE, A_BOOL, OBJ_IDX); break;
  case ET_DAMAGE_HEAL_UNIT:       EvAct_DamageHealUnit      (A_SIDE, A_ENUM, A_BOOL, A_VALUE, OBJ_IDX); break;
  case ET_CHANGE_UNIT_OWNER:      ChangeUnitOwner           (A_SIDE, A_ITEM, OBJ_IDX, 0); break;
  case ET_CHANGE_UNIT_TYPE:       EvAct_ChangeUnitType      (A_SIDE, A_ITEM, A_BOOL, OBJ_IDX); break;
  case ET_SET_UNIT_FLAG:          EvAct_SetUnitFlag         (A_SIDE, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_UNIT_PROPERTY:      EvAct_SetUnitProperty     (A_SIDE, A_ITEM, A_VALUE, OBJ_IDX); break;
  case ET_AIRLIFT_UNIT:           EvAct_AirliftUnit         (A_SIDE, COORD0, A_BOOL, OBJ_IDX); break;
  case ET_SHOW_UNIT_DATA:         EvAct_ShowUnitData        (A_SIDE, OBJ_IDX); break;
  // Building manipulation
  case ET_DESTROY_BUILDING:       DestroyBuilding           (A_SIDE, OBJ_IDX, 0); break;
  case ET_DAMAGE_HEAL_BUILDING:   EvAct_DamageHealBuilding  (A_SIDE, A_ENUM, A_BOOL, A_VALUE, OBJ_IDX); break;
  case ET_CHANGE_BUILDING_OWNER:  EvAct_ChangeBuildingOwner (A_SIDE, A_ITEM, OBJ_IDX); break;
  case ET_CHANGE_BUILDING_TYPE:   EvAct_ChangeBuildingType  (A_SIDE, A_ITEM, OBJ_IDX); break;
  case ET_SET_BUILDING_FLAG:      EvAct_SetBuildingFlag     (A_SIDE, A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_BUILDING_PROPERTY:  EvAct_SetBuildingProperty (A_SIDE, A_ITEM, A_VALUE, OBJ_IDX); break;
  case ET_SHOW_BUILDING_DATA:     EvAct_ShowBuildingData    (A_SIDE, OBJ_IDX); break;
  // Crate manipulation
  case ET_REMOVE_CRATE:           EvAct_RemoveCrate         (OBJ_IDX); break;
  // Tile manipulation
  case ET_SET_TILE_ATTRIBUTE:     EvAct_SetTileAttribute    (A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_SET_TILE_DAMAGE:        EvAct_SetTileDamage       (A_ENUM, A_VALUE, OBJ_IDX); break;
  case ET_REVEAL_TILE:            EvAct_RevealTile          (OBJ_IDX); break;
  default:
    DebugFatal("event-core.c", "Unknown event type %d", event_type);
  }
}
