#include "dune2000.h"
#include "event-filters.h"
#include "event-utils.h"
#include "event-core.h"
#include "utils.h"

bool EvaluateFilterPosition(ObjectFilterStruct *filter, int pos_pixels_x, int pos_pixels_y)
{
  if (filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK)
  {
    int pos_x = pos_pixels_x >> 5;
    int pos_y = pos_pixels_y >> 5;
    int pos_type = (filter->pos_flags >> 2) & 3;
    int pos_min_x = GetVariableValueOrConst(filter->pos_flags, 4, filter->pos_min_x);
    int pos_min_y = GetVariableValueOrConst(filter->pos_flags, 5, filter->pos_min_y);
    int pos_max_x = GetVariableValueOrConst(filter->pos_flags, 6, filter->pos_max_x);
    int pos_max_y = GetVariableValueOrConst(filter->pos_flags, 7, filter->pos_max_y);
    int center_x = pos_min_x;
    int center_y = pos_min_y;
    int center_pixels_x = center_x * 32 + 16;
    int center_pixels_y = center_y * 32 + 16;
    int radius = pos_max_x;
    int radius_pixels = radius;
    bool result = false;
    switch (pos_type)
    {
      case 0: result = (pos_x >= pos_min_x) && (pos_y >= pos_min_y) && (pos_x <= pos_max_x) && (pos_y <= pos_max_y); break;
      case 1: result = (abs(pos_x - center_x) <= radius) && (abs(pos_y - center_y) <= radius); break;
      case 2: result = (pos_x - center_x) * (pos_x - center_x) + (pos_y - center_y) * (pos_y - center_y) <= radius * radius; break;
      case 3: result = (pos_pixels_x - center_pixels_x) * (pos_pixels_x - center_pixels_x) + (pos_pixels_y - center_pixels_y) * (pos_pixels_y - center_pixels_y) <= radius_pixels * radius_pixels; break;
    }
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

bool CheckIfUnitMatchesCriteria(Unit *unit, eSideType side_id, eUnitFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfUnitMatchesFilter(ObjectFilterStruct *filter, Unit *unit, eSideType side_id)
{
  // Check for position
  if (!EvaluateFilterPosition(filter, unit->__PosX >> 16, unit->__PosY >> 16))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfUnitMatchesCriteria(unit, side_id, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfBuildingMatchesCriteria(Building *building, eSideType side_id, eBuildingFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfBuildingMatchesFilter(ObjectFilterStruct *filter, Building *building, eSideType side_id)
{
  // Check for position
  int pos_x = (building->__PosX >> 16) + 16;
  int pos_y = (building->__PosY >> 16) - _templates_buildattribs[building->Type]._____ArtHeight + 16;
  if (!EvaluateFilterPosition(filter, pos_x, pos_y))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfBuildingMatchesCriteria(building, side_id, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfBulletMatchesCriteria(Bullet *bullet, eBulletFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfBulletMatchesFilter(ObjectFilterStruct *filter, Bullet *bullet)
{
  // Check for position
  if (!EvaluateFilterPosition(filter, bullet->__PosX >> 16, bullet->__PosY >> 16))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfBulletMatchesCriteria(bullet, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfExplosionMatchesCriteria(Explosion *explosion, eExplosionFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfExplosionMatchesFilter(ObjectFilterStruct *filter, Explosion *explosion)
{
  // Check for position
  if (!EvaluateFilterPosition(filter, explosion->__PosX >> 16, explosion->__PosY >> 16))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfExplosionMatchesCriteria(explosion, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfCrateMatchesCriteria(CrateStruct *crate, eCrateFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfCrateMatchesFilter(ObjectFilterStruct *filter, CrateStruct *crate)
{
  if (!crate->__is_active)
    return false;
  // Check for position
  if (!EvaluateFilterPosition(filter, crate->__x * 32 + 16, crate->__y * 32 + 16))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfCrateMatchesCriteria(crate, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfTileMatchesCriteria(GameMapTileStruct *tile, int pos_x, int pos_y, eTileFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfTileMatchesFilter(ObjectFilterStruct *filter, GameMapTileStruct *tile, int pos_x, int pos_y)
{
  // Check for position
  if (!EvaluateFilterPosition(filter, pos_x * 32 + 16, pos_y * 32 + 16))
    return false;
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfTileMatchesCriteria(tile, pos_x, pos_y, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfSideMatchesCriteria(eSideType side_id, eSideFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfSideMatchesFilter(ObjectFilterStruct *filter, int side_id)
{
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfSideMatchesCriteria(side_id, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfUnitTypeMatchesCriteria(int unit_type, eUnitTypeFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfUnitTypeMatchesFilter(ObjectFilterStruct *filter, int unit_type)
{
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfUnitTypeMatchesCriteria(unit_type, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool CheckIfBuildingTypeMatchesCriteria(int building_type, eBuildingTypeFilterCriteriaType criteria_type, bool negation, bool comparison, int value);

bool CheckIfBuildingTypeMatchesFilter(ObjectFilterStruct *filter, int building_type)
{
  // Check for criteria
  bool criteria_result[8];
  for (int i = 0; i < 8; i++)
  {
    int value = GetVariableValueOrConst(filter->criteria_var_flags, i, filter->criteria_value[i]);
    criteria_result[i] = CheckIfBuildingTypeMatchesCriteria(building_type, filter->criteria_type[i] & 63, filter->criteria_type[i] & 64, filter->criteria_type[i] & 128, value);
  }
  return EvaluateFilterExpression(filter, criteria_result);
}

bool TileCheck(int pos_x, int pos_y, eTileCheckType check_type, bool comparison, int value);
bool ObjectCheck(int side_id, int obj_index, eObjectCheckType check_type, int pos_x, int pos_y, bool comparison, int value);

bool CheckIfUnitMatchesCriteria(Unit *unit, eSideType side_id, eUnitFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
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
  int pos_x = unit->__PosX >> 21;
  int pos_y = unit->__PosY >> 21;
  int next_x = unit->BlockToX;
  int next_y = unit->BlockToY;
  int targ_x = unit->TargetX;
  int targ_y = unit->TargetY;
  bool tile_valid = (pos_x >= 0 && pos_y >= 0 && pos_x < gGameMapWidth && pos_y < gGameMapHeight);
  bool next_valid = (next_x >= 0 && next_y >= 0 && next_x < gGameMapWidth && next_y < gGameMapHeight);
  bool targ_valid = (targ_x >= 0 && targ_y >= 0 && targ_x < gGameMapWidth && targ_y < gGameMapHeight);
  switch(criteria_type)
  {
  case UNITCRITERIATYPE_NONE:           result = true; break;
  case UNITCRITERIATYPE_TYPE:           result = CompareValue(unit->Type, value, comparison); break;
  case UNITCRITERIATYPE_GROUP:          result = CompareValue(unit_template->__UnitType, value, comparison); break;
  case UNITCRITERIATYPE_BEHAVIOR:       result = CompareValue(unit_template->__Behavior, value, comparison); break;
  case UNITCRITERIATYPE_CATEGORY:
    switch(value)
    {
    case UNITCATEGORY_INFANTRY:           result = unit_template->__IsInfantry; break;
    case UNITCATEGORY_VEHICLE:            result = !(unit_template->__IsInfantry) && !is_special_unit; break;
    case UNITCATEGORY_LIGHT_VEH:          result = !(unit_template->__IsInfantry) && !(unit_template->__CanCrush) && !is_special_unit; break;
    case UNITCATEGORY_HEAVY_VEH:          result = !(unit_template->__IsInfantry) && (unit_template->__CanCrush) && !is_special_unit; break;
    case UNITCATEGORY_SPECIAL:            result = is_special_unit; break;
    case UNITCATEGORY_HAS_PRI_WEAPON:     result = unit_template->__PrimaryWeapon != -1; break;
    case UNITCATEGORY_HAS_SEC_WEAPON:     result = unit_template->__SecondaryWeapon != -1; break;
    case UNITCATEGORY_HAS_BARREL:         result = unit_template->__HasBarrel; break;
    case UNITCATEGORY_CAN_BE_UPGRADED:    result = unit_template->UnitUpgradeAllowed; break;
    case UNITCATEGORY_MY_SIDE:            result = side_id == gSideId; break;
    case UNITCATEGORY_ALLY_SIDE:          result = (_gDiplomacy[side_id][gSideId] == 0) && (side_id != gSideId); break;
    case UNITCATEGORY_ENEMY_SIDE:         result = (_gDiplomacy[side_id][gSideId] == 1) && (side_id != gSideId); break;
    case UNITCATEGORY_NEUTRAL_SIDE:       result = (_gDiplomacy[side_id][gSideId] == 2) && (side_id != gSideId); break;
    case UNITCATEGORY_IS_SELECTED:        result = unit->__IsSelected; break;
    case UNITCATEGORY_UNDER_CURSOR:       result = MousePositionX < _ViewportWidth && MousePositionY >= _OptionsBarHeight && UnitOccupiesTile(unit, (_ViewportXPos + MousePositionX) / 32, (_ViewportYPos + MousePositionY - _OptionsBarHeight) / 32); break;
    case UNITCATEGORY_CENTER_OF_TILE:     result = unit->pos_steps == 0; break;
    case UNITCATEGORY_REVEALED_TILE_FULL: result = tile_valid && ((gGameMap.map[pos_x + _CellNumbersWidthSpan[pos_y]].__shroud & 15) == 0); break;
    case UNITCATEGORY_REVEALED_TILE_PART: result = tile_valid && ((gGameMap.map[pos_x + _CellNumbersWidthSpan[pos_y]].__shroud & 15) != 1); break;
    case UNITCATEGORY_LYING:              result = unit->__Lying; break;
    case UNITCATEGORY_ENEMY_VALID:        result = unit->EnemyIndex != -1; break;
    case UNITCATEGORY_ENEMY_UNIT:         result = (unit->EnemyIndex != -1) && !is_special_unit && GetSide(unit->EnemySide)->__ObjectArray[unit->EnemyIndex].ObjectType == 1; break;
    case UNITCATEGORY_ENEMY_BUILDING:     result = (unit->EnemyIndex != -1) && !is_special_unit && GetSide(unit->EnemySide)->__ObjectArray[unit->EnemyIndex].ObjectType == 2; break;
    case UNITCATEGORY_ENEMY_MY_SIDE:      result = (unit->EnemyIndex != -1) && (unit->EnemySide == gSideId); break;
    case UNITCATEGORY_ENEMY_ALLY_SIDE:    result = (unit->EnemyIndex != -1) && (_gDiplomacy[side_id][(unsigned)unit->EnemySide] == 0) && (side_id != (unsigned)unit->EnemySide); break;
    case UNITCATEGORY_ENEMY_ENEMY_SIDE:   result = (unit->EnemyIndex != -1) && (_gDiplomacy[side_id][(unsigned)unit->EnemySide] == 1) && (side_id != (unsigned)unit->EnemySide); break;
    case UNITCATEGORY_ENEMY_NEUTRAL_SIDE: result = (unit->EnemyIndex != -1) && (_gDiplomacy[side_id][(unsigned)unit->EnemySide] == 2) && (side_id != (unsigned)unit->EnemySide); break;
    case UNITCATEGORY_ATTACKER_VALID:       result = unit->__AttackerIndex != -1; break;
    case UNITCATEGORY_ATTACKER_UNIT:        result = (unit->__AttackerIndex != -1) && GetSide(unit->__AttackerSide)->__ObjectArray[unit->__AttackerIndex].ObjectType == 1; break;
    case UNITCATEGORY_ATTACKER_BUILDING:    result = (unit->__AttackerIndex != -1) && GetSide(unit->__AttackerSide)->__ObjectArray[unit->__AttackerIndex].ObjectType == 2; break;
    case UNITCATEGORY_ATTACKER_MY_SIDE:     result = (unit->__AttackerIndex != -1) && (unit->__AttackerSide == gSideId); break;
    case UNITCATEGORY_ATTACKER_ALLY_SIDE:   result = (unit->__AttackerIndex != -1) && (_gDiplomacy[side_id][(unsigned)unit->__AttackerSide] == 0) && (side_id != (unsigned)unit->__AttackerSide); break;
    case UNITCATEGORY_ATTACKER_ENEMY_SIDE:  result = (unit->__AttackerIndex != -1) && (_gDiplomacy[side_id][(unsigned)unit->__AttackerSide] == 1) && (side_id != (unsigned)unit->__AttackerSide); break;
    case UNITCATEGORY_ATTACKER_NEUTRAL_SIDE:result = (unit->__AttackerIndex != -1) && (_gDiplomacy[side_id][(unsigned)unit->__AttackerSide] == 2) && (side_id != (unsigned)unit->__AttackerSide); break;
    case UNITCATEGORY_OLD_SIDE_MY_SIDE:   result = (unit->Flags & UFLAGS_1000_DEVIATED) && unit->OldSide == gSideId; break;
    case UNITCATEGORY_OLD_SIDE_ALLY:      result = (unit->Flags & UFLAGS_1000_DEVIATED) && (_gDiplomacy[side_id][(unsigned)unit->OldSide] == 0) && (side_id != (unsigned)unit->OldSide); break;
    case UNITCATEGORY_OLD_SIDE_ENEMY:     result = (unit->Flags & UFLAGS_1000_DEVIATED) && (_gDiplomacy[side_id][(unsigned)unit->OldSide] == 1) && (side_id != (unsigned)unit->OldSide); break;
    case UNITCATEGORY_OLD_SIDE_NEUTRAL:   result = (unit->Flags & UFLAGS_1000_DEVIATED) && (_gDiplomacy[side_id][(unsigned)unit->OldSide] == 2) && (side_id != (unsigned)unit->OldSide); break;
    case UNITCATEGORY_ROUTE_MODE_0:       result = unit->RouteMode == 0; break;
    case UNITCATEGORY_ROUTE_MODE_1:       result = unit->RouteMode == 1; break;
    case UNITCATEGORY_ROUTE_MODE_2:       result = unit->RouteMode == 2; break;
    case UNITCATEGORY_ROUTE_MODE_3:       result = unit->RouteMode == 3; break;
    }
    break;
  case UNITCRITERIATYPE_TAG:            result = CompareValue(unit->Tag, value, comparison); break;
  case UNITCRITERIATYPE_TYPE_OWNER:     result = unit_template->__OwnerSide & (1 << value); break;
  case UNITCRITERIATYPE_ARMOR:          result = CompareValue(unit_template->__Armour, value, comparison); break;
  case UNITCRITERIATYPE_SPEED_TYPE:     result = CompareValue(unit_template->__VehicleType, value, comparison); break;
  case UNITCRITERIATYPE_PRI_WEAPON:     result = CompareValue(unit_template->__PrimaryWeapon, value, comparison); break;
  case UNITCRITERIATYPE_SEC_WEAPON:     result = CompareValue(unit_template->__SecondaryWeapon, value, comparison); break;
  case UNITCRITERIATYPE_SIGHT:          result = CompareValue(unit_template->__ViewDistance, value, comparison); break;
  case UNITCRITERIATYPE_RANGE:          result = CompareValue((unit_template->__PrimaryWeapon != -1)?_templates_bulletattribs[(int)unit_template->__PrimaryWeapon].__Range:0, value, comparison); break;
  case UNITCRITERIATYPE_SPEED:          result = CompareValue(unit_template->__Speed >> 12, value, comparison); break;
  case UNITCRITERIATYPE_RATE_OF_FIRE:   result = CompareValue(unit_template->__RateOfFire, value, comparison); break;
  case UNITCRITERIATYPE_HP100_MAX:      result = CompareValue(unit_template->__Strength / 100, value, comparison); break;
  case UNITCRITERIATYPE_HP100_CUR:      result = CompareValue(unit->Health / 100, value, comparison); break;
  case UNITCRITERIATYPE_HEALTH_PERCENT: result = CompareValue((unit->Health * 100) / unit_template->__Strength, value, comparison); break;
  case UNITCRITERIATYPE_FLAG:           result = unit->Flags & (1 << value); break;
  case UNITCRITERIATYPE_STATE:          result = CompareValue(unit->State, value, comparison); break;
  case UNITCRITERIATYPE_GROUPNO:        result = CompareValue(unit->__GroupID, value, comparison); break; // To be clarified
  case UNITCRITERIATYPE_TILE_ATTRIB:    if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_ATTRIB,  comparison, value); break;
  case UNITCRITERIATYPE_TILE_TERRAIN:   if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_TERRAIN, comparison, value); break;
  case UNITCRITERIATYPE_TILE_BLD_TYPE:  if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_BUILDING_TYPE,  comparison, value); break;
  case UNITCRITERIATYPE_TILE_BLD_GROUP: if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_BUILDING_GROUP, comparison, value); break;
  case UNITCRITERIATYPE_TILE_BLD_TAG:   if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_BUILDING_TAG,   comparison, value); break;
  case UNITCRITERIATYPE_TILE_BLD_FLAG:  if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_BUILDING_FLAG,  comparison, value); break;
  case UNITCRITERIATYPE_TILE_BLD_TILENO:if (!tile_valid) return false; result = TileCheck(pos_x, pos_y, TILECHECK_BUILDING_TILENO,comparison, value); break;
  case UNITCRITERIATYPE_NEXT_XPOS:      result = CompareValue(next_x, value, comparison); break;
  case UNITCRITERIATYPE_NEXT_YPOS:      result = CompareValue(next_y, value, comparison); break;
  case UNITCRITERIATYPE_NEXT_ATTRIB:    if (!next_valid) return false; result = TileCheck(next_x, next_y, TILECHECK_ATTRIB, comparison, value); break;
  case UNITCRITERIATYPE_NEXT_TERRAIN:   if (!next_valid) return false; result = TileCheck(next_x, next_y, TILECHECK_TERRAIN, comparison, value); break;
  case UNITCRITERIATYPE_TARG_XPOS:      if (pos_x == targ_x && pos_y == targ_y) return false; result = CompareValue(targ_x, value, comparison); break;
  case UNITCRITERIATYPE_TARG_YPOS:      if (pos_x == targ_x && pos_y == targ_y) return false; result = CompareValue(targ_y, value, comparison); break;
  case UNITCRITERIATYPE_TARG_ATTRIB:    if (pos_x == targ_x && pos_y == targ_y) return false; result = targ_valid && TileCheck(targ_x, targ_y, TILECHECK_ATTRIB, comparison, value); break;
  case UNITCRITERIATYPE_TARG_TERRAIN:   if (pos_x == targ_x && pos_y == targ_y) return false; result = targ_valid && TileCheck(targ_x, targ_y, TILECHECK_TERRAIN, comparison, value); break;
  case UNITCRITERIATYPE_TARG_DIST:      if (pos_x == targ_x && pos_y == targ_y) return false; result = CompareDistance(unit->__PosX >> 16, unit->__PosY >> 16, targ_x * 32 + 16, targ_y * 32 + 16, value, comparison); break;
  case UNITCRITERIATYPE_ENEMY_SIDE:     if (unit->EnemyIndex == -1 || is_special_unit) return false; result = CompareValue(unit->EnemySide, value, comparison); break;
  case UNITCRITERIATYPE_ENEMY_TYPE:     if (unit->EnemyIndex == -1 || is_special_unit) return false; result = ObjectCheck(unit->EnemySide, unit->EnemyIndex, OBJECTCHECK_TYPE, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ENEMY_GROUP:    if (unit->EnemyIndex == -1 || is_special_unit) return false; result = ObjectCheck(unit->EnemySide, unit->EnemyIndex, OBJECTCHECK_GROUP, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ENEMY_CATEGORY: if (unit->EnemyIndex == -1 || is_special_unit) return false; result = ObjectCheck(unit->EnemySide, unit->EnemyIndex, OBJECTCHECK_CATEGORY, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ENEMY_TAG:      if (unit->EnemyIndex == -1 || is_special_unit) return false; result = ObjectCheck(unit->EnemySide, unit->EnemyIndex, OBJECTCHECK_TAG, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ENEMY_ARMOR:    if (unit->EnemyIndex == -1 || is_special_unit) return false; result = ObjectCheck(unit->EnemySide, unit->EnemyIndex, OBJECTCHECK_ARMOR, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ENEMY_DIST:     if (unit->EnemyIndex == -1 || is_special_unit) return false; result = ObjectCheck(unit->EnemySide, unit->EnemyIndex, OBJECTCHECK_DIST, unit->__PosX >> 16, unit->__PosY >> 16, comparison, value); break;
  case UNITCRITERIATYPE_ENEMY_INDEX:    result = unit->EnemyIndex == value; break;
  case UNITCRITERIATYPE_ATTACKER_SIDE:  if (unit->__AttackerIndex == -1) return false; result = CompareValue(unit->__AttackerSide, value, comparison); break;
  case UNITCRITERIATYPE_ATTACKER_TYPE:  if (unit->__AttackerIndex == -1) return false; result = ObjectCheck(unit->__AttackerSide, unit->__AttackerIndex, OBJECTCHECK_TYPE, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ATTACKER_GROUP: if (unit->__AttackerIndex == -1) return false; result = ObjectCheck(unit->__AttackerSide, unit->__AttackerIndex, OBJECTCHECK_GROUP, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ATTACKER_CATEGORY: if (unit->__AttackerIndex == -1) return false; result = ObjectCheck(unit->__AttackerSide, unit->__AttackerIndex, OBJECTCHECK_CATEGORY, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ATTACKER_TAG:   if (unit->__AttackerIndex == -1) return false; result = ObjectCheck(unit->__AttackerSide, unit->__AttackerIndex, OBJECTCHECK_TAG, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ATTACKER_WEAPON:   if (unit->__AttackerIndex == -1) return false; result = ObjectCheck(unit->__AttackerSide, unit->__AttackerIndex, OBJECTCHECK_WEAPON, 0, 0, comparison, value); break;
  case UNITCRITERIATYPE_ATTACKER_DIST:  if (unit->__AttackerIndex == -1) return false; result = ObjectCheck(unit->__AttackerSide, unit->__AttackerIndex, OBJECTCHECK_DIST, unit->__PosX >> 16, unit->__PosY >> 16, comparison, value); break;
  case UNITCRITERIATYPE_ATTACKER_INDEX: result = unit->__AttackerIndex == value; break;
  case UNITCRITERIATYPE_FACING:         result = CompareValue(unit->__Facing, value, comparison); break;
  case UNITCRITERIATYPE_FACING_TURRET:  result = CompareValue(unit->__FacingTurret, value, comparison); break;
  case UNITCRITERIATYPE_ANIM_FRAME:     result = CompareValue(unit->__CurrentAnimFrame, value, comparison); break;
  case UNITCRITERIATYPE_SPEC_PURPOSE:   result = CompareValue(unit->__SpecialPurpose, value, comparison); break;
  case UNITCRITERIATYPE_LAST_ATTACKED:  result = CompareValue(unit->LastAttacked, value, comparison); break;
  case UNITCRITERIATYPE_LAST_FIRED:     result = CompareValue(unit->LastFired, value, comparison); break;
  case UNITCRITERIATYPE_LAST_MOVED:     result = CompareValue(unit->LastMoved, value, comparison); break;
  case UNITCRITERIATYPE_OLD_SIDE:       result = CompareValue(unit->OldSide, value, comparison); break;
  case UNITCRITERIATYPE_DEVIATED_TIME:  result = CompareValue(unit->__DeviatedTimeCounter, value, comparison); break;
  case UNITCRITERIATYPE_DEAD_STATE_TIME:result = CompareValue(unit->__DeadStateTimeCounter, value, comparison); break;
  case UNITCRITERIATYPE_RELOAD_DELAY:   result = CompareValue(unit->__ReloadDelayCounter, value, comparison); break;
  }
  return result != negation;
}

bool CheckIfBuildingMatchesCriteria(Building *building, eSideType side_id, eBuildingFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  BuildingAtrbStruct *building_template = &_templates_buildattribs[building->Type];
  CSide *side = GetSide(side_id);
  int pos_x = building->__PosX >> 21;
  int pos_y = ((building->__PosY >> 16) - building_template->_____ArtHeight) >> 5;
  unsigned int buildTileCount, concreteTileCount;
  switch(criteria_type)
  {
  case BUILDINGCRITERIATYPE_NONE:           result = true; break;
  case BUILDINGCRITERIATYPE_TYPE:           result = CompareValue(building->Type, value, comparison); break;
  case BUILDINGCRITERIATYPE_GROUP:          result = CompareValue(building_template->GroupType, value, comparison); break;
  case BUILDINGCRITERIATYPE_BEHAVIOR:       result = CompareValue(building_template->__Behavior, value, comparison); break;
  case BUILDINGCRITERIATYPE_CATEGORY:
    switch(value)
    {
    case BUILDINGCATEGORY_POWER_CONS:       result = building_template->__PowerDrain > 0; break;
    case BUILDINGCATEGORY_POWER_PROD:       result = building_template->__PowerDrain < 0; break;
    case BUILDINGCATEGORY_REQ_ENOUGH_POW:   result = building_template->_____RequireEnoughPower; break;
    case BUILDINGCATEGORY_SHAKE_SCREEN:     result = building_template->_____ScreenShake > 0; break;
    case BUILDINGCATEGORY_HAS_PRI_WEAPON:   result = building_template->_____PrimaryWeapon != -1; break;
    case BUILDINGCATEGORY_HAS_SEC_WEAPON:   result = building_template->_____SecondaryWeapon != -1; break;
    case BUILDINGCATEGORY_HAS_BARREL:       result = building_template->_____BarrelArt != -1; break;
    case BUILDINGCATEGORY_CAN_BE_UPGRADED:  result = CanSideUpgradeBuildingGroup(side_id, building_template->GroupType); break;
    case BUILDINGCATEGORY_MY_SIDE:          result = side_id == gSideId; break;
    case BUILDINGCATEGORY_ALLY_SIDE:        result = (_gDiplomacy[side_id][gSideId] == 0) && (side_id != gSideId); break;
    case BUILDINGCATEGORY_ENEMY_SIDE:       result = (_gDiplomacy[side_id][gSideId] == 1) && (side_id != gSideId); break;
    case BUILDINGCATEGORY_NEUTRAL_SIDE:     result = (_gDiplomacy[side_id][gSideId] == 2) && (side_id != gSideId); break;
    case BUILDINGCATEGORY_IS_SELECTED:      result = building->__IsSelected; break;
    case BUILDINGCATEGORY_UNDER_CURSOR:     result = MousePositionX < _ViewportWidth && MousePositionY >= _OptionsBarHeight && BuildingOccupiesTile(building, (_ViewportXPos + MousePositionX) / 32, (_ViewportYPos + MousePositionY - _OptionsBarHeight) / 32); break;
    case BUILDINGCATEGORY_ENEMY_VALID:          result = building->EnemyIndex != -1; break;
    case BUILDINGCATEGORY_ENEMY_UNIT:           result = (building->EnemyIndex != -1) && GetSide(building->EnemySide)->__ObjectArray[building->EnemyIndex].ObjectType == 1; break;
    case BUILDINGCATEGORY_ENEMY_BUILDING:       result = (building->EnemyIndex != -1) && GetSide(building->EnemySide)->__ObjectArray[building->EnemyIndex].ObjectType == 2; break;
    case BUILDINGCATEGORY_ENEMY_MY_SIDE:        result = (building->EnemyIndex != -1) && (building->EnemySide == gSideId); break;
    case BUILDINGCATEGORY_ENEMY_ALLY_SIDE:      result = (building->EnemyIndex != -1) && (_gDiplomacy[side_id][(unsigned)building->EnemySide] == 0) && (side_id != (unsigned)building->EnemySide); break;
    case BUILDINGCATEGORY_ENEMY_ENEMY_SIDE:     result = (building->EnemyIndex != -1) && (_gDiplomacy[side_id][(unsigned)building->EnemySide] == 1) && (side_id != (unsigned)building->EnemySide); break;
    case BUILDINGCATEGORY_ENEMY_NEUTRAL_SIDE:   result = (building->EnemyIndex != -1) && (_gDiplomacy[side_id][(unsigned)building->EnemySide] == 2) && (side_id != (unsigned)building->EnemySide); break;
    case BUILDINGCATEGORY_ATTACKER_VALID:       result = building->__AttackerIndex != -1; break;
    case BUILDINGCATEGORY_ATTACKER_UNIT:        result = (building->__AttackerIndex != -1) && GetSide(building->__AttackerSide)->__ObjectArray[building->__AttackerIndex].ObjectType == 1; break;
    case BUILDINGCATEGORY_ATTACKER_BUILDING:    result = (building->__AttackerIndex != -1) && GetSide(building->__AttackerSide)->__ObjectArray[building->__AttackerIndex].ObjectType == 2; break;
    case BUILDINGCATEGORY_ATTACKER_MY_SIDE:     result = (building->__AttackerIndex != -1) && (building->__AttackerSide == gSideId); break;
    case BUILDINGCATEGORY_ATTACKER_ALLY_SIDE:   result = (building->__AttackerIndex != -1) && (_gDiplomacy[side_id][(unsigned)building->__AttackerSide] == 0) && (side_id != (unsigned)building->__AttackerSide); break;
    case BUILDINGCATEGORY_ATTACKER_ENEMY_SIDE:  result = (building->__AttackerIndex != -1) && (_gDiplomacy[side_id][(unsigned)building->__AttackerSide] == 1) && (side_id != (unsigned)building->__AttackerSide); break;
    case BUILDINGCATEGORY_ATTACKER_NEUTRAL_SIDE:result = (building->__AttackerIndex != -1) && (_gDiplomacy[side_id][(unsigned)building->__AttackerSide] == 2) && (side_id != (unsigned)building->__AttackerSide); break;
    }
    break;
  case BUILDINGCRITERIATYPE_TAG:            result = CompareValue(building->Tag, value, comparison); break;
  case BUILDINGCRITERIATYPE_TYPE_OWNER:     result = building_template->_____OwnerSide & (1 << value); break;
  case BUILDINGCRITERIATYPE_ARMOR:          result = CompareValue(building_template->Armour, value, comparison); break;
  case BUILDINGCRITERIATYPE_PRI_WEAPON:     result = CompareValue(building_template->_____PrimaryWeapon, value, comparison); break;
  case BUILDINGCRITERIATYPE_SEC_WEAPON:     result = CompareValue(building_template->_____SecondaryWeapon, value, comparison); break;
  case BUILDINGCRITERIATYPE_SIGHT:          result = CompareValue(building_template->_____SightRadius, value, comparison); break;
  case BUILDINGCRITERIATYPE_POWER_CONS:     result = CompareValue((building_template->__PowerDrain > 0)?building_template->__PowerDrain:0, value, comparison); break;
  case BUILDINGCRITERIATYPE_POWER_PROD:     result = CompareValue((building_template->__PowerDrain < 0)?building_template->__PowerDrain * -1:0, value, comparison); break;
  case BUILDINGCRITERIATYPE_RANGE:          result = CompareValue((building_template->_____PrimaryWeapon != -1)?_templates_bulletattribs[(int)building_template->_____PrimaryWeapon].__Range:0, value, comparison); break;
  case BUILDINGCRITERIATYPE_RATE_OF_FIRE:   result = CompareValue(building_template->_____RateOfFire, value, comparison); break;
  case BUILDINGCRITERIATYPE_HP100_MAX:      result = CompareValue(building_template->_____HitPoints / 100, value, comparison); break;
  case BUILDINGCRITERIATYPE_HP100_CUR:      result = CompareValue(building->Health / 100, value, comparison); break;
  case BUILDINGCRITERIATYPE_HEALTH_PERCENT: result = CompareValue((building->Health * 100) / building_template->_____HitPoints, value, comparison); break;
  case BUILDINGCRITERIATYPE_FLAG:           result = building->Flags & (1 << value); break;
  case BUILDINGCRITERIATYPE_STATE:          result = CompareValue(building->__State, value, comparison); break;
  case BUILDINGCRITERIATYPE_UPGRADES:       result = CompareValue(side->__BuildingGroupUpgradeCount[(int)building_template->GroupType], value, comparison); break;
  case BUILDINGCRITERIATYPE_TILES_TOTAL:    GetBuildingOnConcreteCount(side_id, building->Type, pos_x, pos_y, &buildTileCount, &concreteTileCount); result = CompareValue(buildTileCount, value, comparison); break;
  case BUILDINGCRITERIATYPE_TILES_ON_CONC:  GetBuildingOnConcreteCount(side_id, building->Type, pos_x, pos_y, &buildTileCount, &concreteTileCount); result = CompareValue(concreteTileCount, value, comparison); break;
  case BUILDINGCRITERIATYPE_TILES_ON_CONC_PERCENT: GetBuildingOnConcreteCount(side_id, building->Type, pos_x, pos_y, &buildTileCount, &concreteTileCount); result = CompareValue((concreteTileCount * 100) / buildTileCount, value, comparison); break;
  case BUILDINGCRITERIATYPE_RESERVED1:      return false;
  case BUILDINGCRITERIATYPE_RESERVED2:      return false;
  case BUILDINGCRITERIATYPE_RESERVED3:      return false;
  case BUILDINGCRITERIATYPE_RESERVED4:      return false;
  case BUILDINGCRITERIATYPE_ENEMY_SIDE:     if (building->EnemyIndex == -1) return false; result = CompareValue(building->EnemySide, value, comparison); break;
  case BUILDINGCRITERIATYPE_ENEMY_TYPE:     if (building->EnemyIndex == -1) return false; result = ObjectCheck(building->EnemySide, building->EnemyIndex, OBJECTCHECK_TYPE, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ENEMY_GROUP:    if (building->EnemyIndex == -1) return false; result = ObjectCheck(building->EnemySide, building->EnemyIndex, OBJECTCHECK_GROUP, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ENEMY_CATEGORY: if (building->EnemyIndex == -1) return false; result = ObjectCheck(building->EnemySide, building->EnemyIndex, OBJECTCHECK_CATEGORY, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ENEMY_TAG:      if (building->EnemyIndex == -1) return false; result = ObjectCheck(building->EnemySide, building->EnemyIndex, OBJECTCHECK_TAG, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ENEMY_ARMOR:    if (building->EnemyIndex == -1) return false; result = ObjectCheck(building->EnemySide, building->EnemyIndex, OBJECTCHECK_ARMOR, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ENEMY_DIST:     if (building->EnemyIndex == -1) return false; result = ObjectCheck(building->EnemySide, building->EnemyIndex, OBJECTCHECK_DIST, (building->__PosX >> 16) + 16, (building->__PosY >> 16) - building_template->_____ArtHeight + 16, comparison, value); break;
  case BUILDINGCRITERIATYPE_ENEMY_INDEX:    result = building->EnemyIndex == value; break;
  case BUILDINGCRITERIATYPE_ATTACKER_SIDE:  if (building->__AttackerIndex == -1) return false; result = CompareValue(building->__AttackerSide, value, comparison); break;
  case BUILDINGCRITERIATYPE_ATTACKER_TYPE:  if (building->__AttackerIndex == -1) return false; result = ObjectCheck(building->__AttackerSide, building->__AttackerIndex, OBJECTCHECK_TYPE, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ATTACKER_CATEGORY: if (building->__AttackerIndex == -1) return false; result = ObjectCheck(building->__AttackerSide, building->__AttackerIndex, OBJECTCHECK_GROUP, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ATTACKER_GROUP: if (building->__AttackerIndex == -1) return false; result = ObjectCheck(building->__AttackerSide, building->__AttackerIndex, OBJECTCHECK_CATEGORY, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ATTACKER_TAG:   if (building->__AttackerIndex == -1) return false; result = ObjectCheck(building->__AttackerSide, building->__AttackerIndex, OBJECTCHECK_TAG, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ATTACKER_WEAPON: if (building->__AttackerIndex == -1) return false; result = ObjectCheck(building->__AttackerSide, building->__AttackerIndex, OBJECTCHECK_WEAPON, 0, 0, comparison, value); break;
  case BUILDINGCRITERIATYPE_ATTACKER_DIST:  if (building->__AttackerIndex == -1) return false; result = ObjectCheck(building->__AttackerSide, building->__AttackerIndex, OBJECTCHECK_DIST, (building->__PosX >> 16) + 16, (building->__PosY >> 16) - building_template->_____ArtHeight + 16, comparison, value); break;
  case BUILDINGCRITERIATYPE_ATTACKER_INDEX: result = building->__AttackerIndex == value; break;
  case BUILDINGCRITERIATYPE_FACING:         result = CompareValue(building->__Facing, value, comparison); break;
  case BUILDINGCRITERIATYPE_DIR_FRAME:      result = CompareValue(building->__DirectionFrame, value, comparison); break;
  case BUILDINGCRITERIATYPE_BUILDUP_FRAME:  result = CompareValue(building->__BuildupAnimCounter, value, comparison); break;
  case BUILDINGCRITERIATYPE_ANIM_FRAME:     result = CompareValue(building->__BuildingAnimCounter, value, comparison); break;
  case BUILDINGCRITERIATYPE_SPEC_PURPOSE:   result = CompareValue(building->SpecialPurpose, value, comparison); break;
  case BUILDINGCRITERIATYPE_LAST_ATTACKED:  result = CompareValue(building->__LastAttacked, value, comparison); break;
  case BUILDINGCRITERIATYPE_RESERVED5:      return false;
  case BUILDINGCRITERIATYPE_DEAD_TIME:      result = CompareValue(building->__DeadStateTimeCounter, value, comparison); break;
  case BUILDINGCRITERIATYPE_RELOAD_TIME:    result = CompareValue(building->__ReloadDelayCounter_refinery, value, comparison); break;
  case BUILDINGCRITERIATYPE_TURN_TIME:      result = CompareValue(building->__TurretTurnDelayCounter, value, comparison); break;
  }
  return result != negation;
}

bool CheckIfBulletMatchesCriteria(Bullet *bullet, eBulletFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  BullAtrbStruct *bullet_template = &_templates_bulletattribs[bullet->Type];
  switch(criteria_type)
  {
  case BULLETCRITERIATYPE_NONE:             result = true; break;
  case BULLETCRITERIATYPE_TYPE:             result = CompareValue(bullet->Type, value, comparison); break;
  case BULLETCRITERIATYPE_CATEGORY:
    switch(value)
    {
    case BULLETCATEGORY_ANTI_AIRCRAFT:  result = bullet_template->AntiAircraft != 0; break;
    }
    break;
  case BULLETCRITERIATYPE_TAG:              result = CompareValue(bullet->Tag, value, comparison); break;
  case BULLETCRITERIATYPE_DAMAGE:           result = CompareValue(bullet_template->__Damage, value, comparison); break;
  case BULLETCRITERIATYPE_RANGE:            result = CompareValue(bullet_template->__Range, value, comparison); break;
  case BULLETCRITERIATYPE_SPEED:            result = CompareValue(bullet_template->__ProjectileSpeed, value, comparison); break;
  case BULLETCRITERIATYPE_WARHEAD:          result = CompareValue(bullet_template->Warhead, value, comparison); break;
  case BULLETCRITERIATYPE_HIT_EXPLOSION:    result = CompareValue(bullet_template->__HitExplosion, value, comparison); break;
  case BULLETCRITERIATYPE_TRAIL_EXPLOSION:  result = CompareValue(bullet_template->__TrailExplosion, value, comparison); break;
  case BULLETCRITERIATYPE_FLAG:             result = bullet->Flags & (1 << value); break;
  case BULLETCRITERIATYPE_MOVE_STEPS:       result = CompareValue(bullet->__MoveSteps, value, comparison); break;
  case BULLETCRITERIATYPE_SPEED_X:          result = CompareValue(bullet->__SpeedX, value, comparison); break;
  case BULLETCRITERIATYPE_SPEED_Y:          result = CompareValue(bullet->__SpeedY, value, comparison); break;
  case BULLETCRITERIATYPE_FIRER_INDEX:      result = CompareValue(bullet->__FirerIndex, value, comparison); break;
  case BULLETCRITERIATYPE_HOMING_INDEX:     result = CompareValue(bullet->__HomingIndex, value, comparison); break;
  case BULLETCRITERIATYPE_HOMING_SIDE:      result = CompareValue(bullet->__HomingSideId, value, comparison); break;
  case BULLETCRITERIATYPE_TARGET_X:         result = CompareValue(bullet->__TargetX, value, comparison); break;
  case BULLETCRITERIATYPE_TARGET_Y:         result = CompareValue(bullet->__TargetY, value, comparison); break;
  case BULLETCRITERIATYPE_POS_Z:            result = CompareValue(bullet->__PosZHeight, value, comparison); break;
  case BULLETCRITERIATYPE_ANIM_FRAME:       result = CompareValue(bullet->__AnimationFrame, value, comparison); break;
  //default: result = TileCheck(crate->__x, crate->__y, criteria_type - CRATECRITERIATYPE_TILECHECK, comparison, value);
  }
  return result != negation;
}

bool CheckIfExplosionMatchesCriteria(Explosion *explosion, eExplosionFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  switch(criteria_type)
  {
  case EXPLOSIONCRITERIATYPE_NONE:        result = true; break;
  case EXPLOSIONCRITERIATYPE_TYPE:        result = CompareValue(explosion->Type, value, comparison); break;
  case EXPLOSIONCRITERIATYPE_TAG:         result = CompareValue(explosion->Tag, value, comparison); break;
  case EXPLOSIONCRITERIATYPE_FLAG:        result = explosion->Flags & (1 << value); break;
  case EXPLOSIONCRITERIATYPE_ANIM_FRAME:  result = CompareValue(explosion->__AnimationFrame, value, comparison); break;
  case EXPLOSIONCRITERIATYPE_ANIM_DELAY:  result = CompareValue(explosion->__AnimationDelay, value, comparison); break;
  case EXPLOSIONCRITERIATYPE_POS_Z:       result = CompareValue(explosion->__PosZHeight, value, comparison); break;
  //default: result = TileCheck(crate->__x, crate->__y, criteria_type - CRATECRITERIATYPE_TILECHECK, comparison, value);
  }
  return result != negation;
}

bool CheckIfCrateMatchesCriteria(CrateStruct *crate, eCrateFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  switch(criteria_type)
  {
  case CRATECRITERIATYPE_NONE:          result = true; break;
  case CRATECRITERIATYPE_TYPE:          result = CompareValue(crate->__type, value, comparison); break;
  case CRATECRITERIATYPE_IMAGE:         result = CompareValue(crate->__image, value, comparison); break;
  case CRATECRITERIATYPE_CATEGORY:
    switch(value)
    {
    case CRATECATEGORY_UNDER_CURSOR:  result = MousePositionX < _ViewportWidth && MousePositionY >= _OptionsBarHeight && crate->__x == (_ViewportXPos + MousePositionX) / 32 && crate->__y == ((_ViewportYPos + MousePositionY - _OptionsBarHeight) / 32); break;
    case CRATECATEGORY_SPICE_BLOOM:   result = crate->__type >= CT_SPICE_BLOOM_SMALL && crate->__type <= CT_SPICE_BLOOM_LARGE; break;
    case CRATECATEGORY_SHOOTABLE:     result = (crate->__type >= CT_SPICE_BLOOM_SMALL && crate->__type <= CT_SPICE_BLOOM_LARGE && !(crate->ext_data_field & 128)) || (crate->__type == CT_EXPLODE && crate->ext_data_field & 128); break;
    case CRATECATEGORY_IGNORED_BY_AI: result = (crate->__type == CT_EXPLODE && crate->ext_data_field) || (crate->__image == 4); break;
    }
    break;
  case CRATECRITERIATYPE_EXT_DATA:      result = CompareValue(crate->ext_data_field, value, comparison); break;
  case CRATECRITERIATYPE_EXT_DATA_BIT:  result = crate->ext_data_field & (1 << value); break;
  case CRATECRITERIATYPE_RESPAWN_TIMES: result = CompareValue(crate->__times_to_respawn, value, comparison); break;
  case CRATECRITERIATYPE_TIME_TICKS:    result = CompareValue(crate->__timing, value, comparison); break;
  case CRATECRITERIATYPE_TIME_SECS:     result = CompareValue(crate->__timing / 25, value, comparison); break;
  default: result = TileCheck(crate->__x, crate->__y, criteria_type - CRATECRITERIATYPE_TILECHECK, comparison, value);
  }
  return result != negation;
}

bool CheckIfTileMatchesCriteria(GameMapTileStruct *tile, int pos_x, int pos_y, eTileFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = false;
  switch(criteria_type)
  {
    case TILECRITERIATYPE_NONE:       result = true; break;
    case TILECRITERIATYPE_IDX:        result = CompareValue(tile->__tile_index, value, comparison); break;
    case TILECRITERIATYPE_COL:        result = CompareValue(tile->__tile_index % 20, value, comparison); break;
    case TILECRITERIATYPE_ROW:        result = CompareValue(tile->__tile_index / 20, value, comparison); break;
    case TILECRITERIATYPE_CATEGORY:
      switch(value)
      {
      case TILECATEGORY_UNDER_CURSOR:     result = MousePositionX < _ViewportWidth && MousePositionY >= _OptionsBarHeight && pos_x == (_ViewportXPos + MousePositionX) / 32 && pos_y == ((_ViewportYPos + MousePositionY - _OptionsBarHeight) / 32); break;
      case TILECATEGORY_FULLY_HIDDEN:     result = (tile->__shroud & 15) == 1; break;
      case TILECATEGORY_HIDDEN:           result = (tile->__shroud & 15) != 0; break;
      case TILECATEGORY_PART_REVEALED:    result = (tile->__shroud & 15) != 1; break;
      case TILECATEGORY_REVEALED:         result = (tile->__shroud & 15) == 0; break;
      case TILECATEGORY_OVERRIDEN:        result = tile->__tile_index != tile->back_up_tile_index; break;
      case TILECATEGORY_HAS_INFANTRY:     result = (tile->__tile_bitflags & 0x3E0) != 0; break;
      case TILECATEGORY_HAS_ANY_UNIT:     result = (tile->__tile_bitflags & 0x3E8) != 0; break;
      case TILECATEGORY_HAS_STRUCTURE:    result = (tile->__tile_bitflags & 0x3F8) != 0; break;
      }
      break;
    case TILECRITERIATYPE_DAMAGE:     result = CompareValue(tile->__damage, value, comparison); break;
    case TILECRITERIATYPE_SHROUD:     result = CompareValue(tile->__shroud, value, comparison); break;
    default: result = TileCheck(pos_x, pos_y, criteria_type - TILECRITERIATYPE_TILECHECK, comparison, value);
  }
  return result != negation;
}

bool CheckIfSideMatchesCriteria(eSideType side_id, eSideFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = false;
  CSide *side = GetSide(side_id);
  switch(criteria_type)
  {
    case SIDECRITERIATYPE_NONE:             result = true; break;
    case SIDECRITERIATYPE_INDEX:            result = CompareValue(side_id, value, comparison); break;
    case SIDECRITERIATYPE_HOUSE_ID:         result = CompareValue(side->fHouseID, value, comparison); break;
    case SIDECRITERIATYPE_TECH:             result = CompareValue(_gMiscData.Tech[side_id], value, comparison); break;
    case TILECRITERIATYPE_CATEGORY:
      switch(value)
      {
      case SIDECATEGORY_MY_SIDE:            result = side_id == gSideId; break;
      case SIDECATEGORY_ALLY_TO_MY_SIDE:    result = _gDiplomacy[side_id][gSideId] == 0; break;
      case SIDECATEGORY_ENEMY_TO_MY_SIDE:   result = _gDiplomacy[side_id][gSideId] == 1; break;
      case SIDECATEGORY_NEUTRAL_TO_MY_SIDE: result = _gDiplomacy[side_id][gSideId] == 2; break;
      case SIDECATEGORY_AI_ENABLED:         result = _gAIArray[side_id].__IsAI != 0; break;
      case SIDECATEGORY_PARTICIPATES_GAME:  result = side->__ParticipatesInGame != 0; break;
      case SIDECATEGORY_UNITS_EXIST:        result = _gUnitsExist[side_id] != 0; break;
      case SIDECATEGORY_BUILDINGS_EXIST:    result = _gBuildingsExist[side_id] != 0; break;
      }
      break;
    case SIDECRITERIATYPE_ALLY_TO:          result = _gDiplomacy[side_id][value] == 0; break;
    case SIDECRITERIATYPE_ENEMY_TO:         result = _gDiplomacy[side_id][value] == 1; break;
    case SIDECRITERIATYPE_NEUTRAL_TO:       result = _gDiplomacy[side_id][value] == 2; break;
    case SIDECRITERIATYPE_CREDITS:          result = CompareValue(side->CashReal + side->SpiceReal, value, comparison); break;
    case SIDECRITERIATYPE_SPICE:            result = CompareValue(side->SpiceReal, value, comparison); break;
    case SIDECRITERIATYPE_CASH:             result = CompareValue(side->CashReal, value, comparison); break;
    case SIDECRITERIATYPE_STORAGE:          result = CompareValue(side->__MaxStorage, value, comparison); break;
    case SIDECRITERIATYPE_POWER_PERCENT:    result = CompareValue(side->__PowerPercent1, value, comparison); break;
    case SIDECRITERIATYPE_POWER_OUTPUT:     result = CompareValue(side->__PowerOutput, value, comparison); break;
    case SIDECRITERIATYPE_POWER_DRAIN:      result = CompareValue(side->__PowerDrained, value, comparison); break;
    case SIDECRITERIATYPE_1_UPGRADE:        result = side->__BuildingGroupUpgradeCount[value] >= 1; break;
    case SIDECRITERIATYPE_2_UPGRADES:       result = side->__BuildingGroupUpgradeCount[value] >= 2; break;
    case SIDECRITERIATYPE_3_UPGRADES:       result = side->__BuildingGroupUpgradeCount[value] >= 3; break;
    case SIDECRITERIATYPE_HARVESTED:        result = CompareValue(side->__SpiceHarvested, value, comparison); break;
    case SIDECRITERIATYPE_UNITS_BUILT:      result = CompareValue(side->__UnitsBuilt, value, comparison); break;
    case SIDECRITERIATYPE_BUILDINGS_BUILT:  result = CompareValue(side->__BuildingsBuilt, value, comparison); break;
    case SIDECRITERIATYPE_UNITS_LOST:       result = CompareValue(side->__UnitsLost, value, comparison); break;
    case SIDECRITERIATYPE_BUILDINGS_LOST:   result = CompareValue(side->__BuildingsLost, value, comparison); break;
    case SIDECRITERIATYPE_UNITS_KILLED:     result = CompareValue(side->__UnitsKilled, value, comparison); break;
    case SIDECRITERIATYPE_BUILDINGS_KILLED: result = CompareValue(side->__BuildingsKilled, value, comparison); break;
  }
  return result != negation;
}

bool CheckIfUnitTypeMatchesCriteria(int unit_type, eUnitTypeFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = false;
  UnitAtribStruct *unit_template = &_templates_unitattribs[unit_type];
  int behavior = unit_template->__Behavior;
  bool is_special_unit =
         behavior == UnitBehavior_SANDWORM
      || behavior == UnitBehavior_FRIGATE
      || behavior == UnitBehavior_ORNITHOPTER
      || behavior == UnitBehavior_CARRYALL
      || behavior == UnitBehavior_DEATH_HAND;
  switch(criteria_type)
  {
  case UNITTYPECRITERIATYPE_NONE:           result = true; break;
  case UNITTYPECRITERIATYPE_TYPE:           result = CompareValue(unit_type, value, comparison); break;
  case UNITTYPECRITERIATYPE_GROUP:          result = CompareValue(unit_template->__UnitType, value, comparison); break;
  case UNITTYPECRITERIATYPE_BEHAVIOR:       result = CompareValue(unit_template->__Behavior, value, comparison); break;
  case UNITTYPECRITERIATYPE_CATEGORY:
    switch(value)
    {
    case UNITTYPECATEGORY_INFANTRY:           result = unit_template->__IsInfantry; break;
    case UNITTYPECATEGORY_VEHICLE:            result = !(unit_template->__IsInfantry) && !is_special_unit; break;
    case UNITTYPECATEGORY_LIGHT_VEH:          result = !(unit_template->__IsInfantry) && !(unit_template->__CanCrush) && !is_special_unit; break;
    case UNITTYPECATEGORY_HEAVY_VEH:          result = !(unit_template->__IsInfantry) && (unit_template->__CanCrush) && !is_special_unit; break;
    case UNITTYPECATEGORY_SPECIAL:            result = is_special_unit; break;
    case UNITTYPECATEGORY_HAS_PRI_WEAPON:     result = unit_template->__PrimaryWeapon != -1; break;
    case UNITTYPECATEGORY_HAS_SEC_WEAPON:     result = unit_template->__SecondaryWeapon != -1; break;
    case UNITTYPECATEGORY_HAS_BARREL:         result = unit_template->__HasBarrel; break;
    case UNITTYPECATEGORY_CAN_BE_UPGRADED:    result = unit_template->UnitUpgradeAllowed; break;
    }
    break;
  case UNITTYPECRITERIATYPE_OWNER:          result = unit_template->__OwnerSide & (1 << value); break;
  case UNITTYPECRITERIATYPE_TECH:           result = CompareValue(unit_template->__TechReq, value, comparison); break;
  case UNITTYPECRITERIATYPE_PREREQ1:        result = CompareValue(unit_template->__PreReq1, value, comparison); break;
  case UNITTYPECRITERIATYPE_PREREQ2:        result = CompareValue(unit_template->__PreReq2, value, comparison); break;
  case UNITTYPECRITERIATYPE_ARMOR:          result = CompareValue(unit_template->__Armour, value, comparison); break;
  case UNITTYPECRITERIATYPE_SPEED_TYPE:     result = CompareValue(unit_template->__VehicleType, value, comparison); break;
  case UNITTYPECRITERIATYPE_PRI_WEAPON:     result = CompareValue(unit_template->__PrimaryWeapon, value, comparison); break;
  case UNITTYPECRITERIATYPE_SEC_WEAPON:     result = CompareValue(unit_template->__SecondaryWeapon, value, comparison); break;
  case UNITTYPECRITERIATYPE_SIGHT:          result = CompareValue(unit_template->__ViewDistance, value, comparison); break;
  case UNITTYPECRITERIATYPE_RANGE:          result = CompareValue((unit_template->__PrimaryWeapon != -1)?_templates_bulletattribs[(int)unit_template->__PrimaryWeapon].__Range:0, value, comparison); break;
  case UNITTYPECRITERIATYPE_SPEED:          result = CompareValue(unit_template->__Speed >> 12, value, comparison); break;
  case UNITTYPECRITERIATYPE_RATE_OF_FIRE:   result = CompareValue(unit_template->__RateOfFire, value, comparison); break;
  case UNITTYPECRITERIATYPE_HP100_MAX:      result = CompareValue(unit_template->__Strength / 100, value, comparison); break;
  case UNITTYPECRITERIATYPE_FLAG:           result = unit_template->Flags & (1 << value); break;
  }
  return result != negation;
}

bool CheckIfBuildingTypeMatchesCriteria(int building_type, eBuildingTypeFilterCriteriaType criteria_type, bool negation, bool comparison, int value)
{
  bool result = true;
  BuildingAtrbStruct *building_template = &_templates_buildattribs[building_type];
  switch(criteria_type)
  {
  case BUILDINGTYPECRITERIATYPE_NONE:           result = true; break;
  case BUILDINGTYPECRITERIATYPE_TYPE:           result = CompareValue(building_type, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_GROUP:          result = CompareValue(building_template->GroupType, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_BEHAVIOR:       result = CompareValue(building_template->__Behavior, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_CATEGORY:
    switch(value)
    {
    case BUILDINGTYPECATEGORY_POWER_CONS:       result = building_template->__PowerDrain > 0; break;
    case BUILDINGTYPECATEGORY_POWER_PROD:       result = building_template->__PowerDrain < 0; break;
    case BUILDINGTYPECATEGORY_REQ_ENOUGH_POW:   result = building_template->_____RequireEnoughPower; break;
    case BUILDINGTYPECATEGORY_SHAKE_SCREEN:     result = building_template->_____ScreenShake > 0; break;
    case BUILDINGTYPECATEGORY_HAS_PRI_WEAPON:   result = building_template->_____PrimaryWeapon != -1; break;
    case BUILDINGTYPECATEGORY_HAS_SEC_WEAPON:   result = building_template->_____SecondaryWeapon != -1; break;
    case BUILDINGTYPECATEGORY_HAS_BARREL:       result = building_template->_____BarrelArt != -1; break;
    }
    break;
  case BUILDINGTYPECRITERIATYPE_OWNER:          result = building_template->_____OwnerSide & (1 << value); break;
  case BUILDINGTYPECRITERIATYPE_TECH:           result = CompareValue(building_template->_____TechLevelBuild, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_PREREQ1:        result = CompareValue(building_template->_____Prereq1BuildingType, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_PREREQ2:        result = CompareValue(building_template->_____Prereq2BuildingType, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_ARMOR:          result = CompareValue(building_template->Armour, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_PRI_WEAPON:     result = CompareValue(building_template->_____PrimaryWeapon, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_SEC_WEAPON:     result = CompareValue(building_template->_____SecondaryWeapon, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_SIGHT:          result = CompareValue(building_template->_____SightRadius, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_POWER_CONS:     result = CompareValue((building_template->__PowerDrain > 0)?building_template->__PowerDrain:0, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_POWER_PROD:     result = CompareValue((building_template->__PowerDrain < 0)?building_template->__PowerDrain * -1:0, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_RANGE:          result = CompareValue((building_template->_____PrimaryWeapon != -1)?_templates_bulletattribs[(int)building_template->_____PrimaryWeapon].__Range:0, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_RATE_OF_FIRE:   result = CompareValue(building_template->_____RateOfFire, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_HP100_MAX:      result = CompareValue(building_template->_____HitPoints / 100, value, comparison); break;
  case BUILDINGTYPECRITERIATYPE_FLAG:           result = building_template->_____Flags & (1 << value); break;
  }
  return result != negation;
}

bool TileCheck(int pos_x, int pos_y, eTileCheckType check_type, bool comparison, int value)
{
  uint32_t attributes = gGameMap.map[pos_x + _CellNumbersWidthSpan[pos_y]].__tile_bitflags;
  switch (check_type)
  {
    case TILECHECK_XPOS:     return CompareValue(pos_x, value, comparison);
    case TILECHECK_YPOS:     return CompareValue(pos_y, value, comparison);
    case TILECHECK_ATTRIB:   return attributes & (1 << value);
    case TILECHECK_TERRAIN:  return CompareValue((attributes >> 29) & 7, value, comparison);
    case TILECHECK_SPICE:    return CompareValue((attributes >> 20) & 7, value, comparison);
    case TILECHECK_CONCOWN:  return CompareValue((attributes >> 17) & 7, value, comparison);
    case TILECHECK_OWNER1:   return CompareValue((attributes >> 0) & 7, value, comparison);
    case TILECHECK_OWNER2:   return CompareValue((attributes >> 25) & 7, value, comparison);
    case TILECHECK_UNIT_TYPE:
    case TILECHECK_UNIT_GROUP:
    case TILECHECK_UNIT_TAG:
    case TILECHECK_UNIT_FLAG:
    case TILECHECK_UNIT_STATE:
    {
      eSideType side_id = 0;
      _WORD index;
      Unit *unit;
      if ((attributes & 0x3E8) && (GetUnitOnTile(32 * pos_x, 32 * pos_y, &side_id, &index, false)))
      {
        index = -1;
        while ((unit = GetNextUnitOnTile(pos_x, pos_y, side_id, &index)))
        {
          switch(check_type)
          {
            case TILECHECK_UNIT_TYPE:   if (CompareValue(unit->Type, value, comparison)) return true; break;
            case TILECHECK_UNIT_GROUP:  if (CompareValue(_templates_unitattribs[unit->Type].__UnitType, value, comparison)) return true; break;
            case TILECHECK_UNIT_TAG:    if (CompareValue(unit->Tag, value, comparison)) return true; break;
            case TILECHECK_UNIT_FLAG:   if (unit->Flags & (1 << value)) return true; break;
            case TILECHECK_UNIT_STATE:  if (CompareValue(unit->State, value, comparison)) return true; break;
            default: ;
          }
        }
      }
      break;
    }
    case TILECHECK_BUILDING_TYPE:
    case TILECHECK_BUILDING_GROUP:
    case TILECHECK_BUILDING_TAG:
    case TILECHECK_BUILDING_FLAG:
    case TILECHECK_BUILDING_STATE:
    case TILECHECK_BUILDING_TILENO:
    {
      eSideType side_id = 0;
      _WORD index;
      Building *bld;
      if ((attributes & TileFlags_10_OCC_BUILDING) && GetBuildingOnTile_0(pos_x, pos_y, &bld, &side_id, &index))
      {
        switch(check_type)
        {
          case TILECHECK_BUILDING_TYPE:   return CompareValue(bld->Type, value, comparison);
          case TILECHECK_BUILDING_GROUP:  return CompareValue(_templates_buildattribs[bld->Type].GroupType, value, comparison);
          case TILECHECK_BUILDING_TAG:    return CompareValue(bld->Tag, value, comparison);
          case TILECHECK_BUILDING_FLAG:   return bld->Flags & (1 << value);
          case TILECHECK_BUILDING_STATE:  return CompareValue(bld->__State, value, comparison);
          case TILECHECK_BUILDING_TILENO: return CompareValue((pos_x - (bld->__PosX >> 21)) + (pos_y - (((bld->__PosY >> 16) - _templates_buildattribs[bld->Type]._____ArtHeight) >> 5)) * 4, value, comparison);
          default: ;
        }
      }
      break;
    }
  }
  return false;
}

bool ObjectCheck(int side_id, int obj_index, eObjectCheckType check_type, int pos_x, int pos_y, bool comparison, int value)
{
  CSide *side = GetSide(side_id);
  Unit *unit = &side->__ObjectArray[obj_index];
  Building *bld = (Building *)unit;
  if (unit->ObjectType == 1)
  {
    UnitAtribStruct *ut = &_templates_unitattribs[unit->Type];
    switch(check_type)
    {
      case OBJECTCHECK_TYPE: return CompareValue(unit->Type, value, comparison);
      case OBJECTCHECK_GROUP: return CompareValue(ut->__UnitType, value, comparison);
      case OBJECTCHECK_CATEGORY: return CheckIfUnitMatchesCriteria(unit, side_id, UNITCRITERIATYPE_CATEGORY, false, comparison, value);
      case OBJECTCHECK_TAG: return CompareValue(unit->Tag, value, comparison);
      case OBJECTCHECK_ARMOR: return CompareValue(ut->__Armour, value, comparison);
      case OBJECTCHECK_WEAPON: return ((ut->__PrimaryWeapon != -1) && CompareValue(ut->__PrimaryWeapon, value, comparison)) || ((ut->__SecondaryWeapon != -1) && CompareValue(ut->__SecondaryWeapon, value, comparison));
      case OBJECTCHECK_DIST: return CompareDistance(unit->__PosX >> 16, unit->__PosY >> 16, pos_x, pos_y, value, comparison);
    }
  }
  else if (unit->ObjectType == 2)
  {
    BuildingAtrbStruct *bt = &_templates_buildattribs[bld->Type];
    switch(check_type)
    {
      case OBJECTCHECK_TYPE: return CompareValue(bld->Type, value, comparison);
      case OBJECTCHECK_GROUP: return CompareValue(bt->GroupType, value, comparison);
      case OBJECTCHECK_CATEGORY: return CheckIfBuildingMatchesCriteria(bld, side_id, BUILDINGCRITERIATYPE_CATEGORY, false, comparison, value);
      case OBJECTCHECK_TAG: return CompareValue(bld->Tag, value, comparison);
      case OBJECTCHECK_ARMOR: return CompareValue(bt->Armour, value, comparison);
      case OBJECTCHECK_WEAPON: return ((bt->_____PrimaryWeapon != -1) && CompareValue(bt->_____PrimaryWeapon, value, comparison)) || ((bt->_____SecondaryWeapon != -1) && CompareValue(bt->_____SecondaryWeapon, value, comparison));
      case OBJECTCHECK_DIST: return CompareDistance((bld->__PosX >> 16) + 16, (bld->__PosY >> 16) - bt->_____ArtHeight + 16, pos_x, pos_y, value, comparison);
    }
  }
  //else
  //  DebugFatal("event-filters.c", "Invalid object type %d to check for (side %d index %d)", unit->ObjectType, side_id, obj_index);
  return false;
}

void GetBoundsForPosFilter(ObjectFilterStruct *filter, int *min_x, int *min_y, int *max_x, int *max_y)
{
  *min_x = 0;
  *min_y = 0;
  *max_x = gGameMapWidth-1;
  *max_y = gGameMapHeight-1;
  if ((filter->pos_flags & OBJFILTERPOSFLAG_DOCHECK) && !(filter->pos_flags & OBJFILTERPOSFLAG_NEGATE))
  {
    int pos_min_x = GetVariableValueOrConst(filter->pos_flags, 4, filter->pos_min_x);
    int pos_min_y = GetVariableValueOrConst(filter->pos_flags, 5, filter->pos_min_y);
    int pos_max_x = GetVariableValueOrConst(filter->pos_flags, 6, filter->pos_max_x);
    int pos_max_y = GetVariableValueOrConst(filter->pos_flags, 7, filter->pos_max_y);
    int pos_type = (filter->pos_flags >> 2) & 3;
    switch (pos_type)
    {
      case 0:
        *min_x = pos_min_x;
        *min_y = pos_min_y;
        *max_x = pos_max_x;
        *max_y = pos_max_y;
        break;
      case 1:
      case 2:
        *min_x = LLIMIT(pos_min_x - pos_max_x, *min_x);
        *min_y = LLIMIT(pos_min_y - pos_max_x, *min_y);
        *max_x = HLIMIT(pos_min_x + pos_max_x, *max_x);
        *max_y = HLIMIT(pos_min_y + pos_max_x, *max_y);
        break;
      case 3:
        *min_x = LLIMIT(pos_min_x - pos_max_x / 32, *min_x);
        *min_y = LLIMIT(pos_min_y - pos_max_x / 32, *min_y);
        *max_x = HLIMIT(pos_min_x + pos_max_x / 32, *max_x);
        *max_y = HLIMIT(pos_min_y + pos_max_x / 32, *max_y);
        break;
    }
  }
}
