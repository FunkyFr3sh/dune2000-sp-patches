#include "macros/patch.h"
#include "dune2000.h"
#include "extended-templates.h"

// Custom implementation of function ModelAddExplosion
DETOUR(0x00456DF0, 0x00456EDB, _Mod__ModelAddExplosion);

short Mod__ModelAddExplosion(eSideType side_id, unsigned char explosionType, unsigned short x, unsigned short y, int z, int extraFlags, char a7, int a8, int a9)
{
  short result; // ax
  CSide *side; // edi
  Explosion *exp; // esi
  char animation_delay; // cl
  int flags; // eax

  // New logic start
  // Extend number of explosion types to 128
  if ( explosionType < MAX_EXPLOSION_TYPES )
  // New logic end
  {
    side = GetSide(side_id);
    exp = (Explosion *)CSide__GetNextFreeObject(side);
    animation_delay = 0;
    if ( exp )
    {
      exp->Type = explosionType;
      exp->__PosX = x << 16;
      exp->__PosY = y << 16;
      exp->__PosZHeight = z;
      exp->__AnimationFrame = 0;
      exp->__AssociatedUnitIndex = -1;
      exp->Flags = _templates_Explosiondata_AnimationArtFlags[explosionType];
      if ( extraFlags & EXPFLAGS_20 )
      {
        animation_delay = a7;
      }
      exp->__AnimationDelay = animation_delay;
      flags = extraFlags ^ exp->Flags;
      exp->Flags = flags;
      if ( flags & EXPFLAGS_1000 )
      {
        exp->dw_field_30 = a8;
        exp->dw_field_34 = a9;
      }
      // New logic start
      // Extra explosion properties
      exp->RepeatCount = 0;
      exp->AssocUnitOffsetX = 0;
      exp->AssocUnitOffsetY = 0;
      exp->custom_byte_1 = 0;
      exp->custom_byte_2 = 0;
      exp->custom_byte_3 = 0;
      exp->custom_byte_4 = 0;
      exp->custom_word_1 = 0;
      exp->custom_word_2 = 0;
      exp->custom_dword_1 = 0;
      exp->custom_dword_2 = 0;
      exp->custom_dword_3 = 0;
      exp->custom_dword_4 = 0;
      // New logic end
      exp->dw_field_84_ptr = (int)&side->__FirstExplosionPtr;
      exp->dw_field_88_ptr = (int)&side->__LastExplosionPtr;
      CSide__SetupObject(side, (Unit *)exp, OBJECT_EXPLOSION);
      result = exp->MyIndex;
    }
    else
    {
      result = -1;
    }
  }
  else
  {
    DebugFatal("Model.CPP", "explosionType >= kMaxAllowableExplosionTypes (%d)", explosionType);
    result = -1;
  }
  return result;
}
