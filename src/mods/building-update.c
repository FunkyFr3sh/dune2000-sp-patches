#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function UpdateBuilding
DETOUR(0x0049B7F0, 0x0049BE2E, _Mod__UpdateBuilding);

bool Mod__UpdateBuilding(Building *bld, unsigned char side_id, short building_index)
{
  Building *bld_; // esi
  int bld_flags; // ecx MAPDST
  int building_type; // edx
  unsigned char buildup_anim_counter; // al MAPDST
  int building_type_; // edi MAPDST
  short enemy_index; // di
  int v14; // eax
  Building *enemy_building; // eax
  Unit *unit; // eax
  UnitFlags unit_flags; // eax
  int bld_flags_; // ecx
  unsigned char side_id_; // bl
  int building_health; // ecx
  bool result; // al
  unsigned char last_attacked; // dl
  unsigned int counter; // ebx
  int building_art; // eax
  TImage *image; // ebp
  char can_attack; // al
  char dead_state_time_counter; // al
  CSide *side; // eax
  int arg0a; // [esp+14h] [ebp+4h]

  bld_ = bld;
  bld_flags = bld->Flags;
  result = 0;
  // Handle buildup and building animation
  if ( bld_flags & (BFLAGS_4_SELLING|BFLAGS_2_BUILDUP) )
  {
    if ( bld_flags & BFLAGS_2_BUILDUP )
    {
      building_type = bld->Type;
      buildup_anim_counter = bld->__BuildupAnimCounter + 1;
      bld->__BuildupAnimCounter = buildup_anim_counter;
      if ( buildup_anim_counter / 3 < (unsigned char)_templates_BuildupArtFrames[(unsigned char)_templates_buildattribs[building_type]._____BuildupArt] )
      {
        goto LABEL_17;
      }
      bld_flags &= ~(BFLAGS_4_SELLING|BFLAGS_2_BUILDUP);
      goto LABEL_16;
    }
    buildup_anim_counter = bld->__BuildupAnimCounter;
    if ( buildup_anim_counter == 1
      || (_templates_buildattribs[bld->Type].__Behavior == BuildingBehavior_CONSTRUCTION && buildup_anim_counter == 60) )
    {
      SetBuildingState(bld, BLD_STATE_17_DEAD);
      bld_flags = bld->Flags;
      bld_flags = bld_flags | BFLAGS_8_SOLD;
      bld->Flags = bld_flags;
    }
    else
    {
      bld->__BuildupAnimCounter = buildup_anim_counter - 1;
    }
  }
  // New logic start
  // Allow permanent animation for factories
  else if ( (bld_flags & (BFLAGS_20_ANIM_ONCE|BFLAGS_10_ANIM_PERMANENT)) || (_templates_buildattribs[bld->Type]._____Flags & BFLAGS_10_ANIM_PERMANENT) )
  // New logic end
  {
    if ( bld_flags & BFLAGS_40_HAS_ANIMATION )
    {
      building_type_ = 268 * bld->Type;
      if ( bld->Health >= (signed int)(*(unsigned int *)((char *)&_templates_buildattribs[0]._____HitPoints
                                                       + building_type_) >> 1) )
      {
        buildup_anim_counter = bld->__BuildingAnimCounter + 1;
        bld->__BuildingAnimCounter = buildup_anim_counter;
        if ( buildup_anim_counter
           / (signed int)*((unsigned char *)&_templates_buildattribs[0]._____AnimationSpeed + building_type_) >= (unsigned char)_templates_BuildingAnimationFrames[*((unsigned char *)&_templates_buildattribs[0]._____BuildingAnimation + building_type_)] )
        {
          bld->__BuildingAnimCounter = 0;
          if ( bld_flags & BFLAGS_20_ANIM_ONCE )
          {
            bld_flags &= ~BFLAGS_20_ANIM_ONCE;
LABEL_16:
            bld->Flags = bld_flags;
            goto LABEL_17;
          }
        }
      }
    }
  }
LABEL_17:
  if ( bld->__ReloadDelayCounter_refinery )
  {
    bld->__ReloadDelayCounter_refinery--;
  }
  // New logic start
  // New building shooting features
  if ( bld->__ReloadDelayCounter_refinery == 0 )
  {
    bld->ShotsRemaining = 0;
    bld->LastUsedWeapon = 0;
  }
  // New logic end
  // Clear enemy index if enemy building/unit is dead or not targetable
  if ( bld->EnemyIndex != -1 )
  {
    enemy_index = bld->EnemyIndex;
    v14 = GetSide(bld->EnemySide)->__ObjectArray[(unsigned short)enemy_index].ObjectType - 1;
    if ( v14 )
    {
      if ( v14 != 1
        || ((enemy_building = GetBuilding((eSideType)bld->EnemySide, enemy_index)) != 0
        && enemy_building->__State == BLD_STATE_17_DEAD ))
      {
LABEL_30:
        bld->EnemyIndex = -1;
        goto LABEL_31;
      }
    }
    else
    {
      unit = GetUnit((eSideType)bld->EnemySide, enemy_index);
      if ( unit )
      {
        if ( unit->State == UNIT_STATE_17_DEAD )
        {
          goto LABEL_30;
        }
        unit_flags = unit->Flags;
        if ( unit_flags & (UFLAGS_200000_NOT_DRAWN|UFLAGS_4_CLOAKED)
          || ((unit_flags & UFLAGS_100_CARRYING) && !(unit_flags & UFLAGS_40_FLYING)) )
        {
          goto LABEL_30;
        }
      }
    }
  }
LABEL_31:
  bld_flags_ = bld->Flags;
  // Mark building as it needs to release infantry during selling
  if ( !(bld_flags_ & BFLAGS_80000_INFANTRY_RELEASED) )
  {
    if ( bld_flags_ & BFLAGS_4_SELLING )
    {
      if ( bld->__BuildupAnimCounter / 3 == _templates_buildattribs[bld->Type]._____BuildupFramesToShow )
      {
        bld->Flags = bld_flags_ | BFLAGS_40000_NEED_RELEASE_INFANTRY;
      }
    }
  }
  bld_flags = bld->Flags;
  // Release infantry during selling
  if ( !(bld_flags & BFLAGS_40000_NEED_RELEASE_INFANTRY) || bld_flags & BFLAGS_80000_INFANTRY_RELEASED )
  {
    side_id_ = side_id;
  }
  else
  {
    side_id_ = side_id;
    ReleaseInfantryFromBuilding(bld, (eSideType)side_id);
    bld->Flags &= ~BFLAGS_40000_NEED_RELEASE_INFANTRY;
  }
  building_type_ = bld->Type;
  building_health = bld->Health;
  // Decay building
  if ( building_health > (signed int)((unsigned int)_templates_buildattribs[building_type_]._____HitPoints >> 1) )
  {
    bld_flags = bld->Flags;
    if ( (bld_flags & 0x8000) != 0 && !(gGameTicks & 0xF) )
    {
      bld->Health = building_health - 1;
    }
  }
  bld_flags = bld->Flags;
  if ( !(bld_flags & BFLAGS_8_SOLD) || bld_flags & BFLAGS_20000_WAS_SOLD )
  {
    arg0a = (unsigned short)building_index;
    if ( !(((unsigned char)building_index ^ (unsigned char)gGameTicks) & 7) )
    {
      last_attacked = 255;
      if ( (unsigned char)bld_->__LastAttacked + 1 <= 255 )
      {
        last_attacked = bld_->__LastAttacked + 1;
      }
      bld_->__LastAttacked = last_attacked;
    }
    if ( bld_flags & BFLAGS_1000_WAS_ATTACKED )
    {
      bld_flags &= ~BFLAGS_1000_WAS_ATTACKED;
      bld_->__LastAttacked = 0;
      bld_->Flags = bld_flags;
    }
    // Do smoke animations
    if ( bld_->__MakeSmokeTimeCounter )
    {
      bld_flags = _templates_buildattribs[building_type_]._____Flags;
      if ( bld_flags & BFLAGS_200_SELECTABLE_REPAIRABLE )
      {
        if ( bld_->__State != BLD_STATE_17_DEAD && !(bld_->Flags & (BFLAGS_4_SELLING|BFLAGS_2_BUILDUP)) )
        {
          counter = 0;
          if ( _SmokeDataStructs[building_type_].__AnimCount )
          {
            do
            {
              building_art = _templates_buildattribs[building_type_]._____BuildingArt;
              image = gBuildingElements[building_art][0];
              if ( !image )
              {
                image = dataimage[building_art];
              }
              if ( !((gGameTicks + (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6483) & 1u))
                   % (unsigned char)_SmokeDataStructs[bld_->Type].Frames[counter]) )
              {
                if ( (1 << counter) & bld_->__SmokeAnimActiveBitfield )
                {
                  ModelAddExplosion(
                    (eSideType)side_id,
                    _SmokeDataStructs[bld_->Type].__Explosion[counter],
                    bld_->__PosX / 0x10000 + _SmokeDataStructs[bld_->Type].__AnimOffsetX[counter],
                    bld_->__PosY / 0x10000
                  + _SmokeDataStructs[bld_->Type].__AnimOffsetY[counter]
                  - (unsigned char)image->c_field_19__Height2,
                    0,
                    0,
                    0,
                    0,
                    0);
                }
              }
              if (( !(GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 6487) & 0x7F)
                && bld_->__MakeSmokeTimeCounter < 0x32u)
                || bld_->Health > (signed int)((unsigned int)_templates_buildattribs[bld_->Type]._____HitPoints >> 1) )
              {
                bld_->__SmokeAnimActiveBitfield &= ~(1 << counter);
              }
              building_type_ = bld_->Type;
              ++counter;
            }
            while ( counter < (unsigned char)_SmokeDataStructs[building_type_].__AnimCount );
          }
          if ( !(gGameTicks & 3) )
          {
            --bld_->__MakeSmokeTimeCounter;
          }
          side_id_ = side_id;
        }
      }
    }
    switch ( bld_->__State )
    {
      case BLD_STATE_2_IDLE:
        if ( _templates_buildattribs[bld_->Type]._____PrimaryWeapon == -1
          || (((arg0a >> 31) ^ (abs((_BYTE)arg0a) & 7)) - (arg0a >> 31) != ((int)gGameTicks & 7) && bld_->EnemyIndex == -1) )
        {
          goto LABEL_92;
        }
        BuildingAttack(bld_, side_id_, building_index, &bld_->EnemySide, &bld_->EnemyIndex);
        return 1;
      case BLD_STATE_3:
      case BLD_STATE_7:
      case BLD_STATE_8:
      case BLD_STATE_9:
      case BLD_STATE_10:
      case BLD_STATE_11:
      case BLD_STATE_12:
      case BLD_STATE_13:
      case BLD_STATE_14:
      case BLD_STATE_15:
      case BLD_STATE_16:
        DebugFatal("UpdateUnit", "Invalid building state %d", bld_->__State);
        goto LABEL_92;
      case BLD_STATE_4_ATTACKING_UNIT:
        can_attack = BuildingAttackUnit(bld_, side_id_, building_index);
        goto LABEL_77;
      case BLD_STATE_5_ATTACKING_BUILDING:
        can_attack = BuildingAttackBuilding(bld_, side_id_, building_index);
LABEL_77:
        if ( can_attack )
        {
LABEL_92:
          result = 1;
        }
        else
        {
          SetBuildingState(bld_, BLD_STATE_2_IDLE);
          result = 1;
        }
        break;
      case BLD_STATE_6_ATTACKING_TILE:
        BuildingAttackTile(bld_, (eSideType)side_id_, building_index);
        result = 1;
        break;
      case BLD_STATE_17_DEAD:
        if ( !(bld_->Flags & BFLAGS_10000_DONT_REMOVE_STUFF) && bld_->__DeadStateTimeCounter == 25 )
        {
          ReleaseInfantryFromBuilding(bld_, (eSideType)side_id_);
        }
        dead_state_time_counter = bld_->__DeadStateTimeCounter;
        if ( dead_state_time_counter )
        {
          bld_->__DeadStateTimeCounter = dead_state_time_counter - 1;
          result = 1;
        }
        else
        {
          if ( _templates_buildattribs[bld_->Type].GroupType == _templates_GroupIDs.Barrel
          // New logic start
          // Exploding barrel-type building should not make damage when it is sold or captured
            && (bld_->Flags & (BFLAGS_4_SELLING | BFLAGS_1000000_INFILTRATED)) == 0
          // New logic end
            && _templates_GroupIDs.BARREL != -1 )
          {
            DamageTiles(
              // New logic start
              // Fix misplaced exploding barrel damage
              bld_->__PosX / 0x10000 + 16,
              bld_->__PosY / 0x10000 - 16,
              // New logic end
              0,
              _templates_GroupIDs.BARREL,
              (eSideType)side_id_,
              0xFFFF,
              0);
          }
          side = GetSide(side_id_);
          CSide__FindOtherPrimaryBuilding(side, bld_->Type, building_index);
          if ( !(bld_->Flags & BFLAGS_10000_DONT_REMOVE_STUFF) )
          {
            RemoveBuildingStuff(
              bld_->Type,
              bld_->__PosX / 0x10000 / 32,
              (bld_->__PosY / 0x10000 - _templates_buildattribs[bld_->Type]._____ArtHeight) / 32,
              (eSideType)side_id_);
          }
          result = 0;
        }
        break;
    }
  }
  else
  {
    GetSide(side_id_);
    SetBuildingState17andFlags20000(bld);
    result = 1;
  }
  return result;
}
