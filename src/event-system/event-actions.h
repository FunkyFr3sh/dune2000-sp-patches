
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

typedef enum eEventOperation
{
  EVENTOPERATION_SET,
  EVENTOPERATION_PLUS,
  EVENTOPERATION_MINUS
} eEventOperation;

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
void EvAct_SetCash(int side_id, eEventOperation operation, int value);
void EvAct_SetTech(int side_id, bool immediate_update, int value);
void EvAct_SwitchMySide(int side_id, int ai_switch, bool reveal_base);
void EvAct_HideMap();
void EvAct_RevealMap(int xpos, int ypos, int radius);
void EvAct_ShowMessage(int duration, ShowMessageEventData *data);
void EvAct_UnitSpawn(int xpos, int ypos, int side_id, int amount, char *unit_list);
void EvAct_UnBlockEvent(int operation, int event_index);
void EvAct_PlayMusic(char *name);
void EvAct_DamageTiles(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool hit_explosion);
int  EvAct_AddUnit(int xpos, int ypos, int side_id, int properties, int unit_type, int movement, int facing);
int  EvAct_AddBuilding(int xpos, int ypos, int side_id, int properties, int building_type, int method, int facing);
void EvAct_AddProjectile(int src_x, int src_y, int targ_x, int targ_y, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int weapon_type, bool circle_spread, bool play_sound);
void EvAct_AddExplosion(int xpos, int ypos, int pixel_x, int pixel_y, int spread_x, int spread_y, int side_id, int explosion_type, bool circle_spread, bool play_sound);
int  EvAct_AddCrate(int xpos, int ypos, int crate_type, int image, int ext_data, int respawns, int expiration);
void EvAct_AddConcrete(int xpos, int ypos, int width, int height, int side_id, int tilebitmask);
void EvAct_SpiceBloom(int xpos, int ypos, int range, eSpiceBloomMode mode, bool randomizer);
void EvAct_CenterViewport(int xpos, int ypos);
void EvAct_ChangeMapBlock(int xpos, int ypos, int width, int height, uint16_t *tiles);
void EvAct_TransformTiles(int amount, uint16_t *tiles);
void EvAct_ChangeTileAttributes(int xpos, int ypos, int width, int height, eEventOperation operation, uint32_t attributes);
void EvAct_ChangeTileDamage(int xpos, int ypos, int width, int height, eEventOperation operation, int value);
void EvAct_ActivateTimer(int condition_index);
