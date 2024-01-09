#include "macros/patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"
#include "messages-func.h"
#include "utils.h"
#include "hotkeys.h"
#include "rules.h"
#include "patch.h"

void SetMouseCursorForUnitMovementRestriction();

// Fix cursor over enemy building when only armed engineer/saboteur is selected
bool IsAnyArmedUnitSelectedExceptEngineerAndSaboteur()
{
  for (Unit *unit = GetSide(gSideId)->__FirstUnitPtr; unit; unit = unit->Next)
  {
    UnitAtribStruct *unit_template = &_templates_unitattribs[unit->Type];
    if (unit->__IsSelected &&
        unit_template->__PrimaryWeapon != -1 &&
        unit_template->__Behavior != UnitBehavior_SABOTEUR &&
        unit_template->__Behavior != UnitBehavior_ENGINEER)
      return 1;
  }
  return 0;
}

// Custom implementation of function HandleGameLoopEvents
CALL(0x00449852, _Mod__HandleGameLoopEvents);

void Mod__HandleGameLoopEvents()
{
  int map_height; // edi
  int radar_image_width; // ecx
  int radar_image_height; // esi
  int radar_image_left; // ebx MAPDST
  int radar_image_top; // ebp MAPDST
  unsigned int pixel_x; // ebx MAPDST
  unsigned int pixel_y; // ebp MAPDST
  int tile_x; // esi MAPDST
  signed int tile_y; // edi
  bool hidden_under_shroud; // al MAPDST
  int cell_index; // ecx MAPDST
  Unit *target; // eax
  char unit_behavior; // cl MAPDST
  char tile_flags_byte1; // bh
  int building_type; // eax MAPDST
  int building_flags; // ecx MAPDST
  Unit *unit; // esi MAPDST
  int unit_type; // eax MAPDST
  bool dragging_bandbox; // bl
  int sidebar_icon_count; // ebx
  int pixel_x_; // ecx
  int pixel_y_; // esi
  int starport_icon_index; // esi MAPDST
  int starport_unit_type; // esi
  int upgrade_icon_index; // esi MAPDST
  int upgrade_building_type; // esi
  int unit_icon_index; // esi
  unsigned int building_icon_count; // eax
  unsigned int unit_icon_count; // eax
  char *v42; // eax
  int sound_id; // eax MAPDST
  char *v47; // eax
  char building_group; // si
  signed int sidebar_icon_mouse_pos; // esi MAPDST
  int building_icon_index; // esi
  int v56; // eax
  char v57; // dl
  unsigned __int8 v60; // di
  LONG mouse_x; // ebx
  char v62; // bl
  LONG mouse_y; // edx
  unsigned __int8 place_y; // al
  int atreides_sound_id; // eax MAPDST
  int unit_type_; // eax
  char is_ornithopter; // bl
  BuildQueueStruct *build_queue; // eax MAPDST
  char is_deathhand; // bl
  signed int counter; // edi MAPDST
  int v87; // eax
  eSideType my_side_id; // al
  char unit_is_selected; // al
  Building *building_; // ecx MAPDST
  Building *v101; // eax
  int viewporty; // edi
  signed int viewportx; // esi
  char ctrl_key_is_down; // dl
  MACRO_VK bookmark_key_id; // ecx
  signed int v117; // eax
  int unit_group_key_id; // eax
  char v119; // cl
  char unit_group_id; // bl
  CSide *side; // eax MAPDST
  char *v122; // esi
  unsigned __int8 v125; // al
  unsigned __int8 v129; // al
  CUIManager *uimgr; // ebp
  char v135; // dl
  _BYTE *v136; // eax
  int v137; // ecx
  unsigned __int8 v138; // si
  char v139; // cl
  int v140; // eax
  char v141; // bl
  unsigned __int8 v142; // dl
  int v143; // edx
  char *string; // eax MAPDST
  int string_id; // eax MAPDST
  int ordos_sound_id; // [esp-14h] [ebp-F0h] MAPDST
  int harkonnen_sound_id; // [esp-10h] [ebp-ECh] MAPDST
  char v155; // [esp-Ch] [ebp-E8h]
  char side_id; // [esp+10h] [ebp-CCh]
  __int16 object_index; // [esp+14h] [ebp-C8h] MAPDST
  Building *building; // [esp+18h] [ebp-C4h]
  unsigned __int8 a1[4]; // [esp+1Ch] [ebp-C0h]
  int a1_1;
  Unit *a1_2;
  int tile_flags; // [esp+20h] [ebp-BCh]
  CSide *my_side; // [esp+24h] [ebp-B8h]
  unsigned __int8 my_side_1;
  unsigned __int8 y[4]; // [esp+28h] [ebp-B4h]
  unsigned __int8 x[4]; // [esp+2Ch] [ebp-B0h]
  int v166; // [esp+30h] [ebp-ACh]
  unsigned __int8 v166_1;
  Orderdata order; // [esp+34h] [ebp-A8h]

  my_side = GetSide(gSideId);
  if ( gGameMap.width % 2 )
  {
    DebugFatal("HandleGameLoopEvents", " gGameMap.width = %d, not even", gGameMap.width);
  }
  map_height = gGameMap.height;
  if ( gGameMap.height % 2 )
  {
    DebugFatal("HandleGameLoopEvents", " gGameMap.height = %d, not even", gGameMap.height);
    map_height = gGameMap.height;
  }
  radar_image_width = _RadarHouseImages[0]->width;
  radar_image_height = _RadarHouseImages[0]->height;
  radar_image_left = (_RadarHouseImages[0]->width - gGameMap.width) / 2;
  radar_image_top = (_RadarHouseImages[0]->height - map_height) / 2;
  a1_1 = (_RadarHouseImages[0]->width - gGameMap.width) / 2;
  v166 = radar_image_top;
  if ( !_TacticalData.__DraggingBandbox )
  {
    if ( _gMousePos.x >= _ViewportWidth || _gMousePos.y <= _OptionsBarHeight )
    {
LABEL_146:
      if ( _gMousePos.x > radar_image_left + _RadarLocationX
        && _gMousePos.x < radar_image_width + _RadarLocationX - radar_image_left
        && _gMousePos.y > radar_image_top + _RadarLocationY
        && _gMousePos.y < radar_image_height + _RadarLocationY - radar_image_top
        && !_TacticalData.__RadarScoreView )
      {
        // New logic start
        // Always show radar map if rule alwaysShowRadar is set to true
        if ( (IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_OUTPOST) && my_side->__PowerPercent1 >= 100u) || rulesExt__alwaysShowRadar  )
        // New logic end
        {
          if ( IsAnyUnitSelected() )
          {
            SetMouseCursor(CURSOR_MOVE_RADAR);
            goto LABEL_164;
          }
        }
      }
      goto LABEL_163;
    }
    pixel_x = _ViewportXPos + _gMousePos.x;
    pixel_y = _gMousePos.y + _ViewportYPos - _OptionsBarHeight;
    tile_x = (unsigned int)(_ViewportXPos + _gMousePos.x) >> 5;
    tile_y = pixel_y >> 5;
    if ( tile_x >= gGameMap.width || tile_y >= gGameMap.height )
    {
      // New logic start
      // High resolution patch - fix Model__HandleGameLoopEvents() - Invalid index to gGameMap.map (1) error
      goto LABEL_146;
      // New logic end
    }
    hidden_under_shroud = 0;
    cell_index = tile_x + _CellNumbersWidthSpan[tile_y];
    tile_flags = gGameMap.map[cell_index].__tile_bitflags;
    if ( _bool_shroud_4DFB04 )
    {
      hidden_under_shroud = (gGameMap.map[cell_index].__shroud & 0xF) == 1;
    }
    if ( _TacticalData.__SidebarButtonMode )
    {
      switch ( _TacticalData.__SidebarButtonMode )
      {
        case 2:
          SetMouseCursor(CURSOR_CANT_REPAIR);
          if ( GetBuildingOnTile_0(tile_x, pixel_y >> 5, &building, (eSideType *)&side_id, &object_index) )
          {
            if ( side_id == gSideId )
            {
              building_type = 268 * building->Type;
              if ( building->Health < *(int *)((char *)&_templates_buildattribs[0]._____HitPoints + building_type) )
              {
                building_flags = *(int *)((char *)&_templates_buildattribs[0]._____Flags + building_type);
                if ( building_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
                {
                  SetMouseCursor(CURSOR_REPAIR);
                }
              }
            }
          }
          else
          {
            unit = GetUnitOnTile(32 * tile_x, 32 * tile_y, (eSideType *)&side_id, &object_index, 0);
            if ( unit )
            {
              if ( IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_REPAIR) )
              {
                if ( side_id == gSideId )
                {
                  unit_type = unit->Type << 8;
                  if ( unit->Health < *(int *)((char *)&_templates_unitattribs[0].__Strength + unit_type)
                    && !_templates_unitattribs[unit->Type].__IsInfantry )
                  {
                    SetMouseCursor(CURSOR_REPAIR);
                  }
                }
              }
            }
          }
          break;
        case 3:
          SetMouseCursor(CURSOR_CANT_SELL);
          // New logic start
          // Fix improper mouse cursor (engineer/saboteur/sell) when building has infiltrated flag
          if ( GetBuildingOnTile_0(tile_x, pixel_y >> 5, &building, (eSideType *)&side_id, &object_index)
            && side_id == gSideId
            && !(building->Flags & BFLAGS_1000000_INFILTRATED) )
          // New logic end
          {
            SetMouseCursor(CURSOR_SELL);
          }
          break;
        case 4:
          SetMouseCursor(CURSOR_SPECIAL);
          break;
        case 5:
          SetMouseCursor(CURSOR_DEATHHAND);
          break;
        default:
          goto LABEL_163;
      }
    }
    else
    {
      if ( hidden_under_shroud )
      {
        if ( IsAnyUnitSelected() )
        {
          SetMouseCursor(CURSOR_MOVE);
          goto LABEL_164;
        }
        goto LABEL_163;
      }
      if ( !IsAnyUnitSelected() )
      {
        building_ = GetSelectedBuildingWithBarrel();
        if ( building_ )
        {
          if ( !(tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)) )
          {
            goto LABEL_163;
          }
          side_id = tile_flags & 7;
          if ( _gDiplomacy[(unsigned __int8)gSideId][tile_flags & 7] || _KeyboardKeyState[VK_CONTROL] )
          {
            if ( GetBuildingOnTile_0(tile_x, pixel_y >> 5, &building, (eSideType *)&side_id, &object_index) )
            {
              if ( (!building->__IsSelected || side_id != gSideId) && IsBuildingInRange_0(building_, building) )
              {
                SetMouseCursor(CURSOR_ATTACK);
                goto LABEL_164;
              }
            }
            else if ( CanBuildingAttackTile(building_, tile_x, pixel_y >> 5, 0) )
            {
              SetMouseCursor(CURSOR_ATTACK);
              goto LABEL_164;
            }
            goto LABEL_163;
          }
          if ( tile_flags & TileFlags_400_HAS_WALL )
          {
            if ( GetBuildingOnTile_0(tile_x, pixel_y >> 5, &building, (eSideType *)&side_id, &object_index)
              && _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_TURRET )
            {
              SetMouseCursor(CURSOR_OVER_UNIT);
              goto LABEL_164;
            }
            goto LABEL_163;
          }
        }
        else if ( IsAnyBuildingSelected() )
        {
          tile_flags_byte1 = (tile_flags >> 8) & 0xFF;
          if ( !(tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)) )
          {
            goto LABEL_163;
          }
          if ( GetBuildingOnTile_0(tile_x, pixel_y >> 5, &building, (eSideType *)&side_id, &object_index) )
          {
            if ( building->__IsSelected && side_id == gSideId )
            {
              if ( MoreProductionBuildingsOfSameGroupExist(building->Type) && !(building->Flags & BFLAGS_80_PRIMARY) )
              {
                SetMouseCursor(CURSOR_DEPLOY);
                goto LABEL_164;
              }
              goto LABEL_163;
            }
            if ( tile_flags_byte1 & 4 )
            {
              if ( _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_TURRET )
              {
                SetMouseCursor(CURSOR_OVER_UNIT);
                goto LABEL_164;
              }
              goto LABEL_163;
            }
          }
        }
        else
        {
          if ( !(tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)) )
          {
            goto LABEL_163;
          }
          if ( tile_flags & TileFlags_400_HAS_WALL )
          {
            if ( GetBuildingOnTile_0(tile_x, pixel_y >> 5, &building, (eSideType *)&side_id, &object_index)
              && _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_TURRET )
            {
              SetMouseCursor(CURSOR_OVER_UNIT);
              goto LABEL_164;
            }
            goto LABEL_163;
          }
          side_id = tile_flags & 7;
          if ( (tile_flags & 7) != gSideId && tile_flags & 0x3E8 )
          {
            if ( GetUnitOnTile(32 * tile_x, 32 * tile_y, (eSideType *)&side_id, &object_index, 1) )
            {
              SetMouseCursor(CURSOR_OVER_UNIT);
              goto LABEL_164;
            }
            goto LABEL_163;
          }
        }
        goto LABEL_130;
      }
      side_id = tile_flags & 7;
      if ( (tile_flags & 7) != gSideId
        && tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT) )
      {
        target = GetUnitOnTile(pixel_x, pixel_y, (eSideType *)&side_id, &object_index, 1);
      }
      else
      {
        target = GetUnitOnTile(pixel_x, pixel_y, (eSideType *)&side_id, &object_index, 0);
      }
      if ( target
        && (!(target->Flags & UFLAGS_4_CLOAKED) || !_gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id]) )
      {
        if ( target->__IsSelected && side_id == gSideId && !_KeyboardKeyState[VK_CONTROL] )
        {
          if ( _KeyboardKeyState[VK_SHIFT] )
          {
            SetMouseCursor(CURSOR_OVER_UNIT);
            goto LABEL_164;
          }
          unit_behavior = _templates_unitattribs[target->Type].__Behavior;
          if ( unit_behavior == UnitBehavior_MCV )
          {
            if ( CheckIfMCVCanBeDeployedOn(tile_x, tile_y) )
            {
              SetMouseCursor(CURSOR_DEPLOY);
              goto LABEL_164;
            }
LABEL_41:
            SetMouseCursor(CURSOR_CANT_DEPLOY);
            goto LABEL_164;
          }
          switch ( _templates_unitattribs[target->Type].__Behavior )
          {
            case UnitBehavior_DEVASTATOR:
              SetMouseCursor(CURSOR_DEPLOY);
              goto LABEL_164;
            case UnitBehavior_THUMPER:
              if ( tile_flags & TileFlags_10000_SANDY )
              {
                SetMouseCursor(CURSOR_DEPLOY);
                goto LABEL_164;
              }
              goto LABEL_41;
            case UnitBehavior_SABOTEUR:
              if ( target->__SpecialPurpose >= 160u )
              {
                SetMouseCursor(CURSOR_DEPLOY);
                goto LABEL_164;
              }
              goto LABEL_41;
          }
LABEL_163:
          SetMouseCursor(CURSOR_POINTER);
          goto LABEL_164;
        }
        // New logic start
        // Allow infantry to move on tile where some infantry is already standing (with Alt key)
        if ( _KeyboardKeyState[VK_MENU] && IsAnyInfantrySelected() && _templates_unitattribs[target->Type].__IsInfantry && side_id == gSideId )
        {
          SetMouseCursor(CURSOR_MOVE);
          goto LABEL_164;
        }
        // New logic end
        if ( !_gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id] && !_KeyboardKeyState[VK_CONTROL] )
        {
          SetMouseCursor(CURSOR_OVER_UNIT);
          goto LABEL_164;
        }
        if ( _KeyboardKeyState[VK_MENU] && _templates_unitattribs[target->Type].__IsInfantry && side_id != gSideId )
        {
          if ( IsAnyCrusherSelected() && tile_flags & TileFlags_2000_DRIVE_ON )
          {
            SetMouseCursor(CURSOR_MOVE);
            goto LABEL_164;
          }
          if ( IsAnyArmedUnitSelected() )
          {
            SetMouseCursor(CURSOR_ATTACK);
            goto LABEL_164;
          }
        }
        else if ( IsAnyArmedUnitSelectedExceptThis(target) )
        {
          SetMouseCursor(CURSOR_ATTACK);
          goto LABEL_164;
        }
        SetMouseCursor(CURSOR_CANT_MOVE);
        goto LABEL_164;
      }
      if ( GetBuildingOnTile_0(tile_x, tile_y, &building, (eSideType *)&side_id, &object_index) )
      {
        if ( side_id != gSideId || _KeyboardKeyState[VK_CONTROL] )
        {
          if ( _gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id] || _KeyboardKeyState[VK_CONTROL] )
          {
            // New logic start
            // Fix cursor over enemy building when only armed engineer/saboteur is selected
            if ( IsAnyArmedUnitSelectedExceptEngineerAndSaboteur() )
            // New logic end
            {
              SetMouseCursor(CURSOR_ATTACK);
              goto LABEL_164;
            }
            if ( IsAnyUnitWithBehaviorSelected(UnitBehavior_SABOTEUR) )
            {
              if ( side_id != gSideId )
              {
                building_flags = building->Flags;
                // New logic start
                // Fix improper mouse cursor (engineer/saboteur/sell) when building has infiltrated flag
                if ( (building_flags & (BFLAGS_200_SELECTABLE_REPAIRABLE | BFLAGS_1000000_INFILTRATED)) == BFLAGS_200_SELECTABLE_REPAIRABLE )
                // New logic end
                {
                  SetMouseCursor(CURSOR_DEMOLISH);
                  goto LABEL_164;
                }
              }
            }
            else if ( IsAnyUnitWithBehaviorSelected(UnitBehavior_ENGINEER) )
            {
              if ( side_id != gSideId )
              {
                building_flags = building->Flags;
                // New logic start
                // Fix improper mouse cursor (engineer/saboteur/sell) when building has infiltrated flag
                if ( (building_flags & (BFLAGS_400_CAN_BE_CAPTURED | BFLAGS_1000000_INFILTRATED)) == BFLAGS_400_CAN_BE_CAPTURED )
                // New logic end
                {
                  SetMouseCursor(CURSOR_ENTER);
                  goto LABEL_164;
                }
              }
            }
            SetMouseCursor(CURSOR_CANT_MOVE);
            goto LABEL_164;
          }
          if ( tile_flags & TileFlags_400_HAS_WALL )
          {
            if ( _templates_buildattribs[building->Type].__Behavior == UnitBehavior_DEVIATOR )
            {
              SetMouseCursor(CURSOR_OVER_UNIT);
              goto LABEL_164;
            }
            SetMouseCursor(CURSOR_CANT_MOVE);
            goto LABEL_164;
          }
        }
        else
        {
          if ( _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_REPAIR && IsAnyVehicleSelected() )
          {
            SetMouseCursor(CURSOR_ENTER);
            goto LABEL_164;
          }
          if ( AllSelectedUnitsHaveBehavior(UnitBehavior_HARVESTER)
            && _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_REFINERY )
          {
            SetMouseCursor(CURSOR_ENTER);
            goto LABEL_164;
          }
          if ( tile_flags & TileFlags_400_HAS_WALL )
          {
            if ( _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_TURRET )
            {
              SetMouseCursor(CURSOR_OVER_UNIT);
              goto LABEL_164;
            }
            SetMouseCursor(CURSOR_CANT_MOVE);
            goto LABEL_164;
          }
        }
LABEL_130:
        SetMouseCursor(CURSOR_OVER_UNIT);
        goto LABEL_164;
      }
      if ( _KeyboardKeyState[VK_CONTROL] && IsAnyArmedUnitSelected() )
      {
        SetMouseCursor(CURSOR_ATTACK);
        goto LABEL_164;
      }
      if ( !(tile_flags & TileFlags_2000_DRIVE_ON) )
      {
        if ( tile_flags & TileFlags_4000_WALK_ON && IsAnyInfantrySelected() )
        {
          SetMouseCursor(CURSOR_MOVE_MOUNTAIN);
          goto LABEL_164;
        }
        goto LABEL_94;
      }
      if ( !(tile_flags & TileFlags_4000_WALK_ON) )
      {
LABEL_94:
        // New logic start
        // Unit movement restrictions
        SetMouseCursorForUnitMovementRestriction();
        // New logic end
        goto LABEL_164;
      }
      if ( tile_flags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE)
        && AllSelectedUnitsHaveBehavior(UnitBehavior_HARVESTER) )
      {
        SetMouseCursor(CURSOR_ATTACK);
      }
      else
      {
        // New logic start
        // Unit movement restrictions
        SetMouseCursorForUnitMovementRestriction();
        // New logic end
      }
    }
  }
LABEL_164:
  // New logic start
  // Allows scrolling the map with hotkeys, normally this feature is only available in debug window mode and the hotkeys are hardcoded by default
  if ( !((_KeyboardKeyState[ScrollDown] || _gMousePos.y >= _ScreenClipHeight - 1) && gGameTicks > MapScrollLockTicks) )
  {
    if ( (_KeyboardKeyState[ScrollUp] || _gMousePos.y < 1) && gGameTicks > MapScrollLockTicks )
    // New logic end
    {
      if ( _ViewportYPos - gScrollSize < 0 )
      {
        _ViewportYPos = 0;
        if ( !_TacticalData.__SidebarButtonMode && !_TacticalData.__DraggingBandbox )
        {
          SetMouseCursor(CURSOR_CANT_SCROLL_N);
        }
      }
      else
      {
        _ViewportYPos -= gScrollSize;
        if ( !_TacticalData.__SidebarButtonMode && !_TacticalData.__DraggingBandbox )
        {
          SetMouseCursor(CURSOR_SCROLL_N);
        }
      }
    }
  }
  else if ( gScrollSize + _ViewportYPos > 32 * gGameMap.height - _ViewportHeight )
  {
    _ViewportYPos = 32 * gGameMap.height - _ViewportHeight;
    if ( !_TacticalData.__SidebarButtonMode && !_TacticalData.__DraggingBandbox )
    {
      SetMouseCursor(CURSOR_CANT_SCROLL_S);
    }
  }
  else
  {
    _ViewportYPos += gScrollSize;
    if ( !_TacticalData.__SidebarButtonMode && !_TacticalData.__DraggingBandbox )
    {
      SetMouseCursor(CURSOR_SCROLL_S);
    }
  }
  // New logic start
  // Allows scrolling the map with hotkeys, normally this feature is only available in debug window mode and the hotkeys are hardcoded by default
  if ( (_KeyboardKeyState[ScrollRight] || _gMousePos.x >= _ScreenClipWidth - 1) && gGameTicks > MapScrollLockTicks )
  // New logic end
  {
    dragging_bandbox = _TacticalData.__DraggingBandbox;
  }
  else
  {
    dragging_bandbox = _TacticalData.__DraggingBandbox;
    if ( _gMousePos.x <= _ViewportWidth || !_TacticalData.__DraggingBandbox )
    {
      // New logic start
      // Allows scrolling the map with hotkeys, normally this feature is only available in debug window mode and the hotkeys are hardcoded by default
      if ( (_KeyboardKeyState[ScrollLeft] || _gMousePos.x < 1) && gGameTicks > MapScrollLockTicks )
      // New logic end
      {
        if ( _ViewportXPos - gScrollSize < 0 )
        {
          _ViewportXPos = 0;
          if ( !_TacticalData.__SidebarButtonMode && !_TacticalData.__DraggingBandbox )
          {
            if ( _MouseCursorID == CURSOR_CANT_SCROLL_S )
            {
              SetMouseCursor(CURSOR_CANT_SCROLL_SW);
            }
            else if ( _MouseCursorID == CURSOR_CANT_SCROLL_N )
            {
              SetMouseCursor(CURSOR_CANT_SCROLL_NW);
            }
            else if ( _MouseCursorID != CURSOR_SCROLL_N && _MouseCursorID != CURSOR_SCROLL_S )
            {
              SetMouseCursor(CURSOR_CANT_SCROLL_W);
            }
          }
        }
        else
        {
          _ViewportXPos -= gScrollSize;
          if ( !_TacticalData.__SidebarButtonMode && !_TacticalData.__DraggingBandbox )
          {
            if ( _MouseCursorID == CURSOR_SCROLL_S )
            {
              SetMouseCursor(CURSOR_SCROLL_SW);
            }
            else if ( _MouseCursorID == CURSOR_SCROLL_N )
            {
              SetMouseCursor(CURSOR_SCROLL_NW);
            }
            else
            {
              SetMouseCursor(CURSOR_SCROLL_W);
            }
          }
        }
      }
      goto LABEL_227;
    }
  }
  if ( gScrollSize + _ViewportXPos > 32 * gGameMap.width - _ViewportWidth )
  {
    _ViewportXPos = 32 * gGameMap.width - _ViewportWidth;
    if ( !_TacticalData.__SidebarButtonMode && !dragging_bandbox )
    {
      if ( _MouseCursorID == CURSOR_CANT_SCROLL_S )
      {
        SetMouseCursor(CURSOR_CANT_SCROLL_SE);
      }
      else if ( _MouseCursorID == CURSOR_CANT_SCROLL_N )
      {
        SetMouseCursor(CURSOR_CANT_SCROLL_NE);
      }
      else if ( _MouseCursorID != CURSOR_SCROLL_N && _MouseCursorID != CURSOR_SCROLL_S )
      {
        SetMouseCursor(CURSOR_CANT_SCROLL_E);
      }
    }
  }
  else
  {
    _ViewportXPos += gScrollSize;
    if ( !_TacticalData.__SidebarButtonMode && !dragging_bandbox )
    {
      if ( _MouseCursorID == CURSOR_SCROLL_S )
      {
        SetMouseCursor(CURSOR_SCROLL_SE);
      }
      else if ( _MouseCursorID == CURSOR_SCROLL_N )
      {
        SetMouseCursor(CURSOR_SCROLL_NE);
      }
      else
      {
        SetMouseCursor(CURSOR_SCROLL_E);
      }
    }
  }
LABEL_227:
  sidebar_icon_count = _SideBarIconCount;
  if ( _TacticalData.__SidebarMode != 1 || !_gFullscreen_DebugModes_pathfinddebug )
  {
    goto LABEL_240;
  }
  // New logic start
  // Mouse wheel scrolling
  if ( !_KeyboardKeyState[VK_DOWN] && !MouseWheelDown )
  // New logic end
  {
    if ( _gFullscreen_DebugModes_pathfinddebug )
    {
      // New logic start
      // Mouse wheel scrolling
      if ( _KeyboardKeyState[VK_UP] || MouseWheelUp )
      {
        MouseWheelUp = false;
        if (SlowSideBarScrolling)
        {
          _KeyboardKeyState[VK_UP] = false;
        }
        // New logic end
        if ( _TacticalData.__Strip1ScrollPos1 > 0 )
        {
          --_TacticalData.__Strip1ScrollPos1;
        }
        if ( _TacticalData.__Strip2ScrollPos1 > 0 )
        {
          --_TacticalData.__Strip2ScrollPos1;
        }
      }
    }
LABEL_240:
    side = my_side;
    goto LABEL_241;
  }
  side = my_side;
  // New logic start
  // Mouse wheel scrolling
  MouseWheelDown = false;
  if (SlowSideBarScrolling)
  {
    _KeyboardKeyState[VK_DOWN] = false;
  }
  // New logic end
  if ( (unsigned int)(_SideBarIconCount + _TacticalData.__Strip1ScrollPos1) < my_side->__BuildingIconCount )
  {
    ++_TacticalData.__Strip1ScrollPos1;
  }
  if ( (unsigned int)(_SideBarIconCount + _TacticalData.__Strip2ScrollPos1) < my_side->__UnitIconCount )
  {
    ++_TacticalData.__Strip2ScrollPos1;
  }
LABEL_241:
  if ( _MouseLeftState )
  {
    if ( !_TacticalData.__DraggingBandbox && !_TacticalData.__RadarScoreView )
    {
      radar_image_left = a1_1;
      if ( _gMousePos.x > a1_1 + _RadarLocationX
        && _gMousePos.x < _RadarHouseImages[0]->width + _RadarLocationX - a1_1 )
      {
        radar_image_top = v166;
        if ( _gMousePos.y > v166 + _RadarLocationY
          && _gMousePos.y < _RadarLocationY + _RadarHouseImages[0]->height - v166 )
        {
          // New logic start
          // Always show radar map if rule alwaysShowRadar is set to true
          if ( (IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_OUTPOST) && my_side->__PowerPercent1 >= 100u) || rulesExt__alwaysShowRadar  )
          // New logic end
          {
            if ( !IsAnyUnitSelected() && gGameTicks > MapScrollLockTicks )
            {
              pixel_x_ = 32 * (_gMousePos.x - radar_image_left - _RadarLocationX) - _ViewportWidth / 2;
              _ViewportXPos = pixel_x_;
              pixel_y_ = 32 * (_gMousePos.y - radar_image_top - _RadarLocationY) - _ViewportHeight / 2;
              _ViewportYPos = pixel_y_;
              if ( pixel_x_ <= 32 * gGameMap.width - _ViewportWidth )
              {
                if ( pixel_x_ < 0 )
                {
                  _ViewportXPos = 0;
                }
              }
              else
              {
                _ViewportXPos = 32 * gGameMap.width - _ViewportWidth;
              }
              if ( pixel_y_ <= 32 * gGameMap.height - _ViewportHeight )
              {
                if ( pixel_y_ < 0 )
                {
                  _ViewportYPos = 0;
                }
              }
              else
              {
                _ViewportYPos = 32 * gGameMap.height - _ViewportHeight;
              }
            }
          }
          sidebar_icon_count = _SideBarIconCount;
        }
      }
    }
  }
  if ( !_MouseRightDown )
  {
    goto LABEL_340;
  }
  _MouseRightDown = 0;
  _TacticalData.__SidebarButtonMode = 0;
  _TacticalData.__DraggingBandbox = 0;
  if ( _TacticalData.__SidebarMode == 2 )
  {
    if ( _gMousePos.x > _SidebarStrip1XPos
      && _gMousePos.x < _SidebarIconWidth + _SidebarStrip2XPos
      && _gMousePos.y > _SidebarStrip1YPos
      && _gMousePos.y < _SidebarStrip1YPos + _SideBarIconCount * _SideBarIconHeight2 )
    {
      if ( !side->__StarportDeliveryInProgress )
      {
        starport_icon_index = 2 * ((_gMousePos.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2);
        if ( _gMousePos.x > _SidebarIconWidth + _SidebarStrip1XPos )
        {
          ++starport_icon_index;
        }
        if ( starport_icon_index >= 8 )
        {
          // New logic start
          // High resolution patch
          return;
          // New logic end
        }
        starport_unit_type = side->__StarportIcons[starport_icon_index];
        if ( starport_unit_type >= 60 )
        {
          DebugFatal("HandleGameLoopEvents", "Invalid unitType");
        }
        GenerateStarportUnpickOrder(gSideId, starport_unit_type);
      }
      goto LABEL_340;
    }
LABEL_339:
    DeselectAllForAllSides();
    goto LABEL_340;
  }
  if ( _TacticalData.__SidebarMode != 3 )
  {
    if ( _gMousePos.x > _SidebarStrip1XPos
      && _gMousePos.x < _SidebarIconWidth + _SidebarStrip1XPos
      && _gMousePos.y > _SidebarStrip1YPos
      && _gMousePos.y < _SidebarStrip1YPos + sidebar_icon_count * _SideBarIconHeight2 )
    {
      sidebar_icon_mouse_pos = (_gMousePos.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2;
      if ( sidebar_icon_mouse_pos >= sidebar_icon_count )
      {
        DebugFatal("HandleGameLoopEvents", "Miscalculation in UI");
      }
      building_type = side->__BuildingIcons[_TacticalData.__Strip1ScrollPos1 + sidebar_icon_mouse_pos];
      // New logic start
      // Cannot place building fix
      if ( building_type >= 0 && building_type == side->__BuildingBuildQueue.__type )
      // New logic end
      {
        side->__BuildingBuildQueue.c_field_11_cancel = 1;
        GenerateBuildBuildingCancelOrder(gSideId, building_type);
      }
      goto LABEL_340;
    }
    if ( _gMousePos.x > _SidebarStrip2XPos
      && _gMousePos.x < _SidebarIconWidth + _SidebarStrip2XPos
      && _gMousePos.y > _SidebarStrip1YPos
      && _gMousePos.y < _SidebarStrip1YPos + sidebar_icon_count * _SideBarIconHeight2 )
    {
      sidebar_icon_mouse_pos = (_gMousePos.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2;
      if ( sidebar_icon_mouse_pos >= sidebar_icon_count )
      {
        DebugFatal("HandleGameLoopEvents", "Miscalculation in UI");
      }
      unit_icon_index = _TacticalData.__Strip2ScrollPos1 + sidebar_icon_mouse_pos;
      unit_type = side->__UnitIcons[unit_icon_index];
      if ( unit_type >= 0
        && _templates_unitattribs[side->__UnitIcons[unit_icon_index]].__Behavior != UnitBehavior_ORNITHOPTER
        && _templates_unitattribs[unit_type].__Behavior != UnitBehavior_DEATH_HAND
        && _templates_unitattribs[unit_type].__Behavior != UnitBehavior_SABOTEUR
        && _templates_unitattribs[unit_type].__Behavior != UnitBehavior_FREMEN )
      {
        GenerateBuildUnitCancelOrder(gSideId, side->__UnitIcons[unit_icon_index]);
      }
      goto LABEL_340;
    }
    if ( HandleSidebarButton(SIDEBARBUTTON_DOWNARROW1, 1) )
    {
      building_icon_count = side->__BuildingIconCount;
      if ( (unsigned int)_SideBarIconCount + _TacticalData.__Strip1ScrollPos1 < building_icon_count )
      {
        if ( building_icon_count - _TacticalData.__Strip1ScrollPos1 - _SideBarIconCount < (unsigned int)_SideBarIconCount )
        {
          _TacticalData.__Strip1ScrollPos1 = building_icon_count - 1;
        }
        else
        {
          _TacticalData.__Strip1ScrollPos1 += _SideBarIconCount;
        }
        sound_id = GetSoundTableID("S_ICONSCROLL");
        goto LABEL_332;
      }
    }
    else if ( HandleSidebarButton(0, 1) )
    {
      if ( _TacticalData.__Strip1ScrollPos1 > 0 )
      {
        if ( _TacticalData.__Strip1ScrollPos1 < _SideBarIconCount )
        {
          _TacticalData.__Strip1ScrollPos1 = 0;
        }
        else
        {
          _TacticalData.__Strip1ScrollPos1 -= _SideBarIconCount;
        }
        sound_id = GetSoundTableID("S_ICONSCROLL");
        goto LABEL_332;
      }
    }
    else if ( HandleSidebarButton(SIDEBARBUTTON_DOWNARROW2, 1) )
    {
      unit_icon_count = side->__UnitIconCount;
      if ( (unsigned int)_SideBarIconCount + _TacticalData.__Strip2ScrollPos1 < unit_icon_count )
      {
        if ( unit_icon_count - _TacticalData.__Strip2ScrollPos1 - _SideBarIconCount < (unsigned int)_SideBarIconCount )
        {
          _TacticalData.__Strip2ScrollPos1 = unit_icon_count - 1;
        }
        else
        {
          _TacticalData.__Strip2ScrollPos1 += _SideBarIconCount;
        }
        sound_id = GetSoundTableID("S_ICONSCROLL");
        goto LABEL_332;
      }
    }
    else
    {
      if ( !HandleSidebarButton(SIDEBARBUTTON_UPARROW2, 1) )
      {
        if ( (_gMousePos.x < _ViewportWidth && _gMousePos.y > _OptionsBarHeight)
          || (_gMousePos.x > _RadarLocationX
          && _gMousePos.x < _RadarHouseImages[0]->width + _RadarLocationX
          && _gMousePos.y > _RadarLocationY
          && _gMousePos.y < _RadarLocationY + _RadarHouseImages[0]->height) )
        {
          goto LABEL_339;
        }
        goto LABEL_340;
      }
      if ( _TacticalData.__Strip2ScrollPos1 > 0 )
      {
        if ( _TacticalData.__Strip2ScrollPos1 < _SideBarIconCount )
        {
          _TacticalData.__Strip2ScrollPos1 = 0;
        }
        else
        {
          _TacticalData.__Strip2ScrollPos1 -= _SideBarIconCount;
        }
        sound_id = GetSoundTableID("S_ICONSCROLL");
        goto LABEL_332;
      }
    }
    sound_id = GetSoundTableID("S_ENDSCROLL");
LABEL_332:
    QueueAudioToPlay(sound_id, 0, 0, 0);
    goto LABEL_340;
  }
  if ( _gMousePos.x <= _SidebarStrip1XPos
    || _gMousePos.x >= _SidebarIconWidth + _SidebarStrip2XPos
    || _gMousePos.y <= _SidebarStrip1YPos
    || _gMousePos.y >= _SidebarStrip1YPos + _SideBarIconCount * _SideBarIconHeight2 )
  {
    goto LABEL_339;
  }
  upgrade_icon_index = 2 * ((_gMousePos.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2);
  if ( _gMousePos.x > _SidebarIconWidth + _SidebarStrip1XPos )
  {
    ++upgrade_icon_index;
  }
  if ( upgrade_icon_index >= 8 )
  {
    // New logic start
    // High resolution patch
    return;
    // New logic end
  }
  upgrade_building_type = side->__UpgradeIcons[upgrade_icon_index];
  if ( upgrade_building_type >= 100 )
  {
    DebugFatal("HandleGameLoopEvents", "Invalid unitType");
  }
  GenerateUpgradeCancelOrder(gSideId, upgrade_building_type);
LABEL_340:
  if ( _MouseRightUp )
  {
    v42 = &_SidebarButtons[0].__status;
    do
    {
      if ( *v42 == 2 )
      {
        *v42 = 1;
      }
      v42 += sizeof(SidebarButtonData);
    }
    while ( (signed int)v42 <= (signed int)&_SidebarButtons[3].__status );
    _MouseRightUp = 0;
  }
  if ( !_MouseLeftDown )
  {
    goto LABEL_406;
  }
  _MouseLeftDown = 0;
  // New logic start
  // High resolution patch - Do not draw selection rectangle if cursor out of battlefield
  if ( _mouse_dword_798498.x >= _ViewportWidth || _mouse_dword_798498.y <= _OptionsBarHeight || _mouse_dword_798498.y >= _OptionsBarHeight + _ViewportHeight )
  // New logic start
  {
    _TacticalData.__DraggingBandbox = 0;
  }
  else
  {
    _TacticalData.__DraggingBandbox = 1;
    _TacticalData.__BandboxStartX = _mouse_dword_798498.x + _ViewportXPos;
    _TacticalData.__BandboxStartY = _mouse_dword_798498.y + _ViewportYPos - _OptionsBarHeight;
  }
  if ( HandleSidebarButton(SIDEBARBUTTON_REPAIR, 1) )
  {
    _TacticalData.__SidebarButtonMode = _TacticalData.__SidebarButtonMode != 2 ? 2 : 0;
LABEL_355:
    sound_id = GetSoundTableID("BUTTON");
    QueueAudioToPlay(sound_id, 0, 0, 0);
    goto LABEL_370;
  }
  if ( HandleSidebarButton(SIDEBARBUTTON_SELL, 1) )
  {
    _TacticalData.__SidebarButtonMode = _TacticalData.__SidebarButtonMode != 3 ? 3 : 0;
    goto LABEL_355;
  }
  if ( (gGameType || !_gFullscreen_DebugModes_pathfinddebug) && HandleSidebarButton(SIDEBARBUTTON_MAPTOGGLE, 1) )
  {
    _TacticalData.__RadarScoreView = _TacticalData.__RadarScoreView == 0;
    goto LABEL_370;
  }
  if ( HandleSidebarButton(SIDEBARBUTTON_GUARD, 1) )
  {
    if ( !IsAnyArmedUnitSelected() )
    {
      goto LABEL_370;
    }
    GenerateUnitGuardOrder(gSideId);
    harkonnen_sound_id = GetSoundTableID("H_GUARDING");
    ordos_sound_id = GetSoundTableID("O_GUARDING");
    atreides_sound_id = GetSoundTableID("S_GUARDING");
    goto LABEL_367;
  }
  if ( HandleSidebarButton(SIDEBARBUTTON_RETREAT, 1) )
  {
    if ( IsAnyUnitSelected() && CSide__FindBestBasePosition(side, x, y) )
    {
      GenerateUnitRetreatOrder(gSideId, x[0], y[0]);
      harkonnen_sound_id = GetSoundTableID("H_RETREAT");
      ordos_sound_id = GetSoundTableID("O_RETREAT");
      atreides_sound_id = GetSoundTableID("S_RETREAT");
LABEL_367:
      PlayMentatSound(atreides_sound_id, ordos_sound_id, harkonnen_sound_id, 0, 0, 0);
      goto LABEL_370;
    }
  }
  else if ( HandleSidebarButton(SIDEBARBUTTON_OPTIONS, 1) )
  {
    _Mouse_bool_797590 = 0;
  }
LABEL_370:
  switch ( _TacticalData.__SidebarMode )
  {
    case 1:
      if ( HandleSidebarButton(SIDEBARBUTTON_DOWNARROW1, 1) )
      {
        if ( (unsigned int)(_TacticalData.__Strip1ScrollPos1 + _SideBarIconCount) < side->__BuildingIconCount )
        {
          ++_TacticalData.__Strip1ScrollPos1;
          sound_id = GetSoundTableID("S_ICONSCROLL");
LABEL_384:
          QueueAudioToPlay(sound_id, 0, 0, 0);
          break;
        }
        goto LABEL_383;
      }
      if ( HandleSidebarButton(0, 1) )
      {
        if ( _TacticalData.__Strip1ScrollPos1 > 0 )
        {
          --_TacticalData.__Strip1ScrollPos1;
          sound_id = GetSoundTableID("S_ICONSCROLL");
          goto LABEL_384;
        }
LABEL_383:
        sound_id = GetSoundTableID("S_ENDSCROLL");
        goto LABEL_384;
      }
      if ( HandleSidebarButton(SIDEBARBUTTON_DOWNARROW2, 1) )
      {
        if ( (unsigned int)(_TacticalData.__Strip2ScrollPos1 + _SideBarIconCount) < side->__UnitIconCount )
        {
          ++_TacticalData.__Strip2ScrollPos1;
          sound_id = GetSoundTableID("S_ICONSCROLL");
          goto LABEL_384;
        }
        goto LABEL_383;
      }
      if ( HandleSidebarButton(SIDEBARBUTTON_UPARROW2, 1) )
      {
        if ( _TacticalData.__Strip2ScrollPos1 > 0 )
        {
          --_TacticalData.__Strip2ScrollPos1;
          sound_id = GetSoundTableID("S_ICONSCROLL");
          goto LABEL_384;
        }
        goto LABEL_383;
      }
      if ( HandleSidebarButton(SIDEBARBUTTON_UPGRADE, 1) )
      {
        _TacticalData.__SidebarMode = 3;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW1].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_UPARROW1].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW2].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_UPARROW2].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_UPGRADE].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_MAIN1].__status = 1;
      }
      else if ( HandleSidebarButton(SIDEBARBUTTON_STARPORT, 1) )
      {
        _TacticalData.__SidebarMode = 2;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW1].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_UPARROW1].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW2].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_UPARROW2].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_STARPORT].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_MAIN2].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_PURCHASE].__status = 1;
      }
      break;
    case 2:
      if ( HandleSidebarButton(SIDEBARBUTTON_PURCHASE, 1) )
      {
        _TacticalData.__SidebarMode = 1;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW1].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_UPARROW1].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW2].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_UPARROW2].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_MAIN2].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_PURCHASE].__status = 0;
        if ( !IsAnyStarportUnitPicked(gSideId) )
        {
          break;
        }
        GenerateStarportPurchaseOrder(gSideId);
        harkonnen_sound_id = GetSoundTableID("H_PORTORDER");
        ordos_sound_id = GetSoundTableID("O_PORTORDER");
        atreides_sound_id = GetSoundTableID("S_PORTORDER");
      }
      else
      {
        if ( HandleSidebarButton(SIDEBARBUTTON_MAIN2, 1) )
        {
          _TacticalData.__SidebarMode = 1;
          _SidebarButtons[SIDEBARBUTTON_DOWNARROW1].__status = 1;
          _SidebarButtons[SIDEBARBUTTON_UPARROW1].__status = 1;
          _SidebarButtons[SIDEBARBUTTON_DOWNARROW2].__status = 1;
          _SidebarButtons[SIDEBARBUTTON_UPARROW2].__status = 1;
          _SidebarButtons[SIDEBARBUTTON_STARPORT].__status = 1;
          _SidebarButtons[SIDEBARBUTTON_MAIN2].__status = 0;
          _SidebarButtons[SIDEBARBUTTON_PURCHASE].__status = 0;
          if ( !IsAnyStarportUnitPicked(gSideId) )
          {
            break;
          }
          GenerateStarportCancelOrder(gSideId);
        }
        else
        {
          if ( !HandleSidebarButton(SIDEBARBUTTON_UPGRADE, 1) )
          {
            break;
          }
          _TacticalData.__SidebarMode = 3;
          _SidebarButtons[SIDEBARBUTTON_UPGRADE].__status = 0;
          _SidebarButtons[SIDEBARBUTTON_MAIN1].__status = 1;
          _SidebarButtons[SIDEBARBUTTON_MAIN2].__status = 0;
          _SidebarButtons[SIDEBARBUTTON_PURCHASE].__status = 0;
          _SidebarButtons[SIDEBARBUTTON_STARPORT].__status = 1;
          if ( !IsAnyStarportUnitPicked(gSideId) )
          {
            break;
          }
          GenerateStarportCancelOrder(gSideId);
        }
        harkonnen_sound_id = GetSoundTableID("H_CANCELLED");
        ordos_sound_id = GetSoundTableID("O_CANCELLED");
        atreides_sound_id = GetSoundTableID("S_CANCELLED");
      }
      PlayMentatSound(atreides_sound_id, ordos_sound_id, harkonnen_sound_id, 0, 0, 0);
      break;
    case 3:
      if ( HandleSidebarButton(SIDEBARBUTTON_MAIN1, 1) )
      {
        _TacticalData.__SidebarMode = 1;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW1].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_UPARROW1].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_DOWNARROW2].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_UPARROW2].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_UPGRADE].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_MAIN1].__status = 0;
      }
      else if ( HandleSidebarButton(SIDEBARBUTTON_STARPORT, 1) )
      {
        _TacticalData.__SidebarMode = 2;
        _SidebarButtons[SIDEBARBUTTON_STARPORT].__status = 0;
        _SidebarButtons[SIDEBARBUTTON_MAIN2].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_PURCHASE].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_UPGRADE].__status = 1;
        _SidebarButtons[SIDEBARBUTTON_MAIN1].__status = 0;
      }
      break;
  }
LABEL_406:
  if ( !_MouseLeftUp )
  {
    goto LABEL_650;
  }
  if ( _SidebarButtons[SIDEBARBUTTON_OPTIONS].__status == 2 )
  {
    if ( _bool_missionfailed_517400 )
    {
      if ( _IsMultiplayer )
      {
        if ( !_GameOver )
        {
          CUIManager__ReplaceWithOne_470E60(gUIMgr, "ABORT_VM", gBackBuf);
        }
      }
    }
    else
    {
      CUIManager__invoke_473840(gUIMgr, 229, &_Nullstring, 0);
      if ( _IsMultiplayer )
      {
        CUIManager__ReplaceWithOne_470E60(gUIMgr, "INGAME_MP", gBackBuf);
      }
      else if ( gGameType == 1 )
      {
        CUIManager__ReplaceWithOne_470E60(gUIMgr, "INGAME_SKIRMISH", gBackBuf);
      }
      else
      {
        CUIManager__ReplaceWithOne_470E60(gUIMgr, "INGAME", gBackBuf);
      }
    }
    _Mouse_bool_797590 = 0;
    _SidebarButtons[SIDEBARBUTTON_OPTIONS].__status = 1;
  }
  v47 = &_SidebarButtons[0].__status;
  do
  {
    if ( *v47 == 2 )
    {
      *v47 = 1;
    }
    v47 += 24;
  }
  while ( (signed int)v47 < (signed int)&_TacticalData.__Strip2ScrollPos1 );
  if ( _TacticalData.__DraggingBandbox
    && abs(_TacticalData.__BandboxStartX - _ViewportXPos - _MouseClickCoords.x) > 3
    && abs(_OptionsBarHeight + _TacticalData.__BandboxStartY - _ViewportYPos - _MouseClickCoords.y) > 3 )
  {
    if ( SelectAllUnitsInArea(
           _TacticalData.__BandboxStartX,
           _TacticalData.__BandboxStartY,
           _MouseClickCoords.x + _ViewportXPos,
           _MouseClickCoords.y + _ViewportYPos - _OptionsBarHeight) )
    {
      PlayUnitResponse(0);
      _TacticalData.__SidebarButtonMode = 0;
      goto LABEL_649;
    }
LABEL_544:
    _TacticalData.__SidebarButtonMode = 0;
    goto LABEL_649;
  }
  if ( _MouseClickCoords.x > _SidebarStrip1XPos
    && _MouseClickCoords.x < _SidebarIconWidth + _SidebarStrip2XPos
    && _MouseClickCoords.y > _SidebarStrip2YPos
    && _MouseClickCoords.y < _SidebarStrip2YPos + _SideBarIconCount * _SideBarIconHeight2 )
  {
    if ( _TacticalData.__SidebarMode == 3 )
    {
      upgrade_icon_index = 2 * ((_MouseClickCoords.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2);
      if ( _MouseClickCoords.x > _SidebarIconWidth + _SidebarStrip1XPos )
      {
        ++upgrade_icon_index;
      }
      // New logic start
      // High resolution patch
      if ( upgrade_icon_index >= 8 )
      {
        return;
      }
      // New logic end
      building_type = side->__UpgradeIcons[upgrade_icon_index];
      if ( building_type >= 0 )
      {
        building_group = _templates_buildattribs[building_type].GroupType;
        if ( CanSideUpgradeBuildingGroup(gSideId, (eBuildingGroupType)building_group) )
        {
          GenerateUpgradePickOrder(gSideId, (eBuildingGroupType)building_group);
        }
      }
    }
    else if ( _TacticalData.__SidebarMode == 2 )
    {
      if ( !side->__StarportDeliveryInProgress )
      {
        starport_icon_index = 2 * ((_MouseClickCoords.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2);
        if ( _MouseClickCoords.x > _SidebarIconWidth + _SidebarStrip1XPos )
        {
          ++starport_icon_index;
        }
        // New logic start
        // High resolution patch
        if ( upgrade_icon_index >= 8 )
        {
          return;
        }
        // New logic end
        GenerateStarportPickOrder(gSideId, side->__StarportIcons[starport_icon_index]);
      }
    }
    else if ( _MouseClickCoords.x >= _SidebarIconWidth + _SidebarStrip1XPos )
    {
      sidebar_icon_mouse_pos = (_MouseClickCoords.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2;
      if ( sidebar_icon_mouse_pos >= _SideBarIconCount )
      {
        DebugFatal("HandleGameLoopEvents", "Miscalculation in UI");
      }
      unit_type = side->__UnitIcons[_TacticalData.__Strip2ScrollPos1 + sidebar_icon_mouse_pos];
      if ( unit_type >= 0 )
      {
        if ( _templates_unitattribs[side->__UnitIcons[_TacticalData.__Strip2ScrollPos1 + sidebar_icon_mouse_pos]].__Behavior == UnitBehavior_ORNITHOPTER
          || _templates_unitattribs[unit_type].__Behavior == UnitBehavior_DEATH_HAND
          || _templates_unitattribs[unit_type].__Behavior == UnitBehavior_SABOTEUR
          || _templates_unitattribs[unit_type].__Behavior == UnitBehavior_FREMEN )
        {
          v56 = (int)&side->__UnitBuildQueue[0].c_field_11_cancel;
          v57 = 0;
          counter = 10;
          do
          {
            if ( *(signed __int16 *)(v56 - 0x11) == unit_type && *(_WORD *)(v56 - 0xF) == 23040 && !*(_BYTE *)v56 )
            {
              v57 = 1;
              if ( _templates_unitattribs[unit_type].__Behavior == UnitBehavior_SABOTEUR
                || _templates_unitattribs[unit_type].__Behavior == UnitBehavior_FREMEN )
              {
                *(_BYTE *)v56 = 1;
              }
            }
            v56 += 0x14;
            --counter;
          }
          while ( counter );
          if ( v57 )
          {
            unit_behavior = _templates_unitattribs[unit_type].__Behavior;
            if ( _templates_unitattribs[unit_type].__Behavior == UnitBehavior_ORNITHOPTER )
            {
              _TacticalData.__SidebarButtonMode = 4;
            }
            else if ( unit_behavior == UnitBehavior_DEATH_HAND )
            {
              _TacticalData.__SidebarButtonMode = 5;
            }
            else if ( unit_behavior == UnitBehavior_SABOTEUR )
            {
              GenerateSpecialWeaponOrder(gSideId, UnitBehavior_SABOTEUR, 0, 0);
            }
            else
            {
              GenerateSpecialWeaponOrder(gSideId, UnitBehavior_FREMEN, 0, 0);
            }
          }
        }
        else
        {
          GenerateBuildUnitPickOrder(gSideId, unit_type);
        }
      }
    }
    else if ( side->__BuildingBuildQueue.__type < 0 || (unsigned int)side->__BuildingBuildQueue.__build_progress < 0x5A00u )
    {
      sidebar_icon_mouse_pos = (_MouseClickCoords.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2;
      if ( sidebar_icon_mouse_pos >= _SideBarIconCount )
      {
        DebugFatal("HandleGameLoopEvents", "Miscalculation in UI");
      }
      building_icon_index = _TacticalData.__Strip1ScrollPos1 + sidebar_icon_mouse_pos;
      if ( side->__BuildingIcons[building_icon_index] >= 0 )
      {
        GenerateBuildBuildingPickOrder(gSideId, side->__BuildingIcons[building_icon_index]);
      }
    }
    else if ( !side->__BuildingBuildQueue.c_field_11_cancel )
    {
      _TacticalData.__SidebarButtonMode = 1;
    }
    goto LABEL_649;
  }
  if ( _TacticalData.__RadarScoreView )
  {
    mouse_y = _gMousePos.y;
LABEL_485:
    mouse_x = _gMousePos.x;
    goto LABEL_486;
  }
  v60 = a1_1;
  mouse_x = _gMousePos.x;
  if ( _gMousePos.x > a1_1 + _RadarLocationX
    && _gMousePos.x < _RadarHouseImages[0]->width + _RadarLocationX - a1_1 )
  {
    v62 = v166;
    mouse_y = _gMousePos.y;
    if ( _gMousePos.y > v166 + _RadarLocationY && _gMousePos.y < _RadarLocationY + _RadarHouseImages[0]->height - v166 )
    {
      // New logic start
      // Always show radar map if rule alwaysShowRadar is set to true
      if ( !((IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_OUTPOST) && my_side->__PowerPercent1 >= 100u) || rulesExt__alwaysShowRadar) )
      // New logic end
      {
        goto LABEL_649;
      }
      GenerateUnitMoveOrder(
        gSideId,
        LOBYTE(_gMousePos.x) - v60 - _RadarLocationX,
        LOBYTE(_gMousePos.y) - v62 - _RadarLocationY);
      goto LABEL_648;
    }
    goto LABEL_485;
  }
  mouse_y = _gMousePos.y;
LABEL_486:
  switch ( _TacticalData.__SidebarButtonMode )
  {
    case 1:
      // New logic start
      // High resolution patch - Ignore order if cursor out of battlefield
      if ( mouse_x >= _ViewportWidth || mouse_y <= _OptionsBarHeight || mouse_y >= _OptionsBarHeight + _ViewportHeight )
      // New logic end
      {
        goto LABEL_649;
      }
      if ( _TacticalData.__CanPlace )
      {
        pixel_y = _ViewportYPos - _OptionsBarHeight + _MouseClickCoords.y;
        // Code removed
        // Make top row of map buildable
        place_y = pixel_y >> 5;
        GenerateBuildPlaceBuildingOrder(
          gSideId,
          side->__BuildingBuildQueue.__type,
          (_ViewportXPos + _MouseClickCoords.x) / 32,
          place_y);
        side->__BuildingBuildQueue.c_field_11_cancel = 1;
        _TacticalData.__SidebarButtonMode = 0;
        goto LABEL_649;
      }
      v155 = 1;
      harkonnen_sound_id = GetSoundTableID("H_CANTBUILD");
      ordos_sound_id = GetSoundTableID("O_CANTBUILD");
      atreides_sound_id = GetSoundTableID("S_CANTBUILD");
      goto LABEL_618;
    case 2:
      if ( mouse_x < _ViewportWidth && mouse_y > _OptionsBarHeight )
      {
        if ( GetBuildingOnTile_0(
               (_ViewportXPos + mouse_x) / 32,
               (unsigned int)(mouse_y + _ViewportYPos - _OptionsBarHeight) >> 5,
               &building,
               (eSideType *)&side_id,
               &object_index) )
        {
          if ( side_id == gSideId )
          {
            building_type = 268 * building->Type;
            building_flags = *(int *)((char *)&_templates_buildattribs[0]._____Flags + building_type);
            if ( building_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
            {
              if ( building->Health < *(int *)((char *)&_templates_buildattribs[0]._____HitPoints + building_type) )
              {
                GenerateBuildingRepairOrder(side_id, object_index);
              }
            }
          }
        }
        else
        {
          unit = GetUnitOnTile(
                   _gMousePos.x + _ViewportXPos,
                   _gMousePos.y + _ViewportYPos - _OptionsBarHeight,
                   (eSideType *)&side_id,
                   &object_index,
                   0);
          if ( unit )
          {
            if ( IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_REPAIR) )
            {
              if ( side_id == gSideId )
              {
                unit_type_ = unit->Type << 8;
                if ( unit->Health < *(int *)((char *)&_templates_unitattribs[0].__Strength + unit_type_)
                  && !_templates_unitattribs[unit->Type].__IsInfantry )
                {
                  // New logic start
                  // Fix wrong sound played when clicking on unit with repair cursor and playing as Emperor or higher
                  QueueAudioToPlay(
                    _templates_unitattribs[unit->Type].__ConfirmationSounds[HLIMIT(_IRValues[gSideId], 2)][gGameTicks % 3u],
                    0,
                    0,
                    0);
                  // New logic start
                  SetUnitToFlicker(unit);
                  GenerateRepairSingleUnitOrder(side_id, object_index);
                }
              }
            }
          }
        }
      }
      goto LABEL_649;
    case 3:
      if ( mouse_x >= _ViewportWidth
        || mouse_y <= _OptionsBarHeight
        || !GetBuildingOnTile_0(
              (_ViewportXPos + _MouseClickCoords.x) / 32,
              (unsigned int)(_MouseClickCoords.y + _ViewportYPos - _OptionsBarHeight) >> 5,
              &building,
              (eSideType *)&side_id,
              &object_index) )
      {
        goto LABEL_649;
      }
      if ( side_id == gSideId )
      {
        side = GetSide((eSideType)side_id);
        if ( !(side->__ObjectArray[(unsigned __int16)object_index].__GroupID & 6) )
        {
          GenerateBuildingSellOrder(side_id, object_index);
          goto LABEL_649;
        }
      }
      if ( _gFullscreen_DebugModes_pathfinddebug
        || _IsMultiplayer
        || !CaptureBuilding((eSideType)side_id, gSideId, object_index) )
      {
        goto LABEL_649;
      }
      v155 = 1;
      harkonnen_sound_id = GetSoundTableID("H_CAPTURE");
      ordos_sound_id = GetSoundTableID("O_CAPTURE");
      atreides_sound_id = GetSoundTableID("S_CAPTURE");
LABEL_618:
      PlayMentatSound(atreides_sound_id, ordos_sound_id, harkonnen_sound_id, v155, 0, 0);
      goto LABEL_649;
    case 4:
      // New logic start
      // High resolution patch - Ignore order if cursor out of battlefield
      if ( mouse_x >= _ViewportWidth || mouse_y <= _OptionsBarHeight || mouse_y >= _OptionsBarHeight + _ViewportHeight )
      // New logic end
      {
        goto LABEL_649;
      }
      is_ornithopter = 0;
      counter = 10;
      pixel_x = _ViewportXPos + _gMousePos.x;
      pixel_y = _gMousePos.y + _ViewportYPos - _OptionsBarHeight;
      build_queue = my_side->__UnitBuildQueue;
      do
      {
        if ( build_queue->__type >= 0
          && _templates_unitattribs[build_queue->__type].__Behavior == UnitBehavior_ORNITHOPTER
          && build_queue->__build_progress == 23040
          && !build_queue->c_field_11_cancel )
        {
          is_ornithopter = 1;
          build_queue->c_field_11_cancel = 1;
        }
        ++build_queue;
        --counter;
      }
      while ( counter );
      if ( is_ornithopter )
      {
        GenerateSpecialWeaponOrder(gSideId, UnitBehavior_ORNITHOPTER, pixel_x >> 5, pixel_y >> 5);
      }
      goto LABEL_544;
    case 5:
      // New logic start
      // High resolution patch - Ignore order if cursor out of battlefield
      if ( mouse_x >= _ViewportWidth || mouse_y <= _OptionsBarHeight || mouse_y >= _OptionsBarHeight + _ViewportHeight )
      // New logic end
      {
        goto LABEL_649;
      }
      is_deathhand = 0;
      counter = 10;
      pixel_x = _ViewportXPos + _gMousePos.x;
      pixel_y = _gMousePos.y + _ViewportYPos - _OptionsBarHeight;
      build_queue = my_side->__UnitBuildQueue;
      do
      {
        if ( build_queue->__type >= 0
          && _templates_unitattribs[build_queue->__type].__Behavior == UnitBehavior_DEATH_HAND
          && build_queue->__build_progress == 23040
          && !build_queue->c_field_11_cancel )
        {
          is_deathhand = 1;
          build_queue->c_field_11_cancel = 1;
        }
        ++build_queue;
        --counter;
      }
      while ( counter );
      if ( is_deathhand )
      {
        GenerateSpecialWeaponOrder(gSideId, UnitBehavior_DEATH_HAND, pixel_x >> 5, pixel_y >> 5);
      }
      goto LABEL_544;
  }
  // New logic start
  // High resolution patch - Ignore order if cursor out of battlefield
  if ( mouse_x >= _ViewportWidth || mouse_y <= _OptionsBarHeight || mouse_y >= _OptionsBarHeight + _ViewportHeight )
  // New logic end
  {
    goto LABEL_649;
  }
  pixel_x = _ViewportXPos + _gMousePos.x;
  pixel_y = _gMousePos.y + _ViewportYPos - _OptionsBarHeight;
  tile_x = (unsigned int)(_ViewportXPos + _gMousePos.x) >> 5;
  v87 = _CellNumbersWidthSpan[(unsigned int)(_gMousePos.y + _ViewportYPos - _OptionsBarHeight) >> 5];
  v166 = (unsigned int)(_gMousePos.y + _ViewportYPos - _OptionsBarHeight) >> 5;
  y[0] = v166;
  x[0] = tile_x;
  cell_index = tile_x + v87;
  tile_flags = gGameMap.map[cell_index].__tile_bitflags;
  hidden_under_shroud = 0;
  if ( _bool_shroud_4DFB04 )
  {
    hidden_under_shroud = (gGameMap.map[cell_index].__shroud & 0xF) == 1;
  }
  if ( hidden_under_shroud )
  {
    if ( !IsAnyUnitSelected() )
    {
      goto LABEL_649;
    }
    goto LABEL_646;
  }
  side_id = tile_flags & 7;
  if ( (tile_flags & 7) != gSideId
    && tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT) )
  {
    unit = GetUnitOnTile(
             _ViewportXPos + _gMousePos.x,
             _gMousePos.y + _ViewportYPos - _OptionsBarHeight,
             (eSideType *)&side_id,
             &object_index,
             1);
  }
  else
  {
    unit = GetUnitOnTile(
             _ViewportXPos + _gMousePos.x,
             _gMousePos.y + _ViewportYPos - _OptionsBarHeight,
             (eSideType *)&side_id,
             &object_index,
             0);
  }
  a1_2 = unit;
  if ( unit )
  {
    if ( !(unit->Flags & UFLAGS_4_CLOAKED) )
    {
      my_side_id = gSideId;
LABEL_559:
      if ( (unsigned int)side_id == my_side_id && !_KeyboardKeyState[VK_CONTROL] )
      {
        // New logic start
        // Allow infantry to move on tile where some infantry is already standing (with Alt key)
        if ( _KeyboardKeyState[VK_MENU] )
        {
          if ( IsAnyInfantrySelected() && _templates_unitattribs[unit->Type].__IsInfantry )
          {
            goto LABEL_646;
          }
        }
        // New logic end
        if ( _KeyboardKeyState[VK_SHIFT] )
        {
          SelectUnit(side_id, object_index, 1);
LABEL_582:
          PlayUnitResponse(0);
          goto LABEL_649;
        }
        unit_is_selected = unit->__IsSelected;
        if ( unit_is_selected )
        {
          if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_MCV )
          {
            if ( CheckIfMCVCanBeDeployedOn(x[0], y[0]) )
            {
              GenerateUnitDeployOrder(side_id, object_index);
              goto LABEL_649;
            }
            v155 = 1;
            harkonnen_sound_id = GetSoundTableID("H_CANTDEPLOY");
            ordos_sound_id = GetSoundTableID("O_CANTDEPLOY");
            atreides_sound_id = GetSoundTableID("S_CANTDEPLOY");
            goto LABEL_618;
          }
          if ( unit_is_selected )
          {
            if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_DEVASTATOR )
            {
              if ( unit->State != UNIT_STATE_31_SELFDESTRUCT )
              {
                GenerateUnitDeployOrder(side_id, object_index);
                PlayUnitResponse(0);
              }
              goto LABEL_649;
            }
            if ( unit_is_selected )
            {
              if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_THUMPER )
              {
                if ( gGameMap.map[x[0] + _CellNumbersWidthSpan[y[0]]].__tile_bitflags & TileFlags_10000_SANDY )
                {
                  GenerateUnitDeployOrder(side_id, object_index);
                  PlayUnitResponse(0);
                  goto LABEL_649;
                }
LABEL_580:
                SelectUnit(side_id, object_index, 0);
                PlayUnitResponse(0);
                goto LABEL_649;
              }
              if ( unit_is_selected && _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_SABOTEUR )
              {
                if ( unit->__SpecialPurpose >= 160u )
                {
                  GenerateUnitDeployOrder(side_id, object_index);
                  PlayUnitResponse(0);
                  goto LABEL_649;
                }
                goto LABEL_580;
              }
            }
          }
        }
        SelectUnit(side_id, object_index, 0);
        goto LABEL_582;
      }
      if ( !_gDiplomacy[(unsigned __int8)my_side_id][(unsigned __int8)side_id] && !_KeyboardKeyState[VK_CONTROL] )
      {
        SelectUnit(side_id, object_index, 0);
        goto LABEL_649;
      }
      if ( !IsAnyUnitSelected() )
      {
        building_ = GetSelectedBuildingWithBarrel();
        if ( building_ )
        {
          if ( CanBuildingAttackTile(building_, tile_x, v166, 0) )
          {
            SetUnitToFlicker(a1_2);
            GenerateBuildingAttackUnitOrder(gSideId, (eSideType)side_id, object_index);
          }
          else
          {
            SelectUnit(side_id, object_index, 0);
          }
        }
        else
        {
          SelectUnit(side_id, object_index, 0);
        }
        goto LABEL_649;
      }
      unit = a1_2;
      SetUnitToFlicker(a1_2);
      if ( _KeyboardKeyState[VK_MENU] && _templates_unitattribs[unit->Type].__IsInfantry && side_id != gSideId )
      {
        if ( IsAnyCrusherSelected() && tile_flags & TileFlags_2000_DRIVE_ON )
        {
          GenerateUnitMoveOrder(gSideId, x[0], y[0]);
        }
        else
        {
          GenerateUnitAttackUnitOrder(gSideId, (eSideType)side_id, object_index);
        }
      }
      else
      {
        GenerateUnitAttackUnitOrder(gSideId, (eSideType)side_id, object_index);
      }
LABEL_648:
      PlayUnitResponse(1);
      goto LABEL_649;
    }
    my_side_id = gSideId;
    if ( !_gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id] )
    {
      unit = a1_2;
      goto LABEL_559;
    }
  }
  if ( !GetBuildingOnTile_0(x[0], y[0], &building, (eSideType *)&side_id, &object_index) )
  {
    if ( _KeyboardKeyState[VK_CONTROL] )
    {
      if ( !IsAnyArmedUnitSelected() )
      {
        goto LABEL_649;
      }
      GenerateUnitAttackTileOrder(gSideId, x[0], y[0]);
      goto LABEL_647;
    }
    if ( !IsAnyUnitSelected() )
    {
      goto LABEL_649;
    }
LABEL_646:
    GenerateUnitMoveOrder(gSideId, x[0], y[0]);
LABEL_647:
    AddCursorPuffAnimationToQueue(pixel_x, pixel_y);
    goto LABEL_648;
  }
  if ( side_id == gSideId && !_KeyboardKeyState[VK_CONTROL] )
  {
    if ( AllSelectedUnitsHaveBehavior(UnitBehavior_HARVESTER)
      && _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_REFINERY )
    {
      GenerateDockWithRefineryOrder(gSideId, object_index);
LABEL_610:
      PlayUnitResponse(1);
      SetBuildingToFlicker(building);
      goto LABEL_649;
    }
    if ( IsAnyUnitSelected() )
    {
      building_ = building;
      if ( _templates_buildattribs[building->Type].__Behavior != BuildingBehavior_REPAIR )
      {
        goto LABEL_612;
      }
      if ( IsAnyVehicleSelected() )
      {
        GenerateRepairSelectedUnitsOrder(gSideId, object_index);
        goto LABEL_610;
      }
    }
    building_ = building;
LABEL_612:
    building_type = building_->Type;
    if ( !(_templates_buildattribs[building_type]._____Flags & BFLAGS_200_SELECTABLE_REPAIRABLE) )
    {
      goto LABEL_649;
    }
    if ( building_->__IsSelected && side_id == gSideId )
    {
      if ( !MoreProductionBuildingsOfSameGroupExist(building_type) || (building->Flags & BFLAGS_80_PRIMARY) )
      {
        goto LABEL_649;
      }
      GenerateBuildingSetPrimaryOrder(gSideId);
      v155 = 0;
      harkonnen_sound_id = GetSoundTableID("H_PRIMARY");
      ordos_sound_id = GetSoundTableID("O_PRIMARY");
      atreides_sound_id = GetSoundTableID("S_PRIMARY");
      goto LABEL_618;
    }
LABEL_627:
    SelectBuilding((eSideType)side_id, object_index);
    goto LABEL_649;
  }
  if ( !_gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id] && !_KeyboardKeyState[VK_CONTROL] )
  {
    building_flags = _templates_buildattribs[building->Type]._____Flags;
    if ( building_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
    {
      SelectBuilding((eSideType)side_id, object_index);
    }
    goto LABEL_649;
  }
  v101 = GetSelectedBuildingWithBarrel();
  if ( v101 )
  {
    if ( IsBuildingInRange_0(v101, building) )
    {
      SetBuildingToFlicker(building);
      GenerateBuildingAttackBuildingOrder(gSideId, side_id, object_index);
      goto LABEL_649;
    }
    building_flags = _templates_buildattribs[building->Type]._____Flags;
    if ( !(building_flags & BFLAGS_200_SELECTABLE_REPAIRABLE) )
    {
      goto LABEL_649;
    }
    goto LABEL_627;
  }
  if ( !IsAnyUnitSelected() )
  {
    building_flags = _templates_buildattribs[building->Type]._____Flags;
    if ( building_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
    {
      SelectBuilding((eSideType)side_id, object_index);
    }
    goto LABEL_649;
  }
  if ( side_id != gSideId || IsAnyArmedUnitSelected() )
  {
    if ( AllSelectedUnitsHaveBehavior(UnitBehavior_SABOTEUR) )
    {
      building_flags = building->Flags;
      // New logic start
      // Do not order engineer/saboteur to enter building when clicking on building with infiltrated flag
      if ( !((building_flags & (BFLAGS_200_SELECTABLE_REPAIRABLE | BFLAGS_1000000_INFILTRATED)) == BFLAGS_200_SELECTABLE_REPAIRABLE) )
      // New logic end
      {
        goto LABEL_649;
      }
      SetBuildingToFlicker(building);
      GenerateUnitAttackBuildingOrder(gSideId, side_id, object_index);
    }
    else if ( AllSelectedUnitsHaveBehavior(UnitBehavior_ENGINEER) )
    {
      building_flags = building->Flags;
      // New logic start
      // Do not order engineer/saboteur to enter building when clicking on building with infiltrated flag
      if ( !((building_flags & (BFLAGS_400_CAN_BE_CAPTURED | BFLAGS_1000000_INFILTRATED)) == BFLAGS_400_CAN_BE_CAPTURED) )
      // New logic end
      {
        goto LABEL_649;
      }
      SetBuildingToFlicker(building);
      GenerateUnitAttackBuildingOrder(gSideId, side_id, object_index);
    }
    else
    {
      SetBuildingToFlicker(building);
      GenerateUnitAttackBuildingOrder(gSideId, side_id, object_index);
    }
    goto LABEL_648;
  }
LABEL_649:
  side = my_side;
  _MouseLeftUp = 0;
  _TacticalData.__DraggingBandbox = 0;
LABEL_650:
  if ( gUIMgr->dw_field_110_index )
  {
    return;
  }
  if ( _KeyboardKeyDown[VK_D] && _IsMultiplayer && gGameType == GAME_INTERNET && gTournamentGame )
  {
    order.__ObjectsServed1 = gGameTicks + _firgcrap_dword_4E3AE8;
    order.NumObjects = 0;
    order.w_field_4 = 1000;
    if ( !_DrawOffered1 && !_DrawOffered2 )
    {
      order.OrderType = eOrderType_33_OFFERDRAW;
      order.SideId = gSideId;
      order.__PlaceBuildingX = 1;
      Orderdata_add(&order);
LABEL_664:
      _KeyboardKeyDown[VK_D] = 0;
      goto LABEL_665;
    }
    if ( !_DrawOffered1 )
    {
      if ( _DrawOffered2 )
      {
        order.OrderType = eOrderType_34_OFFEREDDRAW;
        order.SideId = gSideId;
        Orderdata_add(&order);
        goto LABEL_664;
      }
      if ( !_DrawOffered1 )
      {
        goto LABEL_664;
      }
    }
    if ( !_DrawOffered2 )
    {
      order.OrderType = eOrderType_33_OFFERDRAW;
      order.SideId = gSideId;
      order.__PlaceBuildingX = 0;
      Orderdata_add(&order);
    }
    goto LABEL_664;
  }
LABEL_665:
  if ( _KeyboardKeyDown[VK_S] )
  {
    GenerateStopOrder(gSideId);
    _KeyboardKeyDown[VK_S] = 0;
  }
  if ( _KeyboardKeyDown[VK_A] )
  {
    if ( _IsMultiplayer )
    {
      GenerateAllyOrder(gSideId);
    }
    _KeyboardKeyDown[VK_A] = 0;
  }
  if ( _KeyboardKeyDown[VK_X] )
  {
    GenerateUnitScatterOrder(gSideId);
    _KeyboardKeyDown[VK_X] = 0;
  }
  if ( _KeyboardKeyDown[VK_P] )
  {
    GenerateBuildingSetPrimaryOrder(gSideId);
    _KeyboardKeyDown[VK_P] = 0;
  }
  if ( _KeyboardKeyDown[VK_G] )
  {
    if ( IsAnyArmedUnitSelected() )
    {
      GenerateUnitGuardOrder(gSideId);
      harkonnen_sound_id = GetSoundTableID("H_GUARDING");
      ordos_sound_id = GetSoundTableID("O_GUARDING");
      atreides_sound_id = GetSoundTableID("S_GUARDING");
      PlayMentatSound(atreides_sound_id, ordos_sound_id, harkonnen_sound_id, 0, 0, 0);
    }
    _KeyboardKeyDown[VK_G] = 0;
  }
  if ( _KeyboardKeyDown[VK_R] )
  {
    if ( IsAnyUnitSelected() )
    {
      if ( CSide__FindBestBasePosition(side, x, y) )
      {
        GenerateUnitRetreatOrder(gSideId, x[0], y[0]);
        harkonnen_sound_id = GetSoundTableID("H_RETREAT");
        ordos_sound_id = GetSoundTableID("O_RETREAT");
        atreides_sound_id = GetSoundTableID("S_RETREAT");
        PlayMentatSound(atreides_sound_id, ordos_sound_id, harkonnen_sound_id, 0, 0, 0);
      }
    }
    _KeyboardKeyDown[VK_R] = 0;
  }
  if ( _KeyboardKeyDown[VK_H] )
  {
    side = GetSide(gSideId);
    CSide_46CF10_HKEY_BattleFieldPos(side, &_ViewportXPos, &_ViewportYPos, 1);
    _KeyboardKeyDown[VK_H] = 0;
  }
  if ( _KeyboardKeyDown[VK_E] )
  {
    SelectAllUnitsInArea(_ViewportXPos, _ViewportYPos, _ViewportXPos + _ViewportWidth, _ViewportYPos + _ViewportHeight);
    _KeyboardKeyDown[VK_E] = 0;
  }
  if ( _KeyboardKeyDown[VK_N] )
  {
    SelectNextUnit();
    CenterViewportOnSelectedUnits(gSideId, &_ViewportXPos, &_ViewportYPos);
    _KeyboardKeyDown[VK_N] = 0;
  }
  if ( _KeyboardKeyDown[VK_T] )
  {
    _TacticalData.__SidebarButtonMode = 2;
    _KeyboardKeyDown[VK_T] = 0;
  }
  if ( _KeyboardKeyDown[VK_Y] )
  {
    _TacticalData.__SidebarButtonMode = 3;
    _KeyboardKeyDown[VK_Y] = 0;
  }
  if ( _KeyboardKeyDown[VK_HOME] )
  {
    if ( !CenterViewportOnSelectedUnits(gSideId, &_ViewportXPos, &_ViewportYPos) )
    {
      CenterViewportOnSelectedBuildings(gSideId, &_ViewportXPos, &_ViewportYPos);
    }
    _KeyboardKeyDown[VK_HOME] = 0;
  }
  viewporty = _ViewportYPos;
  viewportx = _ViewportXPos;
  ctrl_key_is_down = _KeyboardKeyState[VK_CONTROL];
  bookmark_key_id = VK_F9;
  v117 = 0;
  do
  {
    if ( _KeyboardKeyState[bookmark_key_id] )
    {
      if ( ctrl_key_is_down )
      {
        _TacticalData.__Bookmarks[v117].x = viewportx;
        _TacticalData.__Bookmarks[v117].y = viewporty;
      }
      else
      {
        viewportx = _TacticalData.__Bookmarks[v117].x;
        viewporty = _TacticalData.__Bookmarks[v117].y;
      }
    }
    ++v117;
    ++bookmark_key_id;
  }
  while ( v117 < 4 );
  unit_group_key_id = VK_0;
  _ViewportYPos = viewporty;
  _ViewportXPos = viewportx;
  v166 = VK_0;
  while ( 1 )
  {
    v119 = _KeyboardKeyState[unit_group_key_id];
    unit_group_id = unit_group_key_id - VK_0;
    a1[0] = unit_group_key_id - VK_0;
    if ( v119 )
    {
      if ( ctrl_key_is_down )
      {
        SetUnitGroup(unit_group_id);
      }
      else if ( _KeyboardKeyState[VK_MENU] )
      {
        side = GetSide(gSideId);
        if ( CSide__SelectUnitsByGroupId(side, unit_group_id, 1) )
        {
          v122 = &_UnitGroupKeyState[a1[0]];
          if ( !_UnitGroupKeyState[a1[0]] )
          {
            PlayUnitResponse(0);
            *v122 = 1;
          }
          side = GetSide(gSideId);
          CSide__CenterViewportOnFirstUnitFromGroupId(side, unit_group_id, &_ViewportXPos, &_ViewportYPos);
        }
      }
      else if ( _KeyboardKeyState[VK_SHIFT] )
      {
        side = GetSide(gSideId);
        if ( CSide__SelectUnitsByGroupId(side, unit_group_id, 0) )
        {
          if ( _UnitGroupKeyState[a1[0]] )
          {
            _UnitGroupKeyState[a1[0]] = 1;
            PlayUnitResponse(0);
            v125 = 0;
            side_id = 0;
            if ( _SidesToProcess )
            {
              do
              {
                if ( v125 != gSideId )
                {
                  side = GetSide((eSideType)side_id);
                  CSide__DeselectAllUnits(side);
                  side = GetSide((eSideType)side_id);
                  CSide__DeselectAllBuildings(side);
                  v125 = side_id;
                }
                side_id = ++v125;
              }
              while ( v125 < (unsigned __int8)_SidesToProcess );
            }
          }
        }
      }
      else
      {
        side = GetSide(gSideId);
        if ( CSide__SelectUnitsByGroupId(side, unit_group_id, 1) )
        {
          if ( !_UnitGroupKeyState[a1[0]] )
          {
            _UnitGroupKeyState[a1[0]] = 1;
            PlayUnitResponse(0);
            v129 = 0;
            side_id = 0;
            if ( _SidesToProcess )
            {
              do
              {
                if ( v129 != gSideId )
                {
                  side = GetSide((eSideType)side_id);
                  CSide__DeselectAllUnits(side);
                  side = GetSide((eSideType)side_id);
                  CSide__DeselectAllBuildings(side);
                  v129 = side_id;
                }
                side_id = ++v129;
              }
              while ( v129 < (unsigned __int8)_SidesToProcess );
            }
          }
        }
      }
    }
    else
    {
      _UnitGroupKeyState[a1[0]] = 0;
    }
    unit_group_key_id = v166 + 1;
    if ( !(++v166 < 58) )
    {
      break;
    }
    ctrl_key_is_down = _KeyboardKeyState[VK_CONTROL];
  }
  if ( _IsMultiplayer )
  {
    uimgr = gUIMgr;
    if ( !gUIMgr->dw_field_110_index && !_DebugOn_Pathfind_WinLose_DebugNewGame && !gLose )
    {
      v135 = 0;
      a1[0] = 0;
      if ( gTotalPlayers )
      {
        v136 = (_BYTE *)(_OrderData + 55);
        v137 = gTotalPlayers;
        do
        {
          if ( *v136 )
          {
            ++a1[0];
          }
          v136 += 60;
          --v137;
        }
        while ( v137 );
      }
      v138 = a1[0];
      v139 = 0;
      my_side_1 = 0;
      v140 = VK_F1;
      do
      {
        if ( _KeyboardKeyDown[v140] )
        {
          if ( v140 - 112 < v138 - 1 )
          {
            _KeyboardKeyDown[v140] = 0;
            v139 = v140 - 112;
            v135 = 1;
          }
        }
        ++v140;
      }
      while ( v140 <= VK_F5 );
      my_side_1 = v139;
      if ( v135 )
      {
        v141 = 0;
        v142 = 0;
        v166_1 = 0;
        if ( a1[0] )
        {
          do
          {
            if ( _OrderData[(unsigned __int8)v166_1].c_field_37 && gSideId != v142 )
            {
              if ( !v139 )
              {
                my_side_1 = v142;
                v141 = 1;
                break;
              }
              --v139;
            }
            v166_1 = ++v142;
          }
          while ( v142 < a1[0] );
        }
        if ( v141 )
        {
          v143 = (unsigned __int8)my_side_1;
          strcpy(gSendingToPlayer, _NetPlayerNamesArray[v143]);
          strcat(gSendingToPlayer, " : ");
          dword_797E18 = _OrderData[v143].dw_field_10;
          CUIManager__ReplaceWithOne_470E60(uimgr, "GAMEMESSAGE", gBackBuf);
        }
      }
    }
  }
  if ( _KeyboardKeyDown[VK_F7] )
  {
    if ( _IsMultiplayer )
    {
      if ( gGameType == GAME_INTERNET && !gUIMgr->dw_field_110_index )
      {
        if ( (unsigned __int8)IsOnlineGame() )
        {
          if ( strlen(gPageUser) == 0 )
          {
            string_id = GetTextID("NoOneToPage");
            string = GetTextString(string_id, 1);
            QueueMessage(string, -1);
          }
          else
          {
            CUIManager__ReplaceWithOne_470E60(gUIMgr, "PAGE_INGAME", gBackBuf);
          }
        }
      }
    }
    _KeyboardKeyDown[VK_F7] = 0;
  }
  if ( _KeyboardKeyDown[VK_F8] )
  {
    if ( _IsMultiplayer )
    {
      if ( gGameType == GAME_INTERNET )
      {
        SetgAllowPageUser(gAllowPageUser == 0);
        if ( gAllowPageUser )
        {
          string_id = GetTextID("InPageOnState");
        }
        else
        {
          string_id = GetTextID("InPageOffState");
        }
        string = GetTextString(string_id, 1);
        QueueMessage(string, -1);
      }
    }
    _KeyboardKeyDown[VK_F8] = 0;
  }
  if ( _KeyboardKeyDown[VK_F6] )
  {
    if ( _IsMultiplayer )
    {
      if ( !gUIMgr->dw_field_110_index )
      {
        string_id = GetTextID("ToAll");
        strcpy(gSendingToPlayer, GetTextString(string_id, 1));
        dword_797E18 = 0;
        CUIManager__ReplaceWithOne_470E60(gUIMgr, "GAMEMESSAGE", gBackBuf);
      }
    }
    _KeyboardKeyDown[VK_F6] = 0;
  }
}
