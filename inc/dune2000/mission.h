// ### MIS file structs ###

enum EventTypes
{
    ET_REINFORCE,
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
    ET_BLOXFILE,
    ET_ATTRIBFILE,
    ET_REVEAL,
    ET_SHOWTIMER,
    ET_HIDETIMER,
    ET_SHOWMESSAGE,
    ET_ADDUNITS,
    ET_SETFLAG,
    ET_UNUSED,
    ET_PLAYMUSIC
};

enum EventConditions
{
    EC_BUILDINGEXISTS,
    EC_UNITEXISTS,
    EC_INTERVAL,
    EC_TIMER,
    EC_CASUALTIES,
    EC_BASEDESTROYED,
    EC_UNITSDESTROYED,
    EC_REVEALED,
    EC_HARVESTED,
    EC_FLAG
};

typedef struct MiscData
{
  char Tech[8];
  int  Cash[8];
  char Empty[40];
}MiscData;

typedef struct MessageData
{
  int __index;
  int __slot;
  int __slot2;
  int __ticks[5];
  int __type[4];
  char field_30[4];
  char __messages[5][128];
}MessageData;

typedef struct EventData
{
    uint32_t xpos;
    uint32_t ypos;
    int32_t value;      // Flag value, Message unknown value
    uint8_t num_conditions;
    uint8_t event_type;
    uint8_t amount;         // Number of units, reveal Map radius
    uint8_t side_id;        // Also Set Flag flag number
    uint8_t allegiance_target;
    uint8_t allegiance_type;
    uint8_t deploy_action;
    char condition_index[14];
    char condition_not[14];
    char units[21];
    uint32_t message_index;
}EventData;

typedef struct ConditionData
{
  int32_t time_amount;
  int32_t start_delay;
  int32_t value;
  uint32_t xpos;
  uint32_t ypos;
  float casualties_ratio;
  char side_id;
  char condition_type;
  char building_type;
  char unit_type_or_comparison_function;
}ConditionData;
