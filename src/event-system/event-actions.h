
typedef struct ShowMessageEventData
{
  uint8_t unused[20];
  uint32_t message_index;
} ShowMessageEventData;

typedef enum eDeliveryType
{
  DELIVERYTYPE_REINFORCE = 1,
  DELIVERYTYPE_STARPORT = 2
} eDeliveryType;

typedef enum eEventValueOperation
{
  EVENTVALUEOPERATION_SET,
  EVENTVALUEOPERATION_ADD,
  EVENTVALUEOPERATION_SUBSTRACT
} eEventValueOperation;

typedef enum eEventFlagOperation
{
  EVENTFLAGOPERATION_ADD,
  EVENTFLAGOPERATION_REMOVE,
  EVENTFLAGOPERATION_FLIP
} eEventFlagOperation;

typedef enum eSpiceBloomMode
{
  SPICEBLOOM_CLASSIC,
  SPICEBLOOM_SQUARE,
  SPICEBLOOM_CIRCLE,
  SPICEBLOOM_DUNE2
} eSpiceBloomMode;

void EvAct_AddDelivery(int xpos, int ypos, int side_id, int amount, int deploy_action, eDeliveryType delivery_type, char *unit_list);
void EvAct_SetDiplomacy(int source_side, int target_side, int allegiance_type, bool both_sided);
void EvAct_PlaySound(int sample_id, bool point_sound, int xpos, int ypos);
void EvAct_SetCash(int side_id, eEventValueOperation operation, int value);
void EvAct_SetTech(int side_id, bool immediate_update, int value);
void EvAct_SwitchMySide(int side_id, int ai_switch, bool reveal_base);
void EvAct_HideMap();
void EvAct_RevealMap(int xpos, int ypos, int radius);
void EvAct_ShowMessage(int duration, ShowMessageEventData *data);
void EvAct_UnitSpawn(int xpos, int ypos, int side_id, int amount, int facing, int tag, char *unit_list);
void EvAct_UnBlockEvent(int operation, int event_index);
void EvAct_PlayMusic(char *name);
void EvAct_DamageTiles(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool hit_explosion);
int  EvAct_AddUnit(int xpos, int ypos, int side_id, int properties, int unit_type, int movement, int facing, int tag);
int  EvAct_AddBuilding(int xpos, int ypos, int side_id, int properties, int building_type, int method, int facing, int tag);
void EvAct_AddProjectile(int src_x, int src_y, int targ_x, int targ_y, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool play_sound);
void EvAct_AddExplosion(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int explosion_type, bool circle_spread, bool play_sound);
int  EvAct_AddCrate(int xpos, int ypos, int crate_type, int image, int ext_data, int respawns, int expiration);
void EvAct_AddConcrete(int xpos, int ypos, int width, int height, int side_id, int tilebitmask);
void EvAct_SpiceBloom(int xpos, int ypos, int range, eSpiceBloomMode mode, bool randomizer);
void EvAct_CenterViewport(int xpos, int ypos);
void EvAct_ChangeMapBlock(int xpos, int ypos, int width, int height, uint16_t *tiles);
void EvAct_TransformTiles(int amount, uint16_t *tiles);
void EvAct_ActivateTimer(int condition_index);
void EvAct_DestroyUnit(int side_id, bool silent, int unit_index);
void EvAct_DamageHealUnit(int side_id, int action, int units, int value, int unit_index);
void EvAct_ChangeUnitType(int side_id, int target_type, bool defined_type, int unit_index);
void EvAct_SetUnitFlag(int side_id, eEventFlagOperation operation, int flag, int unit_index);
void EvAct_SetUnitProperty(int side_id, int byte, int value, int unit_index);
void EvAct_AirliftUnit(int side_id, int target_x, int target_y, bool units_target, int unit_index);
void EvAct_ShowUnitData(int side_id, int unit_index);
void EvAct_DamageHealBuilding(int side_id, int action, int units, int value, int building_index);
void EvAct_ChangeBuildingOwner(int side_id, int target_side, int building_index);
void EvAct_ChangeBuildingType(int side_id, int target_type, int building_index);
void EvAct_SetBuildingFlag(int side_id, eEventFlagOperation operation, int flag, int building_index);
void EvAct_SetBuildingProperty(int side_id, int byte, int value, int building_index);
void EvAct_ShowBuildingData(int side_id, int building_index);
void EvAct_RemoveCrate(int crate_index);
void EvAct_SetTileAttribute(eEventFlagOperation operation, int attribute, int cell_index);
void EvAct_SetTileDamage(eEventValueOperation operation, int value, int cell_index);
void EvAct_RevealTile(int cell_index);
