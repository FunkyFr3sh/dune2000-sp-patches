#include "event-filters.h"

typedef union int_or_float
{
  int int_val;
  float float_val;
} int_or_float;

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
  MSGVARIABLETYPE_HEXNUMBER,
  MSGVARIABLETYPE_FLOAT1,
  MSGVARIABLETYPE_FLOAT2,
  MSGVARIABLETYPE_FLOAT3,
  MSGVARIABLETYPE_FLOAT4,
  MSGVARIABLETYPE_STRING_FROM_TABLE,
  MSGVARIABLETYPE_UNIT_NAME,
  MSGVARIABLETYPE_BUILDING_NAME,
  MSGVARIABLETYPE_UNIT_TYPE,
  MSGVARIABLETYPE_BUILDING_TYPE,
  MSGVARIABLETYPE_UNIT_GROUP,
  MSGVARIABLETYPE_BUILDING_GROUP,
  MSGVARIABLETYPE_WEAPON_NAME,
  MSGVARIABLETYPE_EXPLOSION_NAME,
  MSGVARIABLETYPE_WARHEAD_NAME,
  MSGVARIABLETYPE_ARMOUR_TYPE
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

typedef enum eSetTooltipColorMode
{
  SETTOOLTIPCOLORMODE_KEEP,
  SETTOOLTIPCOLORMODE_YELLOW,
  SETTOOLTIPCOLORMODE_GRAY,
  SETTOOLTIPCOLORMODE_CUSTOM
} eSetTooltipColorMode;

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

typedef enum eGetMousePositionType
{
  GETMOUSEPOSITIONTYPE_ABSOLUTE,
  GETMOUSEPOSITIONTYPE_MAPPIXEL,
  GETMOUSEPOSITIONTYPE_MAPTILE,
  GETMOUSEPOSITIONTYPE_RADARPOSITION,
  GETMOUSEPOSITIONTYPE_BUILDINGICON,
  GETMOUSEPOSITIONTYPE_UNITICON,
  GETMOUSEPOSITIONTYPE_STARPORTICON,
  GETMOUSEPOSITIONTYPE_UPGRADEICON
}eGetMousePositionType;

typedef enum eIfConditionType
{
  IFCONDTYPE_EXPRESSION,
  IFCONDTYPE_CHECK_UNIT,
  IFCONDTYPE_CHECK_BUILDING,
  IFCONDTYPE_CHECK_BULLET,
  IFCONDTYPE_CHECK_EXPLOSION,
  IFCONDTYPE_CHECK_CRATE,
  IFCONDTYPE_CHECK_TILE,
  IFCONDTYPE_CHECK_SIDE,
  IFCONDTYPE_CHECK_UNIT_TYPE,
  IFCONDTYPE_CHECK_BUILDING_TYPE
}eIfConditionType;

void EvAct_AddDelivery                  (int event_id, int xpos, int ypos, int side_id, int amount, int tag, int deploy_action, int delay, eDeliveryType delivery_type, char *unit_list);
void EvAct_SetDiplomacy                 (int event_id, int source_side, int target_side, int allegiance_type, bool both_sided);
void EvAct_PlaySound                    (int event_id, int xpos, int ypos, int sample_id, bool force, bool point_sound);
void EvAct_SetCash                      (int event_id, int side_id, eValueOperation operation, int value);
void EvAct_SetTech                      (int event_id, int side_id, eValueOperation operation, bool immediate_update, int value);
void EvAct_SwitchMySide                 (int event_id, int side_id, int ai_switch, bool reveal_base);
void EvAct_HideMap                      ();
void EvAct_RevealMap                    (int event_id, int xpos, int ypos, int radius);
void EvAct_ShowMessage                  (int xoff, int yoff, int ref_id, int screen_pos, int color, eMsgSoundMode sound_mode, bool type_on, int duration, ShowMessageEventData *data);
void EvAct_UnitSpawn                    (int event_id, int xpos, int ypos, int side_id, int amount, int facing, int tag, char *unit_list);
void EvAct_UnBlockEvent                 (int event_id, int operation, int target_event_index);
void EvAct_PlayMusic                    (char *name);
void EvAct_DamageTiles                  (int event_id, int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool hit_explosion);
void EvAct_AddUnit                      (int event_id, int xpos, int ypos, int side_id, int properties, int unit_type, int movement, int facing, int tag, int target_var);
void EvAct_AddBuilding                  (int event_id, int xpos, int ypos, int side_id, int properties, int building_type, int method, int facing, int tag, int target_var);
void EvAct_AddBullet                    (int event_id, int xpos, int ypos, int targ_x, int targ_y, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool play_sound, int tag, int target_var);
void EvAct_AddExplosion                 (int event_id, int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int explosion_type, bool circle_spread, bool play_sound, int tag, int target_var);
int  EvAct_AddCrate                     (int event_id, int xpos, int ypos, int crate_type, int image, int ext_data, int respawns, int expiration);
void EvAct_AddConcrete                  (int event_id, int min_x, int min_y, int max_x, int max_y, int side_id, int tilebitmask);
void EvAct_SpiceBloom                   (int event_id, int xpos, int ypos, int range, eSpiceBloomMode mode, bool randomizer);
void EvAct_ChangeViewport               (int xpos, int ypos, int mode, int units);
void EvAct_ChangeMapBlock               (int event_id, int xpos, int ypos, int width, int height, eChangeTileMode mode, uint16_t *tiles);
void EvAct_TransformTiles               (int event_id, int amount, eChangeTileMode mode, uint16_t *tiles);
void EvAct_AddBuildingDestruct          (int event_id, int xpos, int ypos, int side_id, int building_type);
void EvAct_AddHomingBullet              (int event_id, int xpos, int ypos, int pixel_x, int pixel_y, int side_id, int weapon_type, int enemy_side, int enemy_index_var, bool play_sound, int tag, int target_var);
void EvAct_ActivateTimer                (int event_id, int condition_index);
void EvAct_RemoveMessage                (eRemoveMessageMode mode, int ref_id, int amount);
void EvAct_SetMessageColor              (int event_id, int color_index, eSetMessageColorMode mode, int transition_speed, int transition_stages, int color1, int color2);
void EvAct_SetTooltip                   (int event_id, int line, eSetTooltipColorMode color_mode, int color, ShowMessageEventData *data);
// Side manipulation
void EvAct_TransferCredits              (int event_id, int side_id, eTransferCreditsOperation operation, int value);
void EvAct_SetBuildingUpgrades          (int event_id, int side_id, int building_group, eValueOperation operation, int value);
void EvAct_SetStarportCost              (int event_id, int side_id, int unit_type, eValueOperation operation, bool default_cost, int value);
void EvAct_ShowSideData                 (int event_id, int side_id, int offset);
// AI manipulation
void EvAct_SetAIProperty                (int event_id, int side_id, eDataType data_type, eValueOperation operation, int offset, int value);
void EvAct_ShowAIData                   (int event_id, int side_id, int offset);
// Memory manipulation
void EvAct_SetMemoryData                (int event_id, eDataType data_type, eValueOperation operation, int address, int value);
void EvAct_ShowMemoryData               (int event_id, int address);
// Unit manipulation
void EvAct_DestroyUnit                  (int event_id, int side_id, bool silent, int unit_index);
void EvAct_DamageHealUnit               (int event_id, int side_id, int action, int units, int value, int unit_index);
void EvAct_ChangeUnitType               (int event_id, int side_id, int target_type, bool defined_type, int unit_index);
void EvAct_SetUnitFlag                  (int event_id, int side_id, eFlagOperation operation, int flag, int unit_index);
void EvAct_SetUnitProperty              (int event_id, int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int unit_index);
void EvAct_SelectUnit                   (int event_id, int side_id, bool exclude_from_restore, int unit_index);
void EvAct_AirliftUnit                  (int event_id, int side_id, int target_x, int target_y, bool units_target, int unit_index);
void EvAct_ShowUnitData                 (int event_id, int side_id, int unit_index);
// Building manipulation
void EvAct_DestroyBuilding              (int event_id, int side_id, bool silent, int building_index);
void EvAct_DamageHealBuilding           (int event_id, int side_id, int action, int units, int value, int building_index);
void EvAct_ChangeBuildingOwner          (int event_id, int side_id, int target_side, int building_index);
void EvAct_ChangeBuildingType           (int event_id, int side_id, int target_type, int building_index);
void EvAct_SetBuildingFlag              (int event_id, int side_id, eFlagOperation operation, int flag, int building_index);
void EvAct_SetBuildingProperty          (int event_id, int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int building_index);
void EvAct_SelectBuilding               (int event_id, int side_id, bool exclude_from_restore, int building_index);
void EvAct_ShowBuildingData             (int event_id, int side_id, int building_index);
// Bullet manipulation
void EvAct_SetBulletProperty            (int event_id, int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int bullet_index);
// Explosion manipulation
void EvAct_SetExplosionProperty         (int event_id, int side_id, eDataType data_type, int offset, eValueOperation operation, int value, int explosion_index);
// Crate manipulation
void EvAct_RemoveCrate                  (int event_id, int crate_index);
void EvAct_PickupCrate                  (int event_id, int side_id, int crate_index);
void EvAct_SetCrateProperty             (int event_id, eDataType data_type, int offset, eValueOperation operation, int value, int crate_index);
void EvAct_ShowCrateData                (int event_id, int crate_index);
// Tile manipulation
void EvAct_ChangeTile                   (int event_id, eChangeTileMode mode, int tile_index, int xpos, int ypos);
void EvAct_SetTileAttribute             (int event_id, eFlagOperation operation, int attribute, int xpos, int ypos);
void EvAct_SetTileProperty              (int event_id, eDataType data_type, int offset, eValueOperation operation, int value, int xpos, int ypos);
void EvAct_RevealTile                   (int event_id, int radius, int xpos, int ypos);
void EvAct_HideTile                     (int event_id, int xpos, int ypos);
void EvAct_ShowTileData                 (int event_id, int xpos, int ypos);
// Orders
void EvAct_OrderUnitRetreat             (int event_id, int side_id);
void EvAct_OrderBuildBuildingCancel     (int event_id, int side_id, bool force);
void EvAct_OrderBuildPlaceBuilding      (int event_id, int side_id, int xpos, int ypos);
void EvAct_OrderBuildUnitCancel         (int event_id, int side_id, bool any_unit, int unit_type, int queue, bool force);
void EvAct_OrderStarportPick            (int event_id, int side_id, int unit_type);
void EvAct_OrderUpgradeCancel           (int event_id, int side_id, bool force);
// Miscellaneous
void EvAct_AddRadarMarker               (int event_id, int xpos, int ypos, int slot, int ref_id, int color, int thickness, int duration, int custom_color);
// Variable operations
void EvAct_SetVariable                  (int event_id, int target_var, bool use_offset, int offset_var, eValueOperation operation, int value);
void EvAct_GetVariable                  (int target_var, int src_var_base, int src_var_offset);
void EvAct_SetFloatVariable             (int event_id, int target_var, bool use_offset, int offset_var, eValueOperation operation, int value);
void EvAct_ConvertVariable              (int first_var, int number_of_vars, int operation);
void EvAct_DebugVariables               (int first_var, int number_of_vars, int ref_id);
void EvAct_GetRandomValue               (int target_var, int min_value, int max_value);
void EvAct_GetRandomCoords              (int min_x, int min_y, int max_x, int max_y, int first_var);
void EvAct_GetValueFromList             (int event_id, int amount, int target_var, int mode, int index_var, uint8_t *value_list);
void EvAct_GetCoordsFromList            (int event_id, int amount, int first_var, int mode, int index_var, uint8_t *value_list);
void EvAct_GetAreaFromList              (int event_id, int amount, int first_var, int mode, int index_var, uint8_t *value_list);
void EvAct_GetCount                     (int target_var);
void EvAct_GetSpiceCount                (int target_var, int xpos, int ypos);
void EvAct_GetDamageCount               (int target_var, int xpos, int ypos);
void EvAct_GetObjectProperty            (int event_id, int side_id, eDataType data_type, int offset, int index_var, int target_var, eObjectType object_type);
void EvAct_GetCrateProperty             (int event_id, eDataType data_type, int offset, int index_var, int target_var);
void EvAct_GetTileProperty              (int event_id, eDataType data_type, int offset, int first_var, int target_var);
void EvAct_GetSideProperty              (int event_id, int side_id, eDataType data_type, int target_var, int offset);
void EvAct_GetAIProperty                (int event_id, int side_id, eDataType data_type, int target_var, int offset);
void EvAct_GetMemoryData                (int event_id, eDataType data_type, int target_var, int address);
void EvAct_GetUnitTemplateProperty      (int event_id, eDataType data_type, int offset, int unit_type, int target_var);
void EvAct_GetBuildingTemplateProperty  (int event_id, eDataType data_type, int offset, int building_type, int target_var);
void EvAct_GetWeaponTemplateProperty    (int event_id, eDataType data_type, int offset, int weapon_type, int target_var);
void EvAct_GetExplosionTemplateProperty (int event_id, eDataType data_type, int offset, int explosion_type, int target_var);
void EvAct_GetArmourValue               (int event_id, int armour_type, int select_by, int weapon_type, int warhead_type, int target_var);
void EvAct_GetSpeedValue                (int event_id, int vehicle_type, int terrain_type, int target_var);
void EvAct_GetGroupIDValue              (int what, int target_var);
void EvAct_GetUnitType                  (int event_id, int side_id, int target_var, bool my_version_only, bool random, ObjectFilterStruct *filter);
void EvAct_GetBuildingType              (int event_id, int side_id, int target_var, bool my_version_only, bool random, ObjectFilterStruct *filter);
void EvAct_GetGameTicks                 (int target_var);
void EvAct_GetMySideId                  (int target_var);
void EvAct_GetDifficulty                (int target_var);
void EvAct_GetRule                      (int rule, int target_var);
void EvAct_GetDiplomacy                 (int event_id, int source, int target, int target_var);
void EvAct_GetTech                      (int event_id, int side_id, int target_var);
void EvAct_GetHouseId                   (int event_id, int side_id, int target_var);
void EvAct_GetCredits                   (int event_id, int side_id, eGetCreditsType what, int target_var);
void EvAct_GetPower                     (int event_id, int side_id, eGetPowerType what, int target_var);
void EvAct_GetBuildingUpgrades          (int event_id, int side_id, int building_group, int target_var);
void EvAct_GetStarportStock             (int event_id, int side_id, int unit_type, int target_var);
void EvAct_GetStarportCost              (int event_id, int side_id, int unit_type, int target_var);
void EvAct_GetStarportPick              (int event_id, int side_id, int unit_type, int target_var);
void EvAct_GetBuildingQueueState        (int event_id, int side_id, eDataType data_type, int offset, int target_var);
void EvAct_GetUnitQueueState            (int event_id, int side_id, eDataType data_type, int offset, int queue, int queue_num, int target_var);
void EvAct_GetUpgradeQueueState         (int event_id, int side_id, eDataType data_type, int offset, int target_var);
void EvAct_GetSpiceHarvested            (int event_id, int side_id, int target_var);
void EvAct_GetUnitsBuilt                (int event_id, int side_id, int unit_type, bool total, int target_var);
void EvAct_GetBuildingsBuilt            (int event_id, int side_id, int building_type, bool total, int target_var);
void EvAct_GetUnitsLost                 (int event_id, int side_id, int unit_type, bool total, int target_var);
void EvAct_GetBuildingsLost             (int event_id, int side_id, int target_var);
void EvAct_GetUnitsKilled               (int event_id, int side_id, int enemy, int unit_type, bool total, int target_var);
void EvAct_GetBuildingsKilled           (int event_id, int side_id, int enemy, int building_type, bool total, int target_var);
void EvAct_GetMousePosition             (eGetMousePositionType what, int first_var);
void EvAct_GetKeyboardMouseState        (int target_var, int key);
void EvAct_GetUnitUnderCursor           (int side_var, int index_var, bool ignore_shroud, bool ignore_stealth);
void EvAct_GetBuildingUnderCursor       (int side_var, int index_var, bool ignore_shroud);
void EvAct_GetSidebarButtonUnderCursor  (int button, int target_var, bool click_on_it);
void EvAct_GetGameInterfaceData         (eDataType data_type, int offset, int target_var);
void EvAct_GetObjectPosition            (int event_id, int side_id, int index_var, int format, int target_var);
void EvAct_GetDirection                 (int first_pos_var, int second_pos_var, int target_var);
void EvAct_GetPositionOnCircle          (int center_pos_var, int angle, int distance, int target_var);
void EvAct_GetNearestBuildingTile       (int event_id, int side_id, int index_var, int from_pos_var, int format, int target_var);
void EvAct_GetDistance                  (int first_pos_var, int second_pos_var, int mode, int target_var);
void EvAct_CheckDistance                (int first_pos_var, int second_pos_var, int distance, int target_var);

// Blocks
void EvAct_ExecuteBlock                 (int event_id, int target_event_index);

// Conditional expression
void EvAct_If                           (int event_id, eIfConditionType condition_type, int side_var, int object_index_var, CondExprData *cond_expr);

// Loops
void EvAct_LoopWhile                    (int event_id, CondExprData *cond_expr);
void EvAct_LoopValuesFromRange          (int event_id, int loop_var, int min_value, int max_value);
void EvAct_LoopCoordsFromArea           (int event_id, int min_x, int min_y, int max_x, int max_y, int first_var);
void EvAct_LoopValuesFromList           (int event_id, int amount, int loop_var, uint8_t *value_list);
void EvAct_LoopCoordsFromList           (int event_id, int amount, int first_var, uint8_t *value_list);
void EvAct_LoopAreasFromList            (int event_id, int amount, int first_var, uint8_t *value_list);
void EvAct_LoopObject                   (int event_id, int side_var, int index_var, int side_id, int object_index);
void EvAct_LoopItem                     (int event_id, int index_var, int object_index);
void EvAct_LoopTiles                    (int event_id, int first_var, int xpos, int ypos);
