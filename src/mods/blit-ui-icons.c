#include "macros/patch.h"
#include "dune2000.h"

// Implement Max Count building template parameter
// Implement Max Count unit template parameter

// Custom implementation of function BlitUIIcons
DETOUR(0x00426E80, 0x00427685, _Mod__BlitUIIcons);

void Mod__BlitUIIcons(TImage *img, int strip_bld_scroll_pos, int strip_bld_scroll_pos_prev, int strip_unit_scroll_pos, int strip_unit_scroll_pos_prev, bool full_redraw)
{
  int strip_1_draw_left; // ebp
  int img_height; // ecx
  int strip_1_draw_top; // esi
  char v10; // al
  unsigned char colorRB; // cl
  unsigned char colorG; // al
  short color16; // di MAPDST
  char *fontpal; // edx MAPDST
  int color8_dup_word; // edx
  int color8_dup_dword; // ecx
  CSide *side; // edi MAPDST
  short built_building_type; // ax MAPDST
  int building_icon_num; // edi
  int *building_icon_ptr; // eax MAPDST
  int building_type; // eax
  int width; // ST10_4 MAPDST
  int text_draw_y; // ST0C_4 MAPDST
  TImage **unit_icon_construction_img_ptr; // edi
  short built_unit_type; // ax
  int strip_2_draw_left; // ebp
  int strip_2_draw_top; // esi
  int first_unit_icon_to_draw; // ecx
  int unit_icon_num; // edi
  int unit_type; // edx
  int unit_type_offset; // ecx
  int unit_type_; // eax
  int unit_type_offset_; // eax
  char unit_behavior; // al MAPDST
  char v55; // al
  int v56; // eax MAPDST
  int text_id; // eax MAPDST
  unsigned char font_palette_index; // ST18_1 MAPDST
  unsigned char font; // ST14_1 MAPDST
  char *text; // eax MAPDST
  int num_building_icons_to_draw; // [esp+10h] [ebp-4Ch]
  int num_unit_icons_to_draw; // [esp+10h] [ebp-4Ch]
  int *unit_icon_ptr; // [esp+14h] [ebp-48h]
  RECT building_strip_rect; // [esp+1Ch] [ebp-40h]
  RECT img_rect; // [esp+2Ch] [ebp-30h]
  RECT unit_strip_rect; // [esp+3Ch] [ebp-20h]
  RECT icon_rect; // [esp+4Ch] [ebp-10h]
  char color8; // [esp+60h] [ebp+4h]
  BuildQueueStruct *unit_build_queue_ptr; // [esp+64h] [ebp+8h] MAPDST
  int colorRGB; // [esp+68h] [ebp+Ch]
  int a4a; // [esp+6Ch] [ebp+10h]
  unsigned int unit_build_queue_num; // [esp+70h] [ebp+14h]
  bool a6a; // [esp+74h] [ebp+18h]

  strip_1_draw_left = _SidebarStrip1XPos;
  img_height = img->height;
  strip_1_draw_top = _SidebarStrip1YPos;
  img_rect.left = 0;
  img_rect.top = 0;
  img_rect.right = img->width;
  img_rect.bottom = img_height;
  building_strip_rect.left = _SidebarStrip1XPos;
  building_strip_rect.top = _SidebarStrip1YPos;
  building_strip_rect.right = _SidebarStrip1XPos + _image_building_icon_construction->width;
  building_strip_rect.bottom = _SidebarStrip1YPos + _SideBarIconCount * _SideBarIconHeight2;
  num_building_icons_to_draw = _SideBarIconCount;
  _palette_ptr_7984DC = _palette_6E5620;
  if ( strip_bld_scroll_pos_prev != strip_bld_scroll_pos )
  {
    strip_1_draw_top = _image_building_icon_construction->height / -2 + _SidebarStrip1YPos;
    num_building_icons_to_draw = _SideBarIconCount + 1;
    if ( strip_bld_scroll_pos_prev < strip_bld_scroll_pos )
    {
      --strip_bld_scroll_pos;
    }
  }
  v10 = _drawdata_512D70 + 1;
  _drawdata_512D70++;
  if ( _drawdata_512D70 == 40 )
  {
    v10 = 0;
    _drawdata_512D70 = 0;
  }
  colorRB = v10 + 20;
  colorG = v10 - 46;
  colorRGB = colorRB;
  colorRGB |= colorG << 8;
  colorRGB |= colorRB << 16;
  if ( gBitsPerPixel == 16 )
  {
    color16 = GetColor16bit(_colormask1, colorRGB);
    short *fontpal16 = (short *)GetFontPaletteHandle(_fontpal_index_green_flashing);
    for (int i = 2; i < 16; i++)
      fontpal16[i] = color16;
  }
  else
  {
    color8 = GetColor8bit(colorRB >> 2, colorG >> 2, colorRB >> 2, (unsigned char *)_PalettePtr, 0, 0, 1);
    fontpal = GetFontPaletteHandle(_fontpal_index_green_flashing);
    color8_dup_word = color8;
    color8_dup_word |= color8 << 8;
    fontpal += 2;
    color8_dup_dword = color8_dup_word;
    color8_dup_dword |= color8_dup_word << 16;
    *(_DWORD *)fontpal = color8_dup_dword;
    *((_DWORD *)fontpal + 1) = color8_dup_dword;
    *((_DWORD *)fontpal + 2) = color8_dup_dword;
    *((_WORD *)fontpal + 6) = color8_dup_word;
  }
  side = GetSide(gSideId);
  if ( side->__BuildingBuildQueue.__type >= 100 )
  {
    DebugFatal("BlitUIIcons", "Invalid index");
  }
  built_building_type = side->__BuildingBuildQueue.__type;
  if ( built_building_type >= 0 )
  {
    if ( full_redraw )
    {
      BlitClipTImageColoured(
        _image_building_icon_construction,
        &img_rect,
        0,
        0,
        _BuildingUIIcons[built_building_type],
        0);
      UpdateIconImage(
        _BuildingUIIcons[side->__BuildingBuildQueue.__type],
        _image_building_icon_construction,
        0,
        (unsigned short)side->__BuildingBuildQueue.__build_progress >> 6);
    }
    else
    {
      if ( !side->__BuildingBuildQueue.__build_progress_prev )
      {
        BlitClipTImageColoured(
          _image_building_icon_construction,
          &img_rect,
          0,
          0,
          _BuildingUIIcons[built_building_type],
          0);
      }
      UpdateIconImage(
        _BuildingUIIcons[side->__BuildingBuildQueue.__type],
        _image_building_icon_construction,
        (unsigned short)side->__BuildingBuildQueue.__build_progress_prev >> 6,
        (unsigned short)side->__BuildingBuildQueue.__build_progress >> 6);
    }
  }
  building_icon_num = 0;
  if ( num_building_icons_to_draw > 0 )
  {
    building_icon_ptr = &side->__BuildingIcons[strip_bld_scroll_pos];
    building_icon_ptr = &side->__BuildingIcons[strip_bld_scroll_pos];
    do
    {
      if ( *building_icon_ptr == -1 )
      {
        BlitClipTImage2(
          img,
          &building_strip_rect,
          strip_1_draw_left,
          strip_1_draw_top + _SideBarIconHeight2 * building_icon_num,
          _image_empty_icon,
          0,
          0);
      }
      else
      {
        built_building_type = side->__BuildingBuildQueue.__type;
        if ( built_building_type >= 0 )
        {
          building_type = *building_icon_ptr;
          if ( building_type == built_building_type )
          {
            BlitClipTImage2(
              img,
              &building_strip_rect,
              strip_1_draw_left,
              strip_1_draw_top + _SideBarIconHeight2 * building_icon_num,
              _image_building_icon_construction,
              0,
              0);
            if ( strip_1_draw_top == _SidebarStrip1YPos )
            {
              if ( side->__BuildingBuildQueue.__on_hold )
              {
                font_palette_index = _fontpal_index_green_flashing;
                font = _shadowed_font_id;
                width = _SidebarIconWidth;
                text_draw_y = _SideBarIconHeight2 * building_icon_num + strip_1_draw_top + 16;
                text_id = GetTextID("Hold");
                text = GetTextString(text_id, 1);
                GL_DrawStringCentered1Color(img, text, strip_1_draw_left, text_draw_y, width, font, font_palette_index);
              }
              if ( side->__BuildingBuildQueue.__build_progress >= 0x5A00u )
              {
                font_palette_index = _fontpal_index_green_flashing;
                font = _shadowed_font_id;
                width = _SidebarIconWidth;
                text_draw_y = _SideBarIconHeight2 * building_icon_num + strip_1_draw_top + 16;
                text_id = GetTextID("Ready");
                text = GetTextString(text_id, 1);
                GL_DrawStringCentered1Color(img, text, strip_1_draw_left, text_draw_y, width, font, font_palette_index);
                icon_rect.left = strip_1_draw_left;
                icon_rect.top = strip_1_draw_top + _SideBarIconHeight2 * building_icon_num;
                icon_rect.right = strip_1_draw_left + _image_building_icon_construction->width - 1;
                icon_rect.bottom = strip_1_draw_top
                                 + _image_building_icon_construction->height
                                 + _SideBarIconHeight2 * building_icon_num
                                 - 1;
                BlitRectRGB(img, &icon_rect, colorRGB);
              }
            }
          }
          else
          {
            BlitClipTImageColoured(
              img,
              &building_strip_rect,
              strip_1_draw_left,
              strip_1_draw_top + _SideBarIconHeight2 * building_icon_num,
              _BuildingUIIcons[building_type],
              0);
          }
        }
        else
        {
          // New logic start
          // Implement Max Count building template parameter
          building_type = *building_icon_ptr;
          if (_templates_buildattribs[building_type].MaxCount && GetSide(gSideId)->__BuildingsExistPerGroup[_templates_buildattribs[building_type].GroupType] >= _templates_buildattribs[building_type].MaxCount)
          {
            BlitClipTImageColoured(
              img,
              &building_strip_rect,
              strip_1_draw_left,
              strip_1_draw_top + _SideBarIconHeight2 * building_icon_num,
              _BuildingUIIcons[building_type],
              0);
          }
          else
          {
            BlitClipTImage2(
              img,
              &building_strip_rect,
              strip_1_draw_left,
              strip_1_draw_top + _SideBarIconHeight2 * building_icon_num,
              _BuildingUIIcons[building_type],
              0,
              0);
          }
          // New logic end
        }
      }
      ++building_icon_num;
      ++building_icon_ptr;
    }
    while ( building_icon_num < num_building_icons_to_draw );
  }
  unit_icon_construction_img_ptr = _images_unit_icon_construction;
  unit_build_queue_ptr = side->__UnitBuildQueue;
  do
  {
    built_unit_type = unit_build_queue_ptr->__type;
    if ( unit_build_queue_ptr->__type >= 0 )
    {
      if ( full_redraw )
      {
        BlitClipTImageColoured(*unit_icon_construction_img_ptr, &img_rect, 0, 0, _UnitUIIcons[built_unit_type], 0);
        UpdateIconImage(
          _UnitUIIcons[unit_build_queue_ptr->__type],
          *unit_icon_construction_img_ptr,
          0,
          (unsigned short)unit_build_queue_ptr->__build_progress >> 6);
      }
      else
      {
        if ( !unit_build_queue_ptr->__build_progress_prev )
        {
          BlitClipTImageColoured(*unit_icon_construction_img_ptr, &img_rect, 0, 0, _UnitUIIcons[built_unit_type], 0);
        }
        UpdateIconImage(
          _UnitUIIcons[unit_build_queue_ptr->__type],
          *unit_icon_construction_img_ptr,
          (unsigned short)unit_build_queue_ptr->__build_progress_prev >> 6,
          (unsigned short)unit_build_queue_ptr->__build_progress >> 6);
      }
    }
    ++unit_icon_construction_img_ptr;
    ++unit_build_queue_ptr;
  }
  while ( unit_icon_construction_img_ptr < dataimage );
  strip_2_draw_left = _SidebarStrip2XPos;
  strip_2_draw_top = _SidebarStrip2YPos;
  unit_strip_rect.left = _SidebarStrip2XPos;
  unit_strip_rect.top = _SidebarStrip2YPos;
  num_unit_icons_to_draw = _SideBarIconCount;
  unit_strip_rect.right = _SidebarStrip2XPos + _images_unit_icon_construction[0]->width;
  unit_strip_rect.bottom = _SidebarStrip2YPos + _SideBarIconCount * _SideBarIconHeight2;
  first_unit_icon_to_draw = strip_unit_scroll_pos;
  if ( strip_unit_scroll_pos_prev != strip_unit_scroll_pos )
  {
    strip_2_draw_top = _images_unit_icon_construction[0]->height / -2 + _SidebarStrip2YPos;
    num_unit_icons_to_draw = _SideBarIconCount + 1;
    if ( strip_unit_scroll_pos_prev < strip_unit_scroll_pos )
    {
      first_unit_icon_to_draw = strip_unit_scroll_pos - 1;
    }
  }
  unit_icon_num = 0;
  if ( num_unit_icons_to_draw > 0 )
  {
    unit_icon_ptr = &side->__UnitIcons[first_unit_icon_to_draw];
    do
    {
      unit_type = *unit_icon_ptr;
      if ( *unit_icon_ptr == -1 )
      {
        BlitClipTImage2(
          img,
          &unit_strip_rect,
          strip_2_draw_left,
          strip_2_draw_top + _SideBarIconHeight2 * unit_icon_num,
          _image_empty_icon,
          0,
          0);
      }
      else
      {
        unit_type_offset = unit_type << 8;
        a4a = -1;
        a6a = 0;
        unit_build_queue_num = 0;
        unit_build_queue_ptr = side->__UnitBuildQueue;
        do
        {
          unit_type_ = unit_build_queue_ptr->__type;
          if ( unit_type_ >= 0 )
          {
            unit_type_offset_ = unit_type_ << 8;
            if ( *(int *)((char *)&_templates_unitattribs[0].__PreReq1 + unit_type_offset_) == *(int *)((char *)&_templates_unitattribs[0].__PreReq1 + unit_type_offset) )
            {
              unit_behavior = *(&_templates_unitattribs[0].__Behavior + unit_type_offset_);
              if ( unit_behavior != UnitBehavior_ORNITHOPTER
                && unit_behavior != UnitBehavior_DEATH_HAND
                && unit_behavior != UnitBehavior_SABOTEUR
                && unit_behavior != UnitBehavior_FREMEN )
              {
                a6a = 1;
              }
            }
            unit_type = *unit_icon_ptr;
            if ( *unit_icon_ptr == unit_build_queue_ptr->__type )
            {
              a4a = unit_build_queue_num;
            }
          }
          ++unit_build_queue_num;
          ++unit_build_queue_ptr;
        }
        while ( unit_build_queue_num < 0xA );
        unit_behavior = *(&_templates_unitattribs[0].__Behavior + unit_type_offset);
        if ( unit_behavior != UnitBehavior_ORNITHOPTER
          && unit_behavior != UnitBehavior_DEATH_HAND
          && unit_behavior != UnitBehavior_SABOTEUR
          && unit_behavior != UnitBehavior_FREMEN )
        {
          v55 = a6a;
        }
        else
        {
          v55 = 1;
        }
        if ( v55 )
        {
          if ( a4a < 0 )
          {
            BlitClipTImageColoured(
              img,
              &unit_strip_rect,
              strip_2_draw_left,
              strip_2_draw_top + _SideBarIconHeight2 * unit_icon_num,
              _UnitUIIcons[unit_type],
              0);
          }
          else
          {
            BlitClipTImage2(
              img,
              &unit_strip_rect,
              strip_2_draw_left,
              strip_2_draw_top + _SideBarIconHeight2 * unit_icon_num,
              _images_unit_icon_construction[a4a],
              0,
              0);
            if ( strip_2_draw_top == _SidebarStrip2YPos )
            {
              v56 = (int)side + 20 * a4a;
              if ( *(_BYTE *)(v56 + 156992) )
              {
                font_palette_index = _fontpal_index_green_flashing;
                font = _shadowed_font_id;
                width = _SidebarIconWidth;
                text_draw_y = _SideBarIconHeight2 * unit_icon_num + strip_2_draw_top + 16;
                text_id = GetTextID("Hold");
                text = GetTextString(text_id, 1);
                GL_DrawStringCentered1Color(img, text, strip_2_draw_left, text_draw_y, width, font, font_palette_index);
              }
              if ( *(unsigned short *)(v56 + 156978) >= 0x5A00u )
              {
                font_palette_index = _fontpal_index_green_flashing;
                font = _shadowed_font_id;
                width = _SidebarIconWidth;
                text_draw_y = _SideBarIconHeight2 * unit_icon_num + strip_2_draw_top + 16;
                text_id = GetTextID("Ready");
                text = GetTextString(text_id, 1);
                GL_DrawStringCentered1Color(img, text, strip_2_draw_left, text_draw_y, width, font, font_palette_index);
                icon_rect.left = strip_2_draw_left;
                icon_rect.top = strip_2_draw_top + _SideBarIconHeight2 * unit_icon_num;
                icon_rect.right = strip_2_draw_left + _image_building_icon_construction->width - 1;
                icon_rect.bottom = strip_2_draw_top
                                 + _image_building_icon_construction->height
                                 + _SideBarIconHeight2 * unit_icon_num
                                 - 1;
                BlitRectRGB(img, &icon_rect, colorRGB);
              }
            }
          }
        }
        else
        {
          // New logic start
          // Implement Max Count unit template parameter
          int count = 0;
          bool max_count_exist = false;
          if (_templates_unitattribs[unit_type].MaxCount)
            for (Unit *u = GetSide(gSideId)->__FirstUnitPtr; u; u = u->Next)
              if (u->Type == unit_type && ++count >= _templates_unitattribs[unit_type].MaxCount)
              {
                max_count_exist = true;
                break;
              }
          if (max_count_exist)
          {
            BlitClipTImageColoured(
              img,
              &unit_strip_rect,
              strip_2_draw_left,
              strip_2_draw_top + _SideBarIconHeight2 * unit_icon_num,
              _UnitUIIcons[unit_type],
              0);
          }
          else
          {
            BlitClipTImage2(
              img,
              &unit_strip_rect,
              strip_2_draw_left,
              strip_2_draw_top + _SideBarIconHeight2 * unit_icon_num,
              _UnitUIIcons[unit_type],
              0,
              0);
          }
        }
        // New logic end
      }
      ++unit_icon_num;
      ++unit_icon_ptr;
    }
    while ( unit_icon_num < num_unit_icons_to_draw );
  }
}
