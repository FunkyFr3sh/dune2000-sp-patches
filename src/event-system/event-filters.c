#include "dune2000.h"
#include "event-filters.h"

bool EvaluateFilterPosition(ObjectFilterStruct *filter, int pos_x, int pos_y)
{
  if (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK)
  {
    bool result = (pos_x >= filter->pos_min_x) && (pos_y >= filter->pos_min_y) && (pos_x <= filter->pos_max_x) && (pos_y <= filter->pos_max_y);
    return !result != !(filter->pos_flags & OBJFILTERPOSFLAG_NEGATE);
  }
  return true;
}

bool EvaluateFilterExpression(ObjectFilterStruct *filter, bool *p_criteria_result)
{
  char criteria_and_or[8];
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    criteria_and_or[i] = (filter->criteria_and_or >> (i * 2)) & 3;
    criteria_result[i] = p_criteria_result[i];
  }
  int totalshifts = 0;
  for (int operation = 3; operation >= 0; operation--)
  {
    int shifts = 0;
    for (int i = 0; i < 7 - totalshifts; i++)
    {
      if (criteria_and_or[i - shifts] == operation)
      {
        if (operation & 1)
          criteria_result[i - shifts] = criteria_result[i - shifts] || criteria_result[i - shifts + 1];
        else
          criteria_result[i - shifts] = criteria_result[i - shifts] && criteria_result[i - shifts + 1];
        for (int j = i - shifts + 1; j < 7 - totalshifts; j++)
          criteria_result[j] = criteria_result[j+1];
        for (int j = i - shifts; j < 7 - totalshifts; j++)
          criteria_and_or[j] = criteria_and_or[j+1];
        shifts++;
      }
    }
    totalshifts += shifts;
  }
  return criteria_result[0];
}

bool CheckIfUnitMatchesCriteria(Unit *unit, eUnitFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfUnitMatchesFilter(ObjectFilterStruct *filter, Unit *unit)
{
  if (unit->State == 17)
    return false;
  // Check for position
  if (!EvaluateFilterPosition(filter, unit->__posx >> 21, unit->__posy >> 21))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = filter->criteria_value[i];
    criteria_result[i] = CheckIfUnitMatchesCriteria(unit, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfBuildingMatchesCriteria(Building *building, eSideType side_id, eBuildingFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfBuildingMatchesFilter(ObjectFilterStruct *filter, Building *building, eSideType side_id)
{
  if (building->__State == 17)
    return false;
  // Check for position
  int pos_x = building->dw_field_0_x >> 21;
  int pos_y = (building->dw_field_4_y >> 21) - (_templates_buildattribs[building->Type]._____ArtHeight / 32);
  if (!EvaluateFilterPosition(filter, pos_x, pos_y))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = filter->criteria_value[i];
    criteria_result[i] = CheckIfBuildingMatchesCriteria(building, side_id, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfCrateMatchesCriteria(CrateStruct *crate, eCrateFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfCrateMatchesFilter(ObjectFilterStruct *filter, CrateStruct *crate)
{
  if (!crate->__is_active)
    return false;
  // Check for position
  if (!EvaluateFilterPosition(filter, crate->__x, crate->__y))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = filter->criteria_value[i];
    criteria_result[i] = CheckIfCrateMatchesCriteria(crate, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfTileMatchesCriteria(GameMapTileStruct *tile, int pos_x, int pos_y, eTileFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfTileMatchesFilter(ObjectFilterStruct *filter, GameMapTileStruct *tile, int pos_x, int pos_y, bool check_pos)
{
  // Check for position
  if (check_pos && !EvaluateFilterPosition(filter, pos_x, pos_y))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = filter->criteria_value[i];
    criteria_result[i] = CheckIfTileMatchesCriteria(tile, pos_x, pos_y, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool compare_value(int val, int comp_val, bool comparison)
{
  if (comparison)
    return val >= comp_val;
  else
    return val == comp_val;
}

bool CheckIfUnitMatchesCriteria(Unit *unit, eUnitFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  UnitAtribStruct *unit_template = &_templates_unitattribs[unit->Type];
  int behavior = unit_template->__Behavior;
  bool is_special_unit =
         behavior == UnitBehavior_SANDWORM
      || behavior == UnitBehavior_FRIGATE
      || behavior == UnitBehavior_ORNITHOPTER
      || behavior == UnitBehavior_CARRYALL
      || behavior == UnitBehavior_DEATH_HAND;
  int pos_x = unit->__posx >> 21;
  int pos_y = unit->__posy >> 21;
  uint32_t attributes = gGameMap.map[pos_x + _CellNumbersWidthSpan[pos_y]].__tile_bitflags;
  switch(criteria_type)
  {
  case UNITCRITERIATYPE_NONE:         result = true; break;
  case UNITCRITERIATYPE_TYPE:         result = compare_value(unit->Type, value, comparison); break;
  case UNITCRITERIATYPE_GROUP:        result = compare_value(unit_template->__UnitType, value, comparison); break;
  case UNITCRITERIATYPE_BEHAVIOR:     result = compare_value(unit_template->__Behavior, value, comparison); break;
  case UNITCRITERIATYPE_CATEGORY:
    switch(value)
    {
    case UNITCATEGORY_INFANTRY:       result = unit_template->__IsInfantry; break;
    case UNITCATEGORY_VEHICLE:        result = !(unit_template->__IsInfantry) && !is_special_unit; break;
    case UNITCATEGORY_LIGHT_VEH:      result = !(unit_template->__IsInfantry) && !(unit_template->__CanCrush) && !is_special_unit; break;
    case UNITCATEGORY_HEAVY_VEH:      result = !(unit_template->__IsInfantry) && (unit_template->__CanCrush) && !is_special_unit; break;
    case UNITCATEGORY_SPECIAL:        result = is_special_unit; break;
    case UNITCATEGORY_HAS_PRI_WEAPON: result = unit_template->__PrimaryWeapon != -1; break;
    case UNITCATEGORY_HAS_SEC_WEAPON: result = unit_template->__SecondaryWeapon != -1; break;
    case UNITCATEGORY_HAS_BARREL:     result = unit_template->__HasBarrel; break;
    case UNITCATEGORY_CAN_BE_UPGRADED:result = unit_template->UnitUpgradeAllowed; break;
    }
    break;
  case UNITCRITERIATYPE_TAG:          result = compare_value(unit->w_field_E, value, comparison); break;
  case UNITCRITERIATYPE_OWNER_SIDE:   result = unit_template->__OwnerSide & (1 << value); break;
  case UNITCRITERIATYPE_ARMOR:        result = compare_value(unit_template->__Armour, value, comparison); break;
  case UNITCRITERIATYPE_SPEED_TYPE:   result = compare_value(unit_template->__VehicleType, value, comparison); break;
  case UNITCRITERIATYPE_PRI_WEAPON:   result = compare_value(unit_template->__PrimaryWeapon, value, comparison); break;
  case UNITCRITERIATYPE_SEC_WEAPON:   result = compare_value(unit_template->__SecondaryWeapon, value, comparison); break;
  case UNITCRITERIATYPE_SIGHT:        result = compare_value(unit_template->__ViewDistance, value, comparison); break;
  case UNITCRITERIATYPE_RANGE:        result = compare_value((unit_template->__PrimaryWeapon != -1)?_templates_bulletattribs[(int)unit_template->__PrimaryWeapon].__Range:0, value, comparison); break;
  case UNITCRITERIATYPE_SPEED:        result = compare_value(unit->Speed >> 12, value, comparison); break;
  case UNITCRITERIATYPE_RATE_OF_FIRE: result = compare_value(unit->ReloadRate, value, comparison); break;
  case UNITCRITERIATYPE_HP100_MAX:    result = compare_value(unit_template->__Strength / 100, value, comparison); break;
  case UNITCRITERIATYPE_HP100_CUR:    result = compare_value(unit->Health / 100, value, comparison); break;
  case UNITCRITERIATYPE_HEALTH_PERCENT: result = compare_value((unit->Health * 100) / unit_template->__Strength, value, comparison); break;
  case UNITCRITERIATYPE_FLAG:         result = unit->Flags & (1 << value); break;
  case UNITCRITERIATYPE_STATE:        result = compare_value(unit->State, value, comparison); break;
  case UNITCRITERIATYPE_GROUPNO:      result = compare_value(unit->GroupID_28, value, comparison); break; // To be clarified
  case UNITCRITERIATYPE_TILE_ATTRIB:  result = attributes & (1 << value); break;
  case UNITCRITERIATYPE_TILE_TERRAIN: result = compare_value((attributes >> 29) & 7, value, comparison); break;
  case UNITCRITERIATYPE_TILE_SPICE:   result = compare_value((attributes >> 20) & 7, value, comparison); break;
  case UNITCRITERIATYPE_TILE_CONCOWN: result = compare_value((attributes >> 17) & 7, value, comparison); break;
  case UNITCRITERIATYPE_TILE_OWNER1:  result = compare_value((attributes >> 0) & 7, value, comparison); break;
  case UNITCRITERIATYPE_TILE_OWNER2:  result = compare_value((attributes >> 25) & 7, value, comparison); break;
  }
  return result != negation;
}

bool CheckIfBuildingMatchesCriteria(Building *building, eSideType side_id, eBuildingFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  BuildingAtrbStruct *building_template = &_templates_buildattribs[building->Type];
  CSide *side = GetSide(side_id);
  switch(criteria_type)
  {
  case BUILDINGCRITERIATYPE_NONE:         result = true; break;
  case BUILDINGCRITERIATYPE_TYPE:         result = compare_value(building->Type, value, comparison); break;
  case BUILDINGCRITERIATYPE_GROUP:        result = compare_value(building_template->GroupType, value, comparison); break;
  case BUILDINGCRITERIATYPE_BEHAVIOR:     result = compare_value(building_template->__Behavior, value, comparison); break;
  case BUILDINGCRITERIATYPE_CATEGORY:
    switch(value)
    {
    case BUILDINGCATEGORY_POWER_CONS:     result = building_template->__PowerDrain > 0; break;
    case BUILDINGCATEGORY_POWER_PROD:     result = building_template->__PowerDrain < 0; break;
    case BUILDINGCATEGORY_REQ_ENOUGH_POW: result = building_template->_____RequireEnoughPower; break;
    }
    break;
  case BUILDINGCRITERIATYPE_TAG:          result = compare_value(building->field_E, value, comparison); break;
  case BUILDINGCRITERIATYPE_OWNER_SIDE:   result = building_template->_____OwnerSide & (1 << value); break;
  case BUILDINGCRITERIATYPE_ARMOR:        result = compare_value(building_template->Armour, value, comparison); break;
  case BUILDINGCRITERIATYPE_PRI_WEAPON:   result = compare_value(building_template->_____PrimaryWeapon, value, comparison); break;
  case BUILDINGCRITERIATYPE_SEC_WEAPON:   result = compare_value(building_template->_____SecondaryWeapon, value, comparison); break;
  case BUILDINGCRITERIATYPE_SIGHT:        result = compare_value(building_template->_____SightRadius, value, comparison); break;
  case BUILDINGCRITERIATYPE_POWER_CONS:   result = compare_value((building_template->__PowerDrain > 0)?building_template->__PowerDrain:0, value, comparison); break;
  case BUILDINGCRITERIATYPE_POWER_PROD:   result = compare_value((building_template->__PowerDrain < 0)?building_template->__PowerDrain * -1:0, value, comparison); break;
  case BUILDINGCRITERIATYPE_RANGE:        result = compare_value((building_template->_____PrimaryWeapon != -1)?_templates_bulletattribs[(int)building_template->_____PrimaryWeapon].__Range:0, value, comparison); break;
  case BUILDINGCRITERIATYPE_RATE_OF_FIRE: result = compare_value(building->_____RateOfFire_refinery, value, comparison); break;
  case BUILDINGCRITERIATYPE_HP100_MAX:    result = compare_value(building_template->_____HitPoints / 100, value, comparison); break;
  case BUILDINGCRITERIATYPE_HP100_CUR:    result = compare_value(building->Health / 100, value, comparison); break;
  case BUILDINGCRITERIATYPE_HEALTH_PERCENT: result = compare_value((building->Health * 100) / building_template->_____HitPoints, value, comparison); break;
  case BUILDINGCRITERIATYPE_FLAG:         result = building->Flags & (1 << value); break;
  case BUILDINGCRITERIATYPE_STATE:        result = compare_value(building->__State, value, comparison); break;
  case BUILDINGCRITERIATYPE_UPGRADES:     result = compare_value(side->__building_group_upgrade_count[(int)building_template->GroupType], value, comparison); break;
  }
  return result != negation;
}

bool CheckIfCrateMatchesCriteria(CrateStruct *crate, eCrateFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  uint32_t attributes = gGameMap.map[crate->__x + _CellNumbersWidthSpan[crate->__y]].__tile_bitflags;
  switch(criteria_type)
  {
  case CRATECRITERIATYPE_NONE:          result = true; break;
  case CRATECRITERIATYPE_TYPE:          result = compare_value(crate->__type, value, comparison); break;
  case CRATECRITERIATYPE_IMAGE:         result = compare_value(crate->__image, value, comparison); break;
  case CRATECRITERIATYPE_EXT_DATA:      result = compare_value(crate->ext_data_field, value, comparison); break;
  case CRATECRITERIATYPE_EXT_DATA_BIT:  result = crate->ext_data_field & (1 << value); break;
  case CRATECRITERIATYPE_RESPAWN_TIMES: result = compare_value(crate->__times_to_respawn, value, comparison); break;
  case CRATECRITERIATYPE_TIME_TICKS:    result = compare_value(crate->__timing, value, comparison); break;
  case CRATECRITERIATYPE_TIME_SECS:     result = compare_value(crate->__timing / 25, value, comparison); break;
  case CRATECRITERIATYPE_XPOS:          result = compare_value(crate->__x, value, comparison); break;
  case CRATECRITERIATYPE_YPOS:          result = compare_value(crate->__y, value, comparison); break;
  case CRATECRITERIATYPE_TILE_ATTRIB:   result = attributes & (1 << value); break;
  case CRATECRITERIATYPE_TILE_TERRAIN:  result = compare_value((attributes >> 29) & 7, value, comparison); break;
  case CRATECRITERIATYPE_TILE_SPICE:    result = compare_value((attributes >> 20) & 7, value, comparison); break;
  case CRATECRITERIATYPE_TILE_CONCOWN:  result = compare_value((attributes >> 17) & 7, value, comparison); break;
  case CRATECRITERIATYPE_TILE_OWNER1:   result = compare_value((attributes >> 0) & 7, value, comparison); break;
  case CRATECRITERIATYPE_TILE_OWNER2:   result = compare_value((attributes >> 25) & 7, value, comparison); break;
  }
  return result != negation;
}

bool CheckIfTileMatchesCriteria(GameMapTileStruct *tile, int pos_x, int pos_y, eTileFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = false;
  uint32_t attributes = tile->__tile_bitflags;
  switch(criteria_type)
  {
  case TILECRITERIATYPE_NONE:     result = true; break;
  case TILECRITERIATYPE_DAMAGE:   result = compare_value(tile->__damage, value, comparison); break;
  case TILECRITERIATYPE_SHROUD:   result = compare_value(tile->__shroud_flags, value, comparison); break;
  case TILECRITERIATYPE_XPOS:     result = compare_value(pos_x, value, comparison); break;
  case TILECRITERIATYPE_YPOS:     result = compare_value(pos_y, value, comparison); break;
  case TILECRITERIATYPE_ATTRIB:   result = attributes & (1 << value); break;
  case TILECRITERIATYPE_TERRAIN:  result = compare_value((attributes >> 29) & 7, value, comparison); break;
  case TILECRITERIATYPE_SPICE:    result = compare_value((attributes >> 20) & 7, value, comparison); break;
  case TILECRITERIATYPE_CONCOWN:  result = compare_value((attributes >> 17) & 7, value, comparison); break;
  case TILECRITERIATYPE_OWNER1:   result = compare_value((attributes >> 0) & 7, value, comparison); break;
  case TILECRITERIATYPE_OWNER2:   result = compare_value((attributes >> 25) & 7, value, comparison); break;
  case TILECRITERIATYPE_UNIT_TYPE:
  case TILECRITERIATYPE_UNIT_TAG:
  {
    eSideType side_id = 0;
    _WORD index;
    Unit *unit;
    if ((attributes & 0x3E8) && (GetUnitOnTile(32 * pos_x, 32 * pos_y, &side_id, &index, false)))
    {
      index = -1;
      while ((unit = GetNextUnitOnTile(pos_x, pos_y, side_id, &index)))
      {
        switch(criteria_type)
        {
        case TILECRITERIATYPE_UNIT_TYPE: result = compare_value(unit->Type, value, comparison); break;
        case TILECRITERIATYPE_UNIT_TAG: result = compare_value(unit->w_field_E, value, comparison); break;
        default: result = false;
        }
        if (result)
          break;
      }
    }
    break;
  }
  case TILECRITERIATYPE_BUILDING_TYPE:
  case TILECRITERIATYPE_BUILDING_TAG:
  {
    eSideType side_id = 0;
    _WORD index;
    Building *bld;
    if ((attributes & TileFlags_10_OCC_BUILDING) && GetBuildingOnTile_0(pos_x, pos_y, &bld, &side_id, &index))
    {
      switch(criteria_type)
      {
      case TILECRITERIATYPE_BUILDING_TYPE: result = compare_value(bld->Type, value, comparison); break;
      case TILECRITERIATYPE_BUILDING_TAG: result = compare_value(bld->field_E, value, comparison); break;
      default: result = false;
      }
    }
    break;
  }
  }
  return result != negation;
}
