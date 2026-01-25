#include "macros/patch.h"
#include "dune2000.h"
#include "extended-templates.h"

// Custom implementation of function DestroyUnit
DETOUR(0x0049CDE0, 0x0049D162, _Mod__DestroyUnit);

void Mod__DestroyUnit(eSideType side_id, short index)
{
  CSide *side; // ebp
  Unit *unit; // eax MAPDST
  UnitFlags unit_flags; // eax
  short carried_unit; // ax
  signed int current_anim_type; // eax
  char random_death_animation; // al
  CSide *v10; // edi
  int destroy_anim; // edi
  unsigned int v12; // eax MAPDST
  unsigned int random_sound; // eax
  int y; // ST20_4
  int x; // ST1C_4
  int sound_id; // eax
  char behavior; // al
  unsigned char pickup_queue_num; // [esp+10h] [ebp-1Ch] MAPDST
  ExplosionFlags add_explosion_extra_flags; // [esp+14h] [ebp-18h]
  char Buffer; // [esp+18h] [ebp-14h]

  side = GetSide(side_id);
  add_explosion_extra_flags = 0;
  unit = GetUnit(side_id, index);
  if ( unit )
  {
    unit_flags = unit->Flags;
    if ( unit_flags & UFLAGS_40_FLYING )
    {
      add_explosion_extra_flags = EXPFLAGS_1000;
    }
    else if ( unit_flags & UFLAGS_100_CARRYING )
    {
      unit->Flags = unit_flags | UFLAGS_400000;
    }
    if ( (unit->Flags & (UFLAGS_100_CARRYING|UFLAGS_40_FLYING)) == (UFLAGS_100_CARRYING|UFLAGS_40_FLYING)
      && _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_CARRYALL )
    {
      carried_unit = unit->EnemyIndex;
      if ( carried_unit != -1 )
      {
        DestroyUnit(side_id, carried_unit);
      }
    }
    if ( _templates_unitattribs[unit->Type].__IsInfantry )
    {
      current_anim_type = unit->__CurrentAnimType;
      if ( current_anim_type < 9 || current_anim_type > 13 )
      {
        random_death_animation = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7122);
        SetUnitAnimationType(unit, (random_death_animation & 3) + 9);
      }
    }
    if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_CARRYALL )
    {
      pickup_queue_num = 0;
      pickup_queue_num = 0;
      if ( side->__PickupQueueCount )
      {
        do
        {
          v10 = (CSide *)((char *)side + 20 * pickup_queue_num);
          if ( side->__PickupQueue[pickup_queue_num].__state == 2 && v10->__PickupQueue[0].__carryall_index == index )
          {
            if ( v10->__PickupQueue[0].__unit_index == -1 )
            {
              DebugFatal("Unit.cpp", "unitIndex out of range (1)");
            }
            UnitAdjustState(
              &side->__ObjectArray[(unsigned short)v10->__PickupQueue[0].__unit_index],
              (eUnitState)v10->__PickupQueue[0].__unit_state);
            v10->__PickupQueue[0].__state = 3;
          }
          ++pickup_queue_num;
        }
        while ( pickup_queue_num < side->__PickupQueueCount );
      }
    }
    destroy_anim = _templates_unitattribs[unit->Type].__DestroyAnim;
    if ( destroy_anim >= 0 && unit->State != UNIT_STATE_17_DEAD )
    {
      v12 = (unit->__Facing << 9) + ((unsigned int)(32 * unit->Speed) >> 16);
      ModelAddExplosion(
        side_id,
        _templates_GroupIDs.EX_WHITEN1,
        unit->__PosX / 0x10000,
        unit->__PosY / 0x10000,
        0,
        add_explosion_extra_flags,
        0,
        _sinValues[v12],
        -_cosValues[v12]);
      PlaySoundAt(
        _templates_explosionattribs[destroy_anim].__Sound,
        unit->__PosX / 0x10000 / 32,
        unit->__PosY / 0x10000 / 32);
      v12 = (unit->__Facing << 9) + ((unsigned int)(32 * unit->Speed) >> 16);
      ModelAddExplosion(
        side_id,
        destroy_anim,
        unit->__PosX / 0x10000,
        unit->__PosY / 0x10000,
        unit->__PosZHeight,
        add_explosion_extra_flags,
        0,
        _sinValues[v12],
        -_cosValues[v12]);
    }
    if ( _templates_unitattribs[unit->Type].__IsInfantry )
    {
      if ( unit->State != UNIT_STATE_23_INFILTRATING )
      {
        // New logic start
        // Add customizable infantry death sound
        if (_templates_unitattribs[unit->Type].CustomDeathSoundNumber)
        {
          sound_id = _templates_unitattribs[unit->Type].CustomDeathSound + rand() % _templates_unitattribs[unit->Type].CustomDeathSoundNumber;
        }
        // New logic end
        else
        {
          random_sound = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7163);
          sprintf(&Buffer, "S_MANDYING%d", random_sound % 9 + 1);
          sound_id = GetSoundTableID(&Buffer);
        }
        y = unit->__PosY / 0x10000 / 32;
        x = unit->__PosX / 0x10000 / 32;
        PlaySoundAt(sound_id, x, y);
      }
    }
    unit->Health = 0;
    UnitAdjustState(unit, UNIT_STATE_17_DEAD);
    behavior = _templates_unitattribs[unit->Type].__Behavior;
    if ( (behavior == UnitBehavior_SARDAUKAR || _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_SABOTEUR)
      && _templates_GroupIDs.SARDDEATH != -1 )
    {
      // New logic start
      // Fix misplaced sardaukar death damage
      DamageTiles(unit->__PosX / 0x10000, unit->__PosY / 0x10000, 0, _templates_GroupIDs.SARDDEATH, side_id, 0xFFFF, 0);
      // New logic end
    }
  }
}
