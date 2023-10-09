#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "crates-func.h"

// Add tagged deliveries - delivered units are given a tag

// Custom implementation of function UnitDeliver
CALL(0x004985E0, _Mod__UnitDeliver); // UpdateUnit

char Mod__UnitDeliver(Unit *unit, int side_id)
{
  Unit *unit_; // edi
  unsigned char y_; // cl
  unsigned char delivery_index_; // dl
  CSide *side; // eax
  char delivery_type; // cl
  DeliveryStruct *delivery; // ebp
  int add_at_y; // edx
  unsigned char tile_x; // al
  TileFlags tile_flags; // eax
  unsigned char v11; // dl
  char v13; // al
  unsigned char unit_delivery_order; // cl
  unsigned char v15; // al
  char *v16; // ebx
  unsigned char add_at_x; // al
  int16_t unit_index; // si
  Unit *delivered_unit; // esi
  Building *bld; // eax
  int v21; // edx
  unsigned char x; // [esp+10h] [ebp-10h]
  unsigned char y; // [esp+14h] [ebp-Ch]
  unsigned char v24; // [esp+18h] [ebp-8h]
  unsigned char delivery_index; // [esp+1Ch] [ebp-4h]
  char result; // [esp+24h] [ebp+4h]

  unit_ = unit;
  result = 1;
  y_ = unit_->BlockToY;
  delivery_index_ = unit_->LastMoved;
  x = unit_->BlockToX;
  y = y_;
  delivery_index = delivery_index_;
  side = GetSide((eSideType)side_id);
  delivery_type = side->__Deliveries[delivery_index].__delivery_type;
  delivery = &side->__Deliveries[delivery_index];
  v24 = delivery_type != 2 ? 4 : 8;
  if ( gGameTicks % (unsigned int)v24 )
  {
    return result;
  }
  add_at_y = unit_->BlockToY;
  tile_x = unit_->BlockToX;
  v24 = unit_->BlockToY;
  tile_flags = gGameMap.map[tile_x + _CellNumbersWidthSpan[v24]].__tile_bitflags;
  if ( (tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_10_OCC_BUILDING|TileFlags_8_OCC_UNIT)
    && delivery_type == 1)
    || (tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID|TileFlags_8_OCC_UNIT)
    && delivery_type == 2) )
  {
    v11 = unit_->LastFired + 1;
    unit_->LastFired = v11;
    if ( v11 > 0x28u && delivery->__delivery_type == 1 )
    {
      UnitAdjustState(unit_, UNIT_STATE_22_FLYING_UP);
      return 1;
    }
    if ( v11 > 0x50u )
    {
      delivery->__is_active = 0;
      return 0;
    }
    return result;
  }
  unit_->LastFired = 0;
  v13 = delivery->__units[0];
  unit_delivery_order = 0;
  v24 = 0;
  if ( v13 == -2 )
  {
    do
    {
      v24 = ++unit_delivery_order;
      v15 = unit_delivery_order;
    }
    while ( delivery->__units[unit_delivery_order] == -2 );
  }
  else
  {
    v15 = v24;
  }
  v16 = &delivery->__xpos + v15;
  if ( v16[8] == -1 )
  {
    result = 0;
    delivery->__is_active = 0;
    return result;
  }
  if ( gDifficultyLevel
    || !(unit_delivery_order & 1)
    || !_gDiplomacy[(unsigned char)side_id][(unsigned char)gSideId] )
  {
    add_at_x = unit_->BlockToX;
    y = add_at_y;
    x = add_at_x;
    unit_index = ModelAddUnit((eSideType)side_id, v16[8], add_at_x, add_at_y, add_at_x, add_at_y, 0, 0);
    if ( unit_index != -1 )
    {
      delivered_unit = &GetSide((eSideType)side_id)->__ObjectArray[unit_index];
      delivered_unit->__Facing = unit_->__Facing;
      // New logic start
      // Set delivered unit's tag
      delivered_unit->Tag = delivery->tag;
      // Pick up crate
      Mod__PickupCrate(delivered_unit, side_id);
      // New logic end
      if ( _templates_unitattribs[(unsigned char)v16[8]].__Behavior == UnitBehavior_HARVESTER )
      {
        delivered_unit->State = UNIT_STATE_11_MOVING_TO_HARVEST;
        delivered_unit->OldState = UNIT_STATE_11_MOVING_TO_HARVEST;
        if ( delivery->__delivery_type == 2 )
        {
          delivered_unit->RefineryIndex = GetRefineryIndex((eSideType)side_id);
        }
        else
        {
          bld = GetNearestBuildingWithBehavior(x, y, (eSideType)side_id, BuildingBehavior_REFINERY, &x, &y);
          if ( bld )
          {
            delivered_unit->RefineryIndex = bld->__MyIndex;
          }
        }
      }
      else
      {
        delivered_unit->OldState = 2;
      }
      if ( delivery->__delivery_type == 2 )
      {
        if ( _templates_unitattribs[delivered_unit->Type].__Behavior != UnitBehavior_CARRYALL )
        {
          UnitAdjustState(delivered_unit, UNIT_STATE_26_LEAVING_REPAIR_PAD);
        }
      }
      else
      {
        tile_driveon_49E290((eSideType)side_id, &x, &y);
        delivered_unit->TargetX = x;
        delivered_unit->TargetY = y;
        UnitAdjustState(delivered_unit, UNIT_STATE_7_MOVING);
      }
      v21 = delivered_unit->Flags | UFLAGS_40000;
      delivered_unit->Flags = v21;
      if ( v16[9] == -1 )
      {
        delivered_unit->Flags = v21 | UFLAGS_80000;
      }
      delivered_unit->__DeviatedTimeCounter = delivery_index;
    }
  }
  v16[8] = -2;
  return 1;
}
