
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
  uint8_t unused;
  uint8_t event_flags;
  char data[25];
} EventData;

typedef struct EventContext
{
  int coord_x[4];
  int coord_y[4];
  int args[6];
  char *data;
} EventContext;

enum EventFlags
{
  EVENTFLAG_AUTO_BLOCK = 1,
  EVENTFLAG_BLOCKED = 2,
  EVENTFLAG_CONDITIONS_OR = 4
};

enum EventTypes
{
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
  ET_SHOWTIMER,
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
  ET_CHANGE_TILE_ATTRIBUTES,
  ET_CHANGE_TILE_DAMAGE
};

// Condition-related structs

typedef struct ConditionData
{
  int32_t val2; //__time_amount;
  int32_t val1; //__start_delay;
  int32_t value; //__value;
  uint8_t coord_x[2];
  uint8_t coord_var_flags;
  uint8_t arg_var_flags;
  uint8_t coord_y[2];
  uint8_t unused1;
  uint8_t unused2;
  float   float_val; //__casualties_ratio;
  uint8_t side_id; //__side_id;
  uint8_t condition_type;
  uint8_t arg1; //__building_type;
  uint8_t arg2; //__unit_type_or_comparison_function;
} ConditionData;

enum ConditionTypes
{
  CT_BUILDINGEXISTS,
  CT_UNITEXISTS,
  CT_INTERVAL,
  CT_TIMER,
  CT_CASUALTIES,
  CT_BASEDESTROYED,
  CT_UNITSDESTROYED,
  CT_REVEALED,
  CT_HARVESTED,
  CT_FLAG
};

// Variables

#define MAX_EVENTS 1024
#define MAX_CONDITIONS 256

extern EventData _gEventArray[MAX_EVENTS];
extern ConditionData _gConditionArray[MAX_CONDITIONS];

// Functions

bool EvaluateCondition(int condition_index);
void ExecuteEvent(int event_index);
void ExecuteEventAction(int event_type, EventContext *e);
