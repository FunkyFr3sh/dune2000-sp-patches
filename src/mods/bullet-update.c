#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"
#include "shooting-restrictions.h"
#include "extended-tileset.h"
#include "../event-system/event-core.h"

// Implement railgun bullet behavior

int UpdateBullet_Railgun(Bullet *bul, int side_id)
{
  // Handle railgun animation frames
  BullAtrbStruct *bullet_template = &_templates_bulletattribs[bul->Type];
  if (bul->RailgunAnimationDelay == 0)
  {
    bul->RailgunAnimationFrame++;
    bul->RailgunAnimationDelay = 2;
  }
  else
  {
    bul->RailgunAnimationDelay--;
    return 1;
  }
  if (bul->RailgunAnimationFrame == _templates_AnimationArtFrames[(int)bullet_template->__TrailExplosion])
    return 0;
  if (bul->RailgunAnimationFrame > 0)
    return 1;
  // Get firing direction
  int source_xpos = bul->__PosX >> 16;
  int source_ypos = bul->__PosY >> 16;
  int target_xpos = bul->__TargetXpos;
  int target_ypos = bul->__TargetYpos;
  int direction = GetFacing(source_xpos, source_ypos, target_xpos, target_ypos);
  // Get firer position
  int firer_xpos = -1024;
  int firer_ypos = -1024;
  if (bul->__FirerIndex >= 0)
  {
    Unit *firer_unit = GetUnit(side_id, bul->__FirerIndex);
    if (firer_unit->ObjectType == OBJECT_UNIT)
    {
      firer_xpos = firer_unit->__PosX >> 16;
      firer_ypos = firer_unit->__PosY >> 16;
    }
    else if (firer_unit->ObjectType == OBJECT_BUILDING)
    {
      Building *firer_building = (Building *)firer_unit;
      firer_xpos = firer_building->__PosX >> 16;
      firer_ypos = firer_building->__PosY >> 16;
    }
    else
      DebugFatal("UpdateBullet_Railgun", "Object side %d index %d is not a building or unit", side_id, bul->__FirerIndex);
  }
  // Get bullet properties
  int radius = _WarheadData[(int)bullet_template->Warhead].Radius;
  int range = bullet_template->__Range;
  int projectile_speed = bullet_template->__ProjectileSpeed >> 16;
  int restriction_type = (bullet_template->Flags & BULFLAGS_1000_BLOCKED_BY_WALL)?1:2;
  // Calculate positional stuff
  float distance = sqrt((float)((source_xpos - target_xpos) * (source_xpos - target_xpos) + (source_ypos - target_ypos) * (source_ypos - target_ypos)));
  float ratio = (float)range / distance;
  float delta_x = (target_xpos - source_xpos) * ratio;
  float delta_y = (target_ypos - source_ypos) * ratio;
  int num_steps = range;
  if (num_steps == 0)
    num_steps = 1;
  bul->RailgunStepX = delta_x / num_steps;
  bul->RailgunStepY = delta_y / num_steps;
  // Go step by step
  float xpos = source_xpos;
  float ypos = source_ypos;
  bul->RailgunBlockedX = 0;
  bul->RailgunBlockedY = 0;
  int step_counter = 0;
  for (int i = 0; i < num_steps; i++)
  {
    // Check if position is inside map area
    if (!(xpos > 0 && ypos > 0 && xpos < (gGameMapWidth * 32) && ypos < (gGameMapHeight * 32)))
      break;
    // Check for tile restriction and wall
    if (!bul->RailgunBlockedX && !bul->RailgunBlockedY)
    {
      int x = xpos / 32;
      int y = ypos / 32;
      GameMapTileStruct *tile = &gGameMap.map[_CellNumbersWidthSpan[y] + x];
      bool blocked_by_wall = (bullet_template->Flags & BULFLAGS_1000_BLOCKED_BY_WALL) && (tile->__tile_bitflags & TileFlags_400_HAS_WALL);
      // If bullet was blocked by wall, make sure to damage the wall
      if (blocked_by_wall)
        DamageTiles(x * 32 + 16, y * 32 + 16, 0, bul->Type, side_id, bul->__FirerIndex, (bullet_template->Flags & BULFLAGS_40_DEVIATOR) != 0);
      if (((!(source_xpos / 32 == x && source_ypos / 32 == y)) && (tile_restrictions[tile->__tile_index] & (restrictions_check[direction] << restriction_type))) || blocked_by_wall)
      {
        // Find position which is closest to center of current tile and set it as blocked position
        float center_xpos = x * 32 + 16;
        float center_ypos = y * 32 + 16;
        float tmp_xpos = xpos;
        float tmp_ypos = ypos;
        float min_xpos = tmp_xpos;
        float min_ypos = tmp_ypos;
        float min_dist = (center_xpos - tmp_xpos) * (center_xpos - tmp_xpos) + (center_ypos - tmp_ypos) * (center_ypos - tmp_ypos);
        while ((int)tmp_xpos / 32 == x && (int)tmp_ypos / 32 == y)
        {
          float dist = (center_xpos - tmp_xpos) * (center_xpos - tmp_xpos) + (center_ypos - tmp_ypos) * (center_ypos - tmp_ypos);
          if (dist < min_dist)
          {
            min_dist = dist;
            min_xpos = tmp_xpos;
            min_ypos = tmp_ypos;
          }
          tmp_xpos += bul->RailgunStepX;
          tmp_ypos += bul->RailgunStepY;
        }
        bul->RailgunBlockedX = min_xpos;
        bul->RailgunBlockedY = min_ypos;
      }
    }
    // Check if blocked position was reached
    if (bul->RailgunBlockedX == xpos && bul->RailgunBlockedY == ypos)
      break;
    // Do damage only after n steps
    if (step_counter == 0)
    {
      // Do damage far enough from the firer to avoid self-damaging
      int diff_x = firer_xpos - (int)xpos;
      int diff_y = firer_ypos - (int)ypos;
      if (diff_x * diff_x + diff_y * diff_y > (radius) * (radius))
        DamageTiles(xpos, ypos, 0, bul->Type, side_id, bul->__FirerIndex, (bullet_template->Flags & BULFLAGS_40_DEVIATOR) != 0);
    }
    // Move to next step
    step_counter++;
    if (step_counter >= projectile_speed)
      step_counter = 0;
    xpos += bul->RailgunStepX;
    ypos += bul->RailgunStepY;
  }
  return 1;
}

// Update bullet hooks
// Extension wrapper for function UpdateBullet
CALL(0x004590A9, _Ext_UpdateBullet); // ModelUpdates

char Ext_UpdateBullet(Bullet *bul, int side_id)
{
  side_id &= 255;
  if (ExecuteEventHook(HOOK_PREUPDATEBULLET, 3, 0, side_id, bul->MyIndex, 0, 0))
    return 1;
  // Special logic for railgun bullet behavior
  if (_templates_bulletattribs[bul->Type].Behavior == BulletBehavior_RAILGUN)
    return UpdateBullet_Railgun(bul, side_id);
  // Normal logic
  int result = UpdateBullet(bul, side_id);
  return ExecuteEventHook(HOOK_POSTUPDATEBULLET, 3, result, side_id, bul->MyIndex, 0, 0);
}
