#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"

// Custom implementation of function AddBuildingTileDataAndRadar
DETOUR(0x0044DBD0, 0x0044DED2, _Mod__AddBuildingTileDataAndRadar);

void Mod__AddBuildingTileDataAndRadar(eSideType side_id, unsigned char building_type, unsigned char x, unsigned char y)
{
  int check_bit; // esi MAPDST
  int tiles_occupied_all; // ebp
  int *cell_offset_y; // edx
  unsigned int check_x; // ecx
  unsigned int xx_; // eax
  int actual_y_; // edi
  unsigned char actual_y; // al MAPDST
  unsigned char actual_x; // cl MAPDST
  Unit *unit; // esi
  CSide *side; // eax MAPDST
  int cell_index_; // ecx MAPDST
  BuildingBehaviorType behavior; // dl MAPDST
  int cell_index; // ebx
  TileFlags tile_flags; // ecx MAPDST
  int unit_side; // [esp+10h] [ebp-24h]
  int unit_index; // [esp+14h] [ebp-20h]
  unsigned int building_type_; // [esp+1Ch] [ebp-18h]
  int x_; // [esp+20h] [ebp-14h] MAPDST
  int tiles_occupied_solid; // [esp+24h] [ebp-10h]
  int *cell_offset_y_; // [esp+30h] [ebp-4h]
  unsigned int counter_y; // [esp+3Ch] [ebp+8h]
  unsigned int xx; // [esp+3Ch] [ebp+8h]
  unsigned int yy; // [esp+40h] [ebp+Ch] MAPDST

  x_ = x;
  check_bit = 1;
  building_type_ = 0x10C * building_type;
  x_ = x;
  tiles_occupied_all = _templates_buildattribs[building_type_ / 0x10C]._____TilesOccupiedAll;
  counter_y = 4;
  cell_offset_y = &_CellNumbersWidthSpan[y];
  cell_offset_y_ = &_CellNumbersWidthSpan[y];
  do
  {
    check_x = 0;
    do
    {
      if ( check_bit & tiles_occupied_all )
      {
        gGameMap.map[*cell_offset_y + check_x + x].__tile_bitflags &= ~TileFlags_8000_BUILD_ON;
      }
      check_bit *= 2;
      ++check_x;
    }
    while ( check_x < 4 );
    ++cell_offset_y;
    --counter_y;
  }
  while ( counter_y );
  check_bit = 1;
  yy = 0;
  tiles_occupied_solid = _templates_buildattribs[building_type_ / 0x10C]._____TilesOccupiedSolid;
  do
  {
    xx_ = 0;
    xx = 0;
    do
    {
      if ( check_bit & tiles_occupied_solid )
      {
        actual_y_ = yy + y;
        if ( GetUnitOnTile(32 * (x_ + xx_), 32 * (yy + y), (eSideType *)&unit_side, (short *)&unit_index, 0) )
        {
          actual_y = y + yy;
          actual_x = x_ + xx;
          unit_index = 0xFFFF;
          actual_y = y + yy;
          actual_x = x_ + xx;
          do
          {
            unit = GetNextUnitOnTile(actual_x, actual_y, unit_side, (short *)&unit_index);
            if ( !unit )
            {
              break;
            }
            if ( (_BYTE)unit_side != side_id )
            {
              side = GetSide(unit_side);
              ++side->__UnitsLost;
              side = GetSide(unit_side);
              ++side->__UnitsLostPerType[unit->Type];
              ChangeUnitOwner((eSideType)unit_side, side_id, unit_index, 0);
            }
          }
          while ( unit );
        }
        cell_index_ = x_ + xx + _CellNumbersWidthSpan[actual_y_];
        behavior = _templates_buildattribs[building_type_ / 0x10C].__Behavior;
        tile_flags = (unsigned char)side_id | (gGameMap.map[x_ + xx + _CellNumbersWidthSpan[actual_y_]].__tile_bitflags & ~(TileFlags_8000_BUILD_ON|TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER));
        tile_flags = tile_flags | TileFlags_10_OCC_BUILDING;
        gGameMap.map[x_ + xx + _CellNumbersWidthSpan[actual_y_]].__tile_bitflags = tile_flags;
        if ( behavior == BuildingBehavior_REPAIR && (xx != 1 || yy != 1) )
        {
          gGameMap.map[cell_index_].__tile_bitflags = tile_flags | TileFlags_10000000_BUILDING_EXIT;
        }
        if ( behavior == BuildingBehavior_HEAVY_FACTORY && !xx && yy == 2 )
        {
          cell_index_ = x_ + _CellNumbersWidthSpan[y + 2];
          gGameMap.map[cell_index_].__tile_bitflags |= TileFlags_10000000_BUILDING_EXIT;
        }
        if ( behavior == BuildingBehavior_LIGHT_FACTORY && xx == 2 && yy == 1 )
        {
          cell_index_ = x_ + _CellNumbersWidthSpan[y + 1];
          gGameMap.map[cell_index_ + 2].__tile_bitflags |= TileFlags_10000000_BUILDING_EXIT;
        }
        if ( gBitsPerPixel == 16 )
        {
          SetPixelOnRadar16(x_ + xx, y + yy, _radarcolor16_sidecolor[(unsigned char)side_id]);
        }
        else
        {
          SetPixelOnRadar8(x_ + xx, y + yy, _radarcolor8_sidecolor[(unsigned char)side_id]);
        }
      }
      check_bit *= 2;
      xx_ = xx++ + 1;
    }
    while ( xx < 4 );
    ++yy;
  }
  while ( yy < 4 );
  behavior = _templates_buildattribs[building_type_ / 0x10C].__Behavior;
  // New logic start
  // Implement TurretNoWall template property
  if ( (behavior == BuildingBehavior_WALL || behavior == BuildingBehavior_TURRET) && ! _templates_buildattribs[building_type_ / 0x10C].TurretNoWall )
  // New logic end
  {
    cell_index = *cell_offset_y_ + x_;
    tile_flags = gGameMap.map[cell_index].__tile_bitflags;
    tile_flags |= TileFlags_400_HAS_WALL;
    gGameMap.map[cell_index].__tile_bitflags = tile_flags;
  }
}

// Custom implementation of function ModelAddBuilding
DETOUR(0x004563B0, 0x004569DD, _Mod__ModelAddBuilding);

short Mod__ModelAddBuilding(eSideType side_id, char building_type, unsigned char x, unsigned char y, int initialsetup, bool captured, bool captured2)
{
  CSide *side; // edi
  Building *bld; // esi
  unsigned char building_type_; // cl
  short my_index; // bp
  int building_type__; // ebx
  char side_id_; // al
  int building_type___; // eax MAPDST
  int building_flags; // eax MAPDST
  int art_height; // edx
  int tiles_occupied_solid; // ecx MAPDST
  signed int check_bit; // ebp
  unsigned int xx; // eax
  bool yy_less_than_4; // cf
  unsigned char delivery_index; // cl
  CSide *side_delivery; // eax
  BuildingBehaviorType behavior; // al
  int string_id; // eax
  char *string; // eax
  unsigned int check_x; // [esp+10h] [ebp-24h]
  int yy; // [esp+14h] [ebp-20h]
  unsigned int check_y; // [esp+18h] [ebp-1Ch]
  unsigned int concreteTileCount1; // [esp+1Ch] [ebp-18h]
  unsigned int buildTileCount1; // [esp+20h] [ebp-14h]
  int x_; // [esp+24h] [ebp-10h] MAPDST
  int y_; // [esp+30h] [ebp-4h] MAPDST
  unsigned char check_delivery_index; // [esp+4Ch] [ebp+18h]

  side = GetSide(side_id);
  if ( !side->__ParticipatesInGame )
  {
    return -1;
  }
  bld = (Building *)CSide__GetNextFreeObject(side);
  if ( !bld )
  {
    string_id = GetTextID("ReachedUnitLimit");
    string = GetTextString(string_id, 1);
    QueueMessage(string, -1);
    return -1;
  }
  building_type_ = building_type;
  ++side->__BuildingsBuilt;
  ++side->__BuildingsBuiltPerType[(unsigned char)building_type];
  bld->Type = building_type;
  bld->__State = BLD_STATE_2_IDLE;
  bld->__IsSelected = 0;
  my_index = bld->MyIndex;
  building_type__ = (unsigned char)building_type;
  bld->Health = _templates_buildattribs[(unsigned char)building_type]._____HitPoints;
  bld->__TurretTurnDelayCounter = _templates_buildattribs[(unsigned char)building_type]._____BarrelRotationSpeed;
  bld->__ReloadDelayCounter_refinery = _templates_buildattribs[(unsigned char)building_type].PrimaryWeaponLongDelay;
  side_id_ = side_id;
  bld->EnemySide = side_id;
  bld->EnemyIndex = -1;
  bld->__MySide = side_id;
  bld->__MyIndex = my_index;
  bld->__AttackerIndex = -1;
  bld->__AttackerSide = -1;
  if ( !(_templates_buildattribs[building_type__]._____Flags & BFLAGS_400000_NO_CONCRETE)
    && ((_BYTE)initialsetup
     || (_templates_buildattribs[building_type__].__Behavior == BuildingBehavior_CONSTRUCTION && !captured)) )
  {
    ModelAddConcrete(
      side_id,
      building_type,
      x,
      y,
      initialsetup,
      _templates_buildattribs[building_type__]._____TilesOccupiedAll);
    building_type_ = building_type;
    side_id_ = side_id;
  }
  GetBuildingOnConcreteCount(side_id_, building_type_, x, y, &buildTileCount1, &concreteTileCount1);
  bld->__DirectionFrame = 0;
  building_type___ = bld->Type;
  bld->SpecialPurpose = 0;
  bld->__DeadStateTimeCounter = 0;
  bld->__MakeSmokeTimeCounter = 0;
  bld->__FlickerFrames = 0;
  building_flags = _templates_buildattribs[building_type___]._____Flags;
  bld->__BuildingAnimCounter = 0;
  bld->Flags = building_flags;
  if ( !(_templates_buildattribs[building_type__]._____Flags & BFLAGS_400000_NO_CONCRETE) )
  {
    bld->Health = bld->Health / 2 + (bld->Health * concreteTileCount1 >> 1) / buildTileCount1;
    if ( concreteTileCount1 != buildTileCount1 )
    {
      building_flags = bld->Flags;
      building_flags |= BFLAGS_8000_DECAY;
      bld->Flags = building_flags;
    }
  }
  bld->__LastAttacked = -1;
  yy = 0;
  x_ = x;
  bld->__PosX = x << 21;
  art_height = _templates_buildattribs[building_type__]._____ArtHeight;
  bld->__WallFrame = 0;
  bld->__Facing = 0;
  bld->__PosY = (32 * y + art_height) << 16;
  bld->__MinXPos = x;
  bld->__MinYPos = y;
  bld->__MaxXPos = x;
  bld->__MaxYPos = y;
  tiles_occupied_solid = _templates_buildattribs[building_type__]._____TilesOccupiedSolid;
  y_ = y;
  check_bit = 1;
  check_y = y;
  do
  {
    xx = 0;
    check_x = x_;
    do
    {
      if ( check_bit & tiles_occupied_solid )
      {
        if ( (unsigned char)bld->__MaxXPos < check_x )
        {
          bld->__MaxXPos = x + xx;
        }
        if ( (unsigned char)bld->__MaxYPos < check_y )
        {
          bld->__MaxYPos = y + yy;
        }
      }
      check_bit *= 2;
      ++xx;
      ++check_x;
    }
    while ( xx < 4 );
    yy_less_than_4 = (unsigned int)(yy++ + 1) < 4;
    ++check_y;
  }
  while ( yy_less_than_4 );
  bld->__addedonticks2 = gGameTicks;
  if ( !(_BYTE)initialsetup )
  {
    if ( _templates_BuildupArtFrames[building_type___] )
    {
      building_flags = bld->Flags;
      bld->__BuildupAnimCounter = 0;
      building_flags = building_flags | BFLAGS_2_BUILDUP;
      bld->__addedonticks1 = gGameTicks;
      bld->Flags = building_flags;
    }
  }
  bld->dw_field_84_ptr = (int)&side->__FirstBuildingPtr;
  bld->dw_field_88_ptr = (int)&side->__LastBuildingPtr;
  ++side->__BuildingsExistPerGroup[_templates_buildattribs[building_type__].GroupType];
  CSide__SetupObject(side, (Unit *)bld, OBJECT_BUILDING);
  CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(side);
  switch ( (unsigned char)_templates_buildattribs[building_type__].__Behavior )
  {
    case BuildingBehavior_CONSTRUCTION:
      if ( side->__PrimaryConyard == -1 )
      {
        side->__PrimaryConyard = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_WINDTRAP:
    case BuildingBehavior_REFINERY:
    case BuildingBehavior_SILO:
    case BuildingBehavior_REPAIR:
    case BuildingBehavior_WALL:
    case BuildingBehavior_CONCRETE:
    case BuildingBehavior_TURRET:
      break;
    case BuildingBehavior_LIGHT_FACTORY:
      if ( side->__PrimaryLightFactory == -1 )
      {
        side->__PrimaryLightFactory = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_HEAVY_FACTORY:
      if ( side->__PrimaryHeavyFactory == -1 )
      {
        side->__PrimaryHeavyFactory = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_HITEECH:
      if ( side->__PrimaryHitech == -1 )
      {
        side->__PrimaryHitech = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_AT_HITEECH:
      if ( side->__PrimaryAtHitech == -1 )
      {
        side->__PrimaryAtHitech = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_BARRACKS:
      if ( side->__PrimaryBarracks == -1 )
      {
        side->__PrimaryBarracks = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_WOR:
      if ( side->__PrimaryWor == -1 )
      {
        side->__PrimaryWor = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_STARPORT:
      if ( side->__PrimaryStarport == -1 )
      {
        side->__PrimaryStarport = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_HK_PALACE:
      if ( side->__PrimaryHarkPalace == -1 )
      {
        side->__PrimaryHarkPalace = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_OR_PALACE:
      if ( side->__PrimaryOrdPalace == -1 )
      {
        side->__PrimaryOrdPalace = bld->MyIndex;
        goto LABEL_46;
      }
      break;
    case BuildingBehavior_A_PALACE:
      if ( side->__PrimaryAtrPalace == -1 )
      {
        side->__PrimaryAtrPalace = bld->MyIndex;
LABEL_46:
        building_flags = bld->Flags;
        building_flags = building_flags | BFLAGS_80_PRIMARY;
        bld->Flags = building_flags;
      }
      break;
  }
  if ( !(_BYTE)initialsetup && side->__PrimaryConyard != -1 && !captured2 )
  {
    Building *b = (Building *)&side->__ObjectArray[(unsigned short)side->__PrimaryConyard];
    b->Flags |= BFLAGS_20_ANIM_ONCE;
  }
  // New logic start
  // Share buildings view with spectators
  if ( (side_id == gSideId) || (SpawnerActive && gLose && gGameType != GAME_CAMPAIGN) )
  // New logic end
  {
    RevealTilesSeenByBuilding(building_type, x, y);
  }
  AddBuildingTileDataAndRadar(side_id, building_type, x, y);
  if ( _templates_buildattribs[building_type__].__Behavior == BuildingBehavior_REFINERY && !captured )
  {
    check_delivery_index = 0;
    if ( side->__Deliveries[0].__is_active == 1 )
    {
      do
      {
        if ( ++check_delivery_index >= 10u )
        {
          DebugFatal("Model.Cpp", "Too many deliveries (New harv.)");
        }
        delivery_index = check_delivery_index;
      }
      while ( side->__Deliveries[check_delivery_index].__is_active == 1 );
    }
    else
    {
      delivery_index = 0;
    }
    side_delivery = (CSide *)((char *)side + 0x38 * delivery_index);
    side_delivery->__Deliveries[0].__is_active = 1;
    side_delivery->__Deliveries[0].c_field_2 = 0;
    side_delivery->__Deliveries[0].__xpos = x + 2;
    side_delivery->__Deliveries[0].__ypos = y + 1;
    side_delivery->__Deliveries[0].__delivery_time = gGameTicks;
    side_delivery->__Deliveries[0].__deploy_action = 4;
    side_delivery->__Deliveries[0].__delivery_type = 1;
    side_delivery->__Deliveries[0].__units[0] = _templates_GroupIDs.Harvester;
    side_delivery->__Deliveries[0].__units[1] = -1;
  }
  behavior = _templates_buildattribs[building_type__].__Behavior;
  // New logic start
  // Implement TurretNoWall template property
  if ( (behavior == BuildingBehavior_WALL || behavior == BuildingBehavior_TURRET) && !_templates_buildattribs[building_type__].TurretNoWall )
  // New logic end
  {
    bld->__WallFrame = ResolveWall(x_, y_);
    UpdateWallFrameOnAdjacentBuildings(x_, y_);
  }
  if ( _templates_buildattribs[building_type__]._____Flags & BFLAGS_200000_HAS_SKIRT )
  {
    AddSkirt(x, y, building_type);
  }
  return bld->MyIndex;
}
