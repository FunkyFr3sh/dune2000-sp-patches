#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"

// Fix missing check for unit type availability when ordered to build such unit type
// Implement separateBuildingVoiceLines rule

// Custom implementation of function ModelBuildUnitPick
DETOUR(0x00455510, 0x0045574F, _Mod__ModelBuildUnitPick);

void Mod__ModelBuildUnitPick(eSideType side_id, unsigned short unit_type)
{
  CSide *side; // eax
  CSide *side_; // ebp
  BuildQueueStruct *unit_build_queue; // edi
  BuildQueueStruct *unit_build_queue_; // ecx
  unsigned int unit_build_queue_id; // esi MAPDST
  CSide *side_unit_build_queue; // eax
  int atreides_sound; // eax
  int unit_type_offset; // ebx
  char behavior; // al MAPDST
  char *on_hold_ptr; // eax
  int ordos_sound; // [esp-14h] [ebp-24h]
  int harkonnen_sound; // [esp-10h] [ebp-20h]
  char state; // [esp-Ch] [ebp-1Ch]

  side = GetSide(side_id);
  side_ = side;
  unit_build_queue = side->__UnitBuildQueue;
  unit_build_queue_id = 0;
  unit_build_queue_ = unit_build_queue;
  do
  {
    if ( unit_build_queue_->__type == unit_type )
    {
      on_hold_ptr = &side_->__UnitBuildQueue[unit_build_queue_id].__on_hold;
      if ( !*on_hold_ptr )
      {
        return;
      }
      if ( !(side_->SpiceReal + side_->CashReal) && _templates_unitattribs[unit_type].__Cost )
      {
        return;
      }
      *on_hold_ptr = 0;
      if ( side_id != gSideId )
      {
        return;
      }
      state = 0;
      if ( _templates_unitattribs[unit_type].__IsInfantry )
      {
        harkonnen_sound = GetSoundTableID("H_TRAINING");
        ordos_sound = GetSoundTableID("O_TRAINING");
        atreides_sound = GetSoundTableID("S_TRAINING");
        goto LABEL_24;
      }
LABEL_23:
      // New logic start
      // Implement separateBuildingVoiceLines rule
      if (rulesExt__separateBuildingVoiceLines)
      {
        harkonnen_sound = GetSoundTableID("H_PRODUCING");
        ordos_sound = GetSoundTableID("O_PRODUCING");
        atreides_sound = GetSoundTableID("S_PRODUCING");
      }
      else
      {
        harkonnen_sound = GetSoundTableID("H_BUILDING");
        ordos_sound = GetSoundTableID("O_BUILDING");
        atreides_sound = GetSoundTableID("S_BUILDING");
      }
      // New logic end
      goto LABEL_24;
    }
    ++unit_build_queue_id;
    ++unit_build_queue_;
  }
  while ( unit_build_queue_id < 0xA );
  // New logic start
  // Fix missing check for unit type availability when ordered to build such unit type
  bool found = false;
  for (unsigned int i = 0; i < side->__UnitIconCount; i++)
  {
    if (side->__UnitIcons[i] == unit_type)
    {
      found = true;
      break;
    }
  }
  if (!found)
    return;
  // New logic end
  if ( !CanUnitBeBuilt(side_id, unit_type, 1) )
  {
    return;
  }
  unit_build_queue_id = 0;
  while ( unit_build_queue->__type != -1 )
  {
    ++unit_build_queue_id;
    ++unit_build_queue;
    if ( unit_build_queue_id >= 10 )
    {
      DebugFatal("HandleGameLoopEvents", "MAX_BUILDABLE_UNITS exceeded");
    }
  }
  side_unit_build_queue = (CSide *)((char *)side_ + 20 * unit_build_queue_id);
  side_unit_build_queue->__UnitBuildQueue[0].__type = (unsigned char)unit_type;
  side_unit_build_queue->__UnitBuildQueue[0].__build_progress = 0;
  *((_WORD *)&side_->__ObjectArrayPtr + 10 * (unit_build_queue_id + 7849)) = 0;
  side_unit_build_queue->__UnitBuildQueue[0].__credits_spent_float = 0.0;
  side_unit_build_queue->__UnitBuildQueue[0].__credits_spent_integer = 0;
  side_unit_build_queue->__UnitBuildQueue[0].__on_hold = 0;
  if ( side_->SpiceReal + side_->CashReal )
  {
    if ( side_id != gSideId )
    {
      return;
    }
    unit_type_offset = unit_type << 8;
    behavior = *(&_templates_unitattribs[0].__Behavior + unit_type_offset);
    if ( behavior == UnitBehavior_ORNITHOPTER
      || behavior == UnitBehavior_DEATH_HAND
      || behavior == UnitBehavior_SABOTEUR
      || behavior == UnitBehavior_FREMEN )
    {
      return;
    }
    state = 0;
    if ( *(&_templates_unitattribs[0].__IsInfantry + unit_type_offset) )
    {
      harkonnen_sound = GetSoundTableID("H_TRAINING");
      ordos_sound = GetSoundTableID("O_TRAINING");
      atreides_sound = GetSoundTableID("S_TRAINING");
      goto LABEL_24;
    }
    goto LABEL_23;
  }
  side_unit_build_queue->__UnitBuildQueue[0].__on_hold = 1;
  if ( side_id == gSideId )
  {
    behavior = _templates_unitattribs[unit_type].__Behavior;
    if ( behavior != UnitBehavior_ORNITHOPTER
      && behavior != UnitBehavior_DEATH_HAND
      && behavior != UnitBehavior_SABOTEUR
      && behavior != UnitBehavior_FREMEN )
    {
      state = 1;
      harkonnen_sound = GetSoundTableID("H_LOWMONEY");
      ordos_sound = GetSoundTableID("O_LOWMONEY");
      atreides_sound = GetSoundTableID("S_LOWMONEY");
LABEL_24:
      PlayMentatSound(atreides_sound, ordos_sound, harkonnen_sound, state, 0, 0);
      return;
    }
  }
}
