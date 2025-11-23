#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function CAI__DoSaboteur
DETOUR(0x00422900, 0x00422A7D, _Mod__CAI__DoSaboteur);

void __thiscall Mod__CAI__DoSaboteur(CAI_ *this, char activate_ai)
{
  CAI_ *ai; // esi
  CSide *side; // edi
  char found; // bl
  char group_id; // al MAPDST
  Unit *unit; // ecx
  eSideType side_to_attack; // al MAPDST
  Building *building; // eax
  CAI_ *v10; // ecx
  _BYTE y; // [esp+10h] [ebp-Ch]
  _BYTE x; // [esp+14h] [ebp-8h]

  ai = this;
  AIDebugPrint("DoSaboteur(AI)\n");
  if ( activate_ai != 1 )
  {
    side = GetSide(ai->AISide);
    found = 0;
    group_id = CAI__FindFirstFreeGroup(ai);
    if ( group_id )
    {
      unit = side->__FirstUnitPtr;
      while ( unit )
      {
        if ( unit->__GroupID != -1
          || _templates_unitattribs[unit->Type].__Behavior != UnitBehavior_SABOTEUR
          // New logic start
          // Customizable Saboteur cloaking capacity
          || unit->__SpecialPurpose < (_templates_unitattribs[unit->Type].StorageCapacity?_templates_unitattribs[unit->Type].StorageCapacity:160) )
          // New logic end
        {
          unit = unit->Next;
        }
        else
        {
          unit->__GroupID = group_id;
          unit = 0;
          found = 1;
        }
      }
      if ( found )
      {
        side_to_attack = CAI__GetBestSideToAttack(ai);
        CAI__GetAveragePositionOfGroup(ai, group_id, &x, &y);
        building = CAI__GetBestBuilding(ai, side_to_attack, x, y, 0.25);
        if ( building )
        {
          v10 = (CAI_ *)((char *)ai + 44 * (unsigned __int8)group_id);
          v10->__AIGroups[0].Task_1C = TASK_18;
          v10->__AIGroups[0].w_field_2_index = building->MyIndex;
          *(&ai->AISide + 44 * ((unsigned __int8)group_id + 1)) = side_to_attack;
          v10->__AIGroups[0].c_field_24_x = building->__PosX / 0x10000 / 32;
          v10->__AIGroups[0].c_field_25_y = building->__PosY / 0x10000 / 32;
          v10->__AIGroups[0].c_field_1D_delivery_index = 2;
          v10->__AIGroups[0].dw_field_14 = 0;
          v10->__AIGroups[0].dw_field_C_morale_strength = 100;
          v10->__AIGroups[0].__NotFree_26 = 1;
          CAI__ClearUnits(ai);
          CAI__SelectedGroup(ai, group_id, 1);
          CAI__CommenceOperation(ai, group_id);
          CAI__ClearUnits(ai);
        }
        else
        {
          CAI__MergeGroups(ai, group_id, ai->c_field_1D3C_firstfreegroup);
        }
      }
    }
  }
}
