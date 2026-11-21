#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "../event-system/event-core.h"

// Custom implementation of function DoIconBuild
DETOUR(0x004467E0, 0x00447099, _Mod__DoIconBuild);

void Mod__DoIconBuild()
{
  unsigned int side_id; // ebx MAPDST
  CSide *side; // eax
  CSide *side_; // edi
  unsigned int build_step; // esi MAPDST
  int building_cost; // eax
  double credits_step_float; // st7 MAPDST
  int credits_spent_integer; // eax MAPDST
  int credits_step_integer; // ebp MAPDST
  double new_credits_float; // st7 MAPDST
  unsigned short build_progress; // ax MAPDST
  unsigned short new_build_progress; // si MAPDST
  int sound_id_harkonnen; // ST10_4 MAPDST
  int sound_id_ordos; // ST0C_4 MAPDST
  int sound_id_atreides; // eax MAPDST
  BuildQueueStruct *unit_queue; // esi
  Building *building; // eax
  signed int unit_cost; // eax MAPDST
  char behavior; // al MAPDST
  int text_id; // eax
  char *text; // eax
  int building_type_; // eax
  int building_group; // ebp
  unsigned int upgrade_cost_; // eax
  int upgrade_count; // esi
  unsigned char building_type; // al MAPDST
  signed int upgrade_cost; // eax
  CSide *my_side; // esi
  int building_icon_count; // ecx
  unsigned int power_percent; // eax
  unsigned int total_credits; // eax
  unsigned char y2[4]; // [esp+14h] [ebp-50h]
  unsigned char x2[4]; // [esp+18h] [ebp-4Ch]
  unsigned char y1[4]; // [esp+1Ch] [ebp-48h]
  unsigned char x1[4]; // [esp+20h] [ebp-44h]
  float credits_step_float_; // [esp+24h] [ebp-40h]
  int v71; // [esp+28h] [ebp-3Ch]
  Building *v73; // [esp+30h] [ebp-34h]

  side_id = 0;
  side_id = 0;
  if ( _SidesToProcess )
  {
    do
    {
      side = GetSide(side_id);
      side_ = side;
      if ( side->__BuildingBuildQueue.__type >= 0
        && side->__BuildingBuildQueue.__build_progress < 0x5A00u
        && !side->__BuildingBuildQueue.__on_hold )
      {
        // New logic start
        // Build speed hooks
        build_step = ExecuteEventHook(HOOK_GETBUILDINGBUILDSPEEDPERCENTAGE, 2, GetBuildingBuildSpeedPercentage(side_id), side_id, 0, 0, 0)
        // New logic end
                   * _templates_buildattribs[side->__BuildingBuildQueue.__type]._____BuildSpeedBuild
                   / 100;
        building_cost = GetBuildingCost(side_->__BuildingBuildQueue.__type, 0, (eSideType)side_id);
        credits_step_float = (double)build_step * 0.000043402779 * (double)building_cost;
        credits_step_float_ = credits_step_float;
        credits_spent_integer = side_->__BuildingBuildQueue.__credits_spent_integer;
        credits_step_integer = (unsigned long long)(signed long long)(credits_step_float
                                                                + side_->__BuildingBuildQueue.__credits_spent_float)
                             - credits_spent_integer;
        if ( side_->__BuildingBuildQueue.__build_progress + build_step >= 0x5A00 )
        {
          credits_step_integer = building_cost - credits_spent_integer;
        }
        if ( CSide__SpendMoneyIfSufficient(side_, credits_step_integer, "DoIconBuild(BuildingBuilt)") )
        {
          new_credits_float = credits_step_float_ + side_->__BuildingBuildQueue.__credits_spent_float;
          build_progress = side_->__BuildingBuildQueue.__build_progress;
          new_build_progress = build_progress + build_step;
          side_->__BuildingBuildQueue.__credits_spent_integer += credits_step_integer;
          side_->__BuildingBuildQueue.__credits_spent_float = new_credits_float;
          side_->__BuildingBuildQueue.__build_progress_prev = build_progress;
          side_->__BuildingBuildQueue.__build_progress = new_build_progress;
          if ( new_build_progress >= 0x5A00u )
          {
            side_->__BuildingBuildQueue.__build_progress = 0x5A00;
            if ( side_id == (unsigned char)gSideId )
            {
              sound_id_harkonnen = GetSoundTableID("H_BUILDREADY");
              sound_id_ordos = GetSoundTableID("O_BUILDREADY");
              sound_id_atreides = GetSoundTableID("S_BUILDREADY");
              PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 0, 0);
            }
          }
        }
      }
      unit_queue = side_->__UnitBuildQueue;
      v71 = 10;
      do
      {
        if ( unit_queue->__type < 0 )
        {
          goto LABEL_32;
        }
        building = GetBuildingProducedUnitWillArriveFrom(unit_queue->__type, side_id, x1, y1, x2, y2, 0);
        v73 = building;
        if ( building )
        {
          if ( unit_queue->__on_hold )
          {
            goto LABEL_74;
          }
          if ( unit_queue->__build_progress >= 0x5A00u )
          {
            if ( unit_queue->__build_progress != 0x5A00 )
            {
LABEL_74:
              building->Flags &= ~BFLAGS_10_ANIM_PERMANENT;
              goto LABEL_32;
            }
            behavior = _templates_unitattribs[unit_queue->__type].__Behavior;
            if ( behavior != UnitBehavior_ORNITHOPTER
              && _templates_unitattribs[unit_queue->__type].__Behavior != UnitBehavior_SABOTEUR
              && _templates_unitattribs[unit_queue->__type].__Behavior != UnitBehavior_FREMEN
              && _templates_unitattribs[unit_queue->__type].__Behavior != UnitBehavior_DEATH_HAND )
            {
              // New logic start
              // Implement factory rally points
              if ( (building = GetBuildingProducedUnitWillArriveFrom(unit_queue->__type, side_id, x1, y1, x2, y2, 1)) )
              {
                short unit_index = ModelAddUnit((eSideType)side_id, unit_queue->__type, x1[0], y1[0], x2[0], y2[0], 0, 0);
                if (unit_index != -1 && building->RallyPointEnabled)
                {
                  Unit *unit = GetUnit(side_id, unit_index);
                  unit->__RememberPosX = building->RallyPointX;
                  unit->__RememberPosY = building->RallyPointY;
                  unit->OldState = UNIT_STATE_7_MOVING;
                }
              // New logic end
                unit_queue->__type = -1;
                if ( side_id == (unsigned char)gSideId )
                {
                  sound_id_harkonnen = GetSoundTableID("H_UNITREADY");
                  sound_id_ordos = GetSoundTableID("O_UNITREADY");
                  sound_id_atreides = GetSoundTableID("S_UNITREADY");
                  PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 0, 0);
                }
              }
            }
          }
          else
          {
            if ( _templates_unitattribs[unit_queue->__type].__Behavior != UnitBehavior_DEATH_HAND )
            {
              building->Flags |= BFLAGS_10_ANIM_PERMANENT;
            }
            // New logic start
            // Build speed hooks
            build_step = ExecuteEventHook(HOOK_GETUNITBUILDSPEEDPERCENTAGE, 3, GetUnitBuildSpeedPercentage(unit_queue->__type, side_id), side_id, unit_queue->__type, 0, 0)
            // New logic end
                       * _templates_unitattribs[unit_queue->__type].__BuildSpeed
                       / 0x64;
            unit_cost = GetUnitCost(unit_queue->__type, (eSideType)side_id);
            credits_step_float = (double)build_step * 0.000043402779 * (double)unit_cost;
            credits_step_float_ = credits_step_float;
            credits_spent_integer = unit_queue->__credits_spent_integer;
            credits_step_integer = (unsigned long long)(signed long long)(credits_step_float
                                                                    + unit_queue->__credits_spent_float)
                                 - credits_spent_integer;
            if ( unit_queue->__build_progress + build_step >= 0x5A00 )
            {
              credits_step_integer = unit_cost - credits_spent_integer;
            }
            if ( CSide__SpendMoneyIfSufficient(side_, credits_step_integer, "DoIconBuild(UnitBuilt)") )
            {
              unit_queue->__credits_spent_float = credits_step_float_ + unit_queue->__credits_spent_float;
              unit_queue->__credits_spent_integer += credits_step_integer;
              unit_queue->__build_progress_prev = unit_queue->__build_progress;
              unit_queue->__build_progress += build_step;
              if ( unit_queue->__build_progress >= 0x5A00u )
              {
                unit_queue->__build_progress = 0x5A00;
                behavior = _templates_unitattribs[unit_queue->__type].__Behavior;
                if ( behavior == UnitBehavior_ORNITHOPTER )
                {
                  if ( side_id != (unsigned char)gSideId )
                  {
                    goto LABEL_32;
                  }
                  text_id = GetTextID("AirStrikeReady");
LABEL_31:
                  text = GetTextString(text_id, 1);
                  QueueMessage(text, -1);
                  goto LABEL_32;
                }
                switch ( _templates_unitattribs[unit_queue->__type].__Behavior )
                {
                  case UnitBehavior_SABOTEUR:
                    if ( side_id != (unsigned char)gSideId )
                    {
                      break;
                    }
                    text_id = GetTextID("SaboteurReady");
                    goto LABEL_31;
                  case UnitBehavior_FREMEN:
                    if ( side_id != (unsigned char)gSideId )
                    {
                      break;
                    }
                    text_id = GetTextID("FremenReady");
                    goto LABEL_31;
                  case UnitBehavior_DEATH_HAND:
                    if ( side_id == (unsigned char)gSideId )
                    {
                      sound_id_harkonnen = GetSoundTableID("H_DEATHREADY");
                      sound_id_ordos = GetSoundTableID("O_DEATHREADY");
                      sound_id_atreides = GetSoundTableID("S_DEATHREADY");
                      PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 0, 0);
                    }
                    break;
                  default:
                    // New logic start
                    // Implement factory rally points
                    if ( (building = GetBuildingProducedUnitWillArriveFrom(unit_queue->__type, side_id, x1, y1, x2, y2, 1)) )
                    {
                      short unit_index = ModelAddUnit((eSideType)side_id, unit_queue->__type, x1[0], y1[0], x2[0], y2[0], 0, 0);
                      if (unit_index != -1 && building->RallyPointEnabled)
                      {
                        Unit *unit = GetUnit(side_id, unit_index);
                        unit->__RememberPosX = building->RallyPointX;
                        unit->__RememberPosY = building->RallyPointY;
                        unit->OldState = UNIT_STATE_7_MOVING;
                      }
                    // New logic end
                      unit_queue->__type = -1;
                      if ( side_id == (unsigned char)gSideId )
                      {
                        sound_id_harkonnen = GetSoundTableID("H_UNITREADY");
                        sound_id_ordos = GetSoundTableID("O_UNITREADY");
                        sound_id_atreides = GetSoundTableID("S_UNITREADY");
                        PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 0, 0);
                      }
                    }
                    v73->Flags &= ~BFLAGS_10_ANIM_PERMANENT;
                    break;
                }
              }
            }
          }
        }
        else
        {
          unit_queue->__type = -1;
        }
LABEL_32:
        ++unit_queue;
        --v71;
      }
      while ( v71 );
      building_type_ = side_->__BuildingUpgradeQueue.__type;
      if ( (building_type_ & 0x8000u) == 0 )
      {
        building_type_ = (signed short)building_type_;
        building_group = _templates_buildattribs[(signed short)building_type_].GroupType;
        // New logic start
        // When tech is lowered by event, running building upgrades which are no longer possible should get canceled
        if ( side_->__BuildingsExistPerGroup[building_group] && CanSideUpgradeBuildingGroup(side_id, building_group) )
        // New logic end
        {
          if ( side_->__BuildingUpgradeQueue.__build_progress < 0x5A00u && !side_->__BuildingUpgradeQueue.__on_hold )
          {
            upgrade_count = (unsigned char)side_->__BuildingGroupUpgradeCount[building_group];
            building_type = CSide__MyVersionOfBuilding(side_, (eBuildingGroupType)building_group, 0);
            build_step = *(&_templates_buildattribs[0]._____BuildSpeedUpgrade1
                         + building_type
                         + upgrade_count
                         + 66 * building_type);
            building_type = CSide__MyVersionOfBuilding(side_, (eBuildingGroupType)building_group, 0);
            upgrade_cost = GetBuildingCost(building_type, upgrade_count + 1, (eSideType)side_id);
            v73 = (Building *)upgrade_cost;
            credits_step_float = (double)build_step * 0.000043402779 * (double)upgrade_cost;
            credits_spent_integer = side_->__BuildingUpgradeQueue.__credits_spent_integer;
            credits_step_integer = (unsigned long long)(signed long long)(credits_step_float
                                                                    + side_->__BuildingUpgradeQueue.__credits_spent_float)
                                 - credits_spent_integer;
            if ( side_->__BuildingUpgradeQueue.__build_progress + build_step >= 0x5A00 )
            {
              credits_step_integer = (int)v73 - credits_spent_integer;
            }
            if ( CSide__SpendMoneyIfSufficient(side_, credits_step_integer, "DoIconBuild(BuildingUpgraded)") )
            {
              build_progress = side_->__BuildingUpgradeQueue.__build_progress;
              new_credits_float = credits_step_float + side_->__BuildingUpgradeQueue.__credits_spent_float;
              credits_spent_integer = side_->__BuildingUpgradeQueue.__credits_spent_integer;
              side_->__BuildingUpgradeQueue.__build_progress_prev = build_progress;
              new_build_progress = build_step + build_progress;
              side_->__BuildingUpgradeQueue.__credits_spent_float = new_credits_float;
              side_->__BuildingUpgradeQueue.__credits_spent_integer = (unsigned short)credits_step_integer
                                                                    + credits_spent_integer;
              side_->__BuildingUpgradeQueue.__build_progress = new_build_progress;
              if ( new_build_progress >= 0x5A00u )
              {
                ++side_->__BuildingGroupUpgradeCount[building_group];
                side_->__BuildingUpgradeQueue.__type = -1;
                CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(side_);
              }
            }
          }
        }
        else
        {
          upgrade_cost_ = GetBuildingCost(
                            building_type_,
                            (unsigned char)side_->__BuildingGroupUpgradeCount[building_group] + 1,
                            (eSideType)side_id);
          // New logic start
          // Implement returnCreditsToSpiceStorage rule
          CSide_return_credits(side_, upgrade_cost_ * side_->__BuildingUpgradeQueue.__build_progress / 0x5A00);
          // New logic end
          side_->__BuildingUpgradeQueue.__type = -1;
        }
      }
      ++side_id;
    }
    while ( side_id < (unsigned char)_SidesToProcess );
  }
  my_side = GetSide(gSideId);
  if ( my_side->__UnitIconCount > (unsigned int)my_side->__UnitIconCountPrev
    || my_side->__BuildingIconCount > (unsigned int)my_side->__BuildingIconCountPrev )
  {
    sound_id_harkonnen = GetSoundTableID("H_NEWOPTIONS");
    sound_id_ordos = GetSoundTableID("O_NEWOPTIONS");
    sound_id_atreides = GetSoundTableID("S_NEWOPTIONS");
    PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 0, 0);
  }
  building_icon_count = my_side->__BuildingIconCount;
  my_side->__UnitIconCountPrev = my_side->__UnitIconCount;
  power_percent = my_side->__PowerPercent;
  my_side->__BuildingIconCountPrev = building_icon_count;
  if ( power_percent < 100 && my_side->__PowerOutput > 0u )
  {
    sound_id_harkonnen = GetSoundTableID("H_LOWPOWER");
    sound_id_ordos = GetSoundTableID("O_LOWPOWER");
    sound_id_atreides = GetSoundTableID("S_LOWPOWER");
    PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 2000, 0);
  }
  if ( (unsigned int)(my_side->SpiceReal + 100) > (unsigned int)my_side->__MaxStorage )
  {
    if ( my_side->__BuildingsExistPerGroup[(unsigned char)_templates_GroupIDs.Refinery] )
    {
      sound_id_harkonnen = GetSoundTableID("H_SILOSNEEDED");
      sound_id_ordos = GetSoundTableID("O_SILOSNEEDED");
      sound_id_atreides = GetSoundTableID("S_SILOSNEEDED");
      PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 2000, 0);
    }
  }
  total_credits = my_side->CashReal + my_side->SpiceReal;
  if ( total_credits )
  {
    if ( total_credits >= 100 )
    {
      _TacticalData.__isLowOnFunds = 0;
    }
  }
  else if ( !_TacticalData.__isLowOnFunds )
  {
    _TacticalData.__isLowOnFunds = 1;
    sound_id_harkonnen = GetSoundTableID("H_LOWMONEY");
    sound_id_ordos = GetSoundTableID("O_LOWMONEY");
    sound_id_atreides = GetSoundTableID("S_LOWMONEY");
    PlayMentatSound(sound_id_atreides, sound_id_ordos, sound_id_harkonnen, 1, 2000, 0);
  }
}
