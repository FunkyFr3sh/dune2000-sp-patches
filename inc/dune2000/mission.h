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
  char condition_negation[12];
  char unused;
  char event_flags;
  char data[25];
} EventData;

typedef struct ConditionData
{
  uint32_t __time_amount;
  uint32_t __start_delay;
  uint32_t __value;
  int __xpos;
  int __ypos;
  float __casualties_ratio;
  char __side_id;
  char __condition_type;
  char __building_type;
  char __unit_type_or_comparison_function;
} ConditionData;
