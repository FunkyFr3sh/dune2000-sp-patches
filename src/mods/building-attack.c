#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

// Added new building shooting features:
// - Bulk shots with customizable number of shots in bulk, short and long delay, for pri and sec weapon independently
// - Double-shot with customizable split angle, settable for pri and sec weapon independently
// - Customizable inaccuracy of weapons (0 = vanilla behavior, 1 = almost accurate, the higher value the less accurate)
// - Shoot offset
// - Non-muzzle-flash explosions can be used as muzzle flash, they will be spawned at the origin of shot bullet

bool CanBuildingShoot(Building *building)
{
  if (!building->__ReloadDelayCounter_refinery)
    return true;
  if (building->ShotsRemaining)
  {
    BuildingAtrbStruct *building_template = &_templates_buildattribs[building->Type];
    if (building->LastUsedWeapon == 1)
      return building->__ReloadDelayCounter_refinery <= building_template->PrimaryWeaponLongDelay - building_template->PrimaryWeaponShortDelay;
    else
      return building->__ReloadDelayCounter_refinery <= (building_template->SecondaryWeaponLongDelay?building_template->SecondaryWeaponLongDelay:building_template->PrimaryWeaponLongDelay) - building_template->SecondaryWeaponShortDelay;
  }
  return false;
}

// Custom implementation of function BuildingAttack
DETOUR(0x00496790, 0x00496AFC, _Mod__BuildingAttack);

char Mod__BuildingAttack(Building *building, char side_id, int building_index, char *enemy_side_id_ptr, short *enemy_index_ptr)
{
  char side_id_; // bl
  Building *bld_; // ebp
  int bld_type; // ecx
  char bld_sec_weapon; // dl
  int enemy_index; // eax
  int bld_xpos; // edi
  int bld_ypos; // esi
  char enemy_side_id_; // cl
  CSide *enemy_side; // eax
  Unit *enemy_object; // ecx
  int enemy_object_type; // eax
  char result; // al
  Unit *enemy_unit; // eax
  char tile_x; // [esp+10h] [ebp-20h]
  char tile_y;
  char enemy_side_id; // [esp+14h] [ebp-1Ch]
  short e_index; // [esp+18h] [ebp-18h]
  int enemy_object_ypos; // [esp+1Ch] [ebp-14h]
  int enemy_object_xpos; // [esp+20h] [ebp-10h]
  char bld_pri_weapon; // [esp+28h] [ebp-8h]
  bool is_ai; // [esp+2Ch] [ebp-4h]

  side_id_ = side_id;
  bld_ = building;
  tile_x = side_id;
  is_ai = _gAIArray[(int)side_id].__IsAI == 0;
  if ( !CanBuildingShoot(building) )
  {
    return 0;
  }
  bld_type = building->Type;
  bld_sec_weapon = _templates_buildattribs[bld_type]._____SecondaryWeapon;
  bld_pri_weapon = _templates_buildattribs[bld_type]._____PrimaryWeapon;
  if ( bld_pri_weapon == -1 && bld_sec_weapon == -1 )
  {
    return 0;
  }
  enemy_index = *enemy_index_ptr;
  bld_xpos = building->__PosX / 0x10000 + 16;
  bld_ypos = building->__PosY / 0x10000 - _templates_buildattribs[bld_type]._____ArtHeight + 16;
  if ( *enemy_index_ptr != -1 )
  {
    enemy_side_id_ = *enemy_side_id_ptr;
    if ( *enemy_side_id_ptr != side_id )
    {
      enemy_side_id = *enemy_side_id_ptr;
      e_index = enemy_index;
      enemy_side = GetSide((eSideType)enemy_side_id_);
      enemy_object = &enemy_side->__ObjectArray[*enemy_index_ptr];
      if ( (CSide *)((char *)enemy_side + 148 * *enemy_index_ptr) == (CSide *)-48 )
      {
        return 0;
      }
      enemy_object_type = enemy_side->__ObjectArray[*enemy_index_ptr].ObjectType;
      if ( enemy_object_type == OBJECT_UNIT )
      {
        enemy_object_xpos = enemy_object->__PosX / 0x10000;
        enemy_object_ypos = enemy_object->__PosY / 0x10000;
        if ( CanBuildingAttackTile(bld_, enemy_object_xpos / 32, enemy_object_ypos / 32, 0) )
        {
          BuildingShootTarget(
            bld_,
            side_id,
            building_index,
            enemy_object_xpos,
            enemy_object_ypos,
            enemy_side_id,
            e_index,
            1);
          return 1;
        }
        side_id_ = tile_x;
      }
      else if ( enemy_object_type == OBJECT_BUILDING
             && _templates_buildattribs[LOBYTE(enemy_object->__PosX)].__Behavior == BuildingBehavior_TURRET )
      {
        GetBuildingInRange(bld_xpos / 32, bld_ypos / 32, (Building *)enemy_object, &tile_x, &tile_y);
        if ( CanBuildingAttackTile(bld_, tile_x, tile_y, 0) )
        {
          BuildingShootTarget(
            bld_,
            side_id,
            building_index,
            32 * tile_x + 16,
            32 * tile_y + 16,
            enemy_side_id,
            e_index,
            2);
          return 1;
        }
      }
    }
  }
  if ( EnemyUnitInRange(
         bld_xpos,
         bld_ypos,
         side_id_,
         0,
         &enemy_object_xpos,
         &enemy_object_ypos,
         (_BYTE *)&enemy_side_id,
         &e_index,
         bld_pri_weapon,
         bld_sec_weapon,
         0,
         1,
         1) )
  {
    enemy_unit = GetUnit(enemy_side_id, e_index);
    if ( enemy_unit )
    {
      if ( _templates_unitattribs[enemy_unit->Type].__Behavior == UnitBehavior_SANDWORM
        && GetUnitOnTile(enemy_object_xpos, enemy_object_ypos, (eSideType *)&building, (_WORD *)&tile_x, 0) )
      {
        return 1;
      }
      BuildingShootTarget(
        bld_,
        side_id,
        building_index,
        enemy_object_xpos,
        enemy_object_ypos,
        enemy_side_id,
        e_index,
        1);
    }
    goto LABEL_23;
  }
  if ( EnemyBuildingInRange(
         bld_xpos,
         bld_ypos,
         (eSideType)side_id_,
         &enemy_object_xpos,
         &enemy_object_ypos,
         &enemy_side_id,
         &e_index,
         bld_pri_weapon,
         bld_sec_weapon,
         is_ai) )
  {
    BuildingShootTarget(
      bld_,
      side_id,
      building_index,
      enemy_object_xpos,
      enemy_object_ypos,
      enemy_side_id,
      e_index,
      2);
LABEL_23:
    *enemy_side_id_ptr = enemy_side_id;
    result = 1;
    *enemy_index_ptr = e_index;
    return result;
  }
  return 0;
}

// Custom implementation of function BuildingAttackUnit
DETOUR(0x004975D0, 0x004976A8, _Mod__BuildingAttackUnit);

char Mod__BuildingAttackUnit(Building *building, char side_id, short building_index)
{
  CSide *enemy_side; // edi
  short enemy_index; // ax
  Unit *enemy_unit; // edi

  enemy_side = GetSide(building->EnemySide);
  enemy_index = building->EnemyIndex;
  if ( enemy_index == -1 )
  {
    return 0;
  }
  if ( enemy_index >= 1000 )
  {
    DebugFatal("AttackUnit", "unit->enemyIndex >= kMaxUnitsPerSide");
  }
  enemy_unit = &enemy_side->__ObjectArray[building->EnemyIndex];
  if ( !enemy_unit
    || enemy_unit->ObjectType != OBJECT_UNIT
    || enemy_unit->State == UNIT_STATE_17_DEAD
    || !CanBuildingAttackTile(
          building,
          enemy_unit->BlockFromX,
          enemy_unit->BlockFromY,
          (enemy_unit->Flags & UFLAGS_40_FLYING) > 0) )
  {
    return 0;
  }
  if ( CanBuildingShoot(building) )
  {
    BuildingShootTarget(
      building,
      side_id,
      building_index,
      enemy_unit->__PosX / 0x10000,
      enemy_unit->__PosY / 0x10000,
      building->EnemySide,
      building->EnemyIndex,
      1);
  }
  return 1;
}

// Custom implementation of function BuildingAttackBuilding
DETOUR(0x004976B0, 0x0049779B, _Mod__BuildingAttackBuilding);

char Mod__BuildingAttackBuilding(Building *building, char side_id, short building_index)
{
  Building *bld; // esi
  CSide *enemy_side; // edi
  short enemy_index; // ax MAPDST
  int target_xpos;
  int targey_ypos; // [esp+8h] [ebp-4h]

  bld = building;
  enemy_side = GetSide(building->EnemySide);
  enemy_index = bld->EnemyIndex;
  if ( enemy_index == -1 )
  {
    return 0;
  }
  if ( enemy_index >= 1000 )
  {
    DebugFatal("AttackUnit", "unit->enemyIndex >= kMaxUnitsPerSide");
  }
  if ( (CSide *)((char *)enemy_side + 148 * bld->EnemyIndex) == (CSide *)-48 )
  {
    return 0;
  }
  if ( enemy_side->__ObjectArray[bld->EnemyIndex].ObjectType != OBJECT_BUILDING )
  {
    return 0;
  }
  Building *b = (Building *)&enemy_side->__ObjectArray[bld->EnemyIndex];
  if ( b->__State == BLD_STATE_17_DEAD )
  {
    return 0;
  }
  if ( IsBuildingInRange(
         bld,
         (Building *)&enemy_side->__ObjectArray[bld->EnemyIndex],
         &target_xpos,
         &targey_ypos) )
  {
    if ( CanBuildingShoot(bld) )
    {
      target_xpos = 32 * target_xpos + 16;
      targey_ypos = 32 * targey_ypos + 16;
      BuildingShootTarget(bld, side_id, building_index, target_xpos, targey_ypos, bld->EnemySide, bld->EnemyIndex, 2);
      return 1;
    }
  }
  else
  {
    DebugFatal("AttackUnit", "Building out of range - should never happen");
  }
  return 1;
}

// Custom implementation of function BuildingAttackTile
DETOUR(0x00497090, 0x004970EB, _Mod__BuildingAttackTile);

char Mod__BuildingAttackTile(Building *building, eSideType side_id, short building_index)
{
  if ( CanBuildingAttackTile(building, building->__AttackTileX, building->__AttackTileY, 0) )
  {
    if ( CanBuildingShoot(building) )
    {
      BuildingShootTarget(
        building,
        side_id,
        building_index,
        32 * building->__AttackTileX + 16,
        32 * building->__AttackTileY + 16,
        -1,
        -1,
        0);
    }
  }
  return 1;
}

// Custom implementation of function BuildingShootTarget
DETOUR(0x00496030, 0x0049647A, _Mod__BuildingShootTarget);

void Mod__BuildingShootTarget(Building *building, char side_id, short index, int target_xpos, int target_ypos, char enemy_side_id, short enemy_index, int enemy_object_type)
{
  Building *building_; // esi
  BuildingAtrbStruct *building_template;
  int building_xpos; // ebp
  int building_ypos; // edi
  int distance; // ebx
  int muzzle_flash_explosion_type;
  short explosion_index; // bx
  int direction;
  unsigned char building_x; // [esp+14h] [ebp-1Ch]
  unsigned char building_y; // [esp+18h] [ebp-18h]
  RECT rect; // [esp+20h] [ebp-10h]
  unsigned char taget_facing; // [esp+34h] [ebp+4h]
  int bullet_type; // [esp+34h] [ebp+4h]
  int inaccuracy;
  int shoot_offset;
  int shoot_angle;
  bool double_shot;
  int distance_squared;
  int source_xpos;
  int source_ypos;
  int dest_xpos;
  int dest_ypos;
  int homing_index;
  int homing_side;
  bool is_muzzle_flash_explosion;
  int y_offset;
  int bullet_index;

  building_ = building;
  building_template = &_templates_buildattribs[building_->Type];
  if ( building_template->_____PrimaryWeapon == -1 )
  {
    DebugFatal("ShootTarget", "Building %s told to shoot", _templates_BuildingNameList[building->Type]);
  }
  building_xpos = building->__PosX / 0x10000 + 16;
  building_ypos = building->__PosY / 0x10000 - building_template->_____ArtHeight + 16;
  taget_facing = GetFacing(building_xpos, building_ypos, target_xpos, target_ypos);
  if ( building_template->_____ActLikeTurret )
  {
    if ( GetSide(side_id)->__PowerPercent >= 100u || !building_template->_____RequireEnoughPower )
    {
      if ( building_->__Facing == taget_facing )
      {
        // Get distance
        distance_squared = (building_ypos - target_ypos) * (building_ypos - target_ypos) + (building_xpos - target_xpos) * (building_xpos - target_xpos);
        distance = sqrt((double)distance_squared);
        // Get bullet type
        if (building->LastUsedWeapon == 1)
          bullet_type = building_template->_____PrimaryWeapon;
        else if (building->LastUsedWeapon == 2)
          bullet_type = building_template->_____SecondaryWeapon;
        else
          bullet_type = NewBestBullet(
                          building_template->_____PrimaryWeapon,
                          building_template->_____SecondaryWeapon,
                          distance_squared,
                          building_->EnemySide,
                          building_->EnemyIndex,
                          enemy_object_type);
        if ( bullet_type < 0 )
        {
          DebugFatal("ShootTarget", "NewBestBullet failed to find weapon");
        }
        // Play sound
        PlaySoundAt(_templates_bulletattribs[bullet_type].__FiringSound, building_xpos >> 5, building_ypos >> 5);
        // Reveal building shooting player's unit
        if ( enemy_side_id == gSideId )
        {
          building_y = building_ypos >> 5;
          building_x = building_xpos >> 5;
          if ( _mapvisstate_548010 != GetMapVisState() )
          {
            _cheatstates[gSideId] |= 1u;
          }
          gGameMap.map[building_x + _CellNumbersWidthSpan[building_y]].__shroud = 0;
          rect.left = building_x - 2;
          rect.bottom = building_y + 2;
          rect.right = building_x + 2;
          rect.top = building_y - 2;
          UpdateShroudInRegion(&rect, gGameMap.width, gGameMap.height);
          _mapvisstate_548010 = GetMapVisState();
        }
        // Get stuff
        direction = building_->__Facing;
        inaccuracy = ((_templates_bulletattribs[bullet_type].Inaccuracy?_templates_bulletattribs[bullet_type].Inaccuracy:16) * distance) / 128;
        if (bullet_type == building_template->_____PrimaryWeapon)
        {
          double_shot = building_template->PrimaryWeaponDoubleShot;
          shoot_offset = building_template->PrimaryWeaponShootOffset;
          shoot_angle = building_template->PrimaryWeaponShootAngle & 31;
        }
        else
        {
          double_shot = building_template->SecondaryWeaponDoubleShot;
          shoot_offset = building_template->SecondaryWeaponShootOffset;
          shoot_angle = building_template->SecondaryWeaponShootAngle & 31;
        }
        homing_index = (enemy_object_type == OBJECT_UNIT)?enemy_index:-1;
        homing_side = (enemy_object_type == OBJECT_UNIT)?enemy_side_id:-1;
        muzzle_flash_explosion_type = building_template->_____MuzzleFlashExplosion;
        is_muzzle_flash_explosion = (_templates_Explosiondata_AnimationArtFlags[muzzle_flash_explosion_type] & EXPFLAGS_400_MUZZLE_FLASH) != 0;
        y_offset = 0;
        if (building_template->_____BarrelArt != -1)
          y_offset = gBuildingElements[building_template->_____BarrelArt][(int)building_template->_____DirectionFrames[building_->__Facing]]->c_field_1A__Width2 >> 1;
        // Single shot
        if ( !double_shot )
        {
          source_xpos = (_sinValues[shoot_offset + ((16 - direction) & 31) * 512] / 2048) + building_xpos + building_template->_____ExitPoint2X;
          source_ypos = (_cosValues[shoot_offset + ((16 - direction) & 31) * 512] / 2048) + building_ypos + building_template->_____ExitPoint2Y;
          dest_xpos = target_xpos;
          dest_ypos = target_ypos;
          if ( inaccuracy )
          {
            dest_xpos += rand() % (2 * inaccuracy) - inaccuracy;
            dest_ypos += rand() % (2 * inaccuracy) - inaccuracy;
          }
          if ((bullet_index = ModelAddBullet(side_id, bullet_type, 0, index, source_xpos, source_ypos, dest_xpos, dest_ypos, homing_index, homing_side)) != -1)
          {
            Bullet *b = (Bullet *)&GetSide(side_id)->__ObjectArray[bullet_index];
            if ((muzzle_flash_explosion_type != -1) && ((explosion_index = ModelAddExplosion(side_id, muzzle_flash_explosion_type, is_muzzle_flash_explosion?building_xpos:source_xpos, is_muzzle_flash_explosion?(int)building->__PosY/0x10000-y_offset:source_ypos, is_muzzle_flash_explosion?0:b->__PosZHeight, 0, 0, 0, 0)) != -1) && is_muzzle_flash_explosion)
            {
              Explosion *e = (Explosion *)&GetSide(side_id)->__ObjectArray[explosion_index];
              e->__AnimationFrame = direction;
            }
          }
        }
        // Double shot
        else
        {
          // First shot
          source_xpos = (_sinValues[shoot_offset + ((16 - direction + shoot_angle) & 31) * 512] / 2048) + building_xpos + building_template->_____ExitPoint2X;
          source_ypos = (_cosValues[shoot_offset + ((16 - direction + shoot_angle) & 31) * 512] / 2048) + building_ypos + building_template->_____ExitPoint2Y;
          dest_xpos = target_xpos + (_sinValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction + 8) & 31) * 512] / 2048);
          dest_ypos = target_ypos + (_cosValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction + 8) & 31) * 512] / 2048);
          if ( inaccuracy )
          {
            dest_xpos += rand() % (2 * inaccuracy) - inaccuracy;
            dest_ypos += rand() % (2 * inaccuracy) - inaccuracy;
          }
          if ((bullet_index = ModelAddBullet(side_id, bullet_type, 0, index, source_xpos, source_ypos, dest_xpos, dest_ypos, homing_index, homing_side)) != -1)
          {
            Bullet *b = (Bullet *)&GetSide(side_id)->__ObjectArray[bullet_index];
            if ((muzzle_flash_explosion_type != -1) && ((explosion_index = ModelAddExplosion(side_id, muzzle_flash_explosion_type, is_muzzle_flash_explosion?building_xpos:source_xpos, is_muzzle_flash_explosion?(int)building->__PosY/0x10000-y_offset:source_ypos, is_muzzle_flash_explosion?0:b->__PosZHeight, 0, 0, 0, 0)) != -1) && is_muzzle_flash_explosion)
            {
              Explosion *e = (Explosion *)&GetSide(side_id)->__ObjectArray[explosion_index];
              e->__AnimationFrame = (direction + shoot_angle) & 31;
            }
          }
          // Second shot
          source_xpos = (_sinValues[shoot_offset + ((16 - direction - shoot_angle) & 31) * 512] / 2048) + building_xpos + building_template->_____ExitPoint2X;
          source_ypos = (_cosValues[shoot_offset + ((16 - direction - shoot_angle) & 31) * 512] / 2048) + building_ypos + building_template->_____ExitPoint2Y;
          dest_xpos = target_xpos + (_sinValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction - 8) & 31) * 512] / 2048);
          dest_ypos = target_ypos + (_cosValues[(shoot_angle * shoot_offset) / 8 + ((16 - direction - 8) & 31) * 512] / 2048);
          if ( inaccuracy )
          {
            dest_xpos += rand() % (2 * inaccuracy) - inaccuracy;
            dest_ypos += rand() % (2 * inaccuracy) - inaccuracy;
          }
          if ((bullet_index = ModelAddBullet(side_id, bullet_type, 0, index, source_xpos, source_ypos, dest_xpos, dest_ypos, homing_index, homing_side)) != -1)
          {
            Bullet *b = (Bullet *)&GetSide(side_id)->__ObjectArray[bullet_index];
            if ((muzzle_flash_explosion_type != -1) && ((explosion_index = ModelAddExplosion(side_id, muzzle_flash_explosion_type, is_muzzle_flash_explosion?building_xpos:source_xpos, is_muzzle_flash_explosion?(int)building->__PosY/0x10000-y_offset:source_ypos, is_muzzle_flash_explosion?0:b->__PosZHeight, 0, 0, 0, 0)) != -1) && is_muzzle_flash_explosion)
            {
              Explosion *e = (Explosion *)&GetSide(side_id)->__ObjectArray[explosion_index];
              e->__AnimationFrame = (direction - shoot_angle) & 31;
            }
          }
        }
        // Set building properties
        building->__ReloadDelayCounter_refinery = (bullet_type == building_template->_____SecondaryWeapon && building_template->SecondaryWeaponLongDelay)?building_template->SecondaryWeaponLongDelay:building_template->PrimaryWeaponLongDelay;
        if (building->ShotsRemaining)
        {
          building->ShotsRemaining--;
          if (!building->ShotsRemaining)
            building->LastUsedWeapon = 0;
        }
        else
        {
          if (bullet_type == building_template->_____PrimaryWeapon)
          {
            building->ShotsRemaining = building_template->PrimaryWeaponBulkShots;
            building->LastUsedWeapon = 1;
          }
          else
          {
            building->ShotsRemaining = building_template->SecondaryWeaponBulkShots;
            building->LastUsedWeapon = 2;
          }
        }
      }
      else
      {
        TurnBuildingTurret(building_, taget_facing);
      }
    }
  }
}
