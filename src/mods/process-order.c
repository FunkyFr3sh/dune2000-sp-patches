#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"
#include "patch.h"

// Implement harvsCanBeOrderedToUndock rule
// Enable AI Alliances via the the "a" hotkey in skirmish games
// Spawner game end state
// GetTickCount fix

// Custom implementation of function ProcessOrder
DETOUR(0x00457810, 0x00458E84, _Mod__ProcessOrder);

void Mod__ProcessOrder(OrderEntry *order)
{
  OrderEntry *order_; // esi
  short *object_ptr; // ebp MAPDST
  Unit *unit; // edi MAPDST
  unsigned short num_objects; // cx MAPDST
  int object_index; // eax MAPDST
  Unit *target_unit; // eax MAPDST
  unsigned char posx; // al MAPDST
  unsigned char posy; // cl MAPDST
  signed int counter; // ebp MAPDST
  char can_retreat; // al
  int target_side; // eax
  int sound_id; // eax MAPDST
  char *player_name; // ST1C_4 MAPDST
  char target_side_; // ST1C_1
  int text_id; // eax MAPDST
  char *text_string; // eax MAPDST
  int side_id; // eax
  Building *bld; // edi MAPDST
  short building_index; // di
  char behavior; // al
  char devastator_explode_delay; // dl
  UnitFlags unit_flags; // eax
  char queue_found; // dl
  BuildQueueStruct *build_queue; // eax
  short behavior_; // ax
  int death_hand_accuracy; // ebx MAPDST
  signed int target_x; // edi
  signed int target_y; // eax
  unsigned char fremen_added; // bl
  bool zero_objects; // zf
  CSide *side; // [esp+10h] [ebp-14h] MAPDST
  int ypos; // [esp+14h] [ebp-10h]
  int xpos; // [esp+18h] [ebp-Ch]
  unsigned char x1;
  unsigned char y1;
  unsigned char x2;
  unsigned char y2;

  order_ = order;
  switch ( order->orderdata.OrderType )
  {
    case eOrderType_13_UNITMOVE:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      zero_objects = order_->orderdata.NumObjects == 0;
      order = 0;
      if ( !zero_objects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          unit = &side->__ObjectArray[*object_ptr];
          if ( side->__ObjectArray[*object_ptr].ObjectType == 1 && CanUnitAcceptOrders(unit, 1) )
          {
            // New logic start
            // Implement harvsCanBeOrderedToUndock rule
            bool undocking_harvester = rulesExt__harvsCanBeOrderedToUndock && (unit->State == UNIT_STATE_14_ENTERING_REFINERY || unit->State == UNIT_STATE_15_UNLOADING_SPICE || unit->State == UNIT_STATE_16_LEAVING_REFINERY);
            if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER
              && gGameMap.map[(unsigned short)order_->orderdata.BlockToX
                            + _CellNumbersWidthSpan[(unsigned short)order_->orderdata.BlockToY]].__tile_bitflags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE) )
            {
              if ( UnitAdjustState(unit, UNIT_STATE_11_MOVING_TO_HARVEST) || undocking_harvester )
              {
                unit->OldState = 1;
                unit->TargetX = order_->orderdata.BlockToX;
                unit->TargetY = order_->orderdata.BlockToY;
                unit->__RememberPosX = order_->orderdata.BlockToX;
                unit->__RememberPosY = order_->orderdata.BlockToY;
                if (undocking_harvester)
                  unit->__Lying = UNIT_STATE_11_MOVING_TO_HARVEST;
              }
            }
            else if ( UnitAdjustState(unit, UNIT_STATE_7_MOVING) || undocking_harvester )
            {
              unit->OldState = 1;
              unit->TargetX = order_->orderdata.BlockToX;
              unit->TargetY = order_->orderdata.BlockToY;
              if (undocking_harvester)
              {
                unit->__RememberPosX = order_->orderdata.BlockToX;
                unit->__RememberPosY = order_->orderdata.BlockToY;
                unit->__Lying = UNIT_STATE_7_MOVING;
              }
            }
            // New logic end
          }
          num_objects = order_->orderdata.NumObjects;
          ++object_ptr;
          order = (OrderEntry *)((char *)order + 1);
        }
        while ( (signed int)order < num_objects );
      }
      break;
    case eOrderType_14_UNITATTACKUNIT:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      target_unit = &GetSide(order_->orderdata.TargetSideId)->__ObjectArray[(unsigned short)order_->orderdata.TargetObjectIndex];
      if ( target_unit->ObjectType == 1 && !(target_unit->Flags & (UFLAGS_400000|UFLAGS_100_CARRYING|UFLAGS_40_FLYING)) )
      {
        zero_objects = order_->orderdata.NumObjects == 0;
        order = 0;
        if ( !zero_objects )
        {
          object_ptr = order_->orderdata.ObjectArray;
          do
          {
            if ( *object_ptr >= 1000 )
            {
              DebugFatal("ProcessOrder", "fUnitArray index check failed");
            }
            unit = &side->__ObjectArray[*object_ptr];
            if ( side->__ObjectArray[*object_ptr].ObjectType == 1
              && CanUnitAcceptOrders(unit, 1)
              && _templates_unitattribs[unit->Type].__PrimaryWeapon != -1
              && (order_->orderdata.SideId != order_->orderdata.TargetSideId
               || *object_ptr != (unsigned short)order_->orderdata.TargetObjectIndex)
              && UnitAdjustState(unit, UNIT_STATE_4_ATTACKING_UNIT) )
            {
              unit->OldState = 1;
              unit->EnemySide = order_->orderdata.TargetSideId;
              if ( order_->orderdata.TargetSideId > 8u )
              {
                DebugFatal("Model.cpp", "EnemySide > kMaxSides");
              }
              unit->EnemyIndex = order_->orderdata.TargetObjectIndex;
            }
            num_objects = order_->orderdata.NumObjects;
            ++object_ptr;
            order = (OrderEntry *)((char *)order + 1);
          }
          while ( (signed int)order < num_objects );
        }
      }
      break;
    case eOrderType_15_UNITATTACKBUILDING:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      if ( GetSide(order_->orderdata.TargetSideId)->__ObjectArray[(unsigned short)order_->orderdata.TargetObjectIndex].ObjectType == 2 )
      {
        zero_objects = order_->orderdata.NumObjects == 0;
        order = 0;
        if ( !zero_objects )
        {
          object_ptr = order_->orderdata.ObjectArray;
          do
          {
            if ( *object_ptr >= 1000 )
            {
              DebugFatal("ProcessOrder", "fUnitArray index check failed");
            }
            unit = &side->__ObjectArray[*object_ptr];
            if ( side->__ObjectArray[*object_ptr].ObjectType == 1 && CanUnitAcceptOrders(unit, 1) )
            {
              if ( UnitAdjustState(unit, UNIT_STATE_5_ATTACKING_BUILDING) )
              {
                unit->OldState = 1;
                unit->EnemySide = order_->orderdata.TargetSideId;
                if ( order_->orderdata.TargetSideId > 8u )
                {
                  DebugFatal("Model.cpp", "EnemySide > kMaxSides");
                }
                unit->EnemyIndex = order_->orderdata.TargetObjectIndex;
              }
            }
            num_objects = order_->orderdata.NumObjects;
            ++object_ptr;
            order = (OrderEntry *)((char *)order + 1);
          }
          while ( (signed int)order < num_objects );
        }
      }
      break;
    case eOrderType_16_UNITATTACKTILE:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      zero_objects = order_->orderdata.NumObjects == 0;
      order = 0;
      if ( !zero_objects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          unit = &side->__ObjectArray[*object_ptr];
          if ( side->__ObjectArray[*object_ptr].ObjectType == 1
            && CanUnitAcceptOrders(unit, 1)
            && _templates_unitattribs[unit->Type].__PrimaryWeapon != -1
            && UnitAdjustState(unit, UNIT_STATE_6_ATTACKING_TILE) )
          {
            unit->OldState = 1;
            unit->TargetX = order_->orderdata.BlockToX;
            unit->TargetY = order_->orderdata.BlockToY;
          }
          num_objects = order_->orderdata.NumObjects;
          ++object_ptr;
          order = (OrderEntry *)((char *)order + 1);
        }
        while ( (signed int)order < num_objects );
      }
      break;
    case eOrderType_17_DOCKWITHREFINERY:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      zero_objects = order_->orderdata.NumObjects == 0;
      order = 0;
      if ( !zero_objects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          object_index = *object_ptr;
          unit = &side->__ObjectArray[object_index];
          if ( side->__ObjectArray[object_index].ObjectType == 1
            && CanUnitAcceptOrders(unit, 1)
            && _templates_unitattribs[unit->Type].__Behavior == 1
            && UnitAdjustState(unit, UNIT_STATE_13_MOVING_TO_REFINERY) )
          {
            unit->OldState = 1;
            unit->RefineryIndex = order_->orderdata.TargetObjectIndex;
          }
          num_objects = order_->orderdata.NumObjects;
          ++object_ptr;
          order = (OrderEntry *)((char *)order + 1);
        }
        while ( (signed int)order < num_objects );
      }
      break;
    case eOrderType_18_GUARD:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      zero_objects = order_->orderdata.NumObjects == 0;
      order = 0;
      if ( !zero_objects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          unit = &side->__ObjectArray[*object_ptr];
          if ( side->__ObjectArray[*object_ptr].ObjectType == 1
            && CanUnitAcceptOrders(unit, 1)
            && _templates_unitattribs[unit->Type].__PrimaryWeapon != -1
            && UnitAdjustState(unit, UNIT_STATE_3_GUARDING) )
          {
            posx = unit->BlockToX;
            posy = unit->BlockToY;
            unit->OldState = 1;
            unit->__RememberPosX = posx;
            unit->__RememberPosY = posy;
            unit->EnemyIndex = -1;
          }
          num_objects = order_->orderdata.NumObjects;
          ++object_ptr;
          order = (OrderEntry *)((char *)order + 1);
        }
        while ( (signed int)order < num_objects );
      }
      break;
    case eOrderType_19_SCATTER:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      counter = 0;
      if ( order_->orderdata.NumObjects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          if ( side->__ObjectArray[*object_ptr].ObjectType == OBJECT_UNIT
            && CanUnitAcceptOrders(&side->__ObjectArray[*object_ptr], 1) )
          {
            MoveUnitInRandomDirection((eSideType)order_->orderdata.SideId, *object_ptr);
          }
          ++counter;
          ++object_ptr;
        }
        while ( counter < (unsigned short)order_->orderdata.NumObjects );
      }
      break;
    case eOrderType_1A_RETREAT:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      zero_objects = order_->orderdata.NumObjects == 0;
      order = 0;
      if ( !zero_objects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          object_index = *object_ptr;
          unit = &side->__ObjectArray[object_index];
          if ( side->__ObjectArray[object_index].ObjectType == OBJECT_UNIT
            && CanUnitAcceptOrders(&side->__ObjectArray[object_index], 1) )
          {
            if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER
              && side->__BuildingsExistPerGroup[(unsigned char)_templates_GroupIDs.Refinery] )
            {
              UnitAdjustState(unit, UNIT_STATE_13_MOVING_TO_REFINERY);
              can_retreat = 0;
            }
            else
            {
              can_retreat = 1;
            }
            if ( can_retreat )
            {
              if ( UnitAdjustState(unit, UNIT_STATE_7_MOVING) )
              {
                unit->TargetX = order_->orderdata.BlockToX;
                unit->TargetY = order_->orderdata.BlockToY;
              }
            }
          }
          num_objects = order_->orderdata.NumObjects;
          ++object_ptr;
          order = (OrderEntry *)((char *)order + 1);
        }
        while ( (signed int)order < num_objects );
      }
      break;
    case eOrderType_1B_ALLY:
      if ( order->orderdata.NumObjects != 1 )
      {
        DebugFatal("ProcessOrder", "Invalid esUnitAlly command");
      }
      target_side = order_->orderdata.ObjectArray[0];
      if ( _gDiplomacy[order_->orderdata.SideId][target_side] == 1 )
      {
        // New logic start
        // Enable AI Alliances via the the "a" hotkey in skirmish games
        if ( !checksides_4553E0((eSideType)order_->orderdata.SideId, order_->orderdata.ObjectArray[0])
          || (_gAIArray[order_->orderdata.ObjectArray[0]].__IsAI && gGameType != GAME_SKIRMISH) )
        // New logic end
        {
          text_id = GetTextID("UnableToAlly");
          text_string = GetTextString(text_id, 1);
          QueueMessage(text_string, -1);
        }
        else
        {
          sound_id = GetSoundTableID("S_CHATMSG");
          QueueAudioToPlay(sound_id, 0, 0, 0);
          if ( _IsMultiplayer )
          {
            player_name = _NetPlayerNamesArray[order_->orderdata.ObjectArray[0]];
            text_id = GetTextID("Ally");
            text_string = GetTextString(text_id, 1);
            sprintf(
              processorderbuffer,
              "%s %s %s",
              _NetPlayerNamesArray[order_->orderdata.SideId],
              text_string,
              player_name);
          }
          else
          {
            text_id = GetTextID("UI_Computer");
            player_name = GetTextString(text_id, 1);
            text_id = GetTextID("Ally");
            text_string = GetTextString(text_id, 1);
            sprintf(processorderbuffer, "%s %s %s", _someNameString, text_string, player_name);
          }
          QueueMessage(processorderbuffer, -1);
          _gDiplomacy[order_->orderdata.SideId][order_->orderdata.ObjectArray[0]] = 0;
          // New logic start
          // Enable AI Alliances via the the "a" hotkey in skirmish games
          if (gGameType == GAME_SKIRMISH)
          {
            _gDiplomacy[order_->orderdata.ObjectArray[0]][order_->orderdata.SideId] = 0;
          }
          // New logic end
          target_side_ = order_->orderdata.ObjectArray[0];
          side = GetSide(order_->orderdata.SideId);
          CSide__ResetEnemyForSide(side, target_side_);
        }
      }
      else
      {
        if ( _IsMultiplayer )
        {
          player_name = _NetPlayerNamesArray[target_side];
          text_id = GetTextID("BreakAlly");
          text_string = GetTextString(text_id, 1);
          sprintf(
            processorderbuffer,
            "%s %s %s",
            _NetPlayerNamesArray[order_->orderdata.SideId],
            text_string,
            player_name);
        }
        else
        {
          text_id = GetTextID("UI_Computer");
          player_name = GetTextString(text_id, 1);
          text_id = GetTextID("BreakAlly");
          text_string = GetTextString(text_id, 1);
          sprintf(processorderbuffer, "%s %s %s", _someNameString, text_string, player_name);
        }
        QueueMessage(processorderbuffer, -1);
        side_id = order_->orderdata.SideId;
        if ( !_gDiplomacy[order_->orderdata.ObjectArray[0]][side_id] )
        {
          if ( _IsMultiplayer )
          {
            player_name = _NetPlayerNamesArray[side_id];
            text_id = GetTextID("BreakAlly");
            text_string = GetTextString(text_id, 1);
            sprintf(
              processorderbuffer,
              "%s %s %s",
              _NetPlayerNamesArray[order_->orderdata.ObjectArray[0]],
              text_string,
              player_name);
          }
          else
          {
            text_id = GetTextID("BreakAlly");
            text_string = GetTextString(text_id, 1);
            text_id = GetTextID("UI_Computer");
            player_name = GetTextString(text_id, 1);
            sprintf(processorderbuffer, "%s %s %s", player_name, text_string, _someNameString);
          }
          QueueMessage(processorderbuffer, -1);
        }
        sound_id = GetSoundTableID("S_CHATMSG");
        QueueAudioToPlay(sound_id, 0, 0, 0);
        _gDiplomacy[order_->orderdata.SideId][order_->orderdata.ObjectArray[0]] = 1;
        _gDiplomacy[order_->orderdata.ObjectArray[0]][order_->orderdata.SideId] = 1;
      }
      break;
    case eOrderType_1C_STOPUNIT:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      counter = 0;
      if ( order_->orderdata.NumObjects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          unit = &side->__ObjectArray[*object_ptr];
          if ( side->__ObjectArray[*object_ptr].ObjectType == OBJECT_UNIT && CanUnitAcceptOrders(unit, 1) )
          {
            StopUnit(order_->orderdata.SideId, *object_ptr);
          }
          ++counter;
          ++object_ptr;
        }
        while ( counter < (unsigned short)order_->orderdata.NumObjects );
      }
      break;
    case eOrderType_1D_STOPBUILDING:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      counter = 0;
      if ( order_->orderdata.NumObjects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          if ( side->__ObjectArray[*object_ptr].ObjectType == OBJECT_BUILDING )
          {
            StopBuilding(order_->orderdata.SideId, *object_ptr);
          }
          ++counter;
          ++object_ptr;
        }
        while ( counter < (unsigned short)order_->orderdata.NumObjects );
      }
      break;
    case eOrderType_1E_BUILDINGATTACKUNIT:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      target_unit = &GetSide(order_->orderdata.TargetSideId)->__ObjectArray[(unsigned short)order_->orderdata.TargetObjectIndex];
      if ( target_unit->ObjectType == OBJECT_UNIT
        && !(target_unit->Flags & (UFLAGS_400000|UFLAGS_100_CARRYING|UFLAGS_40_FLYING)) )
      {
        counter = 0;
        if ( order_->orderdata.NumObjects )
        {
          object_ptr = order_->orderdata.ObjectArray;
          do
          {
            if ( *object_ptr >= 1000 )
            {
              DebugFatal("ProcessOrder", "fUnitArray index check failed");
            }
            object_index = *object_ptr;
            bld = (Building *)&side->__ObjectArray[object_index];
            if ( side->__ObjectArray[object_index].ObjectType == OBJECT_BUILDING
              && _templates_buildattribs[LOBYTE(side->__ObjectArray[object_index].__PosX)]._____BarrelArt != -1
              && (order_->orderdata.SideId != order_->orderdata.TargetSideId
               || object_index != (unsigned short)order_->orderdata.TargetObjectIndex)
              && SetBuildingState((Building *)&side->__ObjectArray[object_index], BLD_STATE_4_ATTACKING_UNIT) )
            {
              bld->EnemySide = order_->orderdata.TargetSideId;
              bld->EnemyIndex = order_->orderdata.TargetObjectIndex;
            }
            ++counter;
            ++object_ptr;
          }
          while ( counter < (unsigned short)order_->orderdata.NumObjects );
        }
      }
      break;
    case eOrderType_1F_BUILDINGATTACKBUILDING:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      if ( GetSide(order_->orderdata.TargetSideId)->__ObjectArray[(unsigned short)order_->orderdata.TargetObjectIndex].ObjectType == OBJECT_BUILDING )
      {
        counter = 0;
        if ( order_->orderdata.NumObjects )
        {
          object_ptr = order_->orderdata.ObjectArray;
          do
          {
            if ( *object_ptr >= 1000 )
            {
              DebugFatal("ProcessOrder", "fUnitArray index check failed");
            }
            object_index = *object_ptr;
            bld = (Building *)&side->__ObjectArray[object_index];
            if ( side->__ObjectArray[object_index].ObjectType == OBJECT_BUILDING
              && _templates_buildattribs[LOBYTE(side->__ObjectArray[object_index].__PosX)]._____BarrelArt != -1
              && (order_->orderdata.SideId != order_->orderdata.TargetSideId
               || object_index != (unsigned short)order_->orderdata.TargetObjectIndex)
              && SetBuildingState((Building *)&side->__ObjectArray[object_index], BLD_STATE_5_ATTACKING_BUILDING) )
            {
              bld->EnemySide = order_->orderdata.TargetSideId;
              bld->EnemyIndex = order_->orderdata.TargetObjectIndex;
            }
            ++counter;
            ++object_ptr;
          }
          while ( counter < (unsigned short)order_->orderdata.NumObjects );
        }
      }
      break;
    case eOrderType_20_SETPRIMARY:
      building_index = order->orderdata.ObjectArray[0];
      if ( GetSide(order->orderdata.SideId)->__ObjectArray[building_index].ObjectType == OBJECT_BUILDING )
      {
        SetBuildingAsPrimary((eSideType)order_->orderdata.SideId, building_index);
      }
      break;
    case eOrderType_21_UNITREPAIR:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      zero_objects = order_->orderdata.NumObjects == 0;
      order = 0;
      if ( !zero_objects )
      {
        object_ptr = order_->orderdata.ObjectArray;
        do
        {
          if ( *object_ptr >= 1000 )
          {
            DebugFatal("ProcessOrder", "fUnitArray index check failed");
          }
          object_index = *object_ptr;
          unit = &side->__ObjectArray[object_index];
          if ( side->__ObjectArray[object_index].ObjectType == OBJECT_UNIT
            && CanUnitAcceptOrders(unit, 0)
            && !_templates_unitattribs[unit->Type].__IsInfantry
            && UnitAdjustState(unit, UNIT_STATE_24_MOVING_TO_REPAIR_PAD) )
          {
            unit->OldState = OBJECT_UNIT;
            unit->RepairPadIndex = order_->orderdata.TargetObjectIndex;
          }
          num_objects = order_->orderdata.NumObjects;
          ++object_ptr;
          order = (OrderEntry *)((char *)order + 1);
        }
        while ( (signed int)order < num_objects );
      }
      break;
    case eOrderType_22_BUILDINGREPAIR:
      ModelBuildingRepair((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_23_BUILDINGSELL:
      ModelBuildingSell((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_24_BUILDINGPICK:
      ModelBuildBuildingPick(order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_25_BUILDINGCANCEL:
      ModelBuildBuildingCancel((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_26_BUILDINGPLACE:
      side = GetSide(order->orderdata.SideId);
      side->__BuildingBuildQueue.c_field_11_cancel = 0;
      if ( _templates_buildattribs[order_->orderdata.ObjectArray[0]].__Behavior == BuildingBehavior_CONCRETE )
      {
        side->__BuildingBuildQueue.__type = -1;
        ModelAddConcrete(
          (eSideType)order_->orderdata.SideId,
          order_->orderdata.ObjectArray[0],
          order_->orderdata.__PlaceBuildingX,
          order_->orderdata.__PlaceBuildingY,
          0,
          _templates_buildattribs[order_->orderdata.ObjectArray[0]]._____TilesOccupiedSolid);
        if ( order_->orderdata.SideId == gSideId )
        {
          posy = order_->orderdata.__PlaceBuildingY;
          posx = order_->orderdata.__PlaceBuildingX;
          sound_id = GetSoundTableID("S_BUILDCONC");
          goto LABEL_241;
        }
      }
      else if ( CheckBuildingCanBePlacedAt(
                  order_->orderdata.ObjectArray[0],
                  order_->orderdata.__PlaceBuildingX,
                  order_->orderdata.__PlaceBuildingY) )
      {
        side->__BuildingBuildQueue.__type = -1;
        ModelAddBuilding(
          (eSideType)order_->orderdata.SideId,
          order_->orderdata.ObjectArray[0],
          order_->orderdata.__PlaceBuildingX,
          order_->orderdata.__PlaceBuildingY,
          0,
          0,
          0);
        if ( order_->orderdata.SideId == gSideId )
        {
          posy = order_->orderdata.__PlaceBuildingY;
          posx = order_->orderdata.__PlaceBuildingX;
          sound_id = GetSoundTableID("S_BUILDUP");
          goto LABEL_241;
        }
      }
      break;
    case eOrderType_27_UNITPICK:
      ModelBuildUnitPick((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_28_UNITCANCEL:
      ModelBuildUnitCancel((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_29_STARPORTPICK:
      ModelStarportPick((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_2A_STARPORTUNPICK:
      ModelStarportUnpick((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_2B_STARPORTPURCHASE:
      ModelStarportPurchase((eSideType)order->orderdata.SideId);
      break;
    case eOrderType_2C_STARPORTCANCEL:
      ModelStarportCancel((eSideType)order->orderdata.SideId);
      break;
    case eOrderType_2D_UPGRADEPICK:
      ModelUpgradePick((eSideType)order->orderdata.SideId, (eBuildingGroupType)order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_2E_UPGRADECANCEL:
      ModelUpgradeCancel((eSideType)order->orderdata.SideId, order->orderdata.ObjectArray[0]);
      break;
    case eOrderType_2F_DEPLOY:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      if ( order_->orderdata.ObjectArray[0] >= 1000 )
      {
        DebugFatal("ProcessOrder", "fUnitArray index check failed");
      }
      unit = &side->__ObjectArray[order_->orderdata.ObjectArray[0]];
      if ( unit->ObjectType == 1 && CanUnitAcceptOrders(unit, 1) )
      {
        behavior = _templates_unitattribs[unit->Type].__Behavior;
        switch ( behavior )
        {
          case UnitBehavior_MCV:
            if ( UnitAdjustState(unit, UNIT_STATE_30_DEPLOYING) )
            {
              unit->OldState = 1;
            }
            break;
          case UnitBehavior_DEVASTATOR:
            if ( UnitAdjustState(unit, UNIT_STATE_31_SELFDESTRUCT) )
            {
              devastator_explode_delay = _gVariables.DevastatorExplodeDelay;
              unit->OldState = 1;
              unit->__SpecialPurpose = devastator_explode_delay;
            }
            break;
          case UnitBehavior_THUMPER:
            if ( UnitAdjustState(unit, UNIT_STATE_32_THUMPERING) )
            {
              unit->OldState = 1;
            }
            break;
          case UnitBehavior_SABOTEUR:
            unit_flags = unit->Flags;
            if ( !(unit_flags & UFLAGS_10_STEALTH) )
            {
              posx = unit->BlockToX;
              unit_flags = unit_flags | UFLAGS_10_STEALTH;
              unit->Flags = unit_flags;
              posy = unit->BlockToY;
              sound_id = GetSoundTableID("S_STEALTHUP");
LABEL_241:
              PlaySoundAt(sound_id, posx, posy);
            }
            break;
        }
      }
      break;
    case eOrderType_30_SPECIAL:
      side = GetSide(order->orderdata.SideId);
      if ( !side )
      {
        DebugFatal("ProcessOrder", "Invalid side %d", order_->orderdata.SideId);
      }
      queue_found = 0;
      build_queue = side->__UnitBuildQueue;
      counter = 10;
      do
      {
        if ( build_queue->__type >= 0
          && (unsigned char)_templates_unitattribs[build_queue->__type].__Behavior == order_->orderdata.ObjectArray[0]
          && build_queue->__build_progress == 23040 )
        {
          build_queue->__type = -1;
          build_queue->c_field_11_cancel = 0;
          queue_found = 1;
        }
        ++build_queue;
        --counter;
      }
      while ( counter );
      if ( queue_found )
      {
        behavior_ = order_->orderdata.ObjectArray[0];
        switch ( behavior_ )
        {
          case UnitBehavior_DEATH_HAND:
            death_hand_accuracy = (unsigned char)_gVariables.DeathHandAccuracy;
            target_x = death_hand_accuracy
                     + (unsigned short)order_->orderdata.BlockToX
                     - GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\model.cpp", 2685)
                     % (unsigned int)(2 * death_hand_accuracy);
            death_hand_accuracy = (unsigned char)_gVariables.DeathHandAccuracy;
            target_y = death_hand_accuracy
                     + (unsigned short)order_->orderdata.BlockToY
                     - GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\model.cpp", 2686)
                     % (unsigned int)(2 * death_hand_accuracy);
            if ( target_x < 0 )
            {
              target_x = 0;
            }
            if ( target_x > gGameMap.width - 1 )
            {
              target_x = gGameMap.width - 1;
            }
            if ( target_y < 0 )
            {
              target_y = 0;
            }
            if ( target_y > gGameMap.height - 1 )
            {
              target_y = gGameMap.height - 1;
            }
            LaunchDeathHand((eSideType)order_->orderdata.SideId, target_x, target_y);
            break;
          case UnitBehavior_ORNITHOPTER:
            CSide__46CF10_HKEY_BattleFieldPos(side, &xpos, &ypos, 0);
            LaunchOrnithopters(
              order_->orderdata.SideId,
              xpos / 32 & 0xFFFF,
              ypos / 32 & 0xFFFF,
              order_->orderdata.BlockToX,
              order_->orderdata.BlockToY);
            break;
          case UnitBehavior_SABOTEUR:
            if ( GetBuildingProducedUnitWillArriveFrom(
                   _templates_GroupIDs.Saboteur,
                   order_->orderdata.SideId,
                   &x1,
                   &y1,
                   &x2,
                   &y2,
                   1) )
            {
              ModelAddUnit(
                (eSideType)order_->orderdata.SideId,
                _templates_GroupIDs.Saboteur,
                x1,
                y1,
                x2,
                y2,
                0,
                0);
            }
            break;
          case UnitBehavior_FREMEN:
            if ( GetBuildingProducedUnitWillArriveFrom(
                   _templates_GroupIDs.Fremen,
                   order_->orderdata.SideId,
                   &x1,
                   &y1,
                   &x2,
                   &y2,
                   1) )
            {
              fremen_added = 0;
              if ( _gVariables.NumberOfFremen )
              {
                do
                {
                  ModelAddUnit(
                    (eSideType)order_->orderdata.SideId,
                    _templates_GroupIDs.Fremen,
                    x1,
                    y1,
                    x2,
                    y2,
                    0,
                    0);
                  ++fremen_added;
                }
                while ( fremen_added < _gVariables.NumberOfFremen );
              }
            }
            break;
        }
      }
      break;
    case eOrderType_31_SURRENDER:
      side = GetSide(order->orderdata.SideId);
      if ( side )
      {
        CSide__BlowupAll_surrender(side);
      }
      Map__PlayerDefeated(order_->orderdata.SideId);
      if ( gGameType == GAME_INTERNET )
      {
        zero_objects = _GameEndState == END_UNKNOWN;
        goto LABEL_278;
      }
      break;
    case eOrderType_32_AITAKEOVER:
      side = GetSide(order->orderdata.SideId);
      if ( side )
      {
        CSide__LetAITakeOver(side);
      }
      // New logic start
      // Spawner game end state
      if (SpawnerGameEndState == END_NORMALLY)
        SpawnerGameEndState = END_OPPONENT_SURRENDERED;
      // New logic end
      zero_objects = gGameType == GAME_INTERNET;
LABEL_278:
      if ( zero_objects )
      {
        _GameEndState = 2;
        // New logic start
        // GetTickCount fix
        _EndTime = fake_GetTickCount();
        // New logic end
      }
      break;
    case eOrderType_33_OFFERDRAW:
      if ( order->orderdata.__PlaceBuildingX )
      {
        if ( order->orderdata.SideId == gSideId )
        {
          _DrawOffered1 = 1;
          _DrawOffered2 = 0;
        }
        else
        {
          _DrawOffered1 = 0;
          _DrawOffered2 = 1;
        }
      }
      else
      {
        _DrawOffered1 = 0;
        _DrawOffered2 = 0;
      }
      break;
    case eOrderType_34_OFFEREDDRAW:
      if ( order->orderdata.SideId == gSideId )
      {
        _DrawOffered1 = 1;
      }
      else if ( _DrawOffered1 )
      {
        _DrawOffered2 = 1;
      }
      break;
  }
}
