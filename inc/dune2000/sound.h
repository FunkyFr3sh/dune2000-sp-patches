// ### Sound library structs ###

typedef void ISampleManager;

typedef struct SampleTableStruct
{
  char *key;
  char *filename;
  int last_played;
} SampleTableStruct;
