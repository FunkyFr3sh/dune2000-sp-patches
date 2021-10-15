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
