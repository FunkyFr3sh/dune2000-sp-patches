#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "tooltips.h"
#include "../event-system/event-core.h"

TooltipExtraData gTooltipExtraData[TOOLTIP_LINES];
int tooltip_used_lines;

// Custom implementation of function HandleTooltips
CALL(0x00449462, _Mod__HandleTooltips);

void Mod__HandleTooltips()
{
  CSide *side_; // esi MAPDST
  char *tooltip_string; // edi
  unsigned int clip_right; // ebx
  LONG clip_bottom; // ebp
  int tile_x; // eax
  int tile_y; // esi
  Unit *unit; // eax
  int route_mode; // ST18_4
  int state; // ST14_4
  int index_; // ST10_4
  int side_id_; // ST0C_4
  char diplomacy; // cl MAPDST
  int building_icon_index; // eax MAPDST
  int building_cost; // eax
  unsigned int unit_icon_index; // eax MAPDST
  int unit_cost; // eax
  unsigned int upgrade_icon_index; // eax MAPDST
  int building_type; // eax MAPDST
  int building_group; // eax
  int unit_group;
  bool v26; // zf
  int starport_icon_index; // eax MAPDST
  int unit_type; // eax MAPDST
  int string_id; // eax MAPDST
  char *string_text; // eax MAPDST
  int power_percent; // ST18_4
  unsigned __int8 font_height; // al MAPDST
  int sidebar_icons_row; // [esp+10h] [ebp-E4h]
  int show_at_x; // [esp+14h] [ebp-E0h]
  int show_at_y; // [esp+18h] [ebp-DCh]
  int index; // [esp+1Ch] [ebp-D8h]
  char side_id; // [esp+20h] [ebp-D4h]
  unsigned int sidebar_icons_column; // [esp+24h] [ebp-D0h] MAPDST
  CHAR tmp_string; // [esp+2Ch] [ebp-C8h]
  TooltipType tooltip_type;
  int tooltip_arg1;
  int tooltip_arg2;

  side_ = GetSide(gSideId);
  tooltip_string = 0;
  clip_right = _ScreenClipWidth;
  clip_bottom = _ScreenClipHeight;
  _Tooltipdata.__IsShown = 0;
  _Tooltipdata.__Color = _tooltipcolor_yellow;
  show_at_x = _gMousePos.x + 14;
  show_at_y = _gMousePos.y + 14;
  tooltip_type = TOOLTIPTYPE_NONE;
  tooltip_arg1 = 0;
  tooltip_arg2 = 0;
  memset(gTooltipExtraData, 0, sizeof(gTooltipExtraData));
  if ( _gMousePos.x > 0 && _gMousePos.x < _ViewportWidth )
  {
    if ( _gMousePos.y > _OptionsBarHeight && _gMousePos.y < _ScreenClipHeight )
    {
      if ( _Tooltipdata.__LastPosition.x == _ViewportXPos + _gMousePos.x
        && _Tooltipdata.__LastPosition.y == _gMousePos.y + _ViewportYPos )
      {
        tile_x = (_ViewportXPos + _gMousePos.x) / 32;
        tile_y = (unsigned int)(_gMousePos.y + _ViewportYPos - _OptionsBarHeight) >> 5;
        if ( ++_Tooltipdata.__Timer >= 18 )
        {
          if ( tile_x >= gGameMap.width || tile_y >= gGameMap.height )
          {
            goto LABEL_87;
          }
          if ( (gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__shroud & 0xF) == 1 && _bool_shroud_4DFB04 )
          {
            clip_right = _ViewportWidth;
            tooltip_string = GetTextString(_UnrevealedText, 1);
            strcpy(gTooltipExtraData[0].text, tooltip_string);
            tooltip_type = TOOLTIPTYPE_SHROUD;
            tooltip_arg1 = tile_x;
            tooltip_arg2 = tile_y;
          }
          else
          {
            unit = GetUnitOnTile(
                     _gMousePos.x + _ViewportXPos,
                     _gMousePos.y + _ViewportYPos - _OptionsBarHeight,
                     (eSideType *)&side_id,
                     (__int16 *)&index,
                     0);
            if ( unit )
            {
              if ( _gFullscreen_DebugModes_pathfinddebug )
              {
                diplomacy = _gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id];
                if ( diplomacy )
                {
                  if ( !(unit->Flags & UFLAGS_4_CLOAKED) )
                  {
                    clip_right = _ViewportWidth;
                    if ( diplomacy == 2 )
                    {
                      tooltip_string = GetTextString(_NeutralUnitText, 1);
                      tooltip_type = TOOLTIPTYPE_NEUTRAL_UNIT;
                    }
                    else
                    {
                      tooltip_string = GetTextString(_EnemyUnitText, 1);
                      tooltip_type = TOOLTIPTYPE_ENEMY_UNIT;
                    }
                  }
                }
                else
                {
                  clip_right = _ViewportWidth;
                  unit_group = _templates_unitattribs[unit->Type].__UnitType;
                  string_id = _UnitGroupTextIds[unit_group];
                  tooltip_string = (string_id != -1)?GetTextString(string_id, 0):_templates_UnitGroupNameList[unit_group];
                  tooltip_type = TOOLTIPTYPE_UNIT;
                }
              }
              else
              {
                route_mode = (unsigned __int8)unit->RouteMode;
                state = unit->State;
                index_ = (unsigned __int16)index;
                clip_right = _ViewportWidth;
                side_id_ = (unsigned __int8)side_id;

                unit_group = _templates_unitattribs[unit->Type].__UnitType;
                string_id = _UnitGroupTextIds[unit_group];
                string_text = (string_id != -1)?GetTextString(string_id, 0):_templates_UnitGroupNameList[unit_group];
                sprintf(
                  &tmp_string,
                  "%s, Side %d, Index %d, State %d RMode %d",
                  string_text,
                  side_id_,
                  index_,
                  state,
                  route_mode);
                tooltip_string = &tmp_string;
                tooltip_type = TOOLTIPTYPE_UNIT;
              }
              if (tooltip_string)
              {
                strcpy(gTooltipExtraData[0].text, tooltip_string);
                tooltip_arg1 = side_id;
                tooltip_arg2 = index;
              }
            }
            else if ( GetBuildingOnTile_1(tile_x, tile_y, (eSideType *)&side_id, (int)&index) )
            {
              if ( !_IsMultiplayer || _gFullscreen_DebugModes_pathfinddebug )
              {
                clip_right = _ViewportWidth;
                diplomacy = _gDiplomacy[(unsigned __int8)gSideId][(unsigned __int8)side_id];
                if ( diplomacy )
                {
                  if ( diplomacy == 2 )
                  {
                    tooltip_string = GetTextString(_NeutralStructureText, 1);
                    tooltip_type = TOOLTIPTYPE_NEUTRAL_BUILDING;
                  }
                  else
                  {
                    tooltip_string = GetTextString(_EnemyStructureText, 1);
                    tooltip_type = TOOLTIPTYPE_ENEMY_BUILDING;
                  }
                }
                else
                {
                  side_ = GetSide((eSideType)side_id);
                  building_group = _templates_buildattribs[LOBYTE(side_->__ObjectArray[(unsigned __int16)index].__PosX)].GroupType;
                  string_id = _BuildingGroupTextIds[building_group];
                  tooltip_string = (string_id != -1)?GetTextString(string_id, 0):_templates_BuildingGroupNameList[building_group];
                  tooltip_type = TOOLTIPTYPE_BUILDING;
                }
              }
              else
              {
                clip_right = _ViewportWidth;
                sprintf(&tmp_string, "Unit Index %d: side %d", (unsigned __int16)index, (unsigned __int8)side_id);
                tooltip_string = &tmp_string;
                tooltip_type = TOOLTIPTYPE_BUILDING;
              }
              strcpy(gTooltipExtraData[0].text, tooltip_string);
              tooltip_arg1 = side_id;
              tooltip_arg2 = index;
            }
            else
            {
              _Tooltipdata.__Color = _tooltipcolor_gray;
              if ( gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__tile_index >= 800u )
              {
                DebugFatal("MAIN.CPP", "TT: INVALID TILE");
              }
              tooltip_string = GetTextString(
                                 _TileTooltips[(unsigned __int16)gGameMap.map[_CellNumbersWidthSpan[tile_y] + tile_x].__tile_index],
                                 0);
              tooltip_type = TOOLTIPTYPE_EMPTY_TILE;
              tooltip_arg1 = tile_x;
              tooltip_arg2 = tile_y;
              if ( tooltip_string )
              {
                strcpy(gTooltipExtraData[0].text, tooltip_string);
                tooltip_type = TOOLTIPTYPE_TILE_WITH_TOOLTIP;
              }
              clip_right = _ViewportWidth;
            }
          }
        }
      }
      else
      {
        _Tooltipdata.__Timer = 0;
      }
      goto LABEL_87;
    }
  }
  if ( _gMousePos.x <= _SidebarStrip1XPos
    || _gMousePos.x >= _SidebarIconWidth + _SidebarStrip2XPos
    || _gMousePos.y <= _SidebarStrip1YPos
    || _gMousePos.y >= _SideBarIconCount * _SideBarIconHeight2 + _SidebarStrip1YPos )
  {
    if ( _Tooltipdata.__LastPosition.x == _gMousePos.x + _ViewportXPos
      && _Tooltipdata.__LastPosition.y == _gMousePos.y + _ViewportYPos )
    {
      if ( ++_Tooltipdata.__Timer >= 18 )
      {
        int radar_image_width = 128;
        int radar_image_height = 128;
        if (_gMousePos.x >= _RadarLocationX && _gMousePos.x < _RadarLocationX + radar_image_width && _gMousePos.y >= _RadarLocationY && _gMousePos.y < _RadarLocationY + radar_image_height)
        {
          int radar_image_left = (radar_image_width - gGameMap.width) / 2;
          int radar_image_top = (radar_image_height - gGameMap.height) / 2;
          if ( _gMousePos.x >= radar_image_left + _RadarLocationX
            && _gMousePos.x < radar_image_width + _RadarLocationX - radar_image_left
            && _gMousePos.y >= radar_image_top + _RadarLocationY
            && _gMousePos.y < radar_image_height + _RadarLocationY - radar_image_top
            && !_TacticalData.__RadarState
            && _TacticalData.__RadarOnline)
          {
            tooltip_type = TOOLTIPTYPE_RADAR_MAP;
            tooltip_arg1 = _gMousePos.x - radar_image_left - _RadarLocationX;
            tooltip_arg2 = _gMousePos.y - radar_image_top - _RadarLocationY;
          }
          else if (!_TacticalData.__RadarState && !_TacticalData.__RadarOnline)
          {
            tooltip_type = TOOLTIPTYPE_RADAR_HOUSE_IMAGE;
          }
          goto LABEL_87;
        }
        else if (_gMousePos.x >= _CreditsTextXPos && _gMousePos.x < _CreditsTextXPos + 80 && _gMousePos.y >= _CreditsTextYPos && _gMousePos.y < _CreditsTextYPos + 14)
        {
          tooltip_type = TOOLTIPTYPE_CREDITS;
          goto LABEL_87;
        }
        else if ( HandleSidebarButton(SIDEBARBUTTON_REPAIR, 0) )
        {
          string_id = GetTextID("REPAIR");
          tooltip_string = GetTextString(string_id, 0);
          clip_right = _SidebarButtons[SIDEBARBUTTON_REPAIR].__xpos;
          show_at_y = 160;
          tooltip_type = TOOLTIPTYPE_SIDEBAR_BUTTON;
          tooltip_arg1 = SIDEBARBUTTON_REPAIR;
        }
        else if ( HandleSidebarButton(SIDEBARBUTTON_SELL, 0) )
        {
          string_id = GetTextID("SELLBUILDING");
          tooltip_string = GetTextString(string_id, 0);
          clip_right = _SidebarButtons[SIDEBARBUTTON_SELL].__xpos;
          show_at_y = 160;
          tooltip_type = TOOLTIPTYPE_SIDEBAR_BUTTON;
          tooltip_arg1 = SIDEBARBUTTON_SELL;
        }
        else if ( gGameType && HandleSidebarButton(SIDEBARBUTTON_MAPTOGGLE, 0) )
        {
          string_id = GetTextID("MapToggle");
          tooltip_string = GetTextString(string_id, 0);
          clip_right = _SidebarButtons[SIDEBARBUTTON_MAPTOGGLE].__xpos;
          show_at_y = 160;
          tooltip_type = TOOLTIPTYPE_SIDEBAR_BUTTON;
          tooltip_arg1 = SIDEBARBUTTON_MAPTOGGLE;
        }
        else if ( HandleSidebarButton(SIDEBARBUTTON_GUARD, 0) )
        {
          string_id = GetTextID("GUARD");
          tooltip_string = GetTextString(string_id, 0);
          clip_right = _SidebarButtons[SIDEBARBUTTON_GUARD].__xpos;
          show_at_y = 160;
          tooltip_type = TOOLTIPTYPE_SIDEBAR_BUTTON;
          tooltip_arg1 = SIDEBARBUTTON_GUARD;
        }
        else if ( HandleSidebarButton(SIDEBARBUTTON_PURCHASE, 0) )
        {
          clip_right = _gMousePos.x;
          tooltip_string = "";
          tooltip_type = TOOLTIPTYPE_SIDEBAR_BUTTON;
          tooltip_arg1 = SIDEBARBUTTON_PURCHASE;
        }
        else
        {
          if ( !HandleSidebarButton(SIDEBARBUTTON_RETREAT, 0) )
          {
            if ( _gMousePos.x >= _PowerBarUIPosX
              && _gMousePos.y >= _PowerBarUIPosY
              && _gMousePos.x <= _SideBarPowerImages[0]->width + _PowerBarUIPosX
              && _gMousePos.y <= _ScreenClipHeight )
            {
              power_percent = GetSide(gSideId)->__PowerPercent1;
              string_id = GetTextID("POWER");
              string_text = GetTextString(string_id, 0);
              sprintf(&tmp_string, "%s %d%%", string_text, power_percent);
              clip_right = _PowerBarUIPosX;
              tooltip_string = &tmp_string;
              show_at_x = _gMousePos.x;
              show_at_y = _gMousePos.y;
              strcpy(gTooltipExtraData[0].text, tooltip_string);
              tooltip_type = TOOLTIPTYPE_POWER;
            }
            goto LABEL_87;
          }
          string_id = GetTextID("RETREAT");
          tooltip_string = GetTextString(string_id, 0);
          clip_right = _SidebarButtons[SIDEBARBUTTON_RETREAT].__xpos;
          show_at_y = 160;
          tooltip_type = TOOLTIPTYPE_SIDEBAR_BUTTON;
          tooltip_arg1 = SIDEBARBUTTON_RETREAT;
        }
        strcpy(gTooltipExtraData[0].text, tooltip_string);
        show_at_x = clip_right;
        goto LABEL_87;
      }
    }
    else
    {
      _Tooltipdata.__Timer = 0;
    }
  }
  else
  {
    sidebar_icons_column = (_gMousePos.x - _SidebarStrip1XPos) / (unsigned int)_SidebarIconWidth;
    show_at_x = sidebar_icons_column * _SidebarIconWidth + _SidebarStrip1XPos + ((unsigned int)_SidebarIconWidth >> 1);
    sidebar_icons_row = (_gMousePos.y - _SidebarStrip1YPos) / (unsigned int)_SideBarIconHeight2;
    show_at_y = sidebar_icons_row * _SideBarIconHeight1 + _SidebarStrip1YPos + ((unsigned int)_SideBarIconHeight1 >> 1);
    if ( _TacticalData.__SidebarMode == 2 )
    {
      if ( sidebar_icons_column > 1 )
      {
        sidebar_icons_column = 1;
      }
      if ( sidebar_icons_row > 3 )
      {
        goto LABEL_87;
      }
      starport_icon_index = sidebar_icons_column + 2 * sidebar_icons_row;
      unit_type = side_->__StarportIcons[starport_icon_index];
      if ( unit_type == -1 )
      {
        goto LABEL_87;
      }
      unit_group = _templates_unitattribs[unit_type].__UnitType;
      string_id = _UnitGroupTextIds[unit_group];
      tooltip_string = (string_id != -1)?GetTextString(string_id, 0):_templates_UnitGroupNameList[unit_group];
      v26 = (starport_icon_index & 1) == 0;
      unit_cost = side_->__StarportUnitTypeCost[side_->__StarportIcons[starport_icon_index]];
      strcpy(gTooltipExtraData[0].text, tooltip_string);
      sprintf(gTooltipExtraData[1].text, "$%d", unit_cost);
      tooltip_type = TOOLTIPTYPE_STARPORT_UNIT;
      tooltip_arg1 = unit_type;
LABEL_64:
      clip_right = _SidebarStrip2XPos;
      if ( v26 )
      {
        clip_right = _SidebarStrip1XPos;
      }
      goto LABEL_87;
    }
    if ( _TacticalData.__SidebarMode != 3 )
    {
      if ( _gMousePos.x >= _SidebarIconWidth + _SidebarStrip1XPos )
      {
        if ( _gMousePos.x > _SidebarStrip2XPos
          && _gMousePos.y > _SidebarStrip2YPos
          && _gMousePos.y < _SidebarStrip2YPos + _SideBarIconCount * _SideBarIconHeight2 )
        {
          unit_icon_index = _TacticalData.__Strip2ScrollPos1
                          + (_gMousePos.y - _SidebarStrip2YPos) / (unsigned int)_SideBarIconHeight2;
          unit_type = side_->__UnitIcons[unit_icon_index];
          if ( unit_type != -1 )
          {
            unit_group = _templates_unitattribs[unit_type].__UnitType;
            string_id = _UnitGroupTextIds[unit_group];
            tooltip_string = (string_id != -1)?GetTextString(string_id, 0):_templates_UnitGroupNameList[unit_group];
            unit_cost = w__GetUnitCost(side_->__UnitIcons[unit_icon_index], gSideId);
            clip_right = _SidebarStrip2XPos;
            strcpy(gTooltipExtraData[0].text, tooltip_string);
            if (unit_cost)
              sprintf(gTooltipExtraData[1].text, "$%d", unit_cost);
            tooltip_type = TOOLTIPTYPE_BUILD_UNIT;
            tooltip_arg1 = unit_type;
          }
        }
      }
      else
      {
        building_icon_index = _TacticalData.__Strip1ScrollPos1 + sidebar_icons_row;
        building_type = side_->__BuildingIcons[building_icon_index];
        if ( building_type != -1 )
        {
          building_group = _templates_buildattribs[building_type].GroupType;
          string_id = _BuildingGroupTextIds[building_group];
          tooltip_string = (string_id != -1)?GetTextString(string_id, 0):_templates_BuildingGroupNameList[building_group];
          building_cost = GetBuildingCost(side_->__BuildingIcons[building_icon_index], 0, gSideId);
          clip_right = _SidebarStrip1XPos;
          strcpy(gTooltipExtraData[0].text, tooltip_string);
          if (building_cost)
            sprintf(gTooltipExtraData[1].text, "$%d", building_cost);
          tooltip_type = TOOLTIPTYPE_BUILD_BUILDING;
          tooltip_arg1 = building_type;
        }
      }
      goto LABEL_87;
    }
    if ( sidebar_icons_column > 1 )
    {
      sidebar_icons_column = 1;
    }
    upgrade_icon_index = sidebar_icons_column + 2 * sidebar_icons_row;
    if ( upgrade_icon_index > 7 )
    {
      upgrade_icon_index = 7;
    }
    building_type = side_->__UpgradeIcons[upgrade_icon_index];
    if ( building_type != -1 )
    {
      building_group = (unsigned __int8)_templates_buildattribs[building_type].GroupType;
      if ( side_->__BuildingsExistPerGroup[building_group] )
      {
        string_id = _BuildingGroupTextIds[building_group];
        tooltip_string = (string_id != -1)?GetTextString(string_id, 0):_templates_BuildingGroupNameList[building_group];
        if ( CanSideUpgradeBuildingGroup(
               gSideId,
               (eBuildingGroupType)_templates_buildattribs[side_->__UpgradeIcons[upgrade_icon_index]].GroupType) )
        {
          // Fix building upgrade cost per difficulty
          building_cost = GetBuildingCost(side_->__UpgradeIcons[upgrade_icon_index], side_->__BuildingGroupUpgradeCount[(int)_templates_buildattribs[side_->__UpgradeIcons[upgrade_icon_index]].GroupType] + 1, gSideId);
          sprintf(gTooltipExtraData[1].text, "$%d", building_cost);
        }
        strcpy(gTooltipExtraData[0].text, tooltip_string);
        tooltip_type = TOOLTIPTYPE_UPGRADE_BUILDING;
        tooltip_arg1 = building_type;
        v26 = (upgrade_icon_index & 1) == 0;
        goto LABEL_64;
      }
    }
  }
LABEL_87:
  if ( tooltip_type )
  {
    for (int i = 0; i < TOOLTIP_LINES; i++)
      gTooltipExtraData[i].color = _Tooltipdata.__Color;
    ExecuteEventHook(HOOK_HANDLETOOLTIP, 3, tooltip_type, tooltip_arg1, tooltip_arg2);
  }
  if (gTooltipExtraData[0].text[0])
  {
    _Tooltipdata.__IsShown = 1;
    _Tooltipdata.__ScreenPosition.x = show_at_x;
    _Tooltipdata.__ScreenPosition.y = show_at_y;
    _Tooltipdata.__TotalRect.left = show_at_x;
    _Tooltipdata.__TotalRect.top = show_at_y;
    font_height = GetFontHeight(0);
    int max_width = 0;
    int total_height = 0;
    for (int i = 0; i < TOOLTIP_LINES; i++)
    {
      if (gTooltipExtraData[i].text[0])
      {
        int width = GetStringPixelWidth(gTooltipExtraData[i].text, 0);
        gTooltipExtraData[i].width = width;
        if (width > max_width)
          max_width = width;
        total_height = (i + 1) * (font_height + 1) + ((i == 0)?2:0);
        tooltip_used_lines = i + 1;
      }
    }
    _Tooltipdata.__TotalRect.right = max_width + show_at_x + 5;
    _Tooltipdata.__TotalRect.bottom = total_height + show_at_y;
    int total_bottom = _Tooltipdata.__TotalRect.bottom;
    if ( total_bottom > clip_bottom )
    {
      _Tooltipdata.__TotalRect.bottom = clip_bottom;
      _Tooltipdata.__ScreenPosition.y += clip_bottom - total_bottom;
      _Tooltipdata.__TotalRect.top += clip_bottom - total_bottom;
    }
    int total_right = _Tooltipdata.__TotalRect.right;
    if ( total_right > (int)clip_right )
    {
      _Tooltipdata.__TotalRect.right = clip_right;
      _Tooltipdata.__ScreenPosition.x += clip_right - total_right;
      _Tooltipdata.__TotalRect.left += clip_right - total_right;
    }
    _Tooltipdata.__BackupImage = _TooltipBackupImage;
    _TooltipBackupImage->width = _Tooltipdata.__TotalRect.right - _Tooltipdata.__TotalRect.left;
    _TooltipBackupImage->height = _Tooltipdata.__TotalRect.bottom - _Tooltipdata.__TotalRect.top;
  }
  else
  {
    _Tooltipdata.__IsShown = 0;
  }
  _Tooltipdata.__LastPosition.x = _gMousePos.x + _ViewportXPos;
  _Tooltipdata.__LastPosition.y = _gMousePos.y + _ViewportYPos;
}

// Custom implementation of function HandleTooltips
CALL(0x00449476, _Mod__BlitTooltip);

void Mod__BlitTooltip(TImage *img)
{
  int line_length; // eax

  if ( _Tooltipdata.__IsShown )
  {
    BlitClipTImage1(_Tooltipdata.__BackupImage, 0, 0, img, &_Tooltipdata.__TotalRect, 0, 0);
    int font_height = GetFontHeight(0);
    for (int i = 0; i < tooltip_used_lines; i++)
    {
      RECT r;
      r.left = _Tooltipdata.__TotalRect.left;
      r.right = r.left + gTooltipExtraData[i].width + 5;
      r.top = _Tooltipdata.__TotalRect.top + i * (font_height + 1);
      r.bottom = r.top + font_height + (((tooltip_used_lines == 1) || (i < (tooltip_used_lines - 1)))?2:0);
      BlitBeveledRectRGB(img, &r, 0, gTooltipExtraData[0].color, gTooltipExtraData[0].color);
      if (i)
      {
        if (gTooltipExtraData[i].width < gTooltipExtraData[i-1].width)
          line_length = gTooltipExtraData[i].width;
        else
          line_length = gTooltipExtraData[i-1].width;
        BlitHorizontalLineRGB(img, r.left + 1, r.top, line_length + 3, 0);
      }
      BlitFontChars(img, gTooltipExtraData[i].text, r.left + 3, r.top, 0, gTooltipExtraData[i].color, -1);
    }
  }
}
