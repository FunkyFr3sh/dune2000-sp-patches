#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function GenerateUnitMoveOrder
DETOUR(0x00459450, 0x0045953B, _Mod__GenerateUnitMoveOrder);

void Mod__GenerateUnitMoveOrder(char side_id, unsigned char x, unsigned char y)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  if ( x >= gGameMap.width )
  {
    DebugFatal("GenerateUnitMoveOrder", "Invalid targetX");
  }
  if ( y >= gGameMap.height )
  {
    DebugFatal("GenerateUnitMoveOrder", "Invalid targetY");
  }
  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_13_UNITMOVE;
  order.SideId = side_id;
  order.BlockToX = x;
  order.BlockToY = y;
  i = 0;
  for ( unit = GetSide(side_id)->__FirstUnitPtr ; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateDockWithRefineryOrder
DETOUR(0x00459540, 0x004595D4, _Mod__GenerateDockWithRefineryOrder);

void Mod__GenerateDockWithRefineryOrder(char side_id, short refinery_index)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_17_DOCKWITHREFINERY;
  order.SideId = side_id;
  order.TargetSideId = side_id;
  order.TargetObjectIndex = refinery_index;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateRepairSelectedUnitsOrder
DETOUR(0x004595E0, 0x00459686, _Mod__GenerateRepairSelectedUnitsOrder);

void Mod__GenerateRepairSelectedUnitsOrder(char side_id, short repair_pad_index)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_21_UNITREPAIR;
  order.SideId = side_id;
  order.TargetSideId = side_id;
  order.TargetObjectIndex = repair_pad_index;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD && !_templates_unitattribs[unit->Type].__IsInfantry )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateUnitAttackUnitOrder
DETOUR(0x004596F0, 0x004597F2, _Mod__GenerateUnitAttackUnitOrder);

void Mod__GenerateUnitAttackUnitOrder(eSideType side_id, eSideType target_side_id, unsigned short target_unit_index)
{
  CSide *target_side; // eax
  Unit *unit; // eax MAPDST
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  target_side = GetSide(target_side_id);
  unit = &target_side->__ObjectArray[target_unit_index];
  if ( target_side->__ObjectArray[target_unit_index].ObjectType != 1 )
  {
    DebugFatal("GenerateUnitAttackUnitOrder", "Enemy is not a valid unit");
  }
  if ( unit->Flags & (UFLAGS_400000|UFLAGS_100_CARRYING|UFLAGS_40_FLYING) )
  {
    DebugFatal("GenerateUnitAttackUnitOrder", "Enemy is not a valid target unit");
  }
  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_14_UNITATTACKUNIT;
  order.SideId = side_id;
  order.TargetSideId = target_side_id;
  order.TargetObjectIndex = target_unit_index;
  i = 0;
  for ( unit = GetSide(side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD && _templates_unitattribs[unit->Type].__PrimaryWeapon != -1 )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateUnitAttackBuildingOrder
DETOUR(0x00459800, 0x004598C6, _Mod__GenerateUnitAttackBuildingOrder);

void Mod__GenerateUnitAttackBuildingOrder(char side_id, char target_side_id, short target_building_index)
{
  Unit *unit; // esi
  __int16 i; // di
  char behavior; // al
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_15_UNITATTACKBUILDING;
  order.SideId = side_id;
  order.TargetSideId = target_side_id;
  order.TargetObjectIndex = target_building_index;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD )
      {
        behavior = _templates_unitattribs[unit->Type].__Behavior;
        if ( behavior != UnitBehavior_HARVESTER
          && behavior != UnitBehavior_MCV
          && behavior != UnitBehavior_THUMPER
          && (side_id != target_side_id || (behavior != UnitBehavior_ENGINEER && behavior != UnitBehavior_SABOTEUR)) )
        {
          order.ObjectArray[i++] = unit->MyIndex;
        }
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateUnitAttackTileOrder
DETOUR(0x004598D0, 0x00459980, _Mod__GenerateUnitAttackTileOrder);

void Mod__GenerateUnitAttackTileOrder(char side_id, short x, short y)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_16_UNITATTACKTILE;
  order.SideId = side_id;
  order.BlockToX = x;
  order.BlockToY = y;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD && _templates_unitattribs[unit->Type].__PrimaryWeapon != -1 )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateUnitGuardOrder
DETOUR(0x00459980, 0x00459A16, _Mod__GenerateUnitGuardOrder);

void Mod__GenerateUnitGuardOrder(char side_id)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_18_GUARD;
  order.SideId = side_id;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD && _templates_unitattribs[unit->Type].__PrimaryWeapon != -1 )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateUnitScatterOrder
DETOUR(0x00459A20, 0x00459AA3, _Mod__GenerateUnitScatterOrder);

void Mod__GenerateUnitScatterOrder(unsigned char side_id)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_19_SCATTER;
  order.SideId = side_id;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateUnitRetreatOrder
DETOUR(0x00459AB0, 0x00459B4F, _Mod__GenerateUnitRetreatOrder);

void Mod__GenerateUnitRetreatOrder(char side_id, unsigned char x, unsigned char y)
{
  Unit *unit; // eax
  __int16 i; // cx
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_1A_RETREAT;
  order.SideId = side_id;
  order.BlockToX = x;
  order.BlockToY = y;
  i = 0;
  for ( unit = GetSide((eSideType)side_id)->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}

// Custom implementation of function GenerateStopOrder
DETOUR(0x00459C40, 0x00459D11, _Mod__GenerateStopOrder);

void Mod__GenerateStopOrder(char side_id)
{
  CSide *side; // eax MAPDST
  __int16 i; // si
  Unit *unit; // eax
  Building *building; // eax
  Orderdata order; // [esp+Ch] [ebp-A8h]

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_1C_STOPUNIT;
  order.SideId = side_id;
  side = GetSide((eSideType)side_id);
  i = 0;
  for ( unit = side->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( i == 64 )
    {
      order.NumObjects = 64;
      Orderdata_add(&order);
      i = 0;
    }
    if ( unit->__IsSelected )
    {
      if ( unit->State != UNIT_STATE_17_DEAD )
      {
        order.ObjectArray[i++] = unit->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }

  order.OrderType = eOrderType_1D_STOPBUILDING;
  i = 0;
  for ( building = side->__FirstBuildingPtr; building; building = building->Next )
  {
    if ( i >= 64 )
    {
      break;
    }
    if ( building->__IsSelected )
    {
      if ( building->__State != UNIT_STATE_17_DEAD )
      {
        order.ObjectArray[i++] = building->MyIndex;
      }
    }
  }
  if ( i )
  {
    order.NumObjects = i;
    Orderdata_add(&order);
  }
}
