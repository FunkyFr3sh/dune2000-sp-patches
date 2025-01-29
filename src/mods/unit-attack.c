#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

// Added new unit shooting features:
// - Bulk shots with customizable number of shots in bulk, short and long delay, for pri and sec weapon independently
// - Double-shot with customizable split angle, settable for pri and sec weapon independently
// - No auto-attack, settable for pri and sec weapon independently
// - Customizable inaccuracy of weapons (0 = vanilla behavior, 1 = almost accurate, the higher value the less accurate)
// - Fixed shoot offset for higher values
// - Non-muzzle-flash explosions can be used as muzzle flash, they will be spawned at the origin of shot bullet

bool CanUnitShoot(Unit *unit)
{
  if (!unit->__ReloadDelayCounter)
    return true;
  if (unit->ShotsRemaining)
  {
    UnitAtribStruct *unit_template = &_templates_unitattribs[unit->Type];
    if (unit->LastUsedWeapon == 1)
      return unit->__ReloadDelayCounter <= unit_template->PrimaryWeaponLongDelay - unit_template->PrimaryWeaponShortDelay;
    else
      return unit->__ReloadDelayCounter <= (unit_template->SecondaryWeaponLongDelay?unit_template->SecondaryWeaponLongDelay:unit_template->PrimaryWeaponLongDelay) - unit_template->SecondaryWeaponShortDelay;
  }
  return false;
}

// Reimplementation of function CanUnitAttackTile -> CanUnitAttackTarget

bool CanUnitAttackTarget(Unit *unit, int target_xpos, int target_ypos, bool air_target)
{
  int primary_weapon;
  int secondary_weapon;
  char primary_weapon_antiaircraft;
  unsigned int primary_weapon_range;
  int unit_xpos;
  int unit_ypos;
  char secondary_weapon_antiaircraft;
  unsigned int secondary_weapon_range;

  if ( unit->pos_steps )
  {
    DebugFatal("CanUnitAttackTarget", "unit->pos.steps not zero");
  }
  primary_weapon = _templates_unitattribs[unit->Type].__PrimaryWeapon;
  secondary_weapon = _templates_unitattribs[unit->Type].__SecondaryWeapon;
  if ( primary_weapon == -1 )
  {
    return 0;
  }
  primary_weapon_antiaircraft = _templates_bulletattribs[primary_weapon].AntiAircraft;
  primary_weapon_range = _templates_bulletattribs[primary_weapon].__Range;
  if (unit->LastUsedWeapon == 2)
  {
    primary_weapon_antiaircraft = _templates_bulletattribs[secondary_weapon].AntiAircraft;
    primary_weapon_range = _templates_bulletattribs[secondary_weapon].__Range;
  }
  unit_xpos = 32 * unit->BlockFromX + 16;
  unit_ypos = 32 * unit->BlockFromY + 16;
  if ( secondary_weapon == -1 || unit->LastUsedWeapon == 1 )
  {
    secondary_weapon_antiaircraft = 0;
    secondary_weapon_range = 0;
  }
  else
  {
    secondary_weapon_antiaircraft = _templates_bulletattribs[secondary_weapon].AntiAircraft;
    secondary_weapon_range = _templates_bulletattribs[secondary_weapon].__Range;
  }
  if ( air_target )
  {
    if ( !primary_weapon_antiaircraft )
    {
      if ( !secondary_weapon_antiaircraft )
      {
        return 0;
      }
      goto LABEL_17;
    }
    if ( secondary_weapon_antiaircraft )
    {
      if ( secondary_weapon_range <= primary_weapon_range )
      {
        return CheckDistance(unit_xpos, unit_ypos, target_xpos, target_ypos, primary_weapon_range);
      }
      goto LABEL_17;
    }
  }
  else if ( primary_weapon_range <= secondary_weapon_range )
  {
LABEL_17:
    primary_weapon_range = secondary_weapon_range;
    return CheckDistance(unit_xpos, unit_ypos, target_xpos, target_ypos, primary_weapon_range);
  }
  return CheckDistance(unit_xpos, unit_ypos, target_xpos, target_ypos, primary_weapon_range);
}

// Custom implementation of function UnitAttack
DETOUR(0x00496480, 0x0049678E, _Mod__UnitAttack);

char Mod__UnitAttack(Unit *unit, char side_id, short index, char *enemy_side_id_ptr, short *enemy_index_ptr)
{
  Unit *unit_; // ebp
  UnitAtribStruct *unit_template;
  char primary_weapon; // cl MAPDST
  char enemy_side_id; // cl
  CSide *enemy_side; // eax
  Unit *enemy_object; // ecx
  int enemy_object_type; // eax
  int ypos; // edi
  int xpos; // esi
  Unit *enemy_unit; // eax
  short enemy_index_; // ax MAPDST
  int enemy_index; // [esp+10h] [ebp-1Ch]
  int enemy_unit_ypos; // [esp+14h] [ebp-18h] MAPDST
  int enemy_unit_xpos; // [esp+18h] [ebp-14h]
  int enemy_building_y; // [esp+1Ch] [ebp-10h]
  int enemy_building_x; // [esp+20h] [ebp-Ch]
  char secondary_weapon; // [esp+24h] [ebp-8h]
  eSideType s;
  _WORD i;

  unit_ = unit;
  if ( !CanUnitShoot(unit) )
  {
    return 1;
  }
  unit_template = &_templates_unitattribs[unit_->Type];
  secondary_weapon = (unit->LastUsedWeapon == 1 || unit_template->SecondaryWeaponNoAutoAttack)?-1:unit_template->__SecondaryWeapon;
  primary_weapon = (unit->LastUsedWeapon == 2 || unit_template->PrimaryWeaponNoAutoAttack)?secondary_weapon:unit_template->__PrimaryWeapon;
  if ( primary_weapon == -1 && secondary_weapon == -1 )
  {
    return 0;
  }
  if (unit->LastUsedWeapon != 2 && unit_template->SecondaryWeaponNoAutoAttack && !unit_template->PrimaryWeaponNoAutoAttack)
    unit->LastUsedWeapon = 1;
  if (unit->LastUsedWeapon != 1 && unit_template->PrimaryWeaponNoAutoAttack && !unit_template->SecondaryWeaponNoAutoAttack)
    unit->LastUsedWeapon = 2;
  enemy_index = *enemy_index_ptr;
  if ( *enemy_index_ptr != -1 )
  {
    enemy_side_id = *enemy_side_id_ptr;
    if ( *enemy_side_id_ptr != side_id )
    {
      unit = (Unit *)(int)*enemy_side_id_ptr;
      enemy_side = GetSide(enemy_side_id);
      enemy_object = &enemy_side->__ObjectArray[*enemy_index_ptr];
      if ( (CSide *)((char *)enemy_side + 148 * *enemy_index_ptr) == (CSide *)-48 )
      {
        return 0;
      }
      enemy_object_type = enemy_side->__ObjectArray[*enemy_index_ptr].ObjectType;
      if ( enemy_object_type == OBJECT_UNIT )
      {
        enemy_unit_xpos = enemy_object->__PosX / 0x10000;
        enemy_unit_ypos = enemy_object->__PosY / 0x10000;
        if ( CanUnitAttackTarget(unit_, enemy_unit_xpos, enemy_unit_ypos, (enemy_object->Flags & UFLAGS_40_FLYING) != 0) )
        {
          UnitShootTarget(unit_, side_id, index, enemy_unit_xpos, enemy_unit_ypos, (int)unit, enemy_index, 1);
          return 1;
        }
      }
      else if ( enemy_object_type == OBJECT_BUILDING
             && _templates_buildattribs[LOBYTE(enemy_object->__PosX)].__Behavior == BuildingBehavior_TURRET )
      {
        GetBuildingInRange(
          unit_->BlockFromX,
          unit_->BlockFromY,
          (Building *)enemy_object,
          (char *)&enemy_building_x,
          (char *)&enemy_building_y);
        if ( CanUnitAttackTarget(unit_, enemy_building_x * 32 + 16, enemy_building_y * 32 + 16, 0) )
        {
          UnitShootTarget(
            unit_,
            side_id,
            index,
            32 * enemy_building_x + 16,
            32 * enemy_building_y + 16,
            (int)unit,
            enemy_index,
            2);
          return 1;
        }
      }
    }
  }
  ypos = 32 * unit_->BlockFromY + 16;
  xpos = 32 * unit_->BlockFromX + 16;
  if ( !EnemyUnitInRange(
          xpos,
          ypos,
          side_id,
          0,
          &enemy_unit_xpos,
          &enemy_unit_ypos,
          (unsigned char *)&unit,
          (short *)&enemy_index,
          primary_weapon,
          secondary_weapon,
          0,
          1,
          1) )
  {
    if ( EnemyBuildingInRange(
           xpos,
           ypos,
           side_id,
           &enemy_unit_xpos,
           &enemy_unit_ypos,
           (char *)&unit,
           (short *)&enemy_index,
           primary_weapon,
           secondary_weapon,
           1) )
    {
      *enemy_side_id_ptr = (int)unit;
      enemy_index_ = enemy_index;
      *enemy_index_ptr = enemy_index;
      if (CanUnitAttackTarget(unit_, enemy_unit_xpos, enemy_unit_ypos, 0))
        UnitShootTarget(unit_, side_id, index, enemy_unit_xpos, enemy_unit_ypos, (int)unit, enemy_index_, 2);
      return 1;
    }
    return 0;
  }
  *enemy_side_id_ptr = (int)unit;
  enemy_index_ = enemy_index;
  *enemy_index_ptr = enemy_index;
  enemy_unit = GetUnit((int)unit, enemy_index_);
  if ( !enemy_unit )
  {
    return 1;
  }
  if ( _templates_unitattribs[enemy_unit->Type].__Behavior == UnitBehavior_SANDWORM
    && GetUnitOnTile(enemy_unit_xpos, enemy_unit_ypos, &s, &i, 0) )
  {
    return 1;
  }
  if (CanUnitAttackTarget(unit_, enemy_unit_xpos, enemy_unit_ypos, (enemy_unit->Flags & UFLAGS_40_FLYING) != 0))
    UnitShootTarget(unit_, side_id, index, enemy_unit_xpos, enemy_unit_ypos, (int)unit, enemy_index, 1);
  return 1;
}

// Custom implementation of function UnitAttackUnit
DETOUR(0x00496B00, 0x00496D10, _Mod__UnitAttackUnit);

char Mod__UnitAttackUnit(Unit *unit, char side_id, short index)
{
  CSide *enemy_side; // edi
  short enemy_index; // ax MAPDST
  Unit *enemy_unit; // edi
  unsigned char v9; // dl

  if ( unit->EnemyIndex == -1 )
  {
    if ( !unit->pos_steps )
    {
      if ( unit->Flags & UFLAGS_BLOCKTOMARKED
        && (unit->BlockFromX != unit->BlockToX || unit->BlockFromY != unit->BlockToY) )
      {
        DebugFatal(
          "Unit.CPP: AttackUnit",
          "Unit %d Side %d is waiting with 'To' and 'From' blocks different",
          index,
          side_id);
        return 0;
      }
      return 0;
    }
LABEL_21:
    MoveUnit(unit, side_id, index);
    return 1;
  }
  enemy_side = GetSide(unit->EnemySide);
  if ( unit->EnemyIndex >= MAX_OBJECTS )
  {
    DebugFatal("AttackUnit", "unit->enemyIndex >= kMaxUnitsPerSide");
  }
  enemy_index = unit->EnemyIndex;
  enemy_unit = &enemy_side->__ObjectArray[enemy_index];
  if ( !enemy_unit )
  {
    DebugFatal("AttackUnit", "Invalid obj");
    DebugFatal("AttackUnit", "Invalid targetUnit");
  }
  if ( enemy_unit->ObjectType != OBJECT_UNIT
    || enemy_unit->State == UNIT_STATE_17_DEAD
    || enemy_unit->BlockToX >= gGameMap.width
    || enemy_unit->BlockToY >= gGameMap.height )
  {
    if ( unit->pos_steps )
    {
      MoveUnit(unit, side_id, index);
      return 1;
    }
    if ( unit->Flags & UFLAGS_BLOCKTOMARKED )
    {
      if ( unit->BlockFromX != unit->BlockToX || unit->BlockFromY != unit->BlockToY )
      {
        DebugFatal(
          "Unit.CPP: AttackUnit (2)",
          "Unit %d Side %d is waiting with 'To' and 'From' blocks different",
          index,
          side_id);
      }
    }
    return 0;
  }
  if ( !CanUnitAttackTarget(unit, enemy_unit->__PosX / 0x10000, enemy_unit->__PosY / 0x10000, (enemy_unit->Flags & UFLAGS_40_FLYING) != 0) || unit->pos_steps )
  {
    unit->TargetX = enemy_unit->BlockToX;
    unit->TargetY = enemy_unit->BlockToY;
    goto LABEL_21;
  }
  // New logic start
  // Add support for bulk shots
  if ( CanUnitShoot(unit) )
  // New logic end
  {
    v9 = unit->BlockFromY;
    enemy_index = unit->EnemyIndex;
    unit->TargetX = unit->BlockFromX;
    unit->TargetY = v9;
    UnitShootTarget(
      unit,
      side_id,
      index,
      enemy_unit->__PosX / 0x10000,
      enemy_unit->__PosY / 0x10000,
      unit->EnemySide,
      enemy_index,
      1);
  }
  return 1;
}

// Custom implementation of function CanUnitAttackBuilding
DETOUR(0x00497550, 0x004975D0, _Mod__CanUnitAttackBuilding);

bool Mod__CanUnitAttackBuilding(Unit *unit, Building *building, int *tile_x_ptr, int *tile_y_ptr)
{
  int x; // eax
  int y; // ST0C_4 MAPDST

  if ( unit->pos_steps )
  {
    DebugFatal("CanUnitAttackBuilding", "unit->pos.steps not zero");
  }
  GetBuildingInRange(
    (unsigned int)(unit->__PosX / 0x10000) >> 5,
    (unsigned int)(unit->__PosY / 0x10000) >> 5,
    building,
    (char *)&x,
    (char *)&y);
  *tile_x_ptr = x;
  *tile_y_ptr = y;
  return CanUnitAttackTarget(unit, x * 32 + 16, y * 32 + 16, 0);
}

// Custom implementation of function UnitAttackBuilding
DETOUR(0x00496D10, 0x00496FEA, _Mod__UnitAttackBuilding);

char Mod__UnitAttackBuilding(Unit *unit, char side_id, short index)
{
  Unit *unit_; // esi
  char result; // al
  CSide *enemy_side; // edi
  short enemy_index; // ax MAPDST
  Building *targetBuilding; // ebp
  unsigned int xpos; // edi
  char behavior; // al MAPDST
  unsigned int ypos; // ebx
  unsigned char block_to_x; // al
  char enemy_side_id; // ST14_1
  int building_flags; // ecx
  unsigned char v19; // dl MAPDST
  int tile_y; // [esp+10h] [ebp-8h] MAPDST
  int can_attack; // [esp+14h] [ebp-4h]

  unit_ = unit;
  if ( unit->EnemyIndex == -1 )
  {
    if ( unit->pos_steps )
    {
      MoveUnit(unit, side_id, index);
      return 1;
    }
    return 0;
  }
  enemy_side = GetSide(unit->EnemySide);
  if ( unit_->EnemyIndex >= MAX_OBJECTS )
  {
    DebugFatal("AttackUnit", "unit->enemyIndex >= kMaxUnitsPerSide");
  }
  enemy_index = unit_->EnemyIndex;
  targetBuilding = (Building *)&enemy_side->__ObjectArray[enemy_index];
  if ( (CSide *)((char *)enemy_side + 148 * (unsigned short)enemy_index) == (CSide *)-48 )
  {
    DebugFatal("AttackBuilding", "Invalid obj");
    if ( !targetBuilding )
    {
      DebugFatal("AttackBuilding", "Invalid targetBuilding");
    }
  }
  if ( targetBuilding->ObjectType != OBJECT_BUILDING || targetBuilding->__State == BLD_STATE_17_DEAD )
  {
    if ( unit_->pos_steps )
    {
      MoveUnit(unit_, side_id, index);
      return 1;
    }
    return 0;
  }
  xpos = unit_->__PosX / 0x10000;
  behavior = _templates_unitattribs[unit_->Type].__Behavior;
  ypos = unit_->__PosY / 0x10000;
  if ( behavior != UnitBehavior_ENGINEER && behavior != UnitBehavior_SABOTEUR )
  {
    can_attack = CanUnitAttackBuilding(unit_, targetBuilding, (int *)&unit, &tile_y);
    if ( can_attack && !unit_->pos_steps )
    {
      // New logic start
      // Add support for bulk shots
      if ( CanUnitShoot(unit_) )
      // New logic end
      {
        block_to_x = unit_->BlockToX;
        enemy_index = unit_->EnemyIndex;
        unit_->TargetY = unit_->BlockToY;
        unit_->TargetX = block_to_x;
        enemy_side_id = unit_->EnemySide;
        unit = (Unit *)(32 * (_DWORD)unit + 16);
        tile_y = 32 * tile_y + 16;
        UnitShootTarget(unit_, side_id, index, (int)unit, tile_y, enemy_side_id, enemy_index, 2);
      }
      return 1;
    }
LABEL_29:
    GetBuildingInRange(xpos >> 5, ypos >> 5, targetBuilding, (char *)&unit, (char *)&tile_y);
    v19 = tile_y;
    unit_->TargetX = (unsigned int)unit;
    unit_->TargetY = v19;
    MoveUnit(unit_, side_id, index);
    return 1;
  }
  if ( !map_4A47F0(xpos, ypos, targetBuilding, (int *)&unit, &tile_y) || unit_->pos_steps )
  {
    goto LABEL_29;
  }
  building_flags = targetBuilding->Flags;
  if ( building_flags & (BFLAGS_1000000_INFILTRATED|BFLAGS_8_SOLD|BFLAGS_4_SELLING) )
  {
    UnitAdjustState(unit_, UNIT_STATE_2_IDLE);
    result = 1;
  }
  else
  {
    behavior = _templates_unitattribs[unit_->Type].__Behavior;
    if ( behavior != UnitBehavior_ENGINEER || building_flags & BFLAGS_400_CAN_BE_CAPTURED )
    {
      if ( behavior != UnitBehavior_SABOTEUR || building_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
      {
        RemoveUnitTileData(unit_);
        v19 = tile_y;
        unit_->TargetX = (unsigned int)unit;
        unit_->TargetY = v19;
        UnitAdjustState(unit_, UNIT_STATE_23_INFILTRATING);
        unit_->Flags |= UFLAGS_400000;
        targetBuilding->Flags |= BFLAGS_1000000_INFILTRATED;
        UnitUncloakStart(unit_);
        result = 1;
      }
      else
      {
        UnitAdjustState(unit_, UNIT_STATE_2_IDLE);
        result = 1;
      }
    }
    else
    {
      UnitAdjustState(unit_, UNIT_STATE_2_IDLE);
      result = 1;
    }
  }
  return result;
}

// Custom implementation of function UnitAttackTile
DETOUR(0x00496FF0, 0x00497086, _Mod__UnitAttackTile);

char Mod__UnitAttackTile(Unit *unit, char side, short index)
{
  if ( unit->pos_steps || !CanUnitAttackTarget(unit, unit->TargetX * 32 + 16, unit->TargetY * 32 + 16, 0) )
  {
    MoveUnit(unit, side, index);
    return 1;
  }
  // New logic start
  // Add support for bulk shots
  if ( !CanUnitShoot(unit) )
  // New logic end
  {
    return 1;
  }
  if ( unit->BlockFromX == unit->TargetX && unit->BlockFromY == unit->TargetY )
  {
    return 0;
  }
  UnitShootTarget(unit, side, index, 32 * unit->TargetX + 16, 32 * unit->TargetY + 16, SIDE_NONE, -1, 0);
  return 1;
}

// Custom implementation of function UnitShootTarget
DETOUR(0x00495B90, 0x0049602C, _Mod__UnitShootTarget);

void Mod__UnitShootTarget(Unit *unit, char side, unsigned short index, int target_xpos, int target_ypos, char enemySideId, short enemyIndex, char enemy_object_type)
{
  Unit *unit_; // esi
  char unit_is_in_right_direction; // bl
  int unit_xpos; // ebp
  int unit_ypos; // edi
  UnitAtribStruct *unit_template;
  char target_direction; // al MAPDST
  char has_barrel; // cl
  int muzzle_flash_explosion_type; // cl
  short explosion_index; // bx MAPDST
  int direction; // ecx
  int source_xpos; // ebp
  int source_ypos; // edi
  int distance; // ebx
  RECT rect; // [esp+10h] [ebp-10h]
  signed char bullet_type; // [esp+24h] [ebp+4h]
  int inaccuracy;
  int shoot_offset;
  int shoot_angle;
  bool double_shot;
  int distance_squared;
  int dest_xpos;
  int dest_ypos;
  int homing_index;
  int homing_side;
  bool is_muzzle_flash_explosion;
  int bullet_index;

  unit_ = unit;
  unit_is_in_right_direction = 0;
  unit_xpos = unit->__PosX / 0x10000;
  unit_ypos = unit->__PosY / 0x10000;
  unit_template = &_templates_unitattribs[unit_->Type];
  if ( unit_template->__PrimaryWeapon == -1 )
  {
    DebugFatal("ShootTarget", "Unit %s told to shoot", _templates_UnitNameList[unit->Type]);
  }
  if ( enemyIndex > MAX_OBJECTS )
  {
    DebugFatal("UNIT.CPP", "enemyIndex > kMaxUnitsPerSide");
  }
  if ( unit_xpos != target_xpos || unit_ypos != target_ypos )
  {
    target_direction = GetFacing(unit_xpos, unit_ypos, target_xpos, target_ypos);
    has_barrel = unit_template->__HasBarrel;
    if ( has_barrel )
    {
      if ( unit_->__FacingTurret != target_direction )
      {
        goto LABEL_12;
      }
    }
    else if ( unit_->__Facing != target_direction )
    {
      goto LABEL_12;
    }
    unit_is_in_right_direction = 1;
LABEL_12:
    if ( unit_is_in_right_direction )
    {
      if ( UnitIsCloaked(unit_) )
      {
        UnitUncloakStart(unit_);
      }
      else
      {
        // Get distance
        distance_squared = (unit_ypos - target_ypos) * (unit_ypos - target_ypos) + (unit_xpos - target_xpos) * (unit_xpos - target_xpos);
        distance = sqrt((double)distance_squared);
        // Get bullet type
        if (unit->LastUsedWeapon == 1)
          bullet_type = unit_template->__PrimaryWeapon;
        else if (unit->LastUsedWeapon == 2)
          bullet_type = unit_template->__SecondaryWeapon;
        else
          bullet_type = NewBestBullet(
                          unit_template->__PrimaryWeapon,
                          unit_template->__SecondaryWeapon,
                          distance_squared,
                          enemySideId,
                          enemyIndex,
                          enemy_object_type);
        if ( bullet_type < 0 )
        {
          DebugFatal("ShootTarget", "NewBestBullet failed to find weapon");
        }
        // Play sound
        PlaySoundAt(_templates_bulletattribs[bullet_type].__FiringSound, unit_xpos >> 5, unit_ypos >> 5);
        // Reveal unit shooting player's unit
        if ( enemySideId == gSideId )
        {
          if ( _mapvisstate_548010 != GetMapVisState() )
          {
            _cheatstates[gSideId] |= 1u;
          }
          gGameMap.map[unit_->BlockToX + _CellNumbersWidthSpan[unit_->BlockToY]].__shroud = 0;
          rect.left = unit_->BlockToX - 2;
          rect.right = unit_->BlockToX + 2;
          rect.bottom = unit_->BlockToY + 2;
          rect.top = unit_->BlockToY - 2;
          UpdateShroudInRegion(&rect, gGameMap.width, gGameMap.height);
          _mapvisstate_548010 = GetMapVisState();
        }
        // Get stuff
        direction = (unit_template->__BarrelArt == -1)?unit_->__Facing:unit_->__FacingTurret;
        inaccuracy = ((_templates_bulletattribs[bullet_type].Inaccuracy?_templates_bulletattribs[bullet_type].Inaccuracy:16) * distance) / 128;
        if (bullet_type == unit_template->__PrimaryWeapon)
        {
          double_shot = unit_template->PrimaryWeaponDoubleShot;
          shoot_offset = unit_template->PrimaryWeaponShootOffset;
          shoot_angle = unit_template->PrimaryWeaponShootAngle & 31;
        }
        else
        {
          double_shot = unit_template->SecondaryWeaponDoubleShot;
          shoot_offset = unit_template->SecondaryWeaponShootOffset;
          shoot_angle = unit_template->SecondaryWeaponShootAngle & 31;
        }
        homing_index = (enemy_object_type == OBJECT_UNIT)?enemyIndex:-1;
        homing_side = (enemy_object_type == OBJECT_UNIT)?enemySideId:-1;
        muzzle_flash_explosion_type = unit_template->__FireFlashAnim;
        is_muzzle_flash_explosion = (_templates_Explosiondata_AnimationArtFlags[muzzle_flash_explosion_type] & EXPFLAGS_400_MUZZLE_FLASH) != 0;
        // Single shot
        if ( !double_shot )
        {
          source_xpos = (_sinValues[shoot_offset + ((16 - direction) & 31) * 512] / 2048) + unit_xpos;
          source_ypos = (_cosValues[shoot_offset + ((16 - direction) & 31) * 512] / 2048) + unit_ypos;
          dest_xpos = target_xpos;
          dest_ypos = target_ypos;
          if ( inaccuracy )
          {
            dest_xpos += rand() % (2 * inaccuracy) - inaccuracy;
            dest_ypos += rand() % (2 * inaccuracy) - inaccuracy;
          }
          if ((bullet_index = ModelAddBullet(side, bullet_type, 0, index, source_xpos, source_ypos, dest_xpos, dest_ypos, homing_index, homing_side)) != -1)
          {
            Bullet *b = (Bullet *)&GetSide(side)->__ObjectArray[bullet_index];
            if ((muzzle_flash_explosion_type != -1) && ((explosion_index = ModelAddExplosion(side, muzzle_flash_explosion_type, is_muzzle_flash_explosion?unit_xpos:source_xpos, is_muzzle_flash_explosion?unit_ypos:source_ypos, is_muzzle_flash_explosion?0:b->__PosZHeight, 0, 0, 0, 0)) != -1) && is_muzzle_flash_explosion)
            {
              Explosion *e = (Explosion *)&GetSide(side)->__ObjectArray[explosion_index];
              e->__AnimationFrame = direction;
            }
          }
        }
        // Double shot
        else
        {
          // First shot
          source_xpos = (_sinValues[shoot_offset + ((16 - direction + shoot_angle) & 31) * 512] / 2048) + unit_xpos;
          source_ypos = (_cosValues[shoot_offset + ((16 - direction + shoot_angle) & 31) * 512] / 2048) + unit_ypos;
          dest_xpos = target_xpos + (_sinValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction + 8) & 31) * 512] / 2048);
          dest_ypos = target_ypos + (_cosValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction + 8) & 31) * 512] / 2048);
          if ( inaccuracy )
          {
            dest_xpos += rand() % (2 * inaccuracy) - inaccuracy;
            dest_ypos += rand() % (2 * inaccuracy) - inaccuracy;
          }
          if ((bullet_index = ModelAddBullet(side, bullet_type, 0, index, source_xpos, source_ypos, dest_xpos, dest_ypos, homing_index, homing_side)) != -1)
          {
            Bullet *b = (Bullet *)&GetSide(side)->__ObjectArray[bullet_index];
            if ((muzzle_flash_explosion_type != -1) && ((explosion_index = ModelAddExplosion(side, muzzle_flash_explosion_type, is_muzzle_flash_explosion?unit_xpos:source_xpos, is_muzzle_flash_explosion?unit_ypos:source_ypos, is_muzzle_flash_explosion?0:b->__PosZHeight, 0, 0, 0, 0)) != -1) && is_muzzle_flash_explosion)
            {
              Explosion *e = (Explosion *)&GetSide(side)->__ObjectArray[explosion_index];
              e->__AnimationFrame = (direction + shoot_angle) & 31;
            }
          }
          // Second shot
          source_xpos = (_sinValues[shoot_offset + ((16 - direction - shoot_angle) & 31) * 512] / 2048) + unit_xpos;
          source_ypos = (_cosValues[shoot_offset + ((16 - direction - shoot_angle) & 31) * 512] / 2048) + unit_ypos;
          dest_xpos = target_xpos + (_sinValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction - 8) & 31) * 512] / 2048);
          dest_ypos = target_ypos + (_cosValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction - 8) & 31) * 512] / 2048);
          if ( inaccuracy )
          {
            dest_xpos += rand() % (2 * inaccuracy) - inaccuracy;
            dest_ypos += rand() % (2 * inaccuracy) - inaccuracy;
          }
          if ((bullet_index = ModelAddBullet(side, bullet_type, 0, index, source_xpos, source_ypos, dest_xpos, dest_ypos, homing_index, homing_side)) != -1)
          {
            Bullet *b = (Bullet *)&GetSide(side)->__ObjectArray[bullet_index];
            if ((muzzle_flash_explosion_type != -1) && ((explosion_index = ModelAddExplosion(side, muzzle_flash_explosion_type, is_muzzle_flash_explosion?unit_xpos:source_xpos, is_muzzle_flash_explosion?unit_ypos:source_ypos, is_muzzle_flash_explosion?0:b->__PosZHeight, 0, 0, 0, 0)) != -1) && is_muzzle_flash_explosion)
            {
              Explosion *e = (Explosion *)&GetSide(side)->__ObjectArray[explosion_index];
              e->__AnimationFrame = (direction - shoot_angle) & 31;
            }
          }
        }
        // Set unit properties
        unit_->LastFired = 0;
        unit_->Flags |= UFLAGS_2_FIRED;
        unit->__ReloadDelayCounter = (bullet_type == unit_template->__SecondaryWeapon && unit_template->SecondaryWeaponLongDelay)?unit_template->SecondaryWeaponLongDelay:unit_template->PrimaryWeaponLongDelay;
        if (unit->ShotsRemaining)
        {
          unit->ShotsRemaining--;
          if (!unit->ShotsRemaining)
            unit->LastUsedWeapon = 0;
        }
        else
        {
          if (bullet_type == unit_template->__PrimaryWeapon)
          {
            unit->ShotsRemaining = unit_template->PrimaryWeaponBulkShots;
            unit->LastUsedWeapon = 1;
          }
          else
          {
            unit->ShotsRemaining = unit_template->SecondaryWeaponBulkShots;
            unit->LastUsedWeapon = 2;
          }
        }
        if ( unit_template->__IsInfantry )
        {
          SetUnitAnimationType(unit_, 2);
        }
      }
    }
    else if ( unit_template->__HasBarrel )
    {
      TurnUnitBarrelInDirection(unit_, target_direction);
      if ( unit_->Flags & UFLAGS_8000_FIXED_BARREL )
      {
        unit_->__UnitTurnDelayCounter = 0;
        TurnUnitInDirection(unit_, target_direction);
      }
    }
    else
    {
      TurnUnitInDirection(unit_, target_direction);
    }
  }
}
