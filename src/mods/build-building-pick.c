#include "macros/patch.h"
#include "dune2000.h"

// Implement Max Count building template parameter

// Custom implementation of function ModelBuildBuildingPick
DETOUR(0x00455FC0, 0x004560E9, _Mod__ModelBuildBuildingPick);

void Mod__ModelBuildBuildingPick(unsigned char side_id, unsigned short buildingType)
{
  CSide *side; // esi
  bool no_money; // zf
  int atreides_sound_id; // eax
  int ordos_sound_id; // [esp-14h] [ebp-20h]
  int harkonnen_sound_id; // [esp-10h] [ebp-1Ch]
  char state; // [esp-Ch] [ebp-18h]

  side = GetSide(side_id);
  if ( buildingType >= 100u )
  {
    DebugFatal("ModelBuildBuildingPick", "buildingType >= kMaxAllowableBuildingTypes, %d", buildingType);
  }
  if ( side->__BuildingBuildQueue.__type < 0 )
  {
    // New logic start
    // Implement Max Count building template parameter
    if (_templates_buildattribs[buildingType].MaxCount && side->__BuildingsExistPerGroup[_templates_buildattribs[buildingType].GroupType] >= _templates_buildattribs[buildingType].MaxCount)
      return;
    // New logic end
    no_money = side->CashReal + side->SpiceReal == 0;
    side->__BuildingBuildQueue.__type = buildingType;
    side->__BuildingBuildQueue.__build_progress = 0;
    side->__BuildingBuildQueue.__build_progress_prev = 0;
    side->__BuildingBuildQueue.__credits_spent_float = 0.0;
    side->__BuildingBuildQueue.__credits_spent_integer = 0;
    side->__BuildingBuildQueue.__on_hold = 0;
    side->__BuildingBuildQueue.c_field_11_cancel = 0;
    if ( no_money )
    {
      side->__BuildingBuildQueue.__on_hold = 1;
      if ( side_id != gSideId )
      {
        return;
      }
      state = 1;
      harkonnen_sound_id = GetSoundTableID("H_LOWMONEY");
      ordos_sound_id = GetSoundTableID("O_LOWMONEY");
      atreides_sound_id = GetSoundTableID("S_LOWMONEY");
      goto LABEL_13;
    }
    if ( side_id != gSideId )
    {
      return;
    }
LABEL_12:
    state = 0;
    harkonnen_sound_id = GetSoundTableID("H_BUILDING");
    ordos_sound_id = GetSoundTableID("O_BUILDING");
    atreides_sound_id = GetSoundTableID("S_BUILDING");
LABEL_13:
    PlayMentatSound(atreides_sound_id, ordos_sound_id, harkonnen_sound_id, state, 0, 0);
    return;
  }
  if ( side->__BuildingBuildQueue.__on_hold )
  {
    if ( side->CashReal + side->SpiceReal )
    {
      side->__BuildingBuildQueue.__on_hold = 0;
      if ( side_id == gSideId )
      {
        goto LABEL_12;
      }
    }
  }
}
