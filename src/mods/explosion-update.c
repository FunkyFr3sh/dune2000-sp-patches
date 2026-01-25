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

  if ( explosion->w_field_8 != -1 )
  {
    if ( GetSide(side_id)->__ObjectArray[(unsigned short)explosion->w_field_8].ObjectType == 1 )
    {
      if ( GetSide(side_id)->__ObjectArray[(unsigned short)explosion->w_field_8].State == BLD_STATE_17_DEAD )
      {
        return 0;
      }
    }
    else
    {
      explosion->w_field_8 = -1;
    }
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
    if ( (unsigned char)(1 << animation_delay) & _templates_explosionattribs[explosion->Type].__FiringPattern )
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
      explosion->__AnimationDelay = 2;
      explosion->__AnimationFrame = ++animation_frame;
      if ( animation_frame >= _templates_AnimationArtFrames[explosion_type] )
      {
        explosion->__AnimationFrame = 0;
        return 0;
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
