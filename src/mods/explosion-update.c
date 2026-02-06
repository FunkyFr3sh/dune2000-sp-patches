#include "macros/patch.h"
#include "dune2000.h"
#include "extended-templates.h"

// Custom implementation of function UpdateExplosion
DETOUR(0x0049DD80, 0x0049DEA1, _Mod__UpdateExplosion);

bool Mod__UpdateExplosion(Explosion *explosion, eSideType side_id)
{
  int flags; // eax
  int v3; // edi
  int v4; // ecx
  char animation_delay; // cl
  bool result; // al
  int explosion_type; // ecx
  unsigned char animation_frame; // al

  ExploisonAtrbStruct *explosion_template = &_templates_explosionattribs[explosion->Type];
  if ( explosion->__AssociatedUnitIndex != -1 )
  {
    // New logic start
    // Move explosion along with its associated unit
    Unit *associated_unit = &GetSide(side_id)->__ObjectArray[(unsigned short)explosion->__AssociatedUnitIndex];
    if ( associated_unit->ObjectType == OBJECT_UNIT )
    {
      if ( associated_unit->State == UNIT_STATE_17_DEAD )
      {
        return 0;
      }
    }
    else
    {
      return 0;
    }
    explosion->__PosX = associated_unit->__PosX + (explosion->AssocUnitOffsetX << 16);
    explosion->__PosY = associated_unit->__PosY + (explosion->AssocUnitOffsetY << 16);
    // New logic end
  }
  flags = explosion->Flags;
  if ( flags & EXPFLAGS_1000 )
  {
    v3 = explosion->__PosY;
    v4 = explosion->dw_field_34;
    explosion->__PosX += explosion->dw_field_30;
    explosion->dw_field_30 = 0;
    explosion->__PosY = v4 + v3;
    explosion->dw_field_34 = 0;
  }
  animation_delay = explosion->__AnimationDelay;
  if ( flags & EXPFLAGS_400_MUZZLE_FLASH )
  {
    if ( (unsigned char)(1 << animation_delay) & explosion_template->__FiringPattern )
    {
      flags &= ~EXPFLAGS_800_INVISIBLE;
    }
    else
    {
      flags |= EXPFLAGS_800_INVISIBLE;
    }
    explosion->Flags = flags;
    explosion->__AnimationDelay = animation_delay + 1;
    result = animation_delay != 7;
  }
  else
  {
    if ( animation_delay )
    {
      explosion->__AnimationDelay = animation_delay - 1;
    }
    else
    {
      flags = flags & ~EXPFLAGS_20;
      explosion_type = explosion->Type;
      explosion->Flags = flags;
      animation_frame = explosion->__AnimationFrame;
      // New logic start
      // Implement explosion animation delay template property
      explosion->__AnimationDelay = 2 + explosion_template->AnimationDelay;
      // New logic end
      explosion->__AnimationFrame = ++animation_frame;
      if ( animation_frame >= _templates_AnimationArtFrames[explosion_type] )
      {
        explosion->__AnimationFrame = 0;
        // New logic start
        // Implement explosion repeat count template property
        if (explosion->RepeatCount < explosion_template->RepeatCount)
        {
          if (explosion_template->RepeatCount != 255)
            explosion->RepeatCount++;
        }
        else
          return 0;
        // New logic end
      }
    }
    result = 1;
    if ( explosion->Flags & EXPFLAGS_1_RISE_UP )
    {
      explosion->__PosZHeight += 0x20000;
    }
  }
  return result;
}
