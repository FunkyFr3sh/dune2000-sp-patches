
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
  ET_34,
  ET_35,
  ET_ACTIVATE_TIMER,
  ET_37,
  ET_38,
  ET_39,
  ET_40,
  ET_41,
  ET_42,
  ET_43,
  ET_44,
  ET_45,
  ET_46,
  ET_47,
  ET_48,
  ET_49,
  ET_DESTROY_UNIT,
  ET_DAMAGE_HEAL_UNIT,
  ET_CHANGE_UNIT_OWNER,
  ET_CHANGE_UNIT_TYPE,
  ET_SET_UNIT_FLAG,
  ET_SET_UNIT_PROPERTY,
  ET_SHOW_UNIT_DATA,
  ET_57,
  ET_58,
  ET_59,
  ET_DESTROY_BUILDING,
  ET_DAMAGE_HEAL_BUILDING,
  ET_CHANGE_BUILDING_OWNER,
  ET_CHANGE_BUILDING_TYPE,
  ET_SET_BUILDING_FLAG,
  ET_SET_BUILDING_PROPERTY,
  ET_SHOW_BUILDING_DATA,
  ET_67,
  ET_68,
  ET_69,
  ET_REMOVE_CRATE,
  ET_71,
  ET_72,
  ET_73,
  ET_74,
  ET_SET_TILE_ATTRIBUTE,
  ET_SET_TILE_DAMAGE,
  ET_REVEAL_TILE,
  ET_78,
  ET_79
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
  CT_BUILDINGEXISTS,
  CT_UNITEXISTS,
  CT_INTERVAL,
  CT_TIMER,
  CT_CASUALTIES,
  CT_BASEDESTROYED,
  CT_UNITSDESTROYED,
  CT_REVEALED,
  CT_HARVESTED,
  CT_FLAG,
  CT_RANDOMCHANCE,
  CT_RANDOMINTERVAL,
  CT_12,
  CT_13,
  CT_CHECKUNITS,
  CT_CHECKBUILDINGS,
  CT_CHECKCRATES,
  CT_CHECKTILES
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
