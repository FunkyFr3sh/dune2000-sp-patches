// ### MIS file structs ###

typedef struct MiscData
{
  char Tech[8];
  int  Cash[8];
  char Empty[40];
} MiscData;

typedef struct MessageData
{
  int __index;
  int __slot;
  int __slot2;
  int __ticks[5];
  int __type[4];
  char field_30[4];
  char __messages[5][128];
} MessageData;

typedef struct EventData
{
  uint8_t coord_x[4];
  uint8_t coord_y[4];
  int32_t value;
  uint8_t num_conditions;
  uint8_t event_type;
  char args[5];
  char condition_index[12];
  char coord_var_flags;
  char arg_var_flags;
  char condition_not[12];
  char conditions_and_or;
  char blocked_flags;
  char data[25];
} EventData;

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
} ConditionData;
