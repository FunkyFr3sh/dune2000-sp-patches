#include "../../files/WAIL.H"

// ### Sound library structs ###

// Increase number of sound channels from 6 to 15 and increase sound cache size from 16 to 40
#define NUM_HANDLES 15
#define NUM_CACHE_ENTRIES 40

typedef struct WAVEFORMAT_
{
  WORD wFormatTag;
  WORD nChannels;
  DWORD nSamplesPerSec;
  DWORD nAvgBytesPerSec;
  WORD nBlockAlign;
  WORD wBitsPerSample;
} WAVEFORMAT_;

typedef struct SampleData
{
  HSAMPLE sampleptr;
  int position;
  int pan;
  int rate;
  int field_10;
  int field_14;
  int volume;
  int __sound_id;
  int dw_field_20;
} SampleData;

typedef struct SampleCacheEntry
{
  int __sound_id;
  char *filebuffer;
  int __ticks;
} SampleCacheEntry;

typedef struct _SOS_COMPRESS_INFO
{
  char *lpSource;
  char *lpDest;
  int dw_field_8;
  int dw_field_C;
  int dw_field_10;
  _DWORD dwPredicted;
  int dwDifference;
  __int16 wCodeBuf;
  __int16 wCode;
  __int16 wStep;
  _WORD wIndex;
  int dwSampleIndex2;
  _DWORD dwPredicted2;
  int dwDifference2;
  __int16 wCodeBuf2;
  __int16 wCode2;
  __int16 wStep2;
  _WORD wIndex2;
  __int16 wBitSize;
  __int16 wChannels;
} _SOS_COMPRESS_INFO;

typedef struct ISampleManager
{
  HDIGDRIVER __driver;
  SampleData __handles[NUM_HANDLES]; // Extended array size (mod)
  SampleCacheEntry fSampleCache[NUM_CACHE_ENTRIES]; // Extended array size (mod)
  float unused_field_19C;
  char *__musicfilesamplebuffer_1A0;
  void *__musicbuffer_1A4;
  int __musicsampleindex_1A8;
  FILE *__musicfilehandle_1AC;
  HSAMPLE __streamedsamplehandle_1B0;
  int __musicfilesamplesizes_1B4[3];
  _SOS_COMPRESS_INFO __musicsosdata;
} ISampleManager;

typedef struct WaveDataStruct
{
  const char *name;
  _DWORD fileoffset;
  _DWORD filesize;
  struct WaveDataStruct *next;
} WaveDataStruct;

typedef struct SampleTableStruct
{
  char *key;
  char *filename;
  int last_played;
} SampleTableStruct;
