
// Event-related structs

typedef struct EventData
{
  uint8_t coord_x[4];
  uint8_t coord_y[4];
  int32_t value;
  uint8_t num_conditions;
  uint8_t event_type;
  uint8_t args[5];
  uint8_t condition_index[12];
  uint8_t coord_var_flags;
  uint8_t arg_var_flags;
  uint8_t condition_negation[12];
  uint8_t filter_skip;
  uint8_t event_flags;
  char data[25];
} EventData;

typedef struct EventContext
{
  int coord_x[4];
  int coord_y[4];
  int args[6];
  char *data;
  int index;
} EventContext;

enum EventFlags
{
  EVENTFLAG_AUTO_BLOCK = 1,
  EVENTFLAG_BLOCKED = 2,
  EVENTFLAG_CONDITIONS_OR = 4
};

enum EventTypes
{
  // Vanilla / General
  ET_REINFORCEMENT,
  ET_STARPORT_DELIVERY,
  ET_ALLEGIANCE,
  ET_LEAVE,
  ET_BESERK,
  ET_PLAYSOUND,
  ET_SETBUILDRATE,
  ET_SETATTACKBUILDINGRATE,
  ET_SETCASH,
  ET_SETTECH,
  ET_WIN,
  ET_LOSE,
  ET_SWITCH_MY_SIDE,
  ET_HIDE_MAP,
  ET_REVEAL,
  ET_SETTIMER,
  ET_HIDETIMER,
  ET_SHOWMESSAGE,
  ET_UNIT_SPAWN,
  ET_SET_FLAG,
  ET_UN_BLOCK_EVENT,
  ET_PLAY_MUSIC,
  ET_DAMAGE_TILES,
  ET_ADD_UNIT,
  ET_ADD_BUILDING,
  ET_ADD_PROJECTILE,
  ET_ADD_EXPLOSION,
  ET_ADD_CRATE,
  ET_ADD_CONCRETE,
  ET_SPICE_BLOOM,
  ET_SHAKE_SCREEN,
  ET_CENTER_VIEWPORT,
  ET_CHANGE_MAP_BLOCK,
  ET_TRANSFORM_TILES,
  ET_ADD_BUILDING_DESTRUCT,
  ET_35,
  ET_ACTIVATE_TIMER,
  ET_37,
  ET_38,
  ET_39,
  // Side manipulation
  ET_TRANSFER_CREDITS,
  ET_SET_BUILDING_UPGRADES,
  ET_SET_STARPORT_STOCK,
  ET_SET_STARPORT_COST,
  ET_CHANGE_STARPORT_UNIT,
  ET_SHOW_SIDE_DATA,
  // AI manipulation
  ET_SET_AI_PROPERTY,
  ET_SHOW_AI_DATA,
  // Memory manipulation
  ET_SET_MEMORY_DATA,
  ET_SHOW_MEMORY_DATA,
  // Unit manipulation
  ET_DESTROY_UNIT,
  ET_DAMAGE_HEAL_UNIT,
  ET_CHANGE_UNIT_OWNER,
  ET_CHANGE_UNIT_TYPE,
  ET_SET_UNIT_FLAG,
  ET_SET_UNIT_PROPERTY,
  ET_SELECT_UNIT,
  ET_AIRLIFT_UNIT,
  ET_58,
  ET_SHOW_UNIT_DATA,
  // Building manipulation
  ET_DESTROY_BUILDING,
  ET_DAMAGE_HEAL_BUILDING,
  ET_CHANGE_BUILDING_OWNER,
  ET_CHANGE_BUILDING_TYPE,
  ET_SET_BUILDING_FLAG,
  ET_SET_BUILDING_PROPERTY,
  ET_SELECT_BUILDING,
  ET_67,
  ET_68,
  ET_SHOW_BUILDING_DATA,
  // Crate manipulation
  ET_REMOVE_CRATE,
  ET_PICKUP_CRATE,
  ET_SET_CRATE_PROPERTY,
  ET_SHOW_CRATE_DATA,
  // Tile manipulation
  ET_CHANGE_TILE,
  ET_SET_TILE_ATTRIBUTE,
  ET_SET_TILE_DAMAGE,
  ET_REVEAL_TILE,
  ET_HIDE_TILE,
  ET_SHOW_TILE_DATA,
  // Orders
  ET_ORDER_UNIT_MOVE,
  ET_ORDER_DOCK_WITH_REFINERY,
  ET_ORDER_REPAIR_SELECTED_UNITS,
  ET_ORDER_REPAIR_SINGLE_UNIT,
  ET_ORDER_UNIT_ATTACK_UNIT,
  ET_ORDER_UNIT_ATTACK_BUILDING,
  ET_ORDER_UNIT_ATTACK_TILE,
  ET_ORDER_UNIT_GUARD,
  ET_ORDER_UNIT_SCATTER,
  ET_ORDER_UNIT_RETREAT,
  ET_ORDER_UNIT_DEPLOY,
  ET_ORDER_BUILDING_ATTACK_UNIT,
  ET_ORDER_BUILDING_ATTACK_BUILDING,
  ET_ORDER_BUILDING_SET_PRIMARY,
  ET_ORDER_BUILDING_REPAIR,
  ET_ORDER_BUILDING_SELL,
  ET_ORDER_STOP,
  ET_ORDER_BUILD_BUILDING_PICK,
  ET_ORDER_BUILD_BUILDING_CANCEL,
  ET_ORDER_BUILD_PLACE_BUILDING,
  ET_ORDER_BUILD_UNIT_PICK,
  ET_ORDER_BUILD_UNIT_CANCEL,
  ET_ORDER_STARPORT_PICK,
  ET_ORDER_STARPORT_UNPICK,
  ET_ORDER_STARPORT_PURCHASE,
  ET_ORDER_STARPORT_CANCEL,
  ET_ORDER_UPGRADE_PICK,
  ET_ORDER_UPGRADE_CANCEL,
  ET_ORDER_SPECIAL_WEAPON,
  ET_109
};

// Condition-related structs

typedef struct ConditionData
{
  int32_t val2; //__time_amount;
  int32_t val1; //__start_delay;
  int32_t val3; //__value;
  uint8_t coord_x[2];
  uint8_t coord_var_flags;
  uint8_t arg_var_flags;
  uint8_t coord_y[2];
  uint8_t unused1;
  uint8_t unused2;
  union
  {
    int32_t val4;
    float   float_val; //__casualties_ratio;
  };
  uint8_t side_id; //__side_id;
  uint8_t condition_type;
  uint8_t arg1; //__building_type;
  uint8_t arg2; //__unit_type_or_comparison_function;
} ConditionData;

enum ConditionTypes
{
  // Vanilla / General
  CT_BUILDINGEXISTS,
  CT_UNITEXISTS,
  CT_INTERVAL,
  CT_TIMER,
  CT_CASUALTIES,
  CT_BASEDESTROYED,
  CT_UNITSDESTROYED,
  CT_REVEALED,
  CT_CREDITS,
  CT_FLAG,
  CT_RANDOMCHANCE,
  CT_RANDOMINTERVAL,
  CT_DIPLOMACY,
  CT_DIFFICULTY,
  // Environment checking
  CT_CHECKUNITS,
  CT_CHECKBUILDINGS,
  CT_CHECKCRATES,
  CT_CHECKTILES,
  CT_SPICE_IN_AREA,
  CT_DAMAGE_IN_AREA,
  // Side related
  CT_POWER,
  CT_BUILDING_UPGRADES,
  CT_STARPORT_STOCK,
  CT_STARPORT_COST,
  CT_STARPORT_PICK,
  CT_STARPORT_DELIVERY,
  CT_26,
  CT_BUILDING_ICON,
  CT_UNIT_ICON,
  CT_UPGRADE_ICON,
  CT_SPICE_HARVESTED,
  CT_UNITS_BUILT,
  CT_BUILDINGS_BUILT,
  CT_UNITS_LOST,
  CT_BUILDINGS_LOST,
  CT_UNITS_KILLED,
  CT_BUILDINGS_KILLED,
  CT_SIDE_PROPERTY,
  // AI related
  CT_AI_PROPERTY,
  // Memory related
  CT_MEMORY_VALUE
};

enum ConditionFilterFlags
{
  CONDITIONFILTERFLAG_STRICT_EQUAL = 1
};

// Variables

#define MAX_EVENTS 1024
#define MAX_CONDITIONS 256

extern EventData _gEventArray[MAX_EVENTS];
extern ConditionData _gConditionArray[MAX_CONDITIONS];

extern int tick_random_value;

// Functions

bool EvaluateCondition(int condition_index);
void ExecuteEvent(int event_index);
void ExecuteEventAction(int event_type, EventContext *e);
