#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"

// Always reset list of available buildings if rule buildingsAlwaysNeedPrerequisites is set
// Implement unit prerequisite 2 owner side needed and upgrades needed
// Implement returnCreditsToSpiceStorage rule

void __thiscall CSide_return_credits(CSide *this, int amount);

// Custom implementation of function CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries
DETOUR(0x0046BE50, 0x0046C44B, _Mod__CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries);

void __thiscall Mod__CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(CSide *this)
{
  CSide *side; // ebp
  signed int building_icon_index; // eax
  int *building_icons_ptr; // ecx MAPDST
  char building_icon_value; // dl
  signed int unit_icon_index; // eax
  int *unit_icon_ptr; // ecx MAPDST
  char unit_icon_value; // dl
  Building *bld; // ebx
  int building_type; // esi
  int bld_left; // ecx
  unsigned int bld_right; // edx
  int tmp_left_boundary; // ecx
  int tmp_right_boundary; // edx
  Unit *unit; // edi
  int unit_left; // ecx
  int unit_top; // eax
  unsigned char bottom_boundary_; // al
  unsigned char right_boundary_; // al
  signed int building_type_; // esi
  int *prereq_1_building_group_ptr; // eax MAPDST
  int prereq_1_building_group; // ecx MAPDST
  char can_build_building; // dl
  int prereq_2_building_group; // ecx MAPDST
  char *building_can_be_built_ptr; // ebx
  eBuildingGroupType *building_group_ptr; // edi
  unsigned char actual_building_type; // al
  signed int unit_type; // esi
  char unit_can_be_built; // cl MAPDST
  unsigned char actual_unit_type; // al
  int i; // esi
  int building_icon_counter; // ecx
  char can_be_built; // dl
  short built_building_type; // ax
  unsigned int building_cost; // eax
  int j; // ebx
  int unit_icon_counter; // ecx
  BuildQueueStruct *unit_build_queue_ptr; // esi
  signed int unit_build_queue_counter; // edi
  short built_unit_type; // ax
  unsigned int unit_cost; // eax
  int v48; // eax
  int v49; // esi
  int v50; // eax
  int harkonnen_sound; // ST08_4
  int ordos_sound; // ST04_4
  int atreides_sound; // eax
  char has_construction_yard; // [esp+13h] [ebp-FDh]
  int left_boundary; // [esp+14h] [ebp-FCh]
  unsigned char building_icon_count; // [esp+18h] [ebp-F8h]
  int unit_icon_count_; // [esp+18h] [ebp-F8h]
  unsigned char unit_icon_count; // [esp+1Ch] [ebp-F4h]
  int top_boundary; // [esp+1Ch] [ebp-F4h] MAPDST
  int right_boundary; // [esp+20h] [ebp-F0h]
  int bottom_boundary; // [esp+24h] [ebp-ECh]
  int bld_bottom; // [esp+28h] [ebp-E8h]
  int building_icon_count_; // [esp+30h] [ebp-E0h]
  char unit_icons_buffer[60]; // [esp+34h] [ebp-DCh]
  char building_groups_owner_sides[100]; // [esp+70h] [ebp-A0h]
  char building_icons_buffer[60]; // [esp+D4h] [ebp-3Ch]

  side = this;
  building_icon_count = this->__BuildingIconCount;
  building_icon_index = 0;
  has_construction_yard = 0;
  unit_icon_count = this->__UnitIconCount;
  building_icon_count_ = building_icon_count;
  if ( (signed int)building_icon_count > 0 )
  {
    building_icons_ptr = this->__BuildingIcons;
    do
    {
      building_icon_value = *(_BYTE *)building_icons_ptr;
      ++building_icons_ptr;
      building_icons_buffer[building_icon_index++] = building_icon_value;
    }
    while ( building_icon_index < building_icon_count );
  }
  unit_icon_index = 0;
  unit_icon_count_ = unit_icon_count;
  if ( (signed int)unit_icon_count > 0 )
  {
    unit_icon_ptr = side->__UnitIcons;
    do
    {
      unit_icon_value = *(_BYTE *)unit_icon_ptr;
      ++unit_icon_ptr;
      unit_icons_buffer[unit_icon_index++] = unit_icon_value;
    }
    while ( unit_icon_index < unit_icon_count );
  }
  CSide__ResetBuildingAndUnitIcons(side);
  bld = side->__FirstBuildingPtr;
  memset(building_groups_owner_sides, 0, sizeof(building_groups_owner_sides));
  left_boundary = gGameMap.width;
  top_boundary = gGameMap.height;
  right_boundary = 0;
  bottom_boundary = 0;
  if ( bld )
  {
    do
    {
      building_type = bld->Type;
      if ( _templates_buildattribs[building_type]._____TechLevelBuild == 9 )
      {
        has_construction_yard = 1;
      }
      building_groups_owner_sides[(unsigned char)_templates_buildattribs[building_type].GroupType] |= _templates_buildattribs[building_type]._____OwnerSide;
      bld_left = bld->__PosX / 0x10000 / 32;
      bld_bottom = bld->__PosY / 0x10000 / 32;
      bld_right = bld_left + ((unsigned int)_templates_buildattribs[building_type]._____ArtWidth >> 5);
      tmp_left_boundary = bld_left - 2;
      tmp_right_boundary = bld_right + 2;
      if ( tmp_left_boundary < left_boundary )
      {
        left_boundary = tmp_left_boundary;
      }
      if ( tmp_right_boundary > right_boundary )
      {
        right_boundary = tmp_right_boundary;
      }
      if ( (signed int)(-2 - ((unsigned int)_templates_buildattribs[building_type]._____ArtHeight >> 5) + bld_bottom) < top_boundary )
      {
        top_boundary = -2 - ((unsigned int)_templates_buildattribs[building_type]._____ArtHeight >> 5) + bld_bottom;
      }
      if ( bld_bottom + 2 > bottom_boundary )
      {
        bottom_boundary = bld_bottom + 2;
      }
      bld = bld->Next;
    }
    while ( bld );
  }
  else
  {
    top_boundary = 0;
    left_boundary = 0;
  }
  // New logic start
  // Always reset list of available buildings if rule buildingsAlwaysNeedPrerequisites is set
  if ( !has_construction_yard || rulesExt__buildingsAlwaysNeedPrerequisites )
  // New logic end
  {
    memset(side->__BuildingTypeCanBeBuilt, 0, sizeof(side->__BuildingTypeCanBeBuilt));
  }
  if ( !left_boundary && !top_boundary )
  {
    unit = side->__FirstUnitPtr;
    top_boundary = gGameMap.height;
    for ( left_boundary = gGameMap.width; unit; unit = unit->Next )
    {
      unit_left = unit->__PosX / 0x10000 / 32;
      unit_top = unit->__PosY / 0x10000 / 32;
      if ( unit_left < left_boundary )
      {
        left_boundary = unit->__PosX / 0x10000 / 32;
      }
      if ( unit_left + 1 > right_boundary )
      {
        right_boundary = unit_left + 1;
      }
      if ( unit_top < top_boundary )
      {
        top_boundary = unit->__PosY / 0x10000 / 32;
      }
      if ( unit_top + 1 > bottom_boundary )
      {
        bottom_boundary = unit_top + 1;
      }
    }
  }
  side->__BasePosMinY = top_boundary <= 0 ? 0 : top_boundary;
  bottom_boundary_ = LOBYTE(gGameMap.height) - 1;
  if ( bottom_boundary < gGameMap.height - 1 )
  {
    bottom_boundary_ = bottom_boundary;
  }
  side->__BasePosMaxY = bottom_boundary_;
  side->__BasePosMinX = left_boundary <= 0 ? 0 : left_boundary;
  right_boundary_ = LOBYTE(gGameMap.width) - 1;
  if ( right_boundary < gGameMap.width - 1 )
  {
    right_boundary_ = right_boundary;
  }
  side->__BasePosMaxX = right_boundary_;
  if ( has_construction_yard )
  {
    building_type_ = 0;
    if ( gBuildingTypeNum )
    {
      prereq_1_building_group_ptr = &_templates_buildattribs[0]._____Prereq1BuildingType;
      do
      {
        prereq_1_building_group = *prereq_1_building_group_ptr;
        can_build_building = 0;
        if ( *prereq_1_building_group_ptr != -1 )
        {
          if ( (unsigned char)building_groups_owner_sides[prereq_1_building_group] & (_BYTE)prereq_1_building_group_ptr[1] )
          {
            can_build_building = 1;
          }
        }
        if ( *((_BYTE *)prereq_1_building_group_ptr - 48) > (unsigned char)_gMiscData.Tech[(unsigned char)side->__SideId] )
        {
          can_build_building = 0;
        }
        if ( side->__BuildingGroupUpgradeCount[prereq_1_building_group] < *((_BYTE *)prereq_1_building_group_ptr + 5) )
        {
          can_build_building = 0;
        }
        prereq_2_building_group = prereq_1_building_group_ptr[2];
        if ( prereq_2_building_group != -1 )
        {
          if ( !((unsigned char)building_groups_owner_sides[prereq_2_building_group] & (_BYTE)prereq_1_building_group_ptr[3]) )
          {
            can_build_building = 0;
          }
          if ( side->__BuildingGroupUpgradeCount[prereq_2_building_group] < *((_BYTE *)prereq_1_building_group_ptr + 13) )
          {
            can_build_building = 0;
          }
        }
        if ( can_build_building )
        {
          side->__BuildingTypeCanBeBuilt[building_type_] = 1;
        }
        ++building_type_;
        prereq_1_building_group_ptr += 67;
      }
      while ( building_type_ < gBuildingTypeNum );
    }
  }
  building_can_be_built_ptr = side->__BuildingTypeCanBeBuilt;
  building_group_ptr = (eBuildingGroupType *)&_templates_buildattribs[0].GroupType;
  do
  {
    if ( *building_can_be_built_ptr )
    {
      actual_building_type = CSide__MyVersionOfBuilding(side, *building_group_ptr, 1);
      CSide__AddBuildingIcon(side, actual_building_type);
    }
    building_group_ptr += 268;
    ++building_can_be_built_ptr;
  }
  while ( (signed int)building_group_ptr < 7220856 );
  unit_type = 0;
  if ( gUnitTypeNum )
  {
    prereq_1_building_group_ptr = &_templates_unitattribs[0].__PreReq1;
    do
    {
      prereq_1_building_group = *prereq_1_building_group_ptr;
      unit_can_be_built = 0;
      if ( *prereq_1_building_group_ptr != -1 )
      {
        if ( (unsigned char)building_groups_owner_sides[prereq_1_building_group] & (_BYTE)prereq_1_building_group_ptr[1] )
        {
          unit_can_be_built = 1;
        }
        if ( side->__BuildingGroupUpgradeCount[prereq_1_building_group] < *((_BYTE *)prereq_1_building_group_ptr - 1) )
        {
          unit_can_be_built = 0;
        }
      }
      prereq_2_building_group = prereq_1_building_group_ptr[2];
      // New logic start
      // Implement unit prerequisite 2 owner side needed and upgrades needed
      if ( prereq_2_building_group != -1 )
      {
        if ( !((!_templates_unitattribs[unit_type].Prereq2OwnerHouse && building_groups_owner_sides[prereq_2_building_group]) || (building_groups_owner_sides[prereq_2_building_group] & _templates_unitattribs[unit_type].Prereq2OwnerHouse)) )
        {
          unit_can_be_built = 0;
        }
        if ( side->__BuildingGroupUpgradeCount[prereq_2_building_group] < _templates_unitattribs[unit_type].Prereq2UpgradesNeeded )
        {
          unit_can_be_built = 0;
        }
      }
      // New logic end
      if ( *((_BYTE *)prereq_1_building_group_ptr - 4) > (unsigned char)_gMiscData.Tech[(unsigned char)side->__SideId] )
      {
        unit_can_be_built = 0;
      }
      if ( *((_BYTE *)prereq_1_building_group_ptr + 130) )
      {
        if ( !_IsMultiplayer && gGameType != 1 )
        {
          unit_can_be_built = 0;
        }
        if ( *((_BYTE *)prereq_1_building_group_ptr + 5) == UnitBehavior_THUMPER && !gNetWorms )
        {
          unit_can_be_built = 0;
        }
      }
      if ( unit_can_be_built )
      {
        actual_unit_type = CSide__MyVersionOfUnit(side, *((_BYTE *)prereq_1_building_group_ptr - 39), 1);
        CSide__AddUnitIcon(side, actual_unit_type);
      }
      ++unit_type;
      prereq_1_building_group_ptr += 64;
    }
    while ( unit_type < gUnitTypeNum );
  }
  for ( i = 0; i < building_icon_count_; ++i )
  {
    building_icon_counter = side->__BuildingIconCount;
    can_be_built = 0;
    if ( building_icon_counter > 0 )
    {
      building_icons_ptr = side->__BuildingIcons;
      do
      {
        if ( *building_icons_ptr == (unsigned char)building_icons_buffer[i] )
        {
          can_be_built = 1;
        }
        ++building_icons_ptr;
        --building_icon_counter;
      }
      while ( building_icon_counter );
    }
    if ( !can_be_built )
    {
      built_building_type = side->__BuildingBuildQueue.__type;
      if ( built_building_type >= 0 && (unsigned char)building_icons_buffer[i] == built_building_type )
      {
        building_cost = GetBuildingCost(built_building_type, 0, (eSideType)side->__SideId);
        // New logic start
        // Implement returnCreditsToSpiceStorage rule
        CSide_return_credits(side, building_cost * (unsigned short)side->__BuildingBuildQueue.__build_progress / 0x5A00);
        // New logic end
        side->__BuildingBuildQueue.__type = -1;
      }
    }
  }
  for ( j = 0; j < unit_icon_count_; ++j )
  {
    unit_icon_counter = side->__UnitIconCount;
    unit_can_be_built = 0;
    if ( unit_icon_counter > 0 )
    {
      unit_icon_ptr = side->__UnitIcons;
      do
      {
        if ( *unit_icon_ptr == (unsigned char)unit_icons_buffer[j] )
        {
          unit_can_be_built = 1;
        }
        ++unit_icon_ptr;
        --unit_icon_counter;
      }
      while ( unit_icon_counter );
    }
    if ( !unit_can_be_built )
    {
      unit_build_queue_ptr = side->__UnitBuildQueue;
      unit_build_queue_counter = 10;
      do
      {
        built_unit_type = unit_build_queue_ptr->__type;
        if ( unit_build_queue_ptr->__type >= 0 && (unsigned char)unit_icons_buffer[j] == built_unit_type )
        {
          unit_cost = GetUnitCost(built_unit_type, (eSideType)side->__SideId);
          // New logic start
          // Implement returnCreditsToSpiceStorage rule
          CSide_return_credits(side, unit_cost * (unsigned short)unit_build_queue_ptr->__build_progress / 0x5A00);
          // New logic end
          unit_build_queue_ptr->__type = -1;
        }
        ++unit_build_queue_ptr;
        --unit_build_queue_counter;
      }
      while ( unit_build_queue_counter );
    }
  }
  if ( side->__SideId == gSideId )
  {
    v48 = 0;
    if ( unit_icon_count_ <= 0 )
    {
LABEL_110:
      v49 = side->__UnitIconCount;
      v50 = 0;
      if ( v49 > 0 )
      {
        unit_icon_ptr = side->__UnitIcons;
        while ( *unit_icon_ptr != (unsigned char)_templates_GroupIDs.DeathHand )
        {
          ++v50;
          ++unit_icon_ptr;
          if ( v50 >= v49 )
          {
            return;
          }
        }
        harkonnen_sound = GetSoundTableID("H_DEATHPREPPING");
        ordos_sound = GetSoundTableID("O_DEATHPREPPING");
        atreides_sound = GetSoundTableID("S_DEATHPREPPING");
        PlayMentatSound(atreides_sound, ordos_sound, harkonnen_sound, 1, 0, 0);
      }
    }
    else
    {
      while ( unit_icons_buffer[v48] != _templates_GroupIDs.DeathHand )
      {
        if ( ++v48 >= unit_icon_count_ )
        {
          goto LABEL_110;
        }
      }
    }
  }
}
