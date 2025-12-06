#define MAX_BUILD_QUEUE_ENTRIES 100
#define NUM_BUILD_QUEUES 5

typedef struct BuildQueueEntry
{
  char unit_type;
  char next;
} BuildQueueEntry;

typedef struct BuildQueue
{
  BuildQueueEntry entries[MAX_BUILD_QUEUE_ENTRIES];
  char entry_count;
  char front;
  char back;
  char pending_ordered_unit_type;
  char last_built_unit_type;
} BuildQueue;

typedef struct SideExtraData
{
  BuildQueue build_queues[NUM_BUILD_QUEUES];
  char build_queue_unit_type_count[MAX_UNIT_TYPES];
} SideExtraData;

SideExtraData gSideExtraData[MAX_SIDES];
