#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"
#include "extended-tileset.h"

bool CanUnitShoot(Unit *unit);

// Custom implementation of function FindSandwormsTarget
DETOUR(0x0044D630, 0x0044D75B, _Mod__FindSandwormsTarget);

bool Mod__FindSandwormsTarget(unsigned char from_x, unsigned char from_y, unsigned char *target_x_ptr, unsigned char *target_y_ptr)
{
  int map_height; // edi
  int from_y_; // ecx
  unsigned short min_distance; // si
  bool result; // al
  int x; // ebx
  int x_distance; // edi
  int cell_index; // eax
  int new_distance; // ecx
  bool v12; // cf
  int y_distance; // [esp+10h] [ebp-8h]
  int from_x_; // [esp+14h] [ebp-4h]
  int *a1a; // [esp+1Ch] [ebp+4h]
  int y; // [esp+20h] [ebp+8h]
  eSideType s;
  _WORD i;
  Unit *unit_on_tile;

  map_height = gGameMap.height;
  from_y_ = from_y;
  min_distance = gGameMap.height + gGameMap.width;
  from_x_ = from_x;
  // New logic start
  // Implement not edible units
  if ( gGameMap.map[from_x + _CellNumbersWidthSpan[from_y]].__tile_bitflags & TileFlags_8_OCC_UNIT )
  {
    unit_on_tile = GetUnitOnTile(32 * from_x, 32 * from_y, &s, &i, 0);
    if (unit_on_tile && !_templates_unitattribs[unit_on_tile->Type].NotEdible)
    {
      *target_x_ptr = from_x;
      *target_y_ptr = from_y;
      return 1;
    }
  }
  // New logic end
  y = 0;
  if ( gGameMap.height )
  {
    y_distance = from_y_;
    a1a = _CellNumbersWidthSpan;
    do
    {
      x = 0;
      if ( gGameMap.width )
      {
        x_distance = from_x_;
        do
        {
          cell_index = x + *a1a;
          if ( gGameMap.map[cell_index].__tile_bitflags & TileFlags_10000_SANDY )
          {
            if ( gGameMap.map[cell_index].__tile_bitflags & TileFlags_8_OCC_UNIT )
            {
              // New logic start
              // Implement not edible units

              unit_on_tile = GetUnitOnTile(32 * x, 32 * y, &s, &i, 0);
              if (unit_on_tile && !_templates_unitattribs[unit_on_tile->Type].NotEdible)
              // New logic end
              {
                new_distance = abs(y_distance) + abs(x_distance);
                if ( (unsigned short)new_distance < min_distance )
                {
                  min_distance = new_distance;
                  *target_x_ptr = x;
                  *target_y_ptr = y;
                }
              }
            }
          }
          ++x;
          --x_distance;
        }
        while ( x < gGameMap.width );
        map_height = gGameMap.height;
      }
      v12 = y++ + 1 < map_height;
      ++a1a;
      --y_distance;
    }
    while ( v12 );
  }
  result = min_distance < (signed int)(gGameMap.width + map_height);
  return result;
}

// Custom implementation of function UpdateUnit
DETOUR(0x00497B90, 0x0049B76E, _Mod__UpdateUnit);

char Mod__UpdateUnit(Unit *unit, eSideType side_id, short myIndex)
{
  unsigned char v3;
  int side_id_; // ebx MAPDST
  int unit_type_offset; // eax MAPDST
  char is_infantry; // cl MAPDST
  int my_index; // ST30_4
  int health; // eax MAPDST
  unsigned char cloaking_points; // al MAPDST
  int attacker_object_type; // eax
  CSide *attacker_side; // eax
  int enemy_object_type; // eax MAPDST
  Unit *enemy_unit; // eax MAPDST
  UnitFlags enemy_unit_flags; // eax
  char random_value_; // al
  int enemy_index; // edi
  int myindex_and_7; // ebp
  int last_moved; // eax
  unsigned char deviated_time_counter; // al MAPDST
  int last_attacked; // ecx
  char v31; // al
  eSideType old_side; // al
  int last_fired; // eax
  int v36; // eax
  char behavior; // al
  unsigned char v40; // cl
  unsigned char v41; // cl
  CSide *v42; // edi
  signed char v43; // al
  __int64 v44; // rax
  CSide *v45; // ecx
  UnitFlags v47; // eax
  signed int posz; // eax MAPDST
  unsigned char v55; // dl
  unsigned char v56; // al
  int v57; // eax
  int side_id__; // edi
  unsigned char ai_group; // al MAPDST
  char v63; // cl
  int v65; // eax
  int v66; // ebx
  int v68; // edi
  dwXYStruct v71; // ST20_8
  int v75; // eax
  UnitFlags v76; // eax
  unsigned short v78; // ax
  int v79; // edx
  Unit *v81; // edi
  int v82; // eax
  unsigned short v84; // ax
  Unit *v86; // edi
  int v87; // edx
  unsigned int v89; // eax
  unsigned int v90; // ebp
  unsigned char enemy_side_id; // al
  CSide *enemy_side; // eax MAPDST
  int v96; // ebp
  int v97; // kr00_4
  int v98; // eax
  unsigned char remember_pos_x; // al
  unsigned char remember_pos_y; // dl MAPDST
  int v101; // edi
  int v102; // ebp
  short v103; // ST30_2
  bool v104; // cf
  short v105; // ST30_2
  unsigned char v106; // dl
  short v107; // ST30_2
  char can_attack; // al MAPDST
  char v109; // al
  char special_purpose; // al MAPDST
  unsigned int *building_flags; // eax
  CSide *side_; // eax MAPDST
  int old_state; // eax
  int v117; // ecx
  int target_y; // edx
  int target_x; // eax
  Unit *unit_on_target_tile; // edi
  int v122; // eax
  LONG v123; // ecx
  int v124; // edx
  char v125; // al
  char v126; // cl
  unsigned int v127; // ebp
  int v130; // eax
  TileFlags v131; // eax
  unsigned char v132; // dl
  eUnitState v133; // ST30_4
  char v135; // al
  unsigned char v136; // al
  dwXYStruct v137; // ST20_8
  unsigned char v138; // al
  unsigned char v139; // cl
  unsigned char v141; // al
  unsigned char v142; // cl
  unsigned char v143; // al
  int v144; // ecx
  int v145; // edx
  int v146; // ecx
  TileFlags v147; // eax
  int v148; // edx
  int v149; // ecx
  int cell_index; // eax
  unsigned int v151; // edi
  int v152; // eax
  int v153; // ecx
  int v154; // eax
  int v155; // ecx
  unsigned char v156; // dl
  char v157; // al
  int v158; // eax
  int v159; // kr04_4
  unsigned char v160; // al
  int v164; // eax
  short v170; // ST30_2
  int v173; // eax
  char v174; // dl
  unsigned char v175; // cl
  unsigned char v176; // al
  char v177; // al
  unsigned char v178; // al
  char v179; // al
  int v181; // eax
  unsigned char v182; // al
  unsigned char v183; // dl
  int v184; // ST30_4
  char v188; // al
  char v189; // al
  char v191; // al
  int v193; // ecx
  int v194; // edx
  int v196; // ecx
  int v197; // edx
  TileFlags tile_flags; // eax MAPDST
  UnitFlags unit_flags; // eax MAPDST
  int v201; // ecx
  char v202; // dl
  int v203; // eax
  int v204; // edi
  index enemy_index_; // ax
  int text_id; // eax
  char *text_string; // eax
  Building *bld; // edi
  int v224; // eax
  bool is_low_money; // zf
  bool v229; // zf
  Building *nearest_repair_pad; // eax
  int hark_sound_id; // ST24_4 MAPDST
  int ord_sound_id; // ST20_4 MAPDST
  int atr_sound_id; // eax MAPDST
  int distance_x; // edi MAPDST
  int distance_y; // eax MAPDST
  int unit_state; // ST30_4 MAPDST
  int v249; // edx
  int v250; // eax
  unsigned int v251; // eax
  int cost; // eax
  int max_health; // eax
  unsigned int credits; // eax
  char v259; // al
  unsigned char v260; // cl
  Unit *unit_on_tile; // eax MAPDST
  UnitFlags v267; // edi
  int v272; // ST20_4
  int v273; // ST1C_4
  unsigned char v274; // cl
  int explosion_type; // edi
  unsigned int random_value; // eax
  char sleep_time_counter; // al MAPDST
  unsigned char sleep_time_subcounter; // dl
  unsigned int conyard_type; // edi
  char *behavior_ptr; // ebp
  int posy; // ecx MAPDST
  int posx; // edx MAPDST
  double health_; // st7
  short conyard_index; // bp
  unsigned char posy_; // ST30_1
  unsigned char posx_; // ST2C_1 MAPDST
  int sound_id; // eax
  int new_health; // edi
  int ypos; // ST1C_4 MAPDST
  int xpos; // ST18_4 MAPDST
  unsigned int random_spark_explosion; // eax
  int *units_lost_per_type_ptr; // eax
  int current_anim_type; // eax
  int current_anim_frame; // ecx
  unsigned char special_purpose_; // dl
  UnitFlags carried_unit_flags; // edx
  int *posz_ptr; // eax
  eUnitState new_state; // [esp-4h] [ebp-5Ch]
  short index; // [esp+14h] [ebp-44h]
  short a8; // [esp+16h] [ebp-42h]
  unsigned char x; // [esp+18h] [ebp-40h] MAPDST
  unsigned char y; // [esp+1Ch] [ebp-3Ch] MAPDST
  CSide *side; // [esp+20h] [ebp-38h] MAPDST
  unsigned char a1; // [esp+24h] [ebp-34h]
  Building *building; // [esp+28h] [ebp-30h] MAPDST
  int arg18; // [esp+2Ch] [ebp-2Ch]
  RECT rect; // [esp+30h] [ebp-28h]
  unsigned char xx; // [esp+40h] [ebp-18h]
  unsigned char yy; // [esp+44h] [ebp-14h] MAPDST
  int a6; // [esp+48h] [ebp-10h]
  int a5; // [esp+4Ch] [ebp-Ch]
  dwXYStruct point; // [esp+50h] [ebp-8h]

  if ( !unit )
  {
    DebugFatal("Unit.cpp", "Invalid unit, NULL");
  }
  if ( !unit->State )
  {
    DebugFatal("Unit.cpp", "Invalid unit state, esDummyState");
  }
  side_id_ = side_id;
  // Crush infantry
  unit_type_offset = unit->Type << 8;
  is_infantry = *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset);
  is_infantry = *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset);
  if ( is_infantry
    && gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags & TileFlags_8_OCC_UNIT )
  {
    my_index = myIndex;
    side_id_ = side_id;
    unit->Flags |= UFLAGS_1000000_DRAWN_BEHIND_OBJECTS;
    TryCrushUnit(unit, side_id_, my_index);
  }
  else
  {
    unit->Flags &= ~UFLAGS_1000000_DRAWN_BEHIND_OBJECTS;
  }
  // Animate infantry
  if ( is_infantry )
  {
    AnimateInfantryUnit(unit);
  }
  // Do self-healing
  if ( unit->Flags & UFLAGS_800000_SELFHEALING )
  {
    health = unit->Health;
    if ( 2 * health < _templates_unitattribs[unit->Type].__Strength && !(gGameTicks & 3) )
    {
      unit->Health = health + 1;
    }
  }
  // Handle saboteur cloaking
  side = GetSide((eSideType)side_id_);
  if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_SABOTEUR && !(gGameTicks & 0xF) )
  {
    unit_flags = unit->Flags;
    cloaking_points = unit->__SpecialPurpose;
    if ( !(unit_flags & UFLAGS_10_STEALTH) )
    {
      // New logic start
      // Customizable Saboteur cloaking capacity
      if ( cloaking_points >= (_templates_unitattribs[unit->Type].StorageCapacity?_templates_unitattribs[unit->Type].StorageCapacity:160u) )
      {
      // New logic end
        goto LABEL_27;
      }
      ++cloaking_points;
      goto LABEL_26;
    }
    if ( cloaking_points )
    {
      if ( cloaking_points <= 4u )
      {
        unit->__SpecialPurpose = 0;
        goto LABEL_27;
      }
      cloaking_points -= 4;
LABEL_26:
      unit->__SpecialPurpose = cloaking_points;
      goto LABEL_27;
    }
    if ( unit_flags & UFLAGS_4_CLOAKED )
    {
      unit->Flags = unit_flags & ~UFLAGS_10_STEALTH;
      UnitUncloakStart(unit);
    }
  }
LABEL_27:
  // Decrease reload delay
  // New logic start
  // Add support for bulk shots
  if ( unit->__ReloadDelayCounter )
  {
    unit->__ReloadDelayCounter--;
  }
  if ( unit->__ReloadDelayCounter == 0 )
  {
    unit->ShotsRemaining = 0;
    unit->LastUsedWeapon = 0;
  }
  // New logic end
  // Handle cloaking
  UnitHandleCloaking(unit, (eSideType)side_id_);
  // Reset attacker
  if ( unit->__AttackerIndex != -1 )
  {
    attacker_object_type = GetSide((eSideType)unit->__AttackerSide)->__ObjectArray[(unsigned short)unit->__AttackerIndex].ObjectType
                         - 1;
    if ( attacker_object_type )
    {
      if ( attacker_object_type != 1 )
      {
        attacker_side = GetSide((eSideType)unit->__AttackerSide);
        DebugFatal(
          "Unit.cpp",
          "Attacked by strange type (%d)",
          attacker_side->__ObjectArray[(unsigned short)unit->__AttackerIndex].ObjectType);
        goto LABEL_37;
      }
      Building *b = (Building *)&GetSide((eSideType)unit->__AttackerSide)->__ObjectArray[(unsigned short)unit->__AttackerIndex];
      if ( b->__State != UNIT_STATE_17_DEAD )
      {
        goto LABEL_37;
      }
    }
    else if ( GetSide((eSideType)unit->__AttackerSide)->__ObjectArray[(unsigned short)unit->__AttackerIndex].State != UNIT_STATE_17_DEAD )
    {
      goto LABEL_37;
    }
    unit->__AttackerIndex = -1;
  }
LABEL_37:
  // Reset enemy
  if ( _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_CARRYALL && unit->EnemyIndex != -1 )
  {
    enemy_object_type = GetSide((eSideType)unit->EnemySide)->__ObjectArray[(unsigned short)unit->EnemyIndex].ObjectType
                      - 1;
    if ( enemy_object_type )
    {
      Building *b = (Building *)&GetSide((eSideType)unit->EnemySide)->__ObjectArray[(unsigned short)unit->EnemyIndex];
      if ( enemy_object_type != 1
        || b->__State == UNIT_STATE_17_DEAD )
      {
LABEL_48:
        unit->EnemyIndex = -1;
        goto LABEL_49;
      }
    }
    else
    {
      enemy_unit = &GetSide((eSideType)unit->EnemySide)->__ObjectArray[(unsigned short)unit->EnemyIndex];
      if ( enemy_unit->State == UNIT_STATE_17_DEAD )
      {
        goto LABEL_48;
      }
      // New logic start
      // Fix units keeping firing on allied sandworm even after it was killed and put on sleep
      if ( enemy_unit->State == UNIT_STATE_29_SLEEPING )
      {
        goto LABEL_48;
      }
      // New logic end
      enemy_unit_flags = enemy_unit->Flags;
      if ( enemy_unit_flags & (UFLAGS_400000|UFLAGS_100_CARRYING|UFLAGS_4_CLOAKED) )
      {
        if ( _gDiplomacy[(unsigned char)side_id_][(unsigned char)unit->EnemySide] )
        {
          goto LABEL_48;
        }
      }
      if ( enemy_unit_flags & UFLAGS_100_CARRYING && !(enemy_unit_flags & UFLAGS_40_FLYING) )
      {
        goto LABEL_48;
      }
    }
  }
LABEL_49:
  if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_FRIGATE )
  {
    side = GetSide((eSideType)side_id_);
  }
  if ( (unsigned short)myIndex >= 1000u )
  {
    DebugFatal("UpdateUnit", "myIndex >= kMaxUnitsPerSide");
  }
  // Make smoke
  if ( unit->__MakeSmokeTimeCounter )
  {
    unit_type_offset = unit->Type << 8;
    if ( !*(&_templates_unitattribs[0].__IsInfantry + unit_type_offset) )
    {
      if ( unit->Health <= (signed int)((unsigned int)_templates_unitattribs[unit->Type].__Strength >> 1) )
      {
        if ( !(((_BYTE)gGameTicks + (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 3771) & 1)) & 3) )
        {
          posz = unit->__PosZHeight;
          posy = (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 3772) & 3)
               + unit->__PosY / 0x10000
               - 2;
          random_value_ = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 3772);
          ModelAddExplosion(
            (eSideType)side_id_,
            _templates_GroupIDs.EX_SMOKE3,
            (random_value_ & 7) + unit->__PosX / 0x10000 - 4,
            posy,
            posz,
            0,
            0,
            0,
            0);
        }
        if ( !(gGameTicks & 3) )
        {
          --unit->__MakeSmokeTimeCounter;
        }
      }
      else
      {
        unit->__MakeSmokeTimeCounter = 0;
      }
    }
  }
  enemy_index = (unsigned short)myIndex;
  side_id = myIndex & 0xFFFF;
  // Increase last moved
  myindex_and_7 = myIndex & 7;
  if ( myindex_and_7 == (int)(gGameTicks & 7) && !(unit->Flags & UFLAGS_40_FLYING) )
  {
    if ( unit->BlockFromX != unit->__PosX / 0x10000 / 32 || unit->BlockFromY != unit->__PosY / 0x10000 / 32 )
    {
      unit->LastMoved = 0;
    }
    else
    {
      last_moved = (unsigned char)unit->LastMoved + 1;
      if ( last_moved >= 255 )
      {
        last_moved = 255;
      }
      unit->LastMoved = last_moved;
    }
  }
  // Handle last attacked and revertion from deviated state
  unit_flags = unit->Flags;
  if ( unit_flags & UFLAGS_1_WAS_ATTACKED )
  {
    unit_flags = unit_flags & ~UFLAGS_1_WAS_ATTACKED;
    unit->LastAttacked = 0;
    unit->Flags = unit_flags;
    if ( !CanDeviatedUnitRevert(unit) )
    {
      goto LABEL_89;
    }
    deviated_time_counter = unit->__DeviatedTimeCounter;
    if ( deviated_time_counter < 30u )
    {
      unit->__DeviatedTimeCounter = 0;
      goto LABEL_89;
    }
    deviated_time_counter -= 30;
  }
  else
  {
    if ( myindex_and_7 != (int)(gGameTicks & 7) )
    {
      goto LABEL_89;
    }
    if ( unit_flags & UFLAGS_100_CARRYING )
    {
      goto LABEL_89;
    }
    last_attacked = (unsigned char)unit->LastAttacked + 1;
    if ( last_attacked > 255 )
    {
      last_attacked = 255;
    }
    unit->LastAttacked = last_attacked;
    if ( (unsigned char)last_attacked > 100u )
    {
      unit_flags = unit_flags & ~UFLAGS_8;
      unit->Flags = unit_flags;
    }
    unit_flags = unit->Flags;
    if ( !(unit_flags & UFLAGS_1000_DEVIATED) || !CanDeviatedUnitRevert(unit) )
    {
      goto LABEL_89;
    }
    v31 = unit->__DeviatedTimeCounter;
    if ( !v31 )
    {
      if ( !UnitTileOccupiedByBuilding(unit) )
      {
        unit_flags = unit->Flags;
        if ( !(unit_flags & UFLAGS_400000) )
        {
          unit_flags &= ~UFLAGS_1000_DEVIATED;
          unit_flags = unit_flags | UFLAGS_80;
          unit->Flags = unit_flags;
          old_side = unit->OldSide;
          if ( (_BYTE)side_id_ != old_side )
          {
            ChangeUnitOwner((eSideType)side_id_, old_side, enemy_index, 0);
          }
        }
      }
      goto LABEL_89;
    }
    deviated_time_counter = v31 - 1;
  }
  unit->__DeviatedTimeCounter = deviated_time_counter;
LABEL_89:
  // Handle last fired
  unit_flags = unit->Flags;
  if ( unit_flags & UFLAGS_2_FIRED )
  {
    unit_flags = unit_flags & ~UFLAGS_2_FIRED;
    unit->LastFired = 0;
    unit->Flags = unit_flags;
  }
  else if ( myindex_and_7 == (int)(gGameTicks & 7) )
  {
    last_fired = (unsigned char)unit->LastFired + 1;
    if ( last_fired > 255 )
    {
      last_fired = 255;
    }
    unit->LastFired = last_fired;
  }
  // Handle unit state
  v36 = unit->State;
  switch ( v36 )
  {
    case UNIT_STATE_2_IDLE:
      if ( unit->Flags & UFLAGS_BLOCKTOMARKED )
      {
        if ( unit->BlockFromX != unit->BlockToX || unit->BlockFromY != unit->BlockToY )
        {
          DebugFatal(
            "Unit.CPP",
            "Unit %d Side %d is waiting with 'To' and 'From' blocks different",
            side_id,
            (unsigned char)side_id_);
        }
      }
      unit_type_offset = unit->Type << 8;
      if ( *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset) )
      {
        SetUnitAnimationType(unit, 0);
      }
      if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_DEATH_HAND )
      {
        UnitAdjustState(unit, UNIT_STATE_7_MOVING);
      }
      behavior = _templates_unitattribs[unit->Type].__Behavior;
      if ( behavior == UnitBehavior_BIRD )
      {
        unit->TargetX = (unsigned int)GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 3892)
                      % gGameMap.width;
        unit->TargetY = (unsigned int)GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 3893)
                      % gGameMap.height;
        UnitAdjustState(unit, UNIT_STATE_7_MOVING);
      }
      else if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_CARRYALL
             || _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_FRIGATE )
      {
        unit_flags = unit->Flags;
        if ( unit_flags & UFLAGS_800_DELIVERING )
        {
          if ( unit_flags & UFLAGS_100_CARRYING && !(unit_flags & UFLAGS_4000) )
          {
            if ( unit->BlockToX == unit->TargetX && unit->BlockToY == unit->TargetY )
            {
              UnitAdjustState(unit, UNIT_STATE_21_FLYING_DOWN);
              goto LABEL_612;
            }
            goto LABEL_285;
          }
          if ( !(unit_flags & UFLAGS_200) )
          {
            v40 = unit->BlockToY;
            unit_type_offset = unit->BlockToX;
            unit->TargetX = unit_type_offset;
            unit_type_offset = (unsigned char)unit_type_offset;
            unit_flags = (unsigned char)unit_type_offset;
            unit->TargetY = v40;
            unit_flags = v40;
            arg18 = unit_type_offset;
            rect.left = unit_flags;
            GetFlyingUnitSpawnPositionAndFacing((_WORD *)&arg18, (_WORD *)&rect, 0);
            v41 = rect.left;
            unit->TargetX = arg18;
            unit->TargetY = v41;
            UnitAdjustState(unit, UNIT_STATE_7_MOVING);
          }
        }
        else if ( unit_flags & UFLAGS_100_CARRYING )
        {
          v42 = (CSide *)((char *)side + 20 * (char)unit->__SpecialPurpose);
          if ( unit->BlockToX != v42->__PickupQueue[0].__to_x || unit->BlockToY != v42->__PickupQueue[0].__to_y )
          {
            unit_flags &= ~UFLAGS_200;
            unit->Flags = unit_flags;
            unit->TargetX = v42->__PickupQueue[0].__to_x;
            unit->TargetY = v42->__PickupQueue[0].__to_y;
            UnitAdjustState(unit, UNIT_STATE_7_MOVING);
          }
          else
          {
            UnitAdjustState(unit, UNIT_STATE_21_FLYING_DOWN);
          }
        }
        else if ( unit_flags & UFLAGS_200 )
        {
          UnitAdjustState(unit, UNIT_STATE_21_FLYING_DOWN);
        }
        else
        {
          v43 = CSide__46CE00(side, (int)unit);
          if ( v43 == -1 )
          {
            if ( myindex_and_7 == (int)(gGameTicks & 7) )
            {
              v44 = (unsigned char)unit->__FacingTurret + 1;
              unit->__FacingTurret = ((v44 >> 32) ^ (abs(unit->__FacingTurret + 1) & 0x1F)) - (v44 >> 32);
            }
            Unit_movement_bullet_494FD0(unit, side_id_, myIndex, 1);
          }
          else
          {
            v45 = (CSide *)((char *)side + 20 * v43);
            unit->TargetX = side->__PickupQueue[v43].__from_x;
            unit->TargetY = v45->__PickupQueue[0].__from_y;
            unit->EnemyIndex = v45->__PickupQueue[0].__unit_index;
            unit->EnemySide = v45->__PickupQueue[0].__unit_state;
            unit->__SpecialPurpose = v43;
            v45->__PickupQueue[0].__state = 2;
            v45->__PickupQueue[0].__carryall_index = myIndex;
            UnitAdjustState(unit, UNIT_STATE_7_MOVING);
            v47 = unit->Flags;
            v47 |= UFLAGS_200;
            unit->Flags = v47;
          }
        }
      }
      if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER )
      {
        if ( gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE) )
        {
          UnitAdjustState(unit, UNIT_STATE_12_HARVESTING);
        }
      }
      else
      {
        rect.left = gGameTicks & 7;
        if ( ((((signed int)side_id >> 31) ^ (abs(side_id) & 7)) - ((signed int)side_id >> 31) == rect.left
           || unit->EnemyIndex != -1)
          && !UnitIsCloaked(unit) )
        {
          can_attack = UnitAttack(
                         unit,
                         (eSideType)side_id_,
                         myIndex,
                         (eSideType *)&unit->EnemySide,
                         (unsigned short *)&unit->EnemyIndex);
          goto LABEL_220;
        }
      }
      goto LABEL_612;
    case UNIT_STATE_3_GUARDING:
      v36 = unit->EnemyIndex;
      arg18 = v36;
      if ( (_WORD)v36 != -1 && !unit->pos_steps )
      {
        enemy_side_id = unit->EnemySide;
        enemy_index = unit->EnemyIndex;
        arg18 = enemy_index;
        enemy_side = GetSide((eSideType)enemy_side_id);
        enemy_unit = &enemy_side->__ObjectArray[(unsigned short)enemy_index];
        enemy_object_type = enemy_side->__ObjectArray[(unsigned short)enemy_index].ObjectType;
        if ( enemy_object_type == 1 )
        {
          v96 = enemy_unit->__PosX;
          v97 = (unit->__PosX - v96) / 32;
          v98 = unit->__PosY - enemy_unit->__PosY;
          unsigned int p = enemy_unit->__PosY;
          if ( (unsigned int)(v98 / 32 / 0x10000 * (v98 / 32 / 0x10000) + v97 / 0x10000 * (v97 / 0x10000)) >= 0x24
            || _templates_unitattribs[enemy_unit->Type].__Behavior == UnitBehavior_SANDWORM )
          {
            unit->EnemyIndex = -1;
            unit->EnemySide = -1;
          }
          else if ( enemy_unit->Flags & (UFLAGS_100_CARRYING|UFLAGS_40_FLYING) )
          {
            // New logic start
            // Add support for bulk shots
            if ( CanUnitShoot(unit) )
            // New logic end
            {
              UnitShootTarget(
                unit,
                (eSideType)side_id_,
                myIndex,
                v96 / 0x10000,
                p / 0x10000,
                (eSideType)unit->EnemySide,
                arg18,
                1);
            }
          }
          else
          {
            UnitAttackUnit(unit, (eSideType)side_id_, myIndex);
          }
        }
        else if ( enemy_object_type == 2 )
        {
          UnitAttackBuilding(unit, (eSideType)side_id_, myIndex);
        }
        goto LABEL_612;
      }
      remember_pos_x = unit->__RememberPosX;
      remember_pos_y = unit->__RememberPosY;
      enemy_index = remember_pos_x;
      myindex_and_7 = remember_pos_y;
      v101 = 32 * enemy_index + 16;
      v102 = 32 * myindex_and_7 + 16;
      if ( unit->pos_steps )
      {
        unit->TargetX = remember_pos_x;
        v103 = myIndex;
        unit->TargetY = remember_pos_y;
        MoveUnit(unit, (eSideType)side_id_, v103);
        goto LABEL_612;
      }
      if ( (((signed int)side_id >> 31) ^ (abs(side_id) & 7)) - ((signed int)side_id >> 31) != (int)(gGameTicks & 7)
        && (_WORD)arg18 == -1 )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
        goto LABEL_612;
      }
      if ( UnitAttack(
             unit,
             (eSideType)side_id_,
             myIndex,
             (eSideType *)&unit->EnemySide,
             (unsigned short *)&unit->EnemyIndex) )
      {
        goto LABEL_594;
      }
      if ( EnemyUnitInRange(
             (unsigned short)v101,
             (unsigned short)v102,
             (eSideType)side_id_,
             // New logic start
             // Guard mode radius can be customized by map rule guardModeRadius
             rulesExt__guardModeRadius,
             // New logic end
             &a5,
             &a6,
             &v3,
             &a8,
             _templates_unitattribs[unit->Type].__PrimaryWeapon,
             _templates_unitattribs[unit->Type].__SecondaryWeapon,
             0,
             0,
             1) )
      {
        v104 = (unsigned char)v3 < 8u;
        is_low_money = (_BYTE)v3 == 8;
        unit->EnemySide = (char)v3;
        if ( !v104 && !is_low_money )
        {
          DebugFatal("Model.cpp", "EnemySide > kMaxSides");
        }
        v105 = myIndex;
        unit->EnemyIndex = a8;
        UnitAttackUnit(unit, (eSideType)side_id_, v105);
      }
      else
      {
        v106 = unit->__RememberPosY;
        v107 = myIndex;
        unit->TargetX = unit->__RememberPosX;
        unit->TargetY = v106;
        unit->__RouteSteps = 0;
        if ( !MoveUnit(unit, (eSideType)side_id_, v107) )
        {
          if ( _templates_unitattribs[unit->Type].__IsInfantry )
          {
            SetUnitAnimationType(unit, 0);
          }
          if ( !(unit->Flags & UFLAGS_10_STEALTH) )
          {
            can_attack = UnitAttack(
                           unit,
                           (eSideType)side_id_,
                           myIndex,
                           (eSideType *)&unit->EnemySide,
                           (unsigned short *)&unit->EnemyIndex);
LABEL_220:
            if ( !can_attack )
            {
              Unit_movement_494210(unit);
            }
          }
        }
      }
      goto LABEL_612;
    case UNIT_STATE_4_ATTACKING_UNIT:
      if ( unit->pos_steps )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
        goto LABEL_612;
      }
      can_attack = UnitAttackUnit(unit, (eSideType)side_id_, myIndex);
      goto LABEL_228;
    case UNIT_STATE_5_ATTACKING_BUILDING:
      if ( unit->pos_steps )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
      }
      else
      {
        can_attack = UnitAttackBuilding(unit, (eSideType)side_id_, myIndex);
LABEL_228:
        if ( !can_attack )
        {
          UnitAdjustState(unit, UNIT_STATE_7_MOVING);
          unit->OldState = 1;
          unit->EnemyIndex = -1;
        }
      }
      goto LABEL_612;
    case UNIT_STATE_6_ATTACKING_TILE:
      v109 = UnitAttackTile(unit, (eSideType)side_id_, myIndex);
      goto LABEL_246;
    case UNIT_STATE_7_MOVING:
      unit_flags = unit->Flags;
      if ( unit_flags & UFLAGS_20000_DEATHHAND )
      {
        if ( !HandleDeathHand(unit, (eSideType)side_id_, myIndex) )
        {
          UnitAdjustState(unit, UNIT_STATE_17_DEAD);
        }
        goto LABEL_612;
      }
      if ( unit_flags & UFLAGS_40_FLYING )
      {
        if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_FRIGATE )
        {
          if ( unit_flags & UFLAGS_100_CARRYING )
          {
            if ( side->__PrimaryStarport != -1 )
            {
              building_flags = (unsigned int *)&side->__ObjectArray[(unsigned short)side->__PrimaryStarport].__GroupID;
              *building_flags |= BFLAGS_20_ANIM_ONCE;
            }
          }
        }
        unit_flags = unit->Flags;
        if ( !(unit_flags & UFLAGS_200)
          || (side_id = unit->__SpecialPurpose,
              side_ = GetSide((eSideType)side_id_),
              side_->__PickupQueue[(int)side_id].__state != 3)
          || (unit_flags = unit->Flags, unit_flags & UFLAGS_100_CARRYING) )
        {
          v109 = Unit_movement_bullet_494FD0(unit, side_id_, myIndex, 0);
LABEL_246:
          if ( !v109 )
          {
            UnitAdjustState(unit, UNIT_STATE_2_IDLE);
          }
        }
        else
        {
          unit_flags &= ~UFLAGS_200;
          unit->Flags = unit_flags;
          UnitAdjustState(unit, UNIT_STATE_2_IDLE);
        }
        goto LABEL_612;
      }
      if ( MoveUnit(unit, (eSideType)side_id_, myIndex) )
      {
        goto LABEL_612;
      }
      old_state = unit->OldState;
      if ( old_state != UNIT_STATE_1 && old_state != UNIT_STATE_7_MOVING && old_state != UNIT_STATE_18_AWAITING_AIRLIFT )
      {
        UnitAdjustState(unit, (eUnitState)unit->OldState);
        goto LABEL_612;
      }
      goto LABEL_345;
    case UNIT_STATE_8_LEAVING_BUILDING:
      if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_CARRYALL )
      {
        UnitAdjustState(unit, UNIT_STATE_2_IDLE);
        goto LABEL_612;
      }
      v117 = unit->TargetX;
      point.Y = unit->TargetY;
      target_y = unit->TargetY;
      target_x = unit->TargetX;
      point.X = v117;
      unit_on_target_tile = GetUnitOnTile(32 * target_x, 32 * target_y, (eSideType *)&a1, &index, 0);
      if ( !unit_on_target_tile )
      {
        goto LABEL_270;
      }
      if ( (_BYTE)side_id_ == a1 )
      {
        if ( _templates_unitattribs[unit_on_target_tile->Type].__IsInfantry )
        {
          unit_type_offset = unit->Type << 8;
          if ( *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset) )
          {
            if ( FindFreeSpotForInfantry(gGameMap.map[unit->TargetX + _CellNumbersWidthSpan[unit->TargetY]].__tile_bitflags)
              || (unit->BlockToX == unit->TargetX && unit->BlockToY == unit->TargetY) )
            {
LABEL_265:
              unit_on_target_tile = 0;
              goto LABEL_266;
            }
          }
        }
      }
      else if ( _gDiplomacy[(unsigned char)side_id_][a1] && CanUnitUseSquare(point, unit, (eSideType)side_id_, 0) )
      {
        goto LABEL_265;
      }
LABEL_266:
      if ( unit_on_target_tile && unit_on_target_tile != unit )
      {
        v122 = a1;
        if ( !_gDiplomacy[(unsigned char)side_id_][a1] )
        {
          unit_type_offset = unit->TargetY;
          v122 = unit->TargetX;
          KickUnitsOutOfTile(unit, (eSideType)side_id_, v122, unit_type_offset);
        }
      }
      else
      {
LABEL_270:
        if ( !MoveUnitSimple(unit, side_id_, 1, 1) )
        {
          if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER )
          {
            UnitAdjustState(unit, UNIT_STATE_12_HARVESTING);
          }
          else
          {
            UnitAdjustState(unit, UNIT_STATE_2_IDLE);
          }
        }
      }
      if ( unit->pos_steps || UnitTileOccupiedByBuilding(unit) )
      {
        goto LABEL_612;
      }
      if ( _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_HARVESTER )
      {
        goto LABEL_345;
      }
      UnitAdjustState(unit, UNIT_STATE_12_HARVESTING);
      goto LABEL_612;
    case UNIT_STATE_9_MOVING_AWAY:
      if ( unit->pos_steps )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
        goto LABEL_612;
      }
      v123 = unit->BlockFromY;
      rect.left = unit->BlockFromX;
      rect.top = v123;
      if ( GetFreeAdjacentTile(rect.left, v123, unit, side_id_, &point, 0) )
      {
        v124 = unit->OldState;
        v125 = point.X;
        v126 = point.Y;
        unit->TargetX = point.X;
        unit->TargetY = v126;
        if ( v124 == UNIT_STATE_3_GUARDING )
        {
          unit->__RememberPosX = v125;
          unit->__RememberPosY = v126;
        }
        if ( MoveUnit(unit, (eSideType)side_id_, myIndex) )
        {
          if ( unit->Flags & UFLAGS_BLOCKTOMARKED )
          {
LABEL_285:
            UnitAdjustState(unit, UNIT_STATE_7_MOVING);
          }
          goto LABEL_612;
        }
      }
      else
      {
        myIndex = 0;
        v127 = 0;
        do
        {
          point = GetNextSquareInDirection(rect.left, rect.top, v127);
          unit_on_tile = GetUnitOnTile(32 * point.X, 32 * point.Y, (eSideType *)&a1, &index, 0);
          if ( unit_on_tile )
          {
            if ( !_gDiplomacy[(unsigned char)side_id_][a1] && CanUnitAcceptOrders(unit_on_tile, 0) )
            {
              unit_state = unit_on_tile->State;
              if ( unit_state != UNIT_STATE_7_MOVING
                && unit_state != UNIT_STATE_26_LEAVING_REPAIR_PAD
                && unit_state != UNIT_STATE_11_MOVING_TO_HARVEST
                && unit_state != UNIT_STATE_9_MOVING_AWAY )
              {
                UnitAdjustState(unit_on_tile, UNIT_STATE_9_MOVING_AWAY);
                myIndex = 1;
              }
            }
          }
          v127 += 4;
        }
        while ( v127 < 0x20 );
        if ( (_BYTE)myIndex )
        {
          goto LABEL_612;
        }
      }
      v130 = unit->OldState;
      if ( v130 != 1 && v130 != 18 )
      {
        UnitAdjustState(unit, (eUnitState)unit->OldState);
        goto LABEL_612;
      }
      goto LABEL_345;
    case UNIT_STATE_10_ATTACKIG_TILE_ON_ROUTE:
      if ( unit->pos_steps )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
      }
      else
      {
        v131 = gGameMap.map[unit->TargetX + _CellNumbersWidthSpan[unit->TargetY]].__tile_bitflags;
        if ( v131 & 0x3F8 && (v131 & 7) != (_BYTE)side_id_ )
        {
          UnitAttackTile(unit, (eSideType)side_id_, myIndex);
        }
        else
        {
          v132 = unit->__RememberPosY;
          v133 = unit->OldState;
          unit->TargetX = unit->__RememberPosX;
          unit->TargetY = v132;
          unit->RouteMode = 0;
          UnitAdjustState(unit, v133);
        }
      }
      goto LABEL_612;
    case UNIT_STATE_11_MOVING_TO_HARVEST:
      if ( !MoveUnit(unit, (eSideType)side_id_, myIndex) )
      {
        UnitAdjustState(unit, UNIT_STATE_12_HARVESTING);
        goto LABEL_612;
      }
      distance_x = unit->BlockFromX - unit->TargetX;
      goto LABEL_368;
    case UNIT_STATE_12_HARVESTING:
      v135 = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
      if ( v135 )
      {
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = v135 - 1;
        goto LABEL_612;
      }
      // New logic start
      // Customizable Harvester storage capacity
      if ( unit->__SpecialPurpose >= (_templates_unitattribs[unit->Type].StorageCapacity?_templates_unitattribs[unit->Type].StorageCapacity:7u) )
      // New logic end
      {
        UnitAdjustState(unit, UNIT_STATE_13_MOVING_TO_REFINERY);
        goto LABEL_612;
      }
      v136 = unit->BlockFromY;
      x = unit->BlockFromX;
      y = v136;
      if ( spice_44D970(&x, &y) )
      {
        v137.Y = y;
        v137.X = x;
        point.X = x;
        point.Y = y;
        if ( CanUnitUseSquare(v137, unit, (eSideType)side_id_, 0) )
        {
          v138 = x;
          v139 = y;
          unit->TargetX = x;
          unit->TargetY = v139;
          unit->__RememberPosX = v138;
          unit->__RememberPosY = v139;
          UnitAdjustState(unit, UNIT_STATE_11_MOVING_TO_HARVEST);
          goto LABEL_612;
        }
        unit_on_tile = GetUnitOnTile(32 * x, 32 * y, (eSideType *)&a1, &index, 0);
        if ( unit_on_tile && _templates_unitattribs[unit_on_tile->Type].__Behavior != UnitBehavior_HARVESTER )
        {
          v141 = x;
          v142 = y;
          unit->TargetX = x;
          unit->TargetY = v142;
          unit->__RememberPosX = v141;
          unit->__RememberPosY = v142;
          UnitAdjustState(unit, UNIT_STATE_11_MOVING_TO_HARVEST);
          goto LABEL_612;
        }
LABEL_404:
        UnitAdjustState(unit, UNIT_STATE_11_MOVING_TO_HARVEST);
        goto LABEL_612;
      }
      v143 = y;
      unit->__RememberPosX = x;
      v144 = unit->BlockFromY;
      v145 = unit->BlockFromX;
      unit->__RememberPosY = v143;
      v146 = v145 + _CellNumbersWidthSpan[v144];
      v147 = gGameMap.map[v146].__tile_bitflags;
      v148 = gGameMap.map[v146].__tile_bitflags & (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
      if ( v148 )
      {
        if ( v148 <= TileFlags_200000_SPICE )
        {
          if ( v148 == TileFlags_200000_SPICE )
          {
            gGameMap.map[v146].__tile_bitflags = v147 & ~(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
            gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags |= TileFlags_100000_SPICE;
            gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_index = 793;
          }
          else if ( v148 == TileFlags_100000_SPICE )
          {
            gGameMap.map[v146].__tile_bitflags = v147 & ~(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
            // New logic start
            // Restore back-up tile index when all spice on tile is harvested
            gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_index = gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].back_up_tile_index;
            // New logic end
            if ( gBitsPerPixel == 16 )
            {
              SetPixelOnRadar16(unit->BlockFromX, unit->BlockFromY, _radarcolor16_sand);
            }
            else
            {
              SetPixelOnRadar8(unit->BlockFromX, unit->BlockFromY, _radarcolor8_sand);
            }
          }
          goto LABEL_339;
        }
        if ( (unsigned int)v148 > TileFlags_400000_SPICE )
        {
          if ( v148 != (TileFlags_400000_SPICE|TileFlags_100000_SPICE) )
          {
            if ( v148 != (TileFlags_400000_SPICE|TileFlags_200000_SPICE)
              && v148 != (TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE) )
            {
              goto LABEL_339;
            }
            gGameMap.map[v146].__tile_bitflags = v147 & ~(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
            v152 = unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY];
            gGameMap.map[v152].__tile_bitflags |= TileFlags_200000_SPICE;
            goto LABEL_338;
          }
          gGameMap.map[v146].__tile_bitflags = v147 & ~(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
          v153 = unit->BlockFromX;
          cell_index = v153 + _CellNumbersWidthSpan[unit->BlockFromY];
          v151 = gGameMap.map[v153 + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags | TileFlags_400000_SPICE;
        }
        else
        {
          if ( v148 != TileFlags_400000_SPICE )
          {
            if ( v148 == (TileFlags_200000_SPICE|TileFlags_100000_SPICE) )
            {
              // New logic start
              // Infinite spice
              if (!rulesExt__InfiniteSpice)
                gGameMap.map[v146].__tile_bitflags = v147 & ~(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
              // New logic end
              gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags |= TileFlags_200000_SPICE;
              gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_index = 793;
            }
            goto LABEL_339;
          }
          gGameMap.map[v146].__tile_bitflags = v147 & ~(TileFlags_400000_SPICE|TileFlags_200000_SPICE|TileFlags_100000_SPICE);
          v149 = unit->BlockFromX;
          cell_index = v149 + _CellNumbersWidthSpan[unit->BlockFromY];
          v151 = gGameMap.map[v149 + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags | TileFlags_200000_SPICE|TileFlags_100000_SPICE;
        }
        gGameMap.map[cell_index].__tile_bitflags = v151;
LABEL_338:
        gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_index = 749;
LABEL_339:
        v154 = unit->BlockFromX;
        rect.left = v154 - 1;
        v155 = unit->BlockFromY;
        rect.right = v154 + 2;
        rect.top = v155 - 1;
        rect.bottom = v155 + 2;
        UpdateSpiceInRegion(&rect);
        v156 = unit->__Facing;
        v157 = unit->__SpecialPurpose + 1;
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.harvestLoadSpiceDelay;
        unit->__SpecialPurpose = v157;
        v158 = unit->__PosX;
        myIndex = v156 >> 2;
        v159 = v158;
        v160 = *(&_templates_GroupIDs.EX_HARVDUST0 + (v156 >> 2));
        if ( v160 < 0x40u )
        {
          ModelAddExplosion((eSideType)side_id_, v160, v159 / 0x10000, unit->__PosY / 0x10000, 0, 0, 0, 0, 0);
        }
        goto LABEL_612;
      }
      if ( GetSpiceTileToHarvest(unit, (eSideType)side_id_, &x, &y) )
      {
        unit->TargetX = x;
        unit->TargetY = y;
        unit->__RememberPosX = x;
        unit->__RememberPosY = y;
        UnitAdjustState(unit, UNIT_STATE_11_MOVING_TO_HARVEST);
        goto LABEL_612;
      }
      if ( unit->__SpecialPurpose )
      {
        UnitAdjustState(unit, UNIT_STATE_13_MOVING_TO_REFINERY);
        goto LABEL_612;
      }
LABEL_345:
      UnitAdjustState(unit, UNIT_STATE_2_IDLE);
      goto LABEL_612;
    case UNIT_STATE_13_MOVING_TO_REFINERY:
      building = GetBuilding((eSideType)side_id_, unit->RefineryIndex);
      if ( building && _templates_buildattribs[building->Type].__Behavior == BuildingBehavior_REFINERY )
      {
        x = building->__PosX / 0x10000 / 32;
        v164 = (building->__PosY / 0x10000 - _templates_buildattribs[building->Type]._____ArtHeight) / 32;
        y = v164;
        x += _templates_buildattribs[building->Type]._____ExitPoint1X;
        v164 = _templates_buildattribs[building->Type]._____ExitPoint1Y + v164;
        unit->TargetX = x;
        y = v164;
        unit->TargetY = v164;
        MoveUnit(unit, (eSideType)side_id_, myIndex);
        if ( (unit->BlockFromX == x && unit->BlockFromY == y)
          || (unit->Flags & UFLAGS_BLOCKTOMARKED && unit->BlockToX == x && unit->BlockToY == y) )
        {
          unit->__CurrentAnimFrame = 0;
          UnitAdjustState(unit, UNIT_STATE_14_ENTERING_REFINERY);
        }
        unit_on_tile = GetUnitOnTile(32 * x, 32 * y, (eSideType *)&a1, &index, 0);
        if ( unit_on_tile )
        {
          if ( CanUnitAcceptOrders(unit_on_tile, 0) )
          {
            unit_state = unit_on_tile->State;
            if ( (unit_state == UNIT_STATE_2_IDLE || unit_state == UNIT_STATE_3_GUARDING)
              && !_gDiplomacy[(unsigned char)side_id_][a1] )
            {
              UnitAdjustState(unit_on_tile, UNIT_STATE_9_MOVING_AWAY);
            }
          }
        }
      }
      else
      {
        building = GetNearestBuildingWithBehavior(
                     unit->BlockFromX,
                     unit->BlockFromY,
                     (eSideType)side_id_,
                     BuildingBehavior_REFINERY,
                     &x,
                     &y);
        if ( building )
        {
          unit->TargetX = x;
          unit->TargetY = y;
          v170 = myIndex;
          unit->RefineryIndex = building->__MyIndex;
          if ( MoveUnit(unit, (eSideType)side_id_, v170) )
          {
            goto LABEL_367;
          }
          unit->__CurrentAnimFrame = 0;
          new_state = UNIT_STATE_14_ENTERING_REFINERY;
        }
        else
        {
          if ( unit->pos_steps )
          {
            MoveUnit(unit, (eSideType)side_id_, myIndex);
            goto LABEL_367;
          }
          new_state = UNIT_STATE_2_IDLE;
        }
        UnitAdjustState(unit, new_state);
      }
LABEL_367:
      distance_x = unit->BlockFromX - unit->TargetX;
LABEL_368:
      distance_y = unit->BlockFromY - unit->TargetY;
      if ( distance_y * distance_y + distance_x * distance_x > _gVariables.HarvesterDriveDistance
        && !unit->pos_steps
        && !(unit->Flags & UFLAGS_BLOCKTOMARKED) )
      {
        goto LABEL_492;
      }
      goto LABEL_612;
    case UNIT_STATE_14_ENTERING_REFINERY:
      if ( unit->pos_steps )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
        goto LABEL_612;
      }
      if ( !unit->__SpecialPurpose )
      {
        UnitAdjustState(unit, UNIT_STATE_16_LEAVING_REFINERY);
        goto LABEL_612;
      }
      building = GetBuilding((eSideType)side_id_, unit->RefineryIndex);
      if ( !building )
      {
        goto LABEL_396;
      }
      if ( _templates_buildattribs[building->Type].__Behavior != 4 )
      {
        goto LABEL_396;
      }
      x = building->__PosX / 0x10000 / 32;
      v173 = (building->__PosY / 0x10000 - _templates_buildattribs[building->Type]._____ArtHeight) / 32;
      y = v173;
      x += _templates_buildattribs[building->Type]._____ExitPoint1X;
      v174 = _templates_buildattribs[building->Type]._____ExitPoint1Y;
      v175 = unit->BlockFromX;
      v176 = v174 + v173;
      y = v176;
      if ( v175 != x || unit->BlockFromY != v176 )
      {
        goto LABEL_396;
      }
      if ( !TurnUnitInDirection(unit, 0xCu) )
      {
        v177 = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
        if ( v177 )
        {
          unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = v177 - 1;
        }
        else
        {
          v178 = unit->__CurrentAnimFrame;
          unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 5;
          unit->__CurrentAnimFrame = ++v178;
          if ( v178 >= _templates_AnimationArtFrames[(unsigned char)_templates_GroupIDs.EX_HARV] - 1 )
          {
            UnitAdjustState(unit, UNIT_STATE_15_UNLOADING_SPICE);
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.harvestUnloadDelay;
          }
        }
      }
      goto LABEL_612;
    case UNIT_STATE_15_UNLOADING_SPICE:
      v179 = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
      if ( v179 )
      {
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = v179 - 1;
      }
      else
      {
        building = GetBuilding((eSideType)side_id_, unit->RefineryIndex);
        if ( !building )
        {
          goto LABEL_396;
        }
        if ( _templates_buildattribs[building->Type].__Behavior != 4 )
        {
          goto LABEL_396;
        }
        x = building->__PosX / 0x10000 / 32;
        v181 = (building->__PosY / 0x10000 - _templates_buildattribs[building->Type]._____ArtHeight) / 32;
        y = v181;
        x += _templates_buildattribs[building->Type]._____ExitPoint1X;
        v182 = _templates_buildattribs[building->Type]._____ExitPoint1Y + v181;
        v183 = unit->BlockFromX;
        y = v182;
        if ( v183 != x )
        {
          goto LABEL_396;
        }
        if ( unit->BlockFromY == v182
          && (unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.harvestUnloadDelay,
              unit->__SpecialPurpose) )
        {
          v184 = _gVariables.harvestBlobValue;
          side_ = GetSide((eSideType)side_id_);
          CSide__AddSpice(side_, v184);
          side_ = GetSide((eSideType)side_id_);
          side_->__SpiceHarvested += _gVariables.harvestBlobValue;
          --unit->__SpecialPurpose;
          if ( _templates_GroupIDs.EX_REF != -1 )
          {
            if ( unit->RefineryIndex == -1 )
            {
              DebugFatal("Unit.cpp", "refineryIndex out of range (1)");
            }
            side_ = GetSide((eSideType)side_id_);
            ModelAddExplosion(
              (eSideType)side_id_,
              _templates_GroupIDs.EX_REF,
              LOWORD(_gVariables.refineryExplosionOffsetX)
            + side_->__ObjectArray[(unsigned short)unit->RefineryIndex].Speed / 0x10000,
              LOWORD(_gVariables.refineryExplosionOffsetY)
            + side_->__ObjectArray[(unsigned short)unit->RefineryIndex].Health / 0x10000,
              0,
              0,
              0,
              0,
              0);
          }
        }
        else
        {
LABEL_396:
          UnitAdjustState(unit, UNIT_STATE_16_LEAVING_REFINERY);
        }
      }
      goto LABEL_612;
    case UNIT_STATE_16_LEAVING_REFINERY:
      v188 = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
      if ( v188 )
      {
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = v188 - 1;
        goto LABEL_612;
      }
      v189 = unit->__CurrentAnimFrame;
      unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 5;
      if ( v189 )
      {
        unit->__CurrentAnimFrame = v189 - 1;
        goto LABEL_612;
      }
      if ( GetSpiceTileToHarvest(unit, (eSideType)side_id_, &x, &y) )
      {
        unit->TargetX = x;
        unit->TargetY = y;
        UnitAdjustState(unit, UNIT_STATE_11_MOVING_TO_HARVEST);
        goto LABEL_612;
      }
      tile_driveon_49E290((eSideType)side_id_, &unit->TargetX, &unit->TargetY);
      goto LABEL_404;
    case UNIT_STATE_17_DEAD:
      if ( unit->__DeadStateTimeCounter == 46 && !strcmp(_templates_UnitNameList[unit->Type], "GRENADIER") )
      {
        // New logic start
        // Fix misplaced grenadier death damage
        DamageTiles(unit->__PosX / 0x10000, unit->__PosY / 0x10000, 0, 0x21u, (eSideType)side_id_, 0xFFFF, 0);
        // New logic end
      }
      v191 = unit->__DeadStateTimeCounter;
      if ( v191 )
      {
        unit->__DeadStateTimeCounter = v191 - 1;
        unit_flags = unit->Flags;
        if ( unit_flags & UFLAGS_40_FLYING )
        {
          if ( unit_flags & UFLAGS_20000_DEATHHAND )
          {
            HandleDeathHand(unit, (eSideType)side_id_, myIndex);
          }
          else
          {
            Unit_movement_bullet_494FD0(unit, side_id_, myIndex, 0);
          }
        }
        goto LABEL_612;
      }
      if ( unit->Type >= 60u )
      {
        DebugFatal("UpdateUnit", "Invalid unitType %d", unit->Type);
      }
      if ( unit->Flags & UFLAGS_40_FLYING )
      {
        UnmarkTileWithFlyingUnit(unit->BlockFromX, unit->BlockFromY);
      }
      if ( !(unit->Flags & (UFLAGS_400000|UFLAGS_40_FLYING)) )
      {
        if ( is_infantry )
        {
          // New logic start
          // When an infantry is killed, the occupied-spot tile attribute is reset.
          // If a tile underneath has pre-set those attributes (i.e. repulsion field), this patch makes sure
          // the attribute will be kept and repulsion field won't get broken.
          int and_flags = ~(32 << unit->__CellspotFrom) | _TileBitflags[gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].back_up_tile_index];
          gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags &= and_flags;
          if ( _templates_unitattribs[unit->Type].__CanCrush )
            gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].num_uncrushable_infantry--;
          if ( unit->Flags & UFLAGS_BLOCKTOMARKED )
          {
            and_flags = ~(32 << unit->__CellspotTo) | _TileBitflags[gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].back_up_tile_index];
            gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].__tile_bitflags &= and_flags;
            if ( _templates_unitattribs[unit->Type].__CanCrush )
              gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].num_uncrushable_infantry--;
            // New logic end
            return 0;
          }
        }
        else
        {
          v193 = unit->BlockFromY;
          v194 = unit->BlockFromX;
          if ( !(gGameMap.map[v194 + _CellNumbersWidthSpan[v193]].__tile_bitflags & TileFlags_8_OCC_UNIT) )
          {
            DebugFatal(
              "UpdateUnit",
              "Error with unit code. BlockFrom (%d,%d) %x",
              v194,
              v193,
              gGameMap.map[v194 + _CellNumbersWidthSpan[v193]].__tile_bitflags);
          }
          gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags &= ~TileFlags_8_OCC_UNIT;
          tile_flags = gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags;
          if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
          {
            tile_flags = tile_flags & ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
            gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags = tile_flags;
            gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags |= (gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags >> 25) & 7;
          }
          if ( unit->Flags & UFLAGS_BLOCKTOMARKED )
          {
            v196 = unit->BlockToY;
            v197 = unit->BlockToX;
            if ( !(gGameMap.map[v197 + _CellNumbersWidthSpan[v196]].__tile_bitflags & TileFlags_8_OCC_UNIT) )
            {
              DebugFatal(
                "UpdateUnit",
                "Error with unit code. BlockTo (%d,%d) %x",
                v197,
                v196,
                gGameMap.map[v197 + _CellNumbersWidthSpan[v196]].__tile_bitflags);
            }
            gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].__tile_bitflags &= ~TileFlags_8_OCC_UNIT;
            tile_flags = gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].__tile_bitflags;
            if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
            {
              tile_flags = tile_flags & ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
              gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].__tile_bitflags = tile_flags;
              gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].__tile_bitflags |= (gGameMap.map[unit->BlockToX + _CellNumbersWidthSpan[unit->BlockToY]].__tile_bitflags >> 25) & 7;
              return 0;
            }
          }
        }
      }
      return 0;
    case UNIT_STATE_18_AWAITING_AIRLIFT:
      side_ = GetSide((eSideType)side_id_);
      v201 = (unsigned char)side_->__PickupQueueCount;
      v202 = 0;
      if ( side_->__PickupQueueCount )
      {
        v203 = (int)&side_->__PickupQueue[0].__state;
        v204 = v201;
        do
        {
          if ( *(_WORD *)(v203 + 1) == myIndex )
          {
            if ( *(_BYTE *)v203 )
            {
              if ( *(_BYTE *)(v203 - 5) == unit->BlockFromX && *(_BYTE *)(v203 - 4) == unit->BlockFromY && !v202 )
              {
                v202 = 1;
              }
            }
          }
          v203 += 20;
          --v204;
        }
        while ( v204 );
      }
      if ( !v202 )
      {
        UnitAdjustState(unit, UNIT_STATE_2_IDLE);
      }
      unit->LastMoved = 0;
      if ( UnitIsCloaked(unit) )
      {
        UnitUncloakStart(unit);
      }
      if ( ((((signed int)side_id >> 31) ^ (abs(side_id) & 7)) - ((signed int)side_id >> 31) == (int)(gGameTicks & 7)
         || unit->EnemyIndex != -1)
        && !UnitIsCloaked(unit) )
      {
        UnitAttack(
          unit,
          (eSideType)side_id_,
          myIndex,
          (eSideType *)&unit->EnemySide,
          (unsigned short *)&unit->EnemyIndex);
      }
      goto LABEL_612;
    case UNIT_STATE_19_AIRLIFTED:
      goto LABEL_612;
    case UNIT_STATE_20_INVALID:
      DebugFatal("UpdateUnit", "Invalid unit state %d", unit->State);
      goto LABEL_612;
    case UNIT_STATE_21_FLYING_DOWN:
      unit_flags = unit->Flags;
      if ( unit_flags & UFLAGS_800_DELIVERING )
      {
        if ( _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_FRIGATE )
        {
          goto LABEL_622;
        }
        if ( GetSide((eSideType)side_id_)->__PrimaryStarport == -1 )
        {
          unit_flags = unit->Flags;
          if ( !(unit_flags & UFLAGS_4000) )
          {
            unit_flags = (unit_flags & ~UFLAGS_200) | UFLAGS_4000;
            unit->Flags = unit_flags;
            unit->Speed = _templates_unitattribs[unit->Type].__Speed;
            UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
            goto LABEL_612;
          }
        }
        if ( GetSide((eSideType)side_id_)->__PrimaryStarport == -1
          || (unit_flags = unit->Flags, unit_flags & UFLAGS_4000)
          || ((side_ = GetSide((eSideType)side_id_),
              myIndex = side_->__ObjectArray[(unsigned short)GetSide((eSideType)side_id_)->__PrimaryStarport].Speed
                              / 0x10000
                              / 32
                              + 1,
              side_ = GetSide((eSideType)side_id_),
              side_id = side_->__ObjectArray[(unsigned short)GetSide((eSideType)side_id_)->__PrimaryStarport].Health
                      / 0x10000
                      / 32
                      - 2,
              (_BYTE)myIndex == unit->BlockToX)
          && side_id == unit->BlockToY ))
        {
LABEL_622:
          point.X = unit->TargetX;
          point.Y = unit->TargetY;
          if ( unit->__PosZHeight == 655360
            || _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_CARRYALL
            || CanUnitUseSquare(point, unit, (eSideType)side_id_, 0) )
          {
            v57 = unit->__PosZHeight;
            if ( v57 <= 0xA0000 )
            {
              if ( !UnitDeliver(unit, side_id_) )
              {
                if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_FRIGATE )
                {
                  GetSide((eSideType)side_id_)->__StarportDeliveryInProgress = 0;
                  side_ = GetSide((eSideType)side_id_);
                  CSide__ResetStarportOrderCost(side_);
                }
                UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
                unit_flags = unit->Flags;
                unit_flags = (unit_flags & ~(UFLAGS_100_CARRYING | UFLAGS_200)) | UFLAGS_4000;
                unit->Flags = unit_flags;
                side_id__ = (unsigned char)side_id_;
                if ( _gAIArray[side_id__].__IsAI == 1 )
                {
                  ai_group = CAI__FindGroupForDelivery(&_gAIArray[side_id__], unit->LastMoved);
                  *(int *)((char *)&_gAIArray[0].__AIGroups_2C[ai_group].dw_field_20__flags + side_id__ * 7608) |= 1u;
                  myIndex = unit->LastMoved;
                  side_ = GetSide((eSideType)side_id_);
                  v63 = myIndex;
                  v65 = (unsigned char)myIndex;
                  int i = (int)side_;
                  v66 = 7 * v65;
                  ai_group = CAI__FindGroupForDelivery(&_gAIArray[side_id__], v63);
                  *(&_gAIArray[0].__AIGroups_2C[ai_group].field_27 + side_id__ * 7608) = *(_BYTE *)(i
                                                                                                  + 8 * v66
                                                                                                  + 156450);
                }
              }
            }
            else
            {
              unit->__PosZHeight = v57 - 0x10000;
            }
          }
          else
          {
            GetNearestFreeTileForUnit(&unit->TargetX, &unit->TargetY, 8u);
            UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
          }
        }
        else
        {
          UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
          v55 = myIndex;
          v56 = side_id;
          unit->Speed = _templates_unitattribs[unit->Type].__Speed;
          unit->TargetX = v55;
          unit->TargetY = v56;
        }
      }
      else
      {
        v68 = (char)unit->__SpecialPurpose;
        if ( GetSide((eSideType)side_id_)->__PickupQueue[v68].__state != 3
          || (unit_flags = unit->Flags, unit_flags & UFLAGS_100_CARRYING) )
        {
          if ( unit->EnemyIndex == -1 )
          {
            DebugFatal("Unit.cpp", "EnemyIndex out of range (1)");
          }
          unit_flags = unit->Flags;
          if ( !(unit_flags & UFLAGS_100_CARRYING)
            || (v71.Y = unit->TargetY, v71.X = unit->TargetX, CanUnitUseSquare(v71, unit, (eSideType)side_id_, 0))
            || (unit_flags = unit->Flags, unit_flags & UFLAGS_800_DELIVERING) )
          {
            if ( unit->__PosZHeight != 0x3C0000
              || (unit_flags = unit->Flags, unit_flags & UFLAGS_100_CARRYING)
              || TurnUnit(side->__ObjectArray[(unsigned short)unit->EnemyIndex].__Facing, unit) )
            {
              v75 = unit->__PosZHeight;
              if ( v75 <= 655360 )
              {
                v76 = unit->Flags;
                if ( !(v76 & UFLAGS_800_DELIVERING) )
                {
                  if ( v76 & UFLAGS_100_CARRYING )
                  {
                    if ( unit->EnemyIndex == -1 )
                    {
                      DebugFatal("Unit.CPP", "EnemyIndex out of range (2)");
                    }
                    v78 = unit->EnemyIndex;
                    v79 = v78;
                    unit_flags = side->__ObjectArray[v78].Flags;
                    unit_flags &= ~UFLAGS_100_CARRYING;
                    v81 = &side->__ObjectArray[v79];
                    v81->Flags = unit_flags;
                    v81->__PosZHeight = 0;
                    UnitAdjustState(
                      &side->__ObjectArray[(unsigned short)unit->EnemyIndex],
                      (eUnitState)(unsigned char)unit->EnemySide);
                    side_id_ = (unsigned char)side_id_;
                    gGameMap.map[v81->BlockFromX + _CellNumbersWidthSpan[v81->BlockFromY]].__tile_bitflags &= ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
                    gGameMap.map[v81->BlockFromX + _CellNumbersWidthSpan[v81->BlockFromY]].__tile_bitflags |= (unsigned char)side_id_ | (unsigned int)TileFlags_8_OCC_UNIT;
                    if ( _templates_unitattribs[v81->Type].__IsInfantry )
                    {
                      gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__tile_bitflags &= ~(TileFlags_8000000_OWNER2|TileFlags_4000000_OWNER2|TileFlags_2000000_OWNER2);
                      v82 = unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY];
                      gGameMap.map[v82].__tile_bitflags |= side_id_ << 25;
                    }
                    // New logic start
                    // Crates can be picked up by a carryall dropping a unit on them
                    PickupCrate(&side->__ObjectArray[(unsigned short)unit->EnemyIndex], side_id_);
                    // New logic end
                    side->__PickupQueue[(unsigned char)unit->__SpecialPurpose].__state = 0;
                    unit->Flags &= ~UFLAGS_100_CARRYING;
                    UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
                  }
                  else
                  {
                    if ( unit->EnemyIndex == -1 )
                    {
                      DebugFatal("Unit.CPP", "EnemyIndex out of range (3)");
                    }
                    v84 = unit->EnemyIndex;
                    unit_flags = side->__ObjectArray[v84].Flags;
                    v86 = &side->__ObjectArray[v84];
                    if ( unit_flags & UFLAGS_100_CARRYING
                      || side->__ObjectArray[v84].State != UNIT_STATE_18_AWAITING_AIRLIFT )
                    {
                      unit_flags = unit->Flags;
                      unit_flags = (unit_flags & ~UFLAGS_200) | UFLAGS_4000;
                      unit->Flags = unit_flags;
                      UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
                    }
                    else
                    {
                      if ( side->__ObjectArray[v84].BlockFromX != side->__ObjectArray[v84].BlockToX
                        || side->__ObjectArray[v84].BlockFromY != side->__ObjectArray[v84].BlockToY )
                      {
                        if ( unit_flags & UFLAGS_BLOCKTOMARKED )
                        {
                          DebugFatal(
                            "UpdateUnit",
                            "unit has blockTo (%d,%d) and blockFrom (%d,%d) different",
                            side->__ObjectArray[v84].BlockFromX,
                            side->__ObjectArray[v84].BlockFromY,
                            side->__ObjectArray[v84].BlockToX,
                            side->__ObjectArray[v84].BlockToY);
                        }
                      }
                      gGameMap.map[v86->BlockFromX + _CellNumbersWidthSpan[v86->BlockFromY]].__tile_bitflags &= ~TileFlags_8_OCC_UNIT;
                      v87 = v86->BlockFromY;
                      tile_flags = gGameMap.map[v86->BlockFromX + _CellNumbersWidthSpan[v87]].__tile_bitflags;
                      if ( tile_flags & (TileFlags_200_CSPOT_TL|TileFlags_100_CSPOT_DL|TileFlags_80_CSPOT_DR|TileFlags_40_CSPOT_TR|TileFlags_20_CSPOT_MID) )
                      {
                        tile_flags = tile_flags & ~(TileFlags_4_OWNER|TileFlags_2_OWNER|TileFlags_1_OWNER);
                        gGameMap.map[v86->BlockFromX + _CellNumbersWidthSpan[v87]].__tile_bitflags = tile_flags;
                        gGameMap.map[v86->BlockFromX + _CellNumbersWidthSpan[v86->BlockFromY]].__tile_bitflags |= (gGameMap.map[v86->BlockFromX + _CellNumbersWidthSpan[v86->BlockFromY]].__tile_bitflags >> 25) & 7;
                      }
                      if ( v86->Flags & UFLAGS_BLOCKTOMARKED )
                      {
                        DebugFatal("UpdateUnit", "Unit has UFLAGS_BLOCKTOMARKED when being picked up");
                      }
                      v89 = unit->Flags & ~UFLAGS_4000000;
                      v89 |= UFLAGS_100_CARRYING;
                      unit->Flags = v89;
                      v90 = v86->Flags | UFLAGS_100_CARRYING;
                      v86->__IsSelected = 0;
                      v86->Flags = v90;
                      UnitAdjustState(v86, UNIT_STATE_19_AIRLIFTED);
                      side->__PickupQueue[(unsigned char)unit->__SpecialPurpose].__state = 4;
                      UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
                    }
                  }
                }
              }
              else
              {
                unit->__PosZHeight = v75 - 0x10000;
              }
            }
          }
          else
          {
            GetNearestFreeTileForUnit(&unit->TargetX, &unit->TargetY, 8u);
            side->__PickupQueue[v68].__to_x = unit->TargetX;
            side->__PickupQueue[v68].__to_y = unit->TargetY;
            UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
          }
        }
        else
        {
          unit_flags &= ~UFLAGS_200;
          unit->Flags = unit_flags;
          UnitAdjustState(unit, UNIT_STATE_22_FLYING_UP);
        }
      }
      goto LABEL_612;
    case UNIT_STATE_22_FLYING_UP:
      posz = unit->__PosZHeight;
      if ( posz >= 0x3C0000 )
      {
        goto LABEL_345;
      }
      unit->__PosZHeight = posz + 0x10000;
      goto LABEL_612;
    case UNIT_STATE_23_INFILTRATING:
      if ( MoveUnitSimple(unit, side_id_, 0, 0) )
      {
        goto LABEL_612;
      }
      enemy_index_ = unit->EnemyIndex;
      if ( enemy_index_ == -1 )
      {
        goto LABEL_465;
      }
      building = GetBuilding((eSideType)unit->EnemySide, enemy_index_);
      if ( !building || building->Flags & (BFLAGS_8_SOLD|BFLAGS_4_SELLING) )
      {
        goto LABEL_465;
      }
      if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_ENGINEER )
      {
        if ( !CaptureBuilding((eSideType)unit->EnemySide, (eSideType)side_id_, unit->EnemyIndex) )
        {
          goto LABEL_465;
        }
        enemy_side = GetSide((eSideType)unit->EnemySide);
        ++enemy_side->__BuildingsLost;
        GetSide((eSideType)unit->EnemySide);
        side_ = GetSide((eSideType)side_id_);
        ++side_->__BuildingsKilledPerTypeAndSide[0].__kills_per_side[(unsigned char)unit->EnemySide
                                                                   + 8 * building->Type];
        side_ = GetSide((eSideType)side_id_);
        ++side_->__BuildingsKilled;
        side_ = GetSide((eSideType)side_id_);
        ++side_->__BuildingsBuilt;
        if ( _gDiplomacy[(unsigned char)unit->EnemySide][(unsigned char)side_id_] == 2 )
        {
          NeutralBecomeHostile((eSideType)unit->EnemySide, (eSideType)side_id_);
        }
        if ( (_BYTE)side_id_ == gSideId )
        {
          hark_sound_id = GetSoundTableID("H_CAPTURE");
          ord_sound_id = GetSoundTableID("O_CAPTURE");
          atr_sound_id = GetSoundTableID("S_CAPTURE");
LABEL_464:
          PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 1, 0, 0);
          goto LABEL_465;
        }
        if ( unit->EnemySide == gSideId )
        {
          hark_sound_id = GetSoundTableID("H_BUILDINGLOST");
          ord_sound_id = GetSoundTableID("O_BUILDINGLOST");
          atr_sound_id = GetSoundTableID("S_BUILDINGLOST");
          PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 1, 0, 0);
          text_id = GetTextID("BuildingEngineered");
          text_string = GetTextString(text_id, 1);
          QueueMessage(text_string, -1);
        }
      }
      else
      {
        enemy_side = GetSide((eSideType)unit->EnemySide);
        ++enemy_side->__BuildingsLost;
        GetSide((eSideType)unit->EnemySide);
        side_ = GetSide((eSideType)side_id_);
        ++side_->__BuildingsKilledPerTypeAndSide[0].__kills_per_side[(unsigned char)unit->EnemySide
                                                                   + 8 * building->Type];
        side_ = GetSide((eSideType)side_id_);
        ++side_->__BuildingsKilled;
        if ( _gDiplomacy[(unsigned char)unit->EnemySide][(unsigned char)side_id_] == 2 )
        {
          NeutralBecomeHostile((eSideType)unit->EnemySide, (eSideType)side_id_);
        }
        DestroyBuilding((eSideType)unit->EnemySide, unit->EnemyIndex, 0);
        if ( unit->EnemySide == gSideId )
        {
          hark_sound_id = GetSoundTableID("H_BUILDINGLOST");
          ord_sound_id = GetSoundTableID("O_BUILDINGLOST");
          atr_sound_id = GetSoundTableID("S_BUILDINGLOST");
          goto LABEL_464;
        }
      }
LABEL_465:
      unit->Flags |= UFLAGS_200000_NOT_DRAWN;
      SetUnitAnimationType(unit, 0);
      side_ = GetSide((eSideType)side_id_);
      ++side_->__UnitsLost;
      side_ = GetSide((eSideType)side_id_);
      ++side_->__UnitsLostPerType[unit->Type];
      DestroyUnit((eSideType)side_id_, myIndex);
      goto LABEL_612;
    case UNIT_STATE_24_MOVING_TO_REPAIR_PAD:
      bld = GetBuilding((eSideType)side_id_, unit->RepairPadIndex);
      building = bld;
      if ( !bld || _templates_buildattribs[bld->Type].__Behavior != BuildingBehavior_REPAIR )
      {
        nearest_repair_pad = GetNearestBuildingWithBehavior(
                               unit->BlockFromX,
                               unit->BlockFromY,
                               (eSideType)side_id_,
                               BuildingBehavior_REPAIR,
                               &x,
                               &y);
        building = nearest_repair_pad;
        if ( !nearest_repair_pad )
        {
          if ( unit->pos_steps )
          {
            MoveUnit(unit, (eSideType)side_id_, myIndex);
          }
          else
          {
            UnitAdjustState(unit, UNIT_STATE_2_IDLE);
          }
          goto LABEL_489;
        }
        unit->TargetX = x;
        unit->TargetY = y;
        unit->RepairPadIndex = nearest_repair_pad->__MyIndex;
        v229 = MoveUnit(unit, (eSideType)side_id_, myIndex) == 0;
        goto LABEL_479;
      }
      x = bld->__PosX / 0x10000 / 32;
      v224 = (bld->__PosY / 0x10000 - _templates_buildattribs[bld->Type]._____ArtHeight) / 32;
      y = v224;
      x += _templates_buildattribs[bld->Type]._____ExitPoint1X;
      v224 = _templates_buildattribs[bld->Type]._____ExitPoint1Y + v224;
      unit->TargetX = x;
      y = v224;
      unit->TargetY = v224;
      if ( !MoveUnit(unit, (eSideType)side_id_, myIndex) )
      {
        if ( (_BYTE)side_id_ != gSideId )
        {
          goto LABEL_485;
        }
        hark_sound_id = GetSoundTableID("H_REPAIRUNIT");
        ord_sound_id = GetSoundTableID("O_REPAIRUNIT");
        atr_sound_id = GetSoundTableID("S_REPAIRUNIT");
        PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 0, 0, 0);
        if ( (_BYTE)side_id_ != gSideId )
        {
          goto LABEL_485;
        }
        is_low_money = side->SpiceReal + side->CashReal == 0;
LABEL_483:
        if ( is_low_money )
        {
          hark_sound_id = GetSoundTableID("H_LOWMONEY");
          ord_sound_id = GetSoundTableID("O_LOWMONEY");
          atr_sound_id = GetSoundTableID("S_LOWMONEY");
          PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 1, 0, 0);
        }
LABEL_485:
        UnitAdjustState(unit, UNIT_STATE_25_REPAIRING);
        goto LABEL_612;
      }
      if ( unit->BlockFromX == x && unit->BlockFromY == y )
      {
LABEL_480:
        if ( (_BYTE)side_id_ != gSideId )
        {
          goto LABEL_485;
        }
        hark_sound_id = GetSoundTableID("H_REPAIRUNIT");
        ord_sound_id = GetSoundTableID("O_REPAIRUNIT");
        atr_sound_id = GetSoundTableID("S_REPAIRUNIT");
        PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 0, 0, 0);
        if ( (_BYTE)side_id_ != gSideId )
        {
          goto LABEL_485;
        }
        is_low_money = side->SpiceReal + side->CashReal == 0;
        goto LABEL_483;
      }
      if ( unit->Flags & UFLAGS_BLOCKTOMARKED && unit->BlockToX == x )
      {
        v229 = unit->BlockToY == y;
LABEL_479:
        if ( v229 )
        {
          goto LABEL_480;
        }
      }
LABEL_489:
      if ( !unit->pos_steps && !(unit->Flags & UFLAGS_BLOCKTOMARKED) )
      {
        distance_x = unit->BlockFromX - unit->TargetX;
        distance_y = unit->BlockFromY - unit->TargetY;
        if ( distance_y * distance_y + distance_x * distance_x > _gVariables.RepairDriveDistance )
        {
LABEL_492:
          side_ = GetSide((eSideType)side_id_);
          // New logic start
          // Prevent units with special terrain restriction from getting airlifted
          int idx = -1;
          if (!_templates_unitattribs[unit->Type].MovementRestriction)
            idx = CSide__GetQueuePos(side_, unit);
          // New logic end
          if ( idx != -1 )
          {
            unit_state = unit->State;
            side_ = GetSide((eSideType)side_id_);
            if ( CSide__AddToQueue(side_, unit, myIndex, idx, 100, unit_state) )
            {
              UnitAdjustState(unit, UNIT_STATE_18_AWAITING_AIRLIFT);
            }
          }
        }
      }
      goto LABEL_612;
    case UNIT_STATE_25_REPAIRING:
      if ( unit->pos_steps )
      {
        MoveUnit(unit, (eSideType)side_id_, myIndex);
        goto LABEL_612;
      }
      if ( _templates_unitattribs[unit->Type].__Strength == unit->Health )
      {
        unit->OldState = UNIT_STATE_2_IDLE;
        if ( (_BYTE)side_id_ == gSideId )
        {
          hark_sound_id = GetSoundTableID("H_UNITREPAIRED");
          ord_sound_id = GetSoundTableID("O_UNITREPAIRED");
          atr_sound_id = GetSoundTableID("S_UNITREPAIRED");
          PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 0, 0, 0);
          UnitAdjustState(unit, UNIT_STATE_26_LEAVING_REPAIR_PAD);
          goto LABEL_612;
        }
LABEL_503:
        UnitAdjustState(unit, UNIT_STATE_26_LEAVING_REPAIR_PAD);
        goto LABEL_612;
      }
      v249 = unit->BlockFromY;
      v250 = unit->BlockFromX;
      if ( !GetBuildingOnTile_0(v250, v249, &building, (eSideType *)&v3, &a8) )
      {
        UnitAdjustState(unit, UNIT_STATE_26_LEAVING_REPAIR_PAD);
        goto LABEL_612;
      }
      if ( (_BYTE)v3 != (_BYTE)side_id_ )
      {
        goto LABEL_503;
      }
      v251 = _gVariables.UnitRepairValue
           * GetUnitCost(unit->Type, (eSideType)side_id_)
           / _templates_unitattribs[unit->Type].__Strength;
      if ( (v251 & 0xFFFFFFFE) >= 2 )
      {
        cost = v251 >> 1;
      }
      else
      {
        cost = 1;
      }
      if ( CSide__SpendMoneyIfSufficient(side, cost, 0) )
      {
        building->Flags |= BFLAGS_20_ANIM_ONCE;
        health = _gVariables.UnitRepairValue + unit->Health;
        unit->Health = health;
        max_health = _templates_unitattribs[unit->Type].__Strength;
        if ( health <= max_health )
        {
          if ( (_BYTE)side_id_ == gSideId )
          {
            credits = side->CashReal + side->SpiceReal;
            if ( credits )
            {
              if ( credits >= 0x64 )
              {
                _TacticalData.__isLowOnFunds = 0;
              }
            }
            else if ( !_TacticalData.__isLowOnFunds )
            {
              _TacticalData.__isLowOnFunds = 1;
              hark_sound_id = GetSoundTableID("H_LOWMONEY");
              ord_sound_id = GetSoundTableID("O_LOWMONEY");
              atr_sound_id = GetSoundTableID("S_LOWMONEY");
              PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 1, 0, 0);
            }
          }
        }
        else
        {
          unit->Health = _templates_unitattribs[unit->Type].__Strength;
        }
      }
LABEL_612:
      if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_CARRYALL )
      {
        unit_flags = unit->Flags;
        if ( !(unit_flags & UFLAGS_800_DELIVERING) )
        {
          if ( unit_flags & UFLAGS_100_CARRYING )
          {
            if ( unit->EnemyIndex == -1 )
            {
              DebugFatal("Unit.cpp", "enemyIndex out of raneg (5)");
            }
            carried_unit_flags = side->__ObjectArray[(unsigned short)unit->EnemyIndex].Flags;
            if ( carried_unit_flags & UFLAGS_100_CARRYING )
            {
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].BlockToX = unit->BlockToX;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].BlockToY = unit->BlockToY;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].BlockFromX = unit->BlockFromX;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].BlockFromY = unit->BlockFromY;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].__PosX = unit->__PosX;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].pos_steps = 0;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].__PosY = unit->__PosY;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].__PosZHeight = unit->__PosZHeight;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].__Facing = unit->__Facing;
              side->__ObjectArray[(unsigned short)unit->EnemyIndex].__FacingTurret = unit->__FacingTurret;
              posz_ptr = &side->__ObjectArray[(unsigned short)unit->EnemyIndex].__PosZHeight;
              *posz_ptr -= 0xA0000;
            }
          }
        }
      }
      return 1;
    case UNIT_STATE_26_LEAVING_REPAIR_PAD:
      v259 = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
      if ( v259 )
      {
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = v259 - 1;
      }
      if ( GetFreeAdjacentTile(unit->BlockFromX, unit->BlockFromY, unit, side_id_, &point, 0) )
      {
        v260 = point.Y;
        unit->TargetX = point.X;
        unit->TargetY = v260;
        UnitAdjustState(unit, UNIT_STATE_8_LEAVING_BUILDING);
      }
      else if ( !unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter )
      {
        x = unit->BlockFromX + GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 5836) % 3u - 1;
        y = unit->BlockFromY + GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 5837) % 3u - 1;
        unit_on_tile = GetUnitOnTile(32 * x, 32 * y, (eSideType *)&a1, &index, 0);
        if ( unit_on_tile )
        {
          if ( unit_on_tile != unit
            && !_gDiplomacy[(unsigned char)side_id_][a1]
            && CanUnitAcceptOrders(unit_on_tile, 0) )
          {
            UnitAdjustState(unit_on_tile, UNIT_STATE_9_MOVING_AWAY);
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 50;
          }
        }
      }
      goto LABEL_612;
    case UNIT_STATE_27_WANDERING:
      if ( unit->Health >= (signed int)((unsigned int)_templates_unitattribs[unit->Type].__Strength >> 1) )
      {
        if ( MoveUnit(unit, (eSideType)side_id_, myIndex) )
        {
          if ( unit->RouteMode )
          {
            unit_flags = unit->Flags;
            unit_flags = unit_flags & ~UFLAGS_4_CLOAKED;
            unit->Flags = unit_flags;
            v272 = unit->BlockFromY;
            v273 = unit->BlockFromX;
            unit->RouteMode = 0;
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 0;
            if ( GetFreeAdjacentTile(v273, v272, unit, side_id_, &point, 0) )
            {
              v274 = point.Y;
              unit->TargetX = point.X;
              unit->TargetY = v274;
            }
            else
            {
              UnitAdjustState(unit, UNIT_STATE_29_SLEEPING);
              unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.SandWormShotSleep;
            }
          }
          else if ( !(unit->Flags & UFLAGS_4_CLOAKED)
                 && (!unit->__pos_stepsmax
                  || (unsigned char)unit->pos_steps < 2 * (unsigned char)unit->__pos_stepsmax) )
          {
            UpdateParticles(unit->__PosX / 0x10000, unit->__PosY / 0x10000);
            if ( !(GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 5959) % 0x50u) )
            {
              explosion_type = *((unsigned char *)&_templates_GroupIDs.EX_WORMSN1
                               + GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 5961) % 6u);
              ModelAddExplosion(
                (eSideType)side_id_,
                explosion_type,
                unit->__PosX / 0x10000,
                unit->__PosY / 0x10000,
                0,
                0,
                0,
                0,
                0);
              posx = unit->__PosX / 0x10000;
              if ( posx >= _ViewportXPos && posx <= _ViewportWidth + _ViewportXPos )
              {
                posy = unit->__PosY / 0x10000;
                if ( posy >= _ViewportYPos
                  && posy <= _ViewportHeight + _ViewportYPos
                  && (!_bool_shroud_4DFB04
                   || !gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__shroud) )
                {
                  hark_sound_id = GetSoundTableID("H_WORMSIGN");
                  ord_sound_id = GetSoundTableID("O_WORMSIGN");
                  atr_sound_id = GetSoundTableID("S_WORMSIGN");
                  PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 1, 2000, 0);
                  if ( !IsSoundPlaying(_templates_explosionattribs[explosion_type].__Sound) )
                  {
                    PlaySoundAt(
                      _templates_explosionattribs[explosion_type].__Sound,
                      unit->__PosX / 0x10000 / 32,
                      unit->__PosY / 0x10000 / 32);
                  }
                }
              }
            }
          }
        }
        else
        {
          unit_on_tile = GetUnitOnTile(32 * unit->BlockFromX, 32 * unit->BlockFromY, (eSideType *)&a1, &index, 0);
          // New logic start
          // Implement not edible units
          if ( !unit_on_tile || _templates_unitattribs[unit_on_tile->Type].__IsInfantry || _templates_unitattribs[unit_on_tile->Type].NotEdible )
          // New logic end
          {
            unit->Flags &= ~UFLAGS_4_CLOAKED;
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 0;
            UnitAdjustState(unit, UNIT_STATE_28_EATING);
          }
          else
          {
            UnitAdjustState(unit_on_tile, UNIT_STATE_17_DEAD);
            side_ = GetSide((eSideType)a1);
            ++side_->__UnitsLost;
            side_ = GetSide((eSideType)a1);
            ++side_->__UnitsLostPerType[unit_on_tile->Type];
            v267 = unit->Flags & ~UFLAGS_4_CLOAKED;
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 50;
            unit->Flags = v267;
            UnitAdjustState(unit, UNIT_STATE_28_EATING);
            PlaySoundAt(
              _templates_explosionattribs[(int)_templates_unitattribs[unit->Type].__DestroyAnim].__Sound,
              unit->__PosX / 0x10000 / 32,
              unit->__PosY / 0x10000 / 32);
            if ( a1 == gSideId )
            {
              hark_sound_id = GetSoundTableID("H_WORMATTACK");
              ord_sound_id = GetSoundTableID("O_WORMATTACK");
              atr_sound_id = GetSoundTableID("S_WORMATTACK");
              PlayMentatSound(atr_sound_id, ord_sound_id, hark_sound_id, 1, 0, 0);
            }
            ModelAddExplosion(
              (eSideType)side_id_,
              _templates_unitattribs[unit->Type].__DestroyAnim,
              unit->__PosX / 0x10000,
              unit->__PosY / 0x10000,
              0,
              0,
              0,
              0,
              0);
            ModelAddExplosion(
              (eSideType)side_id_,
              _templates_unitattribs[unit->Type].__DestroyAnim + 1,
              unit->__PosX / 0x10000,
              unit->__PosY / 0x10000,
              0,
              0,
              0,
              0,
              0);
            ++unit->__SpecialPurpose;
          }
        }
      }
      else
      {
        UnitAdjustState(unit, UNIT_STATE_29_SLEEPING);
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.SandWormShotSleep;
      }
      goto LABEL_612;
    case UNIT_STATE_28_EATING:
      sleep_time_counter = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
      if ( sleep_time_counter )
      {
        unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = sleep_time_counter - 1;
        goto LABEL_612;
      }
      if ( FindNearestActiveThumper(unit->BlockFromX, unit->BlockFromY, &x, &y) )
      {
        if ( FindSandwormsTarget(x, y, &xx, &yy) )
        {
          if ( abs(x - xx) >= 5 || abs(y - yy) >= 5 )
          {
            unit->TargetX = x;
            unit->TargetY = y;
            UnitAdjustState(unit, UNIT_STATE_27_WANDERING);
          }
          else
          {
            unit->TargetX = xx;
            unit->TargetY = yy;
            UnitAdjustState(unit, UNIT_STATE_27_WANDERING);
          }
        }
        else
        {
          unit->TargetX = x;
          unit->TargetY = y;
          UnitAdjustState(unit, UNIT_STATE_27_WANDERING);
        }
      }
      else
      {
        if ( !FindSandwormsTarget(unit->BlockFromX, unit->BlockFromY, &x, &y) )
        {
          unit->TargetX = (unsigned int)GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6051)
                        % gGameMap.width;
          random_value = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6052);
          goto LABEL_569;
        }
        if ( unit->__SpecialPurpose < (unsigned char)_gVariables.SandWormAppetite )
        {
          unit->TargetX = x;
          unit->TargetY = y;
          UnitAdjustState(unit, UNIT_STATE_27_WANDERING);
        }
        else
        {
          UnitAdjustState(unit, UNIT_STATE_29_SLEEPING);
          unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = _gVariables.SandWormFedSleep;
        }
      }
      goto LABEL_612;
    case UNIT_STATE_29_SLEEPING:
      sleep_time_counter = unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter;
      unit->Flags |= UFLAGS_4_CLOAKED;
      if ( sleep_time_counter )
      {
        sleep_time_subcounter = unit->__TurretTurnDelayCounter + 1;
        unit->__TurretTurnDelayCounter = sleep_time_subcounter;
        if ( sleep_time_subcounter > 100u )
        {
          unit->__TurretTurnDelayCounter = 0;
          unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = sleep_time_counter - 1;
        }
      }
      else
      {
        unit->Health = _templates_unitattribs[unit->Type].__Strength;
        unit->TargetX = (unsigned int)GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6080)
                      % gGameMap.width;
        random_value = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6081);
LABEL_569:
        unit->TargetY = random_value % gGameMap.height;
        UnitAdjustState(unit, UNIT_STATE_27_WANDERING);
      }
      goto LABEL_612;
    case UNIT_STATE_30_DEPLOYING:
      if ( unit->pos_steps )
      {
        remember_pos_y = unit->__RememberPosY;
        unit->TargetX = unit->__RememberPosX;
        unit->TargetY = remember_pos_y;
        MoveUnit(unit, (eSideType)side_id_, myIndex);
      }
      else if ( !TurnUnitInDirection(unit, 29u) )
      {
        conyard_type = 0;
        behavior_ptr = &_templates_buildattribs[0].__Behavior;
        while ( !(behavior_ptr[-0x58u] & (unsigned char)(1 << _IRValues[(unsigned char)side_id_]))
             || *behavior_ptr != BuildingBehavior_CONSTRUCTION )
        {
          ++conyard_type;
          behavior_ptr += sizeof(BuildingAtrbStruct);
          if ( conyard_type > gBuildingTypeNum )
          {
            DebugFatal("UpdateUnit", "Cannot find correct construction yard");
          }
        }
        if ( CheckIfMCVCanBeDeployedOn(unit->BlockFromX, unit->BlockFromY) )
        {
          posy = unit->BlockFromY;
          posx = unit->BlockFromX;
          rect.top = 0;
          gGameMap.map[posx + _CellNumbersWidthSpan[posy]].__tile_bitflags &= ~TileFlags_8_OCC_UNIT;
          health_ = (double)unit->Health;
          posx = unit->BlockFromY - 1;
          posx_ = unit->BlockFromX;
          rect.left = _templates_unitattribs[unit->Type].__Strength;
          float f = health_ / (double)rect.left;
          conyard_index = ModelAddBuilding((eSideType)side_id_, conyard_type, posx_ - 1, posx, 0, 0, 0);
          if ( (_BYTE)side_id_ == gSideId )
          {
            posy_ = unit->BlockFromY;
            posx_ = unit->BlockFromX;
            sound_id = GetSoundTableID("S_BUILDUP");
            PlaySoundAt(sound_id, posx_, posy_);
          }
          if ( conyard_index != -1 )
          {
            rect.top = 0;
            rect.left = _templates_buildattribs[conyard_type]._____HitPoints;
            new_health = rect.left * f;
            Building *b = (Building *)&GetSide((eSideType)side_id_)->__ObjectArray[conyard_index];
            b->Health = new_health;
          }
          unit->Flags |= UFLAGS_400000|UFLAGS_200000_NOT_DRAWN;
          UnitAdjustState(unit, UNIT_STATE_17_DEAD);
        }
        else
        {
          unit->State = UNIT_STATE_2_IDLE;
        }
      }
      goto LABEL_612;
    case UNIT_STATE_31_SELFDESTRUCT:
      if ( unit->pos_steps )
      {
        remember_pos_y = unit->__RememberPosY;
        unit->TargetX = unit->__RememberPosX;
        unit->TargetY = remember_pos_y;
        MoveUnit(unit, (eSideType)side_id_, myIndex);
      }
      else
      {
        if ( !(GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6186) % 3u) )
        {
          ypos = unit->__PosY / 0x10000;
          xpos = unit->__PosX / 0x10000;
          random_spark_explosion = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6187);
          ModelAddExplosion(
            (eSideType)side_id_,
            *(&_templates_GroupIDs.EX_DEVSPARK1 + random_spark_explosion % 3),
            xpos,
            ypos,
            0,
            0,
            0,
            0,
            0);
        }
        special_purpose = unit->__SpecialPurpose;
        if ( special_purpose )
        {
          unit->__SpecialPurpose = special_purpose - 1;
        }
        else
        {
          UnitAdjustState(unit, UNIT_STATE_17_DEAD);
          ModelAddExplosion(
            (eSideType)side_id_,
            _templates_GroupIDs.EX_DEVDEATH,
            unit->__PosX / 0x10000,
            unit->__PosY / 0x10000,
            0,
            0,
            0,
            0,
            0);
          // New logic start
          // Fix missing devastator self-destruct sound
          PlaySoundAt(_templates_explosionattribs[(int)_templates_GroupIDs.EX_DEVDEATH].__Sound , unit->__PosX / 0x10000 / 32, unit->__PosY / 0x10000 / 32);
          // New logic end
          side_ = GetSide((eSideType)side_id_);
          ++side_->__UnitsLost;
          units_lost_per_type_ptr = &GetSide((eSideType)side_id_)->__UnitsLostPerType[unit->Type];
          ++*units_lost_per_type_ptr;
          DamageTiles(
            unit->__PosX / 0x10000,
            unit->__PosY / 0x10000,
            6u,
            _templates_GroupIDs.Devastator,
            (eSideType)side_id_,
            myIndex,
            0);
        }
      }
      goto LABEL_612;
    case UNIT_STATE_32_THUMPERING:
      if ( unit->pos_steps )
      {
        remember_pos_y = unit->__RememberPosY;
        unit->TargetX = unit->__RememberPosX;
        unit->TargetY = remember_pos_y;
LABEL_594:
        MoveUnit(unit, (eSideType)side_id_, myIndex);
      }
      else if ( !TurnUnitInDirection(unit, 8u) )
      {
        if ( !_templates_unitattribs[unit->Type].__IsInfantry )
        {
          DebugFatal("UNIT", "Invalid type for thumper");
        }
        SetUnitAnimationType(unit, 8);
        current_anim_type = unit->__CurrentAnimType;
        current_anim_frame = (unsigned char)unit->__CurrentAnimFrame;
        if ( current_anim_frame == _UnitAnimTypeFrames[current_anim_type].x + 1 )
        {
          special_purpose_ = unit->__SpecialPurpose + 1;
          unit->__SpecialPurpose = special_purpose_;
          if ( special_purpose_ >= 60u )
          {
            unit->__SpecialPurpose = 0;
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 3;
          }
          else
          {
            unit->__CurrentAnimDelayCounter_SandwormSleepTimeCounter = 0;
          }
        }
        if ( current_anim_frame == _UnitAnimTypeFrames[current_anim_type].x )
        {
          ModelAddExplosion(
            (eSideType)side_id_,
            _templates_GroupIDs.EX_THPUFF,
            unit->__PosX / 0x10000,
            unit->__PosY / 0x10000,
            0,
            0,
            0,
            0,
            0);
          xpos = unit->__PosX / 0x10000;
          if ( xpos >= _ViewportXPos && xpos <= _ViewportWidth + _ViewportXPos )
          {
            ypos = unit->__PosY / 0x10000;
            if ( ypos >= _ViewportYPos
              && ypos <= _ViewportHeight + _ViewportYPos
              && (!_bool_shroud_4DFB04
               || !gGameMap.map[unit->BlockFromX + _CellNumbersWidthSpan[unit->BlockFromY]].__shroud)
              && !IsSoundPlaying(_templates_explosionattribs[(unsigned char)_templates_GroupIDs.EX_THPUFF].__Sound) )
            {
              PlaySoundAt(
                _templates_explosionattribs[(unsigned char)_templates_GroupIDs.EX_THPUFF].__Sound,
                unit->__PosX / 0x10000 / 32,
                unit->__PosY / 0x10000 / 32);
            }
          }
        }
      }
      goto LABEL_612;
    case UNIT_STATE_33:
      special_purpose = unit->__SpecialPurpose;
      if ( !special_purpose )
      {
        goto LABEL_345;
      }
      unit->__SpecialPurpose = special_purpose - 1;
      goto LABEL_612;
  }
  return 0;
}
