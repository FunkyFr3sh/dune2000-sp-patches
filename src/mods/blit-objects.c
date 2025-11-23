#include "macros/patch.h"
#include "dune2000.h"

#define MAX_SPRITES_ON_SCREEN 4000

// New extended array for sorting sprites on screen
DisplayListStruct _DisplayList[MAX_SPRITES_ON_SCREEN];

// Increase the number of sprites that could be drawn on screen from 1000 to 4000

// Custom implementation of function AddToDisplayList
DETOUR(0x0042AA90, 0x0042ACD5, _Mod__AddToDisplayList);

char Mod__AddToDisplayList(int xpos, int ypos, int flags, TImage *base_image, TImage *barrel_image, TImage *anim_image, _DWORD *list_pos_ptr, DisplayListStruct **list_item_ptr, RECT *viewport_rect, char side_id, char is_selected, char health_bar_size, int health, int max_health, char object_type, Building *object_ptr)
{
  int ypos_; // ecx
  int sort_ypos; // ebp
  DisplayListStruct *old_ptr; // ebx
  DisplayListStruct *list_item; // edi
  DisplayListStruct *i; // eax
  _DWORD *v21; // esi

  if ( xpos - 128 >= viewport_rect->right )
  {
    return 0;
  }
  if ( xpos + 128 <= viewport_rect->left )
  {
    return 0;
  }
  ypos_ = ypos;
  if ( ypos - 128 >= viewport_rect->bottom || ypos + 128 <= viewport_rect->top )
  {
    return 0;
  }
  sort_ypos = ypos;
  if ( object_ptr->ObjectType == OBJECT_BUILDING )
  {
    if ( _templates_buildattribs[object_ptr->Type]._____Flags & BFLAGS_200000_HAS_SKIRT )
    {
      sort_ypos = ypos - 32;
    }
  }
  old_ptr = 0;
  list_item = *list_item_ptr;
  for ( i = *list_item_ptr; i; i = i->next )
  {
    if ( i->__sort_ypos >= sort_ypos )
    {
      break;
    }
    old_ptr = i;
  }
  if ( i == list_item )
  {
    v21 = list_pos_ptr;
    *list_item_ptr = &_DisplayList[*list_pos_ptr];
  }
  else if ( i )
  {
    v21 = list_pos_ptr;
    list_item = old_ptr->next;
    old_ptr->next = &_DisplayList[*list_pos_ptr];
  }
  else
  {
    if ( !old_ptr )
    {
      DebugFatal("AddToDisplayList", "Old ptr is NULL");
      ypos_ = ypos;
    }
    v21 = list_pos_ptr;
    list_item = 0;
    old_ptr->next = &_DisplayList[*list_pos_ptr];
  }
  if ( *v21 >= MAX_SPRITES_ON_SCREEN )
  {
    DebugFatal("SortSprites", "Sort list too large");
    ypos_ = ypos;
  }
  _DisplayList[*v21].xpos = xpos;
  _DisplayList[*v21].ypos = ypos_;
  _DisplayList[*v21].__sort_ypos = sort_ypos;
  _DisplayList[*v21].__base_image = base_image;
  _DisplayList[*v21].__barrel_image = barrel_image;
  _DisplayList[*v21].__anim_image = anim_image;
  _DisplayList[*v21].__object_type = object_type;
  _DisplayList[*v21].next = list_item;
  _DisplayList[*v21].__side_id = side_id;
  _DisplayList[*v21].__is_selected = is_selected;
  _DisplayList[*v21].__healthbarsize = health_bar_size;
  _DisplayList[*v21].__health = health;
  _DisplayList[*v21].__max_health = max_health;
  _DisplayList[*v21].__object_ptr = object_ptr;
  _DisplayList[(*v21)++].__flags = flags;
  return 1;
}

// Custom implementation of function BlitObjects
DETOUR(0x00428E30, 0x0042A5C8, _Mod__BlitObjects);

void Mod__BlitObjects(TImage *img)
{
  int options_bar_height; // eax
  char *crate_ptr; // esi
  signed int crate_number; // ebp
  CSide *side; // ebx MAPDST
  Building *bld_; // esi MAPDST
  int building_type; // ebp
  int health; // edx
  int half_max_health; // eax
  BuildingBehaviorType behavior; // al
  int building_art; // ecx
  unsigned int max_storage; // edi
  signed int silo_fill_degree; // edx
  int directions; // eax
  int barrel_art; // eax
  int buildup_frame; // ebx
  char building_flags; // al
  Unit *unit; // esi MAPDST
  signed int unit_type; // ebp
  signed int unit_type_; // ebp
  unsigned char rotation; // bl
  signed int unit_art; // edi
  int anim_frame; // ecx
  int xpos; // ebx MAPDST
  int ypos; // edi MAPDST
  DisplayListStruct *list_item_; // ebp
  bool less_than_half_health; // al
  TImagePalette *palette; // eax MAPDST
  char object_type; // al MAPDST
  int bottom; // edx MAPDST
  unsigned char stealth_uncloak_delay_counter; // al MAPDST
  Building *bld; // eax MAPDST
  TImage *data_image; // ecx
  int building_type_; // eax
  int draw_y; // esi MAPDST
  int building_type__; // eax MAPDST
  int health_bar_size_; // ebx
  unsigned char side_id_; // ST3C_1 MAPDST
  BuildingBehaviorType building_behavior; // al
  int filled_blobs; // eax MAPDST
  int drawn_blobs; // eax
  int max_health; // ecx
  char unit_behavior; // cl
  unsigned int spice_blobs; // edx MAPDST
  int cloak_units; // edx
  unsigned int cloak_units_; // eax
  char group_id; // al MAPDST
  int healthbar_height; // ecx
  char flicker_frames; // al MAPDST
  char is_low_health; // [esp+13h] [ebp-4Dh]
  int draw_x; // [esp+14h] [ebp-4Ch] MAPDST
  TImage *base_image; // [esp+18h] [ebp-48h] MAPDST
  short healthbar_color; // [esp+18h] [ebp-48h]
  TImage *barrel_imagea; // [esp+1Ch] [ebp-44h] MAPDST
  unsigned char direction_frame; // [esp+20h] [ebp-40h]
  unsigned char house_id; // [esp+20h] [ebp-40h]
  TImage *anim_image; // [esp+24h] [ebp-3Ch] MAPDST
  unsigned char filled_blobs_; // [esp+24h] [ebp-3Ch]
  unsigned char side_id; // [esp+28h] [ebp-38h]
  int blob_draw_x; // [esp+28h] [ebp-38h] MAPDST
  int blob_draw_y; // [esp+28h] [ebp-38h]
  char health_bar_size; // [esp+2Ch] [ebp-34h] MAPDST
  unsigned char max_blobs; // [esp+2Ch] [ebp-34h]
  DisplayListStruct *list_item; // [esp+30h] [ebp-30h]
  unsigned char v125; // [esp+34h] [ebp-2Ch]
  char v126[4]; // [esp+38h] [ebp-28h]
  int v126_;
  int a3; // [esp+3Ch] [ebp-24h]
  int list_pos; // [esp+40h] [ebp-20h]
  int is_selling_or_buildup; // [esp+44h] [ebp-1Ch]
  unsigned char a4[4]; // [esp+48h] [ebp-18h]
  CSide *a4_;
  int a5; // [esp+4Ch] [ebp-14h]
  RECT rect; // [esp+50h] [ebp-10h]

  options_bar_height = _OptionsBarHeight;
  rect.right = _ViewportWidth;
  list_pos = 0;
  list_item = 0;
  rect.left = 0;
  rect.top = _OptionsBarHeight;
  rect.bottom = _OptionsBarHeight + _ViewportHeight;
  crate_ptr = (char *)&gCrates[0].__image;
  crate_number = 30;
  do
  {
    if ( *(crate_ptr - 1) == 1 )
    {
      BlitClipTImage2(
        img,
        &rect,
        32 * (unsigned char)*(crate_ptr - 4) - _ViewportXPos,
        options_bar_height + 32 * (unsigned char)*(crate_ptr - 3) - _ViewportYPos,
        _images_crate[(unsigned char)*crate_ptr],
        1,
        1);
      options_bar_height = _OptionsBarHeight;
    }
    crate_ptr += 12;
    --crate_number;
  }
  while ( crate_number );
  side_id = 0;
  if ( _SidesToProcess )
  {
    do
    {
      side = GetSide(side_id);
      a4_ = side;
      for ( bld_ = side->__FirstBuildingPtr; bld_; bld_ = bld_->Next )
      {
        anim_image = 0;
        if ( !(bld_->Flags & BFLAGS_100000_INVISIBLE) )
        {
          building_type = bld_->Type;
          v126_ = 0;
          if ( building_type >= gBuildingTypeNum )
          {
            DebugFatal("BlitVehicles", "Error with vehicle data");
          }
          direction_frame = bld_->__DirectionFrame;
          health = bld_->Health;
          building_type__ = building_type;
          half_max_health = _templates_buildattribs[building_type__]._____HitPoints >> 1;
          behavior = _templates_buildattribs[building_type__].__Behavior;
          is_low_health = health < half_max_health;
          if ( behavior == BuildingBehavior_WALL || behavior == BuildingBehavior_TURRET )
          {
            direction_frame = bld_->__WallFrame;
          }
          if ( behavior == BuildingBehavior_CONCRETE )
          {
            DebugFatal("Display.cpp", "Building type is concrete");
          }
          building_art = _templates_buildattribs[building_type__]._____BuildingArt;
          if ( _templates_buildattribs[building_type__].__Behavior == BuildingBehavior_SILO )
          {
            max_storage = side->__MaxStorage;
            if ( max_storage )
            {
              silo_fill_degree = 4500 * side->SpiceReal / max_storage / 1500;
              directions = _templates_BuildingArtDirections[building_art] - 1;
              if ( silo_fill_degree >= directions )
              {
                direction_frame = (_templates_BuildingArtDirections[building_art] & 255) - 1;
              }
              else
              {
                direction_frame = 4500 * side->SpiceReal / max_storage / 1500;
              }
            }
            else
            {
              direction_frame = 0;
            }
          }
          health_bar_size = _templates_buildattribs[building_type__]._____HealthBarSize;
          base_image = gBuildingElements[0][direction_frame + 32 * building_art];
          xpos = bld_->__PosX / 0x10000 - _ViewportXPos;
          xpos = bld_->__PosX / 0x10000 - _ViewportXPos;
          barrel_art = _templates_buildattribs[building_type__]._____BarrelArt;
          ypos = _OptionsBarHeight + bld_->__PosY / 0x10000 - _ViewportYPos;
          if ( barrel_art == -1 )
          {
            barrel_imagea = 0;
          }
          else
          {
            barrel_imagea = gBuildingElements[0][32 * barrel_art
                                               + (unsigned char)_templates_buildattribs[0]._____DirectionFrames[(unsigned char)bld_->__Facing + building_type__ * 268]];
          }
          is_selling_or_buildup = bld_->Flags & (BFLAGS_4_SELLING|BFLAGS_2_BUILDUP);
          if ( is_selling_or_buildup )
          {
            buildup_frame = bld_->__BuildupAnimCounter / 3;
            anim_image = _BuildupAnimationImages[0][buildup_frame
                                                  + 42
                                                  * (unsigned char)_templates_buildattribs[building_type__]._____BuildupArt];
            if ( !base_image
              || buildup_frame < (unsigned char)_templates_buildattribs[building_type__]._____BuildupFramesToShow )
            {
              base_image = _BuildupAnimationImages[0][buildup_frame
                                                    + 42
                                                    * (unsigned char)_templates_buildattribs[building_type__]._____BuildupArt];
              anim_image = 0;
              barrel_imagea = 0;
            }
          }
          building_flags = bld_->Flags;
          if ( building_flags & (BFLAGS_20_ANIM_ONCE|BFLAGS_10_ANIM_PERMANENT) )
          {
            if ( building_flags & BFLAGS_40_HAS_ANIMATION )
            {
              if ( !is_low_health && !is_selling_or_buildup )
              {
                anim_image = _BuildingAnimationImages[0][bld_->__BuildingAnimCounter
                                                       / (signed int)(unsigned char)_templates_buildattribs[(unsigned char)_templates_buildattribs[building_type__]._____BuildingAnimation]._____AnimationSpeed
                                                       + 42
                                                       * (unsigned char)_templates_buildattribs[building_type__]._____BuildingAnimation];
                if ( _templates_buildattribs[building_type__]._____Flags & BFLAGS_800000_ANIM_ALPHA )
                {
                  v126_ = 4;
                }
              }
            }
          }
          AddToDisplayList(
            xpos,
            ypos,
            v126_,
            base_image,
            barrel_imagea,
            anim_image,
            &list_pos,
            &list_item,
            &rect,
            side_id,
            bld_->__IsSelected,
            health_bar_size,
            bld_->Health,
            _templates_buildattribs[building_type__]._____HitPoints,
            1,
            bld_);
          side = a4_;
        }
      }
      for ( unit = side->__FirstUnitPtr; unit; unit = unit->Next )
      {
        if ( !(unit->Flags & UFLAGS_1000000_DRAWN_BEHIND_OBJECTS) )
        {
          unit_type = unit->Type;
          v125 = unit->__CurrentAnimFrame;
          if ( unit_type >= gUnitTypeNum )
          {
            DebugFatal("BlitVehicles", "Error with vehicle data");
          }
          if ( !(unit->Flags & (UFLAGS_200000_NOT_DRAWN|UFLAGS_100_CARRYING|UFLAGS_40_FLYING)) )
          {
            unit_type_ = unit_type << 8;
            a3 = _templates_unitattribs[0].__DirectionFrames[unit->__Facing + unit_type_];
            rotation = a3;
            if ( (unsigned char)a3 >= 32u )
            {
              DebugFatal("BlitVehicles", "Error with vehicle data");
            }
            if ( unit->__CurrentAnimType >= 9 )
            {
              rotation = unit->__CurrentAnimType - 9;
              a3 = unit->__CurrentAnimType - 9;
            }
            unit_art = *(int *)((char *)&_templates_unitattribs[0].__UnitArt + unit_type_);
            if ( unit_art > 90 )
            {
              DebugFatal("Display.cpp", "BaseImage is out of range");
            }
            if ( v125 > 35u )
            {
              DebugFatal("Display.cpp", "Animation frame out of range");
            }
            if ( rotation > 32u )
            {
              DebugFatal("Display.cpp", "Rotation frame out of range");
            }
            if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER && unit->__CurrentAnimFrame )
            {
              anim_frame = (unsigned char)unit->__CurrentAnimFrame;
              base_image = gExplosionElements[0][anim_frame + 40 * (unsigned char)_templates_GroupIDs.EX_HARV];
              if ( !gExplosionElements[0][anim_frame + 40 * (unsigned char)_templates_GroupIDs.EX_HARV] )
              {
                DebugFatal("Display.cpp", "BaseImage is NULL for harvester explosion, frame %d", anim_frame);
              }
            }
            else
            {
              base_image = gUnitElements[0][0][(unsigned char)a3 + 32 * (v125 + 35 * unit_art)];
              if ( !base_image )
              {
                DebugFatal("Display.cpp", "BaseImage is NULL for %s", _templates_UnitNameList[unit->Type]);
              }
            }
            health_bar_size = *(&_templates_unitattribs[0].__HealthbarSize + unit_type_);
            xpos = unit->__PosX / 0x10000 - _ViewportXPos;
            ypos = _OptionsBarHeight + unit->__PosY / 0x10000 - unit->__PosZHeight / 0x10000 - _ViewportYPos;
            if ( *(int *)((char *)&_templates_unitattribs[0].__BarrelArt + unit_type_) == -1 )
            {
              barrel_imagea = 0;
            }
            else
            {
              a5 = _templates_unitattribs[0].__DirectionFrames[(unsigned char)unit->__FacingTurret
                                                                     + unit_type_];
              if ( (unsigned char)a5 >= 32u )
              {
                DebugFatal("BlitVehicles", "Error with vehicle data");
              }
              // New logic start
              // Make barrel art respect unit's animation frame
              barrel_imagea = gUnitElements[0][0][(unsigned char)a5
                                                + 32 * (v125 + 35
                                                * *(int *)((char *)&_templates_unitattribs[0].__BarrelArt + unit_type_))];
              // New logic end
            }
            AddToDisplayList(
              xpos,
              ypos,
              0,
              base_image,
              barrel_imagea,
              0,
              &list_pos,
              &list_item,
              &rect,
              side_id,
              unit->__IsSelected,
              health_bar_size,
              unit->Health,
              *(int *)((char *)&_templates_unitattribs[0].__Strength + unit_type_),
              2,
              (Building *)unit);
          }
        }
      }
      ++side_id;
    }
    while ( side_id < (unsigned char)_SidesToProcess );
  }
  list_item_ = list_item;
  _selectedmaybe_dw_BlitVehicles_512D58 = list_item;
  if ( list_item )
  {
    while ( 1 )
    {
      house_id = _IRValuesBackup[(unsigned char)list_item_->__side_id];
      if ( gBitsPerPixel == 8 )
      {
        _palette_ptr_7984DC = &_palettes_6D9350[256
                                              * (unsigned char)_IRValuesBackup[(unsigned char)list_item_->__side_id]];
      }
      less_than_half_health = 0;
      if ( list_item_->__object_type == 1 )
      {
        if ( list_item_->__object_ptr->Type >= 100u )
        {
          DebugFatal("DISPLAY.CPP", "Building type out of range %d / %d", list_item_->__object_ptr->Type, 100);
        }
        less_than_half_health = list_item_->__object_ptr->Health < (signed int)((unsigned int)_templates_buildattribs[list_item_->__object_ptr->Type]._____HitPoints >> 1);
      }
      base_image = list_item_->__base_image;
      if ( !base_image )
      {
        goto LABEL_149;
      }
      if ( list_item_->__object_type == 1 && less_than_half_health )
      {
        bld = list_item_->__object_ptr;
        if ( !(bld->Flags & (BFLAGS_4_SELLING|BFLAGS_2_BUILDUP)) )
        {
          base_image = _gBuildingElementsDamaged[0][(unsigned char)bld->__WallFrame
                                                  + 32 * _templates_buildattribs[bld->Type]._____BuildingArt];
          if ( !base_image )
          {
            base_image = list_item_->__base_image;
          }
        }
      }
      palette = base_image->palette;
      if ( palette )
      {
        if ( base_image->bitdepth == 8 )
        {
          memcpy(palette->data + 240, &_ColoursBinData[16 * house_id], 0x20u);
        }
      }
      object_type = list_item_->__object_type;
      if ( object_type == 1 )
      {
        break;
      }
      if ( object_type == 2 )
      {
        unit = (Unit *)list_item_->__object_ptr;
        if ( unit->__FlickerFrames & 2 )
        {
          draw_y = list_item_->ypos - base_image->bottom;
          draw_x = list_item_->xpos - base_image->right;
LABEL_90:
          BlitClipTImageColoured(img, &rect, draw_x, draw_y, base_image, 1);
          goto LABEL_105;
        }
        if ( unit->Flags & UFLAGS_4_CLOAKED || unit->__StealthUnCloakDelayCounter )
        {
          if ( list_item_->__side_id == gSideId )
          {
            BlitTImage_4(
              img,
              _colormask1,
              &rect,
              list_item_->xpos - base_image->right,
              list_item_->ypos - base_image->bottom,
              base_image,
              _images_stealths[((_BYTE)gGameTicks + (unsigned char)unit->MyIndex) & 0xF]);
          }
          else
          {
            stealth_uncloak_delay_counter = unit->__StealthUnCloakDelayCounter;
            if ( stealth_uncloak_delay_counter )
            {
              if ( unit->Flags & UFLAGS_4_CLOAKED )
              {
                BlitTImage_12(
                  img,
                  _colormask1,
                  &rect,
                  list_item_->xpos - base_image->right,
                  list_item_->ypos - base_image->bottom,
                  base_image,
                  _images_stealths[((_BYTE)gGameTicks + (unsigned char)unit->MyIndex) & 0xF],
                  11 - (stealth_uncloak_delay_counter >> 1));
              }
              else
              {
                BlitTImage_12(
                  img,
                  _colormask1,
                  &rect,
                  list_item_->xpos - base_image->right,
                  list_item_->ypos - base_image->bottom,
                  base_image,
                  _images_stealths[((_BYTE)gGameTicks + (unsigned char)unit->MyIndex) & 0xF],
                  stealth_uncloak_delay_counter >> 1);
              }
            }
          }
          unit = (Unit *)list_item_->__object_ptr;
          if ( unit->__StealthUnCloakDelayCounter )
          {
            if ( unit->Flags & UFLAGS_4_CLOAKED )
            {
              BlitTImage_11(
                img,
                _colormask1,
                &rect,
                list_item_->xpos - base_image->right,
                list_item_->ypos - base_image->bottom,
                base_image,
                _images_unstealts[-((unsigned int)(unsigned char)unit->__StealthUnCloakDelayCounter >> 1) + 11]);
            }
            else
            {
              BlitTImage_11(
                img,
                _colormask1,
                &rect,
                list_item_->xpos - base_image->right,
                list_item_->ypos - base_image->bottom,
                base_image,
                _images_unstealts[(unsigned int)(unsigned char)unit->__StealthUnCloakDelayCounter >> 1]);
            }
          }
        }
        else
        {
          BlitClipTImage2(
            img,
            &rect,
            list_item_->xpos - base_image->right,
            list_item_->ypos - base_image->bottom,
            base_image,
            1,
            1);
        }
      }
      else
      {
        BlitClipTImage2(
          img,
          &rect,
          list_item_->xpos - base_image->right,
          list_item_->ypos - base_image->bottom,
          base_image,
          1,
          1);
      }
LABEL_105:
      barrel_imagea = list_item_->__barrel_image;
      barrel_imagea = list_item_->__barrel_image;
      if ( barrel_imagea )
      {
        palette = barrel_imagea->palette;
        if ( palette )
        {
          if ( barrel_imagea->bitdepth == 8 )
          {
            memcpy(palette->data + 240, &_ColoursBinData[16 * house_id], 0x20u);
          }
        }
        object_type = list_item_->__object_type;
        if ( object_type == 1 )
        {
          bld_ = list_item_->__object_ptr;
          building_type__ = bld_->Type;
          draw_x = list_item_->xpos + _templates_buildattribs[building_type__]._____ExitPoint1X - barrel_imagea->right;
          draw_y = list_item_->ypos + _templates_buildattribs[building_type__]._____ExitPoint1Y - barrel_imagea->bottom;
          if ( bld_->__FlickerFrames & 2 )
          {
            BlitClipTImageColoured(img, &rect, draw_x, draw_y, barrel_imagea, 1);
          }
          else
          {
            BlitClipTImage2(img, &rect, draw_x, draw_y, barrel_imagea, 1, 1);
          }
        }
        else if ( object_type == 2 )
        {
          xpos = list_item_->xpos;
          bottom = barrel_imagea->bottom;
          draw_x = list_item_->xpos - barrel_imagea->right;
          ypos = list_item_->ypos;
          a3 = ypos;
          draw_y = ypos - bottom;
          unit = (Unit *)list_item_->__object_ptr;
          if ( unit->LastFired < 1u )
          {
            a5 = (unsigned char)list_item_->__object_ptr->custom_byte_2 << 11;
            draw_x += *(int *)((char *)&_sinValues[64] + a5) / -65536;
            draw_y += *(int *)((char *)&_cosValues[64] + a5) / 0x10000;
          }
          unit = (Unit *)list_item_->__object_ptr;
          if ( unit->__FlickerFrames & 2 )
          {
            BlitClipTImageColoured(img, &rect, draw_x, draw_y, barrel_imagea, 1);
          }
          else
          {
            a5 = unit->Flags & UFLAGS_4_CLOAKED;
            if ( a5 || unit->__StealthUnCloakDelayCounter )
            {
              if ( list_item_->__side_id == gSideId )
              {
                BlitTImage_4(
                  img,
                  _colormask1,
                  &rect,
                  draw_x,
                  draw_y,
                  barrel_imagea,
                  _images_stealths[((_BYTE)gGameTicks + (unsigned char)list_item_->__object_ptr->MyIndex) & 0xF]);
              }
              else
              {
                stealth_uncloak_delay_counter = unit->__StealthUnCloakDelayCounter;
                if ( stealth_uncloak_delay_counter )
                {
                  if ( a5 )
                  {
                    BlitTImage_12(
                      img,
                      _colormask1,
                      &rect,
                      xpos - base_image->right,
                      a3 - base_image->bottom,
                      barrel_imagea,
                      _images_stealths[((_BYTE)gGameTicks + (unsigned char)list_item_->__object_ptr->MyIndex) & 0xF],
                      11 - (stealth_uncloak_delay_counter >> 1));
                  }
                  else
                  {
                    BlitTImage_12(
                      img,
                      _colormask1,
                      &rect,
                      xpos - base_image->right,
                      a3 - base_image->bottom,
                      barrel_imagea,
                      _images_stealths[((_BYTE)gGameTicks + (unsigned char)list_item_->__object_ptr->MyIndex) & 0xF],
                      stealth_uncloak_delay_counter >> 1);
                  }
                }
              }
              unit = (Unit *)list_item_->__object_ptr;
              if ( unit->__StealthUnCloakDelayCounter )
              {
                if ( unit->Flags & UFLAGS_4_CLOAKED )
                {
                  BlitTImage_11(
                    img,
                    _colormask1,
                    &rect,
                    list_item_->xpos - barrel_imagea->right,
                    list_item_->ypos - barrel_imagea->bottom,
                    barrel_imagea,
                    _images_unstealts[-((unsigned int)(unsigned char)unit->__StealthUnCloakDelayCounter >> 1) + 11]);
                }
                else
                {
                  BlitTImage_11(
                    img,
                    _colormask1,
                    &rect,
                    list_item_->xpos - barrel_imagea->right,
                    list_item_->ypos - barrel_imagea->bottom,
                    barrel_imagea,
                    _images_unstealts[(unsigned int)(unsigned char)unit->__StealthUnCloakDelayCounter >> 1]);
                }
              }
            }
            else
            {
              BlitClipTImage2(img, &rect, draw_x, draw_y, barrel_imagea, 1, 1);
            }
          }
        }
        else
        {
          BlitClipTImage2(
            img,
            &rect,
            list_item_->xpos - barrel_imagea->right,
            list_item_->ypos - barrel_imagea->bottom,
            barrel_imagea,
            1,
            1);
        }
      }
      anim_image = list_item_->__anim_image;
      if ( anim_image )
      {
        palette = anim_image->palette;
        if ( palette )
        {
          if ( anim_image->bitdepth == 8 )
          {
            memcpy(palette->data + 240, &_ColoursBinData[16 * house_id], 0x20u);
          }
        }
        object_type = list_item_->__object_type;
        if ( object_type == 1 )
        {
          if ( list_item_->__object_ptr->__FlickerFrames & 2 )
          {
            BlitClipTImageColoured(
              img,
              &rect,
              list_item_->xpos - anim_image->right,
              list_item_->ypos - anim_image->bottom,
              anim_image,
              1);
            goto LABEL_149;
          }
          if ( list_item_->__flags & 4 )
          {
            BlitTImage_Flag_200(
              img,
              _colormask1,
              &rect,
              list_item_->xpos - anim_image->right,
              list_item_->ypos - anim_image->bottom,
              anim_image);
            goto LABEL_149;
          }
LABEL_148:
          BlitClipTImage2(
            img,
            &rect,
            list_item_->xpos - anim_image->right,
            list_item_->ypos - anim_image->bottom,
            anim_image,
            1,
            1);
          goto LABEL_149;
        }
        if ( object_type != 2 )
        {
          goto LABEL_148;
        }
        draw_x = list_item_->xpos - anim_image->right;
        draw_y = list_item_->ypos - anim_image->bottom;
        unit = (Unit *)list_item_->__object_ptr;
        if ( unit->__FlickerFrames & 2 )
        {
          BlitClipTImageColoured(img, &rect, draw_x, draw_y, barrel_imagea, 1);
        }
        else if ( unit->Flags & UFLAGS_4_CLOAKED )
        {
          if ( list_item_->__side_id == gSideId )
          {
            BlitTImage_4(
              img,
              _colormask1,
              &rect,
              draw_x,
              draw_y,
              anim_image,
              _images_stealths[((_BYTE)gGameTicks + (unsigned char)list_item_->__object_ptr->MyIndex) & 0xF]);
          }
        }
        else
        {
          BlitClipTImage2(img, &rect, draw_x, draw_y, barrel_imagea, 1, 1);
        }
      }
LABEL_149:
      // Draw spanner on repairing building
      if ( list_item_->__object_type == 1 )
      {
        bld = list_item_->__object_ptr;
        if ( bld->Flags & BFLAGS_1_REPAIRING )
        {
          if ( ((_BYTE)gGameTicks + (bld->MyIndex & 255)) & 0x20 )
          {
            if ( base_image )
            {
              BlitClipTImage2(
                img,
                &rect,
                list_item_->xpos + base_image->width / 2 - base_image->right - _image_spanner->width / 2,
                base_image->height / 2 + list_item_->ypos - base_image->bottom - _image_spanner->height / 2,
                _image_spanner,
                1,
                1);
            }
            else
            {
              data_image = dataimage[_templates_buildattribs[bld->Type]._____BuildingArt];
              BlitClipTImage2(
                img,
                &rect,
                list_item_->xpos + data_image->width / 2 - data_image->right - _image_spanner->width / 2,
                data_image->height / 2 + list_item_->ypos - data_image->bottom - _image_spanner->height / 2,
                _image_spanner,
                1,
                1);
            }
          }
        }
      }
      if ( !list_item_->__is_selected )
      {
        goto LABEL_223;
      }
      // Draw healthbars and selection markers
      if ( base_image )
      {
        bottom = base_image->bottom;
      }
      else
      {
        building_type_ = list_item_->__object_ptr->Type;
        bottom = dataimage[_templates_buildattribs[building_type_]._____BuildingArt]->bottom;
      }
      v126[0] = list_item_->__healthbarsize;
      if ( list_item_->__object_type == 1 )
      {
        draw_y = list_item_->ypos - bottom;
        draw_x = list_item_->xpos;
        building_type__ = list_item_->__object_ptr->Type;
        if ( _templates_buildattribs[building_type__].__Behavior == BuildingBehavior_SILO )
        {
          draw_y -= 6;
        }
        if ( _templates_buildattribs[building_type__]._____BarrelArt != -1 )
        {
          draw_y -= 12;
        }
      }
      else
      {
        draw_x = list_item_->xpos - _images_healthbars[(unsigned char)v126[0]]->width / 2;
        draw_y = list_item_->ypos - 5 * _healthbar_heights[(unsigned char)v126[0]] / 8;
        if ( _templates_unitattribs[(unsigned char)list_item_->__object_ptr->__MaxXPos].__IsInfantry )
        {
          draw_y -= 8;
        }
      }
      health_bar_size_ = (unsigned char)v126[0];
      BlitClipTImage2(img, &rect, draw_x, draw_y, _images_healthbars[(unsigned char)v126[0]], 1, 1);
      BlitClipTImage2(img, &rect, draw_x, draw_y + 4, _image_selection_tl, 1, 1);
      BlitClipTImage2(
        img,
        &rect,
        _images_healthbars[health_bar_size_]->width + draw_x - _image_selection_tr->width,
        draw_y + 4,
        _image_selection_tr,
        1,
        1);
      BlitClipTImage2(
        img,
        &rect,
        draw_x,
        draw_y - _image_selection_tr->height + _healthbar_heights[health_bar_size_] + 4,
        _image_selection_bl,
        1,
        1);
      BlitClipTImage2(
        img,
        &rect,
        _images_healthbars[health_bar_size_]->width + draw_x - _image_selection_tr->width,
        draw_y - _image_selection_tr->height + _healthbar_heights[health_bar_size_] + 4,
        _image_selection_br,
        1,
        1);
      a4[0] = list_item_->__health * (_images_healthbars[health_bar_size_]->width - 3) / list_item_->__max_health;
      if ( list_item_->__object_type == 1 )
      {
        bld = list_item_->__object_ptr;
        flicker_frames = bld->__FlickerFrames;
        if ( flicker_frames )
        {
          bld->__FlickerFrames = flicker_frames - 1;
        }
        if ( list_item_->__side_id == gSideId )
        {
          // Primary building
          if ( list_item_->__object_ptr->Flags & BFLAGS_80_PRIMARY )
          {
            BlitClipTImage2(
              img,
              &rect,
              draw_x + 2,
              draw_y - _image_selection_tr->height + _healthbar_heights[health_bar_size_] - 4,
              _images_small_numbers[1],
              1,
              1);
          }
          side_id_ = list_item_->__side_id;
          a5 = list_item_->__object_ptr->Type;
          side = GetSide(side_id_);
          building_behavior = _templates_buildattribs[a5].__Behavior;
          if ( building_behavior == BuildingBehavior_REFINERY )
          {
            filled_blobs = (unsigned int)(16 * side->SpiceReal) / side->__MaxStorage;
            max_blobs = 16;
            if ( filled_blobs > 15 )
            {
              filled_blobs_ = 15;
LABEL_180:
              if ( max_blobs )
              {
                blob_draw_x = draw_x - 1;
                filled_blobs = filled_blobs_;
                a5 = filled_blobs_;
                if ( filled_blobs_ )
                {
                  a3 = filled_blobs_;
                  do
                  {
                    blob_draw_x += 3;
                    BlitClipTImage2(
                      img,
                      &rect,
                      blob_draw_x,
                      draw_y + _healthbar_heights[health_bar_size_] - 1,
                      _images_blobs[1],
                      1,
                      1);
                    --a3;
                  }
                  while ( a3 );
                  filled_blobs = a5;
                }
                drawn_blobs = max_blobs - 1;
                if ( filled_blobs < drawn_blobs )
                {
                  a3 = drawn_blobs - filled_blobs;
                  do
                  {
                    blob_draw_x += 3;
                    BlitClipTImage2(
                      img,
                      &rect,
                      blob_draw_x,
                      draw_y + _healthbar_heights[health_bar_size_] - 1,
                      _images_blobs[0],
                      1,
                      1);
                    --a3;
                  }
                  while ( a3 );
                }
              }
              side_id_ = list_item_->__side_id;
              if ( side_id_ == gSideId )
              {
                if ( GetSide(side_id_)->__BuildingGroupUpgradeCount[(unsigned char)_templates_buildattribs[list_item_->__object_ptr->Type].GroupType] )
                {
                  BlitClipTImage2(img, &rect, draw_x + 2, draw_y + 6, _images_miscIcon[0], 1, 1);
                }
              }
              goto LABEL_191;
            }
          }
          else
          {
            if ( building_behavior != BuildingBehavior_SILO )
            {
              max_blobs = 0;
              filled_blobs_ = 0;
              goto LABEL_180;
            }
            filled_blobs = (unsigned int)(8 * side->SpiceReal) / side->__MaxStorage;
            max_blobs = 8;
            if ( filled_blobs > 7 )
            {
              filled_blobs_ = 7;
              goto LABEL_180;
            }
          }
          filled_blobs_ = filled_blobs;
          goto LABEL_180;
        }
      }
LABEL_191:
      max_health = list_item_->__max_health;
      if ( gBitsPerPixel == 16 )
      {
        if ( list_item_->__health >= max_health / 3 )
        {
          if ( list_item_->__health >= list_item_->__max_health / 2 )
          {
            healthbar_color = _GreenColor16;
          }
          else
          {
            healthbar_color = _YellowColor16;
          }
        }
        else
        {
          healthbar_color = _RedColor16;
        }
      }
      else if ( list_item_->__health >= max_health / 3 )
      {
        if ( list_item_->__health >= list_item_->__max_health / 2 )
        {
          healthbar_color = (unsigned char)_GreenColor8;
        }
        else
        {
          healthbar_color = (unsigned char)_YellowColor8;
        }
      }
      else
      {
        healthbar_color = (unsigned char)_RedColor8;
      }
      a3 = draw_x + 1;
      a5 = a4[0] + draw_x + 1;
      DrawLineClipTImage(img, &rect, draw_x + 1, draw_y + 1, a5, draw_y + 1, healthbar_color);
      DrawLineClipTImage(img, &rect, a3, draw_y + 2, a5, draw_y + 2, healthbar_color);
      if ( list_item_->__side_id != gSideId || list_item_->__object_type != 2 )
      {
        goto LABEL_223;
      }
      unit = (Unit *)list_item_->__object_ptr;
      v125 = 6;
      unit_behavior = _templates_unitattribs[unit->Type].__Behavior;
      if ( unit_behavior == UnitBehavior_HARVESTER )
      {
        a3 = 0;
        blob_draw_x = draw_x - 1;
        if ( unit->__SpecialPurpose )
        {
          do
          {
            blob_draw_x += 3;
            BlitClipTImage2(
              img,
              &rect,
              blob_draw_x,
              draw_y + _healthbar_heights[health_bar_size_] - 1,
              _images_blobs[1],
              1,
              1);
            spice_blobs = (list_item_->__object_ptr->Flags >> 8) & 255;
            ++a3;
          }
          while ( (unsigned int)a3 < spice_blobs );
        }
        spice_blobs = (list_item_->__object_ptr->Flags >> 8) & 255;
        // New logic start
        // Customizable Harvester storage capacity
        unsigned int storage_capacity = _templates_unitattribs[unit->Type].StorageCapacity?_templates_unitattribs[unit->Type].StorageCapacity:7;
        if ( spice_blobs < storage_capacity )
        {
          a3 = storage_capacity - spice_blobs;
        // New logic end
          do
          {
            blob_draw_x += 3;
            BlitClipTImage2(
              img,
              &rect,
              blob_draw_x,
              draw_y + _healthbar_heights[health_bar_size_] - 1,
              _images_blobs[0],
              1,
              1);
            --a3;
          }
          while ( a3 );
        }
LABEL_217:
        v125 = 10;
        goto LABEL_218;
      }
      if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_SABOTEUR )
      {
        a3 = 0;
        blob_draw_y = draw_y + _healthbar_heights[health_bar_size_] + 1;
        if ( unit->__SpecialPurpose & 0xE0 )
        {
          do
          {
            blob_draw_y -= 3;
            BlitClipTImage2(img, &rect, draw_x, blob_draw_y, _images_blobs[1], 1, 1);
            cloak_units = (unsigned int)((list_item_->__object_ptr->Flags >> 8) & 255) >> 5;
            ++a3;
          }
          while ( a3 < cloak_units );
        }
        cloak_units_ = (unsigned int)((list_item_->__object_ptr->Flags >> 8) & 255) >> 5;
        // New logic start
        // Customizable Saboteur cloaking capacity
        unsigned int storage_capacity = (_templates_unitattribs[unit->Type].StorageCapacity?_templates_unitattribs[unit->Type].StorageCapacity:160) >> 5;
        if ( cloak_units_ < storage_capacity )
        {
          a3 = storage_capacity - cloak_units_;
        // New logic end
          do
          {
            blob_draw_y -= 3;
            BlitClipTImage2(img, &rect, draw_x, blob_draw_y, _images_blobs[0], 1, 1);
            --a3;
          }
          while ( a3 );
        }
        goto LABEL_217;
      }
LABEL_218:
      group_id = list_item_->__object_ptr->Flags;
      if ( group_id != -1 && (unsigned char)group_id < 10u )
      {
        BlitClipTImage2(
          img,
          &rect,
          draw_x + 2,
          _healthbar_heights[health_bar_size_] + draw_y - v125,
          _images_small_numbers[list_item_->__object_ptr->Flags & 255],
          1,
          1);
      }
      if ( list_item_->__object_ptr->field_10 == UNIT_STATE_18_AWAITING_AIRLIFT )
      {
        BlitClipTImage2(img, &rect, draw_x + 2, draw_y + 6, _images_miscIcon[2], 1, 1);
      }
LABEL_223:
      if ( !_gFullscreen_DebugModes_pathfinddebug && list_item_->__object_type == 2 )
      {
        xpos = list_item_->xpos;
        v126[0] = list_item_->__healthbarsize;
        draw_x = xpos - _images_healthbars[(unsigned char)v126[0]]->width / 2;
        healthbar_height = _healthbar_heights[(unsigned char)v126[0]];
        unit = (Unit *)list_item_->__object_ptr;
        draw_y = list_item_->ypos - 5 * healthbar_height / 8;
        group_id = unit->__GroupID;
        if ( group_id != -1 && (unsigned char)group_id < 10u )
        {
          BlitClipTImage2(
            img,
            &rect,
            draw_x + 2,
            draw_y + healthbar_height - 6,
            _images_small_numbers[(unsigned char)unit->__GroupID],
            1,
            1);
        }
        unit = (Unit *)list_item_->__object_ptr;
        if ( unit->State == UNIT_STATE_3_GUARDING )
        {
          BlitClipTImage2(img, &rect, draw_x + 2, draw_y + 2, _images_small_numbers[0], 1, 1);
        }
        else if ( unit->Flags & UFLAGS_100000 )
        {
          BlitClipTImage2(img, &rect, draw_x + 2, draw_y + 2, _images_small_numbers[1], 1, 1);
        }
      }
      if ( list_item_->__object_type == 1 )
      {
        bld = list_item_->__object_ptr;
        flicker_frames = bld->__FlickerFrames;
        if ( flicker_frames )
        {
          bld->__FlickerFrames = flicker_frames - 1;
        }
      }
      if ( list_item_->__object_type == 2 )
      {
        unit = (Unit *)list_item_->__object_ptr;
        flicker_frames = unit->__FlickerFrames;
        if ( flicker_frames )
        {
          unit->__FlickerFrames = flicker_frames - 1;
        }
      }
      list_item_ = list_item_->next;
      if ( !list_item_ )
      {
        goto LABEL_239;
      }
    }
    draw_x = list_item_->xpos - base_image->right;
    draw_y = list_item_->ypos - base_image->bottom;
    if ( list_item_->__barrel_image )
    {
      building_type__ = list_item_->__object_ptr->Type;
      draw_x += _templates_buildattribs[building_type__]._____ExitPoint1X;
      draw_y += _templates_buildattribs[building_type__]._____ExitPoint1Y;
    }
    if ( !(list_item_->__object_ptr->__FlickerFrames & 2) )
    {
      BlitClipTImage2(img, &rect, draw_x, draw_y, base_image, 1, 1);
      goto LABEL_105;
    }
    goto LABEL_90;
  }
LABEL_239:
  BlitExplosions(img, 0);
  BlitFlyingUnits(img, 0);
  BlitExplosions(img, 1);
  BlitFlyingUnits(img, 1);
}
