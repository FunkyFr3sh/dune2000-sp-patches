#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

// Add lightning bullet behavior
// Add railgun bullet behavior

void AddLightning(unsigned char side_id, unsigned char bullet_type, short firer_index, unsigned short source_xpos, unsigned short source_ypos, unsigned short target_xpos, unsigned short target_ypos)
{
  int distance = sqrt((double)((source_xpos - target_xpos) * (source_xpos - target_xpos) + (source_ypos - target_ypos) * (source_ypos - target_ypos)));
  int num_steps = distance / 24;
  if (num_steps == 0)
    num_steps = 1;
  int step_x = ((target_xpos << 16) - (source_xpos << 16)) / num_steps;
  int step_y = ((target_ypos << 16) - (source_ypos << 16)) / num_steps;
  int posx = source_xpos << 16;
  int posy = source_ypos << 16;
  int direction = GetFacing(source_xpos, source_ypos, target_xpos, target_ypos);

  for (int i = 0; i < num_steps; i++)
  {
    int explosion_type = _templates_GroupIDs.EX_DEVSPARK1;
    int rand_anim = rand() % 3;
    if (rand_anim == 1)
      explosion_type = _templates_GroupIDs.EX_DEVSPARK2;
    if (rand_anim == 2)
      explosion_type = _templates_GroupIDs.EX_DEVSPARK3;
    int xpos = posx >> 16;
    int ypos = posy >> 16;
    int offset_side = rand() % 2;
    int offset = rand() % 16;
    xpos += _sinValues[offset + ((16 - direction + (offset_side?8:-8)) & 31) * 512] / 2048;
    ypos += _cosValues[offset + ((16 - direction + (offset_side?8:-8)) & 31) * 512] / 2048;

    ModelAddExplosion(side_id, explosion_type, xpos, ypos, 0, 0, 0, 0, 0);
    posx += step_x;
    posy += step_y;
  }

  BullAtrbStruct *bullet_template = &_templates_bulletattribs[bullet_type];
  DamageTiles(target_xpos, target_ypos, 0, bullet_type, side_id, firer_index, (bullet_template->Flags & BULFLAGS_40_DEVIATOR) != 0);
  if (bullet_template->__HitExplosion != -1)
  {
    ModelAddExplosion(side_id, bullet_template->__HitExplosion, target_xpos, target_ypos, 0, 0, 0, 0, 0);
    ExploisonAtrbStruct *explosion_template = &_templates_explosionattribs[(int)bullet_template->__HitExplosion];
    PlaySoundAt(explosion_template->__Sound, target_xpos / 32, target_ypos / 32);
  }
}

// Custom implementation of function ModelAddBullet
DETOUR(0x00456A30, 0x00456D4B, _Mod__ModelAddBullet);

short Mod__ModelAddBullet(unsigned char side_id, unsigned char bulletype, int delay, short firer, unsigned short source_xpos, unsigned short source_ypos, unsigned short target_xpos, unsigned short target_ypos, short homing_index, char homing_side)
{
  Bullet *bullet; // esi
  int bullet_type; // eax
  int posx; // eax
  int target_distance_x; // edi
  int target_distance_y; // ebp
  int target_distance; // rax
  int target_distance_; // ebx
  int bullet_type__; // ecx
  int v19; // eax
  int projectile_speed_; // ecx
  int firer_posz; // edx
  unsigned char direction; // al
  int frames_per_direction; // edi
  int source_xpos_; // [esp+10h] [ebp-8h]
  CSide *side; // [esp+14h] [ebp-4h]
  int projectile_speed; // [esp+20h] [ebp+8h]
  int bullet_flags; // [esp+24h] [ebp+Ch] MAPDST
  int bullet_type_; // [esp+2Ch] [ebp+14h]
  int move_steps; // [esp+40h] [ebp+28h]
  BullAtrbStruct *bullet_template;

  if ( bulletype >= 64u )
  {
    DebugFatal("Model.CPP", "bulletType >= kMaxAllowableBulletTypes (%d)", bulletype);
    return -1;
  }
  // New logic start
  // Implement lightning bullet behavior
  bullet_template = &_templates_bulletattribs[bulletype];
  if (bullet_template->Behavior == BulletBehavior_LIGHTNING)
  {
    AddLightning(side_id, bulletype, firer, source_xpos, source_ypos, target_xpos, target_ypos);
    return -1;
  }
  // New logic end
  side = GetSide(side_id);
  bullet = (Bullet *)CSide__GetNextFreeObject(side);
  if ( !bullet )
  {
    return -1;
  }
  // New logic start
  // Implement railgun bullet behavior
  if (bullet_template->Behavior == BulletBehavior_RAILGUN)
  {
    if (bullet_template->__TrailExplosion == -1)
      DebugFatal("ModelAddBullet", "Weapon %d has no trail explosion defined.", bulletype);
    bullet->RailgunAnimationFrame = -1;
    bullet->RailgunAnimationDelay = 0;
  }
  // New logic end
  bullet->__FirerIndex = firer;
  source_xpos_ = source_xpos;
  bullet->__PosX = source_xpos << 16;
  bullet->__TargetYpos = target_ypos;
  bullet->__PosY = source_ypos << 16;
  bullet->__TargetXpos = target_xpos;
  bullet->Type = bulletype;
  bullet_type = bulletype;
  bullet->__Delay = delay;
  bullet->__PosZHeight = 0x60000;
  bullet->Flags = _templates_bulletattribs[bullet_type].Flags;
  bullet_type_ = bulletype;
  bullet->c_field_1B_template_c_field_13 = _templates_bulletattribs[bullet_type].c_field_13;
  if ( ((unsigned short)homing_index > 1000u && homing_index != -1) || homing_side > 8 || homing_side < -1 )
  {
    DebugFatal(
      "MODEL.CPP",
      "ModelAddBullet - daft values for homing (S:%d, I:%d)",
      homing_side,
      (unsigned short)homing_index);
  }
  posx = bullet->__PosX;
  bullet->__HomingIndex = homing_index;
  target_distance_x = (unsigned short)bullet->__TargetXpos - posx / 0x10000;
  target_distance_y = (unsigned short)bullet->__TargetYpos - bullet->__PosY / 0x10000;
  bullet->__HomingSideId = homing_side;
  target_distance = sqrt((double)(target_distance_y * target_distance_y + target_distance_x * target_distance_x));
  target_distance_ = target_distance;
  bullet_type__ = bullet->Type;
  projectile_speed = _templates_bulletattribs[bullet_type__].__ProjectileSpeed;
  move_steps = ((_DWORD)target_distance << 16) / projectile_speed;
  v19 = bullet->Flags;
  bullet_flags = bullet->Flags;
  if ( v19 & BULFLAGS_800_SONIC )
  {
    v19 = _templates_bulletattribs[bullet_type__].__Range;
    projectile_speed_ = _templates_bulletattribs[bullet_type__].__ProjectileSpeed;
    move_steps = (v19 << 16) / (unsigned int)projectile_speed;
    v19 = bullet->Flags;
  }
  else
  {
    projectile_speed_ = _templates_bulletattribs[bullet_type__].__ProjectileSpeed;
  }
  bullet->__Speed = projectile_speed_;
  bullet->__MoveSteps = move_steps;
  bullet->__MySideId = side_id;
  if ( target_distance_ )
  {
    bullet->__SpeedX = target_distance_x * projectile_speed_ / target_distance_;
    bullet->__SpeedY = target_distance_y * projectile_speed_ / target_distance_;
    v19 = bullet_flags;
    goto LABEL_16;
  }
  if ( !(v19 & BULFLAGS_100_FALLING) )
  {
    return -1;
  }
  bullet->__SpeedY = 0;
  bullet->__SpeedX = 0;
LABEL_16:
  if ( v19 & BULFLAGS_1_ARC_TRAJECTORY )
  {
    bullet->__FallingSpeed = (signed int)(-0x2000u * move_steps) / 2;
    bullet_flags = bullet_flags | BULFLAGS_2_DRAW_SHADOW;
    bullet->Flags = bullet_flags;
  }
  bullet_flags = bullet->Flags;
  if ( bullet_flags & BULFLAGS_100_FALLING )
  {
    bullet_flags = bullet_flags | BULFLAGS_2_DRAW_SHADOW;
    bullet->Flags = bullet_flags;
    if ( firer == -1 )
    {
      DebugFatal("Model.cpp", "Firer was illegal");
    }
    firer_posz = GetSide(side_id)->__ObjectArray[(unsigned short)firer].__PosZHeight;
    bullet->__FallingSpeed = 0;
    bullet->__PosZHeight = firer_posz;
  }
  bullet_flags = bullet->Flags;
  if ( bullet_flags & BULFLAGS_20_HOMING )
  {
    bullet->__MoveSteps = 2 * move_steps;
  }
  if ( _templates_ProjectileArtDirections[bullet_type_] <= 1 || (bullet_flags & BULFLAGS_80_ANIMATED) != 0 )
  {
    bullet->__AnimationFrame = 0;
    bullet->c_field_45 = 0;
  }
  else
  {
    direction = GetFacing(source_xpos_, source_ypos, target_xpos, target_ypos);
    frames_per_direction = 32 / _templates_ProjectileArtDirections[bullet_type_];
    bullet->__AnimationFrame = direction / frames_per_direction;
    bullet->c_field_45 = direction / frames_per_direction;
  }
  bullet->c_field_46 = bullet->c_field_45;
  bullet->__FirstBulletPtrPtr = &side->__FirstBulletPtr;
  bullet->__LastBulletPtrPtr = &side->__LastBulletPtr;
  CSide__SetupObject(side, (Unit *)bullet, OBJECT_BULLET);
  return bullet->MyIndex;
}
