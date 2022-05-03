// ### MIS file structs ###

typedef struct MiscData
{
  char Tech[8];
  int  Cash[8];
  char Empty[40];
} MiscData;

typedef struct MessageData
{
  unsigned int __count_active;
  unsigned int __next_slot;
  unsigned int __oldest_active;
  unsigned int __ticks[5];
  int __type[5];
  char __messages[5][128];
} MessageData;
