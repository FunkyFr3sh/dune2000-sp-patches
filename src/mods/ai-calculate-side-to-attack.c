#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

// Add DontPreferTargetingHumanPlayers AI option
// Implement AttackSidePriority AI property

// Custom implementation of function CAI__RecalculateSideToAttack
DETOUR(0x0041CE50, 0x0041D06D, _Mod__CAI__RecalculateSideToAttack);

char __thiscall Mod__CAI__RecalculateSideToAttack(CAI_ *this)
{
  CAI_ *ai; // edi MAPDST
  CSide *ai_side; // eax MAPDST
  CSide *other_side; // eax MAPDST
  int weight; // esi MAPDST
  Unit *unit; // eax
  Building *building; // eax
  int flags; // ecx
  int ai_side_center_x; // ecx
  int other_side_center_x; // edi
  int distance_y; // esi
  int distance; // rax
  bool v15; // cf
  char result; // [esp+13h] [ebp-2Dh]
  unsigned char other_side_id; // [esp+18h] [ebp-28h]
  int other_side_id_; // [esp+1Ch] [ebp-24h]
  int maximum; // [esp+28h] [ebp-18h]

  ai = this;
  ai = this;
  AIDebugPrint("RecalculateSideToAttack(AI)\n");
  result = -1;
  maximum = 0;
  ai_side = GetSide(ai->AISide);
  other_side_id = 0;
  other_side_id_ = 0;
  for ( other_side = GetSide(0); ; other_side = GetSide(other_side_id) )
  {
    weight = 0;
    weight = 0;
    if ( other_side_id != ai->AISide )
    {
      if ( other_side->__ParticipatesInGame )
      {
        if ( _gDiplomacy[ai->AISide][other_side_id_] == 1 )
        {
          for ( unit = other_side->__FirstUnitPtr; unit; unit = unit->Next )
          {
            if ( !(unit->Flags & (UFLAGS_40_FLYING|UFLAGS_4_CLOAKED)) )
            {
              ++weight;
            }
          }
          for ( building = other_side->__FirstBuildingPtr; building; building = building->Next )
          {
            flags = building->Flags;
            if ( !(flags & BFLAGS_4000) )
            {
              weight += 3;
            }
          }
        }
      }
    }
    if ( weight )
    {
      ai_side_center_x = ai_side->__BasePosMinX + (ai_side->__BasePosMaxX - ai_side->__BasePosMinX) / 2;
      other_side_center_x = other_side->__BasePosMinX + (other_side->__BasePosMaxX - other_side->__BasePosMinX) / 2;
      distance_y = ai_side->__BasePosMinY
                 + (ai_side->__BasePosMaxY - ai_side->__BasePosMinY) / 2
                 - (other_side->__BasePosMinY
                  + (other_side->__BasePosMaxY - other_side->__BasePosMinY) / 2);
      distance = sqrt((double)(distance_y * distance_y + (ai_side_center_x - other_side_center_x) * (ai_side_center_x - other_side_center_x)));
      if ( (_DWORD)distance )
      {
        weight = ((double)weight / ((double)distance * 0.0078125));
      }
    }
    if ( weight <= 1 )
    {
      weight = 1;
    }
    // New logic start
    // Implement AttackSidePriority AI property
    weight *= (ai->AttackSidePriority[other_side_id] + 1);
    // New logic end
    weight = weight * (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\AI.cpp", 1884) % 40u + 80) / 100;
    // New logic start
    // Add DontPreferTargetingHumanPlayers AI option
    if ( !_gAIArray[other_side_id].__IsAI && !ai->DontPreferTargetingHumanPlayers)
    // New logic end
    {
      weight *= 4;
    }
    if ( weight > maximum )
    {
      maximum = weight;
      result = other_side_id;
    }
    v15 = (unsigned char)(other_side_id++ + 1) < 8u;
    ++other_side_id_;
    if ( !v15 )
    {
      break;
    }
  }
  return result;
}
