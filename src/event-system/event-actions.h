#include "event-filters.h"

typedef struct ShowMessageEventData
{
  uint32_t sample_id;
  uint8_t variable_type[8];
  uint8_t variable_index[8];
  uint32_t string_index;
} ShowMessageEventData;

typedef struct CondExprData
{
  uint32_t operators;
  uint16_t and_or;
  uint8_t value_var_flags;
  uint8_t num_operations;
  uint8_t variable[8];
  uint8_t value[8];
} CondExprData;

typedef enum eDeliveryType
{
  DELIVERYTYPE_REINFORCE = 1,
  DELIVERYTYPE_STARPORT = 2
} eDeliveryType;

typedef enum eMsgSoundMode
{
  MSGSOUNDMODE_DEFAULT,
  MSGSOUNDMODE_NONE,
  MSGSOUNDMODE_CUSTOM,
  MSGSOUNDMODE_CUSTOM_FORCE
} eMsgSoundMode;

typedef enum eMsgVariableType
{
  MSGVARIABLETYPE_NONE,
  MSGVARIABLETYPE_NUMBER,
  MSGVARIABLETYPE_TIME,
  MSGVARIABLETYPE_STRING_FROM_TABLE
} eMsgVariableType;

typedef enum eRemoveMessageMode
{
  REMOVEMSGMODE_REFID,
  REMOVEMSGMODE_CHATOLDEST,
  REMOVEMSGMODE_CHATNEWEST
} eRemoveMessageMode;

typedef enum eSetMessageColorMode
{
  SETMSGCOLORMODE_SOLID_SHADOW,
  SETMSGCOLORMODE_COLOR_GRADIENT,
  SETMSGCOLORMODE_TIME_TRANSITION_1,
  SETMSGCOLORMODE_TIME_TRANSITION_2
} eSetMessageColorMode;

typedef enum eSpiceBloomMode
{
  SPICEBLOOM_CLASSIC,
  SPICEBLOOM_SQUARE,
  SPICEBLOOM_CIRCLE,
  SPICEBLOOM_DUNE2
} eSpiceBloomMode;

typedef enum eTransferCreditsOperation
{
  TRANSFERCREDITS_ALL_TO_CASH,
  TRANSFERCREDITS_ALL_TO_SPICE_STORAGE,
  TRANSFERCREDITS_ALL_TO_SPICE_FORCE,
  TRANSFERCREDITS_VALUE_TO_CASH,
  TRANSFERCREDITS_VALUE_TO_SPICE_STORAGE,
  TRANSFERCREDITS_VALUE_TO_SPICE_FORCE
} eTransferCreditsOperation;

typedef enum eChangeTileMode
{
  CHANGETILE_NORMAL,
  CHANGETILE_VISUAL,
  CHANGETILE_RESTORE
} eChangeTileMode;

typedef enum eGetCreditsType
{
  GETCREDITSTYPE_TOTAL,
  GETCREDITSTYPE_SPICE,
  GETCREDITSTYPE_CASH,
  GETCREDITSTYPE_MAX_STORAGE
}eGetCreditsType;

typedef enum eGetPowerType
{
  GETPOWERTYPE_PERCENT,
  GETPOWERTYPE_TOTAL_OUTPUT,
  GETPOWERTYPE_TOTAL_DRAIN,
  GETPOWERTYPE_EXTRA_OUTPUT,
  GETPOWERTYPE_EXTRA_DRAIN
}eGetPowerType;

void EvAct_AddDelivery(int xpos, int ypos, int side_id, int amount, int deploy_action, int delay, eDeliveryType delivery_type, char *unit_list);
void EvAct_SetDiplomacy(int source_side, int target_side, int allegiance_type, bool both_sided);
void EvAct_PlaySound(int sample_id, bool force, bool point_sound, int xpos, int ypos);
void EvAct_SetCash(int side_id, eValueOperation operation, int value);
void EvAct_SetTech(int side_id, eValueOperation operation, bool immediate_update, int value);
void EvAct_SwitchMySide(int side_id, int ai_switch, bool reveal_base);
void EvAct_HideMap();
void EvAct_RevealMap(int xpos, int ypos, int radius);
void EvAct_ShowMessage(int xoff, int yoff, int ref_id, int screen_pos, int color, eMsgSoundMode sound_mode, bool type_on, int duration, ShowMessageEventData *data);
void EvAct_UnitSpawn(int xpos, int ypos, int side_id, int amount, int facing, int tag, char *unit_list);
void EvAct_UnBlockEvent(int operation, int event_index);
void EvAct_PlayMusic(char *name);
void EvAct_DamageTiles(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool hit_explosion);
void EvAct_AddUnit(int xpos, int ypos, int side_id, int properties, int unit_type, int movement, int facing, int tag, int target_var);
void EvAct_AddBuilding(int xpos, int ypos, int side_id, int properties, int building_type, int method, int facing, int tag, int target_var);
void EvAct_AddProjectile(int src_x, int src_y, int targ_x, int targ_y, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool play_sound);
void EvAct_AddExplosion(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int explosion_type, bool circle_spread, bool play_sound);
int  EvAct_AddCrate(int xpos, int ypos, int crate_type, int image, int ext_data, int respawns, int expiration);
void EvAct_AddConcrete(int min_x, int min_y, int max_x, int max_y, int side_id, int tilebitmask);
void EvAct_SpiceBloom(int xpos, int ypos, int range, eSpiceBloomMode mode, bool randomizer);
void EvAct_CenterViewport(int xpos, int ypos);
void EvAct_ChangeMapBlock(int xpos, int ypos, int width, int height, eChangeTileMode mode, uint16_t *tiles);
void EvAct_TransformTiles(int amount, eChangeTileMode mode, uint16_t *tiles);
void EvAct_AddBuildingDestruct(int xpos, int ypos, int side_id, int building_type);
void EvAct_ActivateTimer(int condition_index);
void EvAct_RemoveMessage(eRemoveMessageMode mode, int ref_id, int amount);
void EvAct_SetMessageColor(int color_index, eSetMessageColorMode mode, int transition_speed, int transition_stages, int color1, int color2);
// Side manipulation
void EvAct_TransferCredits(int side_id, eTransferCreditsOperation operation, int value);
void EvAct_SetBuildingUpgrades(int side_id, int building_group, eValueOperation operation, int value);
void EvAct_SetStarportCost(int side_id, int unit_type, eValueOperation operation, bool default_cost, int value);
void EvAct_ShowSideData(int side_id, int offset);
// AI manipulation
void EvAct_SetAIProperty(int side_id, eDataType data_type, eValueOperation operation, int offset, int value);
void EvAct_ShowAIData(int side_id, int offset);
// Memory manipulation
void EvAct_SetMemoryData(eDataType data_type, eValueOperation operation, int address, int value);
void EvAct_ShowMemoryData(int address);
// Unit manipulation
void EvAct_DestroyUnit(int side_id, bool silent, int unit_index);
void EvAct_DamageHealUnit(int side_id, int action, int units, int value, int unit_index);
void EvAct_ChangeUnitType(int side_id, int target_type, bool defined_type, int unit_index);
void EvAct_SetUnitFlag(int side_id, eFlagOperation operation, int flag, int unit_index);
void EvAct_SetUnitProperty(int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int unit_index);
void EvAct_SelectUnit(int side_id, bool exclude_from_restore, int unit_index);
void EvAct_AirliftUnit(int side_id, int target_x, int target_y, bool units_target, int unit_index);
void EvAct_ShowUnitData(int side_id, int unit_index);
// Building manipulation
void EvAct_DestroyBuilding(int side_id, bool silent, int building_index);
void EvAct_DamageHealBuilding(int side_id, int action, int units, int value, int building_index);
void EvAct_ChangeBuildingOwner(int side_id, int target_side, int building_index);
void EvAct_ChangeBuildingType(int side_id, int target_type, int building_index);
void EvAct_SetBuildingFlag(int side_id, eFlagOperation operation, int flag, int building_index);
void EvAct_SetBuildingProperty(int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int building_index);
void EvAct_SelectBuilding(int side_id, bool exclude_from_restore, int building_index);
void EvAct_ShowBuildingData(int side_id, int building_index);
// Crate manipulation
void EvAct_RemoveCrate(int crate_index);
void EvAct_PickupCrate(int side_id, int crate_index);
void EvAct_SetCrateProperty(eDataType data_type, int offset, eValueOperation operation, int value, int crate_index);
void EvAct_ShowCrateData(int crate_index);
// Tile manipulation
void EvAct_ChangeTile(eChangeTileMode mode, int tile_index, int xpos, int ypos);
void EvAct_SetTileAttribute(eFlagOperation operation, int attribute, int xpos, int ypos);
void EvAct_SetTileDamage(eValueOperation operation, int value, int xpos, int ypos);
void EvAct_RevealTile(int radius, int xpos, int ypos);
void EvAct_HideTile(int xpos, int ypos);
void EvAct_ShowTileData(int xpos, int ypos);
// Orders
void EvAct_OrderUnitRetreat(int side_id);
void EvAct_OrderBuildBuildingCancel(int side_id, bool force);
void EvAct_OrderBuildPlaceBuilding(int side_id, int xpos, int ypos);
void EvAct_OrderBuildUnitCancel(int side_id, bool any_unit, int unit_type, int queue, bool force);
void EvAct_OrderStarportPick(int side_id, int unit_type);
void EvAct_OrderUpgradeCancel(int side_id, bool force);
// Variable operations
void EvAct_SetVariable(int var_index, eValueOperation operation, int value);
void EvAct_GetRandomValue(int target_var, int min_value, int max_value);
void EvAct_GetRandomCoords(int min_x, int min_y, int max_x, int max_y, int first_var);
void EvAct_GetValueFromList(int event_index, int amount, int target_var, int mode, int index_var, uint8_t *value_list);
void EvAct_GetCoordsFromList(int event_index, int amount, int first_var, int mode, int index_var, uint8_t *value_list);
void EvAct_GetAreaFromList(int event_index, int amount, int first_var, int mode, int index_var, uint8_t *value_list);
void EvAct_GetCount(int target_var);
void EvAct_GetSpiceCount(int target_var, int xpos, int ypos);
void EvAct_GetDamageCount(int target_var, int xpos, int ypos);
void EvAct_GetObjectProperty(int side_id, eDataType data_type, int offset, int index_var, int target_var);
void EvAct_GetCrateProperty(eDataType data_type, int offset, int index_var, int target_var);
void EvAct_GetTileProperty(eDataType data_type, int offset, int first_var, int target_var);
void EvAct_GetAIProperty(int side_id, eDataType data_type, int target_var, int offset);
void EvAct_GetMemoryData(eDataType data_type, int target_var, int address);
void EvAct_GetUnitTemplateProperty(eDataType data_type, int offset, int unit_type, int target_var);
void EvAct_GetBuildingTemplateProperty(eDataType data_type, int offset, int building_type, int target_var);
void EvAct_GetWeaponTemplateProperty(eDataType data_type, int offset, int weapon_type, int target_var);
void EvAct_GetExplosionTemplateProperty(eDataType data_type, int offset, int explosion_type, int target_var);
void EvAct_GetUnitType(int target_var, bool random, ObjectFilterStruct *filter);
void EvAct_GetBuildingType(int target_var, bool random, ObjectFilterStruct *filter);
void EvAct_GetGameTicks(int target_var);
void EvAct_GetMySideId(int target_var);
void EvAct_GetDifficulty(int target_var);
void EvAct_GetDiplomacy(int source, int target, int target_var);
void EvAct_GetTech(int side_id, int target_var);
void EvAct_GetHouseId(int side_id, int target_var);
void EvAct_GetCredits(int side_id, eGetCreditsType what, int target_var);
void EvAct_GetPower(int side_id, eGetPowerType what, int target_var);
void EvAct_GetBuildingUpgrades(int side_id, int building_group, int target_var);
void EvAct_GetStarportStock(int side_id, int unit_type, int target_var);
void EvAct_GetStarportCost(int side_id, int unit_type, int target_var);
void EvAct_GetStarportPick(int side_id, int unit_type, int target_var);
void EvAct_GetSpiceHarvested(int side_id, int target_var);
void EvAct_GetUnitsBuilt(int side_id, int unit_type, bool total, int target_var);
void EvAct_GetBuildingsBuilt(int side_id, int building_type, bool total, int target_var);
void EvAct_GetUnitsLost(int side_id, int unit_type, bool total, int target_var);
void EvAct_GetBuildingsLost(int side_id, int target_var);
void EvAct_GetUnitsKilled(int side_id, int enemy, int unit_type, bool total, int target_var);
void EvAct_GetBuildingsKilled(int side_id, int enemy, int building_type, bool total, int target_var);

// Blocks
void EvAct_ExecuteBlock(int event_index, int target_event_index);

// Conditional expression
void EvAct_If(int event_index, CondExprData *cond_expr);

// Loops
void EvAct_LoopWhile(int event_index, CondExprData *cond_expr);
void EvAct_LoopValuesFromRange(int event_index, int loop_var, int min_value, int max_value);
void EvAct_LoopCoordsFromArea(int event_index, int min_x, int min_y, int max_x, int max_y, int first_var);
void EvAct_LoopValuesFromList(int event_index, int amount, int loop_var, uint8_t *value_list);
void EvAct_LoopCoordsFromList(int event_index, int amount, int first_var, uint8_t *value_list);
void EvAct_LoopAreasFromList(int event_index, int amount, int first_var, uint8_t *value_list);
void EvAct_LoopObject(int event_index, int side_var, int index_var, int side_id, int object_index);
void EvAct_LoopItem(int event_index, int index_var, int object_index);
void EvAct_LoopTiles(int event_index, int first_var, int xpos, int ypos);
