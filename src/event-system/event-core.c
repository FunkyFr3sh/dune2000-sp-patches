#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"
#include "event-core.h"
#include "event-actions.h"

CALL(0x00453F3C, _Mod__HandleEvent);

void Mod__HandleEvent(EventData *event)
{
  ProcessEvent(event);
}

void ExecuteEvent(int event_index)
{
  ProcessEvent(&_gEventArray[event_index]);
}

void ProcessEvent(EventData *event)
{
  // Check if event is blocked and block it if auto-block is set
  if (event->blocked_flags & EBF_BLOCKED)
    return;
  if (event->blocked_flags & EBF_AUTO_BLOCK)
    event->blocked_flags |= EBF_BLOCKED;
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
  // Execute event action
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

void ExecuteEventAction(int event_type, EventContext *e)
{
  switch ( event_type )
  {
  case ET_REINFORCEMENT:          EvAct_AddDelivery     (COORD0, A_SIDE, A_AMNT, A_BOOL, DELIVERYTYPE_REINFORCE, e->data); break;
  case ET_STARPORT_DELIVERY:      EvAct_AddDelivery     (COORD0, A_SIDE, A_AMNT, A_BOOL, DELIVERYTYPE_STARPORT,  e->data); break;
  case ET_ALLEGIANCE:             EvAct_SetDiplomacy    (A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_LEAVE:                  CSide__BlowupAll_surrender(GetSide(A_SIDE)); break;
  case ET_BESERK:                 gAIArray[A_SIDE].__GoBeserk_OtherStates = 1; break;
  case ET_PLAYSOUND:              EvAct_PlaySound       (A_VALUE, A_BOOL, COORD0); break;
  case ET_SETBUILDRATE:           gAIArray[A_SIDE].UnitBuildRate = A_VALUE; break;
  case ET_SETATTACKBUILDINGRATE:  gAIArray[A_SIDE].TimeBetweenBuildingAttacks = A_VALUE; break;
  case ET_SETCASH:                EvAct_SetCash         (A_SIDE, A_ENUM, A_VALUE); break;
  case ET_SETTECH:                EvAct_SetTech         (A_SIDE, A_BOOL, A_VALUE); break;
  case ET_WIN:                    if ( !gLose ) gWin = 1; break;
  case ET_LOSE:                   if ( !gWin ) gLose = 1; break;
  case ET_SWITCH_SIDE:            EvAct_SwitchSide      (A_SIDE, A_ENUM, A_BOOL); break;
  case ET_HIDE_MAP:               EvAct_HideMap         (); break;
  case ET_REVEAL:                 EvAct_RevealMap       (COORD0, A_AMNT); break;
  case ET_SHOWTIMER:              gTimerValue = A_VALUE; break;
  case ET_HIDETIMER:              gTimerValue = -1; break;
  case ET_SHOWMESSAGE:            EvAct_ShowMessage     (A_VALUE, (ShowMessageEventData *)&e->data[1]); break;
  case ET_UNIT_SPAWN:             EvAct_UnitSpawn       (COORD0, A_SIDE, A_AMNT, e->data); break;
  case ET_SET_FLAG:               gConditionArray[A_SIDE].value = A_VALUE; break;
  case ET_UN_BLOCK_EVENT:         EvAct_UnBlockEvent    (A_BOOL, A_VALUE); break;
  case ET_PLAY_MUSIC:             EvAct_PlayMusic       (e->data); break;
  case ET_DAMAGE_TILES:           EvAct_DamageTiles     (COORD0, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_UNIT:               EvAct_AddUnit         (COORD0, A_SIDE, A_ITEM, A_ENUM); break;
  case ET_ADD_BUILDING:           EvAct_AddBuilding     (COORD0, A_SIDE, A_ITEM, A_ENUM); break;
  case ET_ADD_PROJECTILE:         EvAct_AddProjectile   (COORD0, COORD1, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_EXPLOSION:          EvAct_AddExplosion    (COORD0, COORD2, COORD3, A_SIDE, A_ITEM, A_ENUM, A_BOOL); break;
  case ET_ADD_CRATE:              EvAct_AddCrate        (COORD0, A_SIDE, A_AMNT, A_ITEM, A_ENUM, A_VALUE); break;
  case ET_ADD_CONCRETE:           EvAct_AddConcrete     (COORD0, COORD1, A_SIDE, A_VALUE); break;
  case ET_SPICE_BLOOM:            EvAct_SpiceBloom      (COORD0, A_AMNT, A_ENUM, A_BOOL); break;
  case ET_SHAKE_SCREEN:           _ScreenShakes = A_VALUE; break;
  case ET_CENTER_VIEWPORT:        EvAct_CenterViewport  (COORD0); break;
  case ET_CHANGE_MAP_BLOCK:       EvAct_ChangeMapBlock  (COORD0, COORD1, (uint16_t *)&e->data[1]); break;
  case ET_TRANSFORM_TILES:        EvAct_TransformTiles  (A_AMNT, (uint16_t *)&e->data[1]); break;
  case ET_CHANGE_TILE_ATTRIBUTES: EvAct_ChangeTileAttributes(COORD0, COORD1, A_ENUM, A_VALUE); break;
  case ET_CHANGE_TILE_DAMAGE:     EvAct_ChangeTileDamage(COORD0, COORD1, A_ENUM, A_VALUE); break;
  default:
    DebugFatal("event-core.c", "Unknown event type %d", event_type);
  }
}
