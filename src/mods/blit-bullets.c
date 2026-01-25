#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"
#include "shooting-restrictions.h"
#include "extended-tileset.h"
#include "extended-templates.h"

// Implement railgun bullet behavior

void BlitBullet_Railgun(TImage *img, Bullet *bul, int side_id)
{
  // Get bullet properties
  BullAtrbStruct *bullet_template = &_templates_bulletattribs[bul->Type];
  int range = bullet_template->__Range;
  int projectile_speed = bullet_template->__ProjectileSpeed >> 16;
  // Calculate positional stuff
  int num_steps = range;
  if (num_steps == 0)
    num_steps = 1;
  // Go step by step
  float xpos = bul->__PosX >> 16;
  float ypos = bul->__PosY >> 16;
  int step_counter = 0;
  for (int i = 0; i < num_steps; i++)
  {
    // Check if position is inside map area
    if (!(xpos > 0 && ypos > 0 && xpos < (gGameMapWidth * 32) && ypos < (gGameMapHeight * 32)))
      break;
    // Check if blocked position was reached
    if (bul->RailgunBlockedX == xpos && bul->RailgunBlockedY == ypos)
      break;
    // Draw explosion only after n steps
    if (step_counter == 0)
    {
      RECT rect;
      rect.top = _OptionsBarHeight;
      rect.left = 0;
      rect.bottom = _ViewportHeight + _OptionsBarHeight;
      rect.right = _ViewportWidth;
      TImage *image_to_draw = gExplosionElements[(int)bullet_template->__TrailExplosion][bul->RailgunAnimationFrame];
      int draw_x = (int)xpos - image_to_draw->right - _ViewportXPos;
      int draw_y = (int)ypos - image_to_draw->bottom - _ViewportYPos + _OptionsBarHeight;
      int flags = _templates_Explosiondata_AnimationArtFlags[(int)bullet_template->__TrailExplosion];
      if ( flags & EXPFLAGS_8_HOUSE_COLORED )
      {
        TImagePalette *palette = image_to_draw->palette;
        if ( palette && image_to_draw->bitdepth == 8 )
          memcpy(palette->data + 240, &_ColoursBinData[16 * _IRValuesBackup[side_id]], 0x20u);
      }
      if ( !(flags & (EXPFLAGS_800_INVISIBLE|EXPFLAGS_20)) )
      {
        if ( flags & EXPFLAGS_2 )
        {
          BlitTImage_Flags_2(img, _colormask1, &rect, draw_x, draw_y, image_to_draw);
        }
        else if ( flags & EXPFLAGS_4 )
        {
          BlitTImage_Flags_4(img, _colormask1, &rect, draw_x, draw_y, image_to_draw);
        }
        else if ( flags & EXPFLAGS_10 )
        {
          BlitTImage_Flags_10(img, _colormask1, &rect, draw_x, draw_y, image_to_draw);
        }
        else if ( flags & EXPFLAGS_40_RED )
        {
          BlitTImage_Flags_40(img, _colormask1, &rect, draw_x, draw_y, image_to_draw);
        }
        else if ( flags & EXPFLAGS_80_GREEN )
        {
          BlitTImage_Flag_80(img, _colormask1, &rect, draw_x, draw_y, image_to_draw);
        }
        else if ( flags & EXPFLAGS_200 )
        {
          BlitTImage_Flag_200(img, _colormask1, &rect, draw_x, draw_y, image_to_draw);
        }
        else
        {
          BlitClipTImage2(img, &rect, draw_x, draw_y, image_to_draw, 1, 1);
        }
      }
    }
    // Move to next step
    step_counter++;
    if (step_counter >= projectile_speed)
      step_counter = 0;
    xpos += bul->RailgunStepX;
    ypos += bul->RailgunStepY;
  }
}

// Custom implementation of function BlitBullets
DETOUR(0x00428070, 0x00428219, _Mod__BlitBullets);

void Mod__BlitBullets(TImage *img)
{
  unsigned int side_id; // esi MAPDST
  Bullet *bullet; // ebp
  TImage *bullet_image; // ebx
  int draw_x; // edi
  int draw_y; // esi
  int draw_z_offset; // eax MAPDST
  int bullet_flags; // ecx MAPDST
  RECT rect; // [esp+Ch] [ebp-10h]

  rect.top = _OptionsBarHeight;
  rect.bottom = _ViewportHeight + _OptionsBarHeight;
  side_id = 0;
  rect.left = 0;
  rect.right = _ViewportWidth;
  side_id = 0;
  if ( _SidesToProcess )
  {
    do
    {
      for ( bullet = GetSide(side_id)->__FirstBulletPtr; bullet; bullet = bullet->Next )
      {
        // New logic start
        // Implement railgun bullet behavior
        if (_templates_bulletattribs[bullet->Type].Behavior == BulletBehavior_RAILGUN)
          BlitBullet_Railgun(img, bullet, side_id);
        // New logic end
        else if ( !bullet->__Delay )
        {
          bullet_image = gBulletElements[0][bullet->__AnimationFrame
                                          + 32 * _templates_bulletattribs[bullet->Type].__ProjectileArt];
          draw_x = bullet->__PosX / 0x10000 - bullet_image->right - _ViewportXPos;
          draw_y = _OptionsBarHeight + bullet->__PosY / 0x10000 - bullet_image->bottom - _ViewportYPos;
          draw_z_offset = bullet->__PosZHeight / 0x10000;
          draw_z_offset = bullet->__PosZHeight / 0x10000;
          if ( bullet->Type >= (signed int)gBulletTypeNum )
          {
            DebugFatal("BlitBullets", "bulletType >= gBulletTypeNum");
          }
          bullet_flags = bullet->Flags;
          if ( bullet_flags & BULFLAGS_800_SONIC )
          {
            SetLensImage(draw_x, draw_y - draw_z_offset, bullet_image, img);
          }
          if ( bullet->Flags & BULFLAGS_2_DRAW_SHADOW )
          {
            BlitTImage_Shadow(img, &rect, draw_x, draw_y, bullet_image);
          }
          bullet_flags = bullet->Flags;
          if ( bullet_flags & BULFLAGS_200_DRAW_ALPHA )
          {
            BlitTImage_Flag_200(img, _colormask1, &rect, draw_x, draw_y - draw_z_offset, bullet_image);
          }
          else
          {
            BlitClipTImage2(img, &rect, draw_x, draw_y - draw_z_offset, bullet_image, 1, 1);
          }
        }
      }
      ++side_id;
    }
    while ( side_id < (unsigned char)_SidesToProcess );
  }
}
