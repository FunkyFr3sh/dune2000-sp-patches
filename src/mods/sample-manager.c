#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"
#include "patch.h"

// Increase number of sound channels from 6 to 15 and increase sound cache size from 16 to 40
// Whole ISampleManager class needs to be re-implemented because the ISampleManager struct has
// extended arrays, so the offsets to access struct members must be changed everywhere

// Custom implementation of function ISampleManager__Init
DETOUR(0x0046EDA0, 0x0046EEDD, _Mod__ISampleManager__Init);

void __thiscall Mod__ISampleManager__Init(ISampleManager *this, int handle_count, int a3)
{
  ISampleManager *sm; // edi
  SampleData *handle; // esi
  int result; // ebx
  LPSTR error_str; // eax
  int handle_id; // ebx
  HSAMPLE sample; // eax
  WAVEFORMAT_ format; // [esp+10h] [ebp-D8h]
  char Buffer[200]; // [esp+20h] [ebp-C8h]

  (void)handle_count;
  (void)a3;
  sm = this;
  this->__musicfilesamplebuffer_1A0 = 0;
  this->__musicfilehandle_1AC = 0;
  this->__streamedsamplehandle_1B0 = 0;
  this->__driver = 0;
  handle = this->__handles;
  result = 0;
  _samplemanunused = 0;
  format.wFormatTag = 1;
  format.nChannels = 2;
  format.nSamplesPerSec = 22050;
  format.nAvgBytesPerSec = 88200;
  format.nBlockAlign = 4;
  format.wBitsPerSample = 16;
  if ( waveOutGetNumDevs() )
  {
    result = AIL_waveOutOpen(&sm->__driver, 0, 0, (LPWAVEFORMAT)&format);
  }
  else
  {
    sprintf(Buffer, "ISampleManager() : No wave playing devices detected");
    OutputDebugStringA(Buffer);
    sm->__driver = 0;
  }
  if ( result )
  {
    error_str = AIL_last_error();
    sprintf(Buffer, "ISampleManager() error: \n%s", error_str);
    OutputDebugStringA(Buffer);
    ISampleManager__Allocate(sm);
  }
  else
  {
    _SampleManagerInitDone = 1;
    handle_id = 0;
    do
    {
      sample = AIL_allocate_sample_handle(sm->__driver);
      handle->sampleptr = sample;
      handle->__sound_id = -1;
      handle->dw_field_20 = -1;
      AIL_init_sample(sample);
      ISampleManager__SetSampleData(sm, handle_id, 64, 64, 0, 22050, 0, 0);
      ++handle;
      ++handle_id;
    }
    while ( (signed __int16)handle_id < NUM_HANDLES );
    ISampleManager__Allocate(sm);
  }
}

// Custom implementation of function ISampleManager__Allocate
DETOUR(0x0046EEE0, 0x0046EF3F, _Mod__ISampleManager__Allocate);

void __thiscall Mod__ISampleManager__Allocate(ISampleManager *this)
{
  char **v1; // esi
  signed int counter; // edi

  v1 = &this->fSampleCache[0].filebuffer;
  counter = NUM_CACHE_ENTRIES;
  do
  {
    *(v1 - 1) = (char *)-1;
    // New logic start
    // Increase WAV inside SOUND.RS size limit to 512 kilobytes
    *v1 = (char *)Alloc(0x80000, "Allocate fSampleCache");
    // New logic end
    v1[1] = 0;
    v1 += 3;
    --counter;
  }
  while ( counter );
  // New logic start
  // Increase WAV inside SOUND.RS size limit to 512 kilobytes
  sample_filebuffer = (char *)Alloc(0x80000, 0);
  sample_buffer = (char *)Alloc(0x80000, 0);
  // New logic end
}

// Custom implementation of function ISampleManager__Deallocate
DETOUR(0x0046EF40, 0x0046EF90, _Mod__ISampleManager__Deallocate);

void __thiscall Mod__ISampleManager__Deallocate(ISampleManager *this)
{
  char **v1; // esi
  signed int counter; // edi

  v1 = &this->fSampleCache[0].filebuffer;
  counter = NUM_CACHE_ENTRIES;
  do
  {
    *(v1 - 1) = (char *)-1;
    v1[1] = 0;
    Free(*v1);
    *v1 = 0;
    v1 += 3;
    --counter;
  }
  while ( counter );
  Free(sample_filebuffer);
  Free(sample_buffer);
}

// Custom implementation of function ISampleManager__LoadSampleCache
DETOUR(0x0046EF90, 0x0046F072, _Mod__ISampleManager__LoadSampleCache);

char __thiscall Mod__ISampleManager__LoadSampleCache(ISampleManager *this, int sound_id, int cache_id)
{
  ISampleManager *sm; // ebx
  char result; // al
  char *sample_filename; // eax
  WaveDataStruct *sfx; // eax
  unsigned int filesize; // ecx
  char ArgList[200]; // [esp+10h] [ebp-C8h]

  sm = this;
  if ( (unsigned int)cache_id >= NUM_CACHE_ENTRIES )
  {
    return 0;
  }
  if ( !_SampleManagerInitDone )
  {
    return 0;
  }
  LoadSoundEffects();
  sample_filename = GetSampleFilename(sound_id);
  sfx = FindSoundEffectByFilename(sample_filename);
  if ( !sfx )
  {
    return 0;
  }
  filesize = sfx->filesize;
  if ( filesize < 0x80000 )
  {
    LoadSoundEffect(sfx, sm->fSampleCache[cache_id].filebuffer);
    sm->fSampleCache[cache_id].__sound_id = sound_id;
    *((_DWORD *)&sm->__driver + 3 * (cache_id + 0x13)) = gGameTicks;
    result = 1;
  }
  else
  {
    DebugFatal("LoadSampleCache", "Sample %s size %d > %d", ArgList, filesize, 0x80000);
    result = 0;
  }
  return result;
}

// Custom implementation of function ISampleManager__HandleCache
DETOUR(0x0046F080, 0x0046F163, _Mod__ISampleManager__HandleCache);

int __thiscall Mod__ISampleManager__HandleCache(ISampleManager *this, int sound_id, int a2)
{
  ISampleManager *sm; // ebp
  int result; // eax
  SampleCacheEntry *v5; // edx MAPDST
  SampleCacheEntry *cache_entry; // ecx
  unsigned int cache_id; // esi
  unsigned int handle_id; // edi
  int *v10; // esi
  int v11; // eax
  unsigned int tick; // esi
  int sample_cache_id; // edi
  unsigned int scid; // eax
  int *ptr; // edx

  sm = this;
  result = 0;
  v5 = this->fSampleCache;
  cache_entry = this->fSampleCache;
  while ( sound_id != cache_entry->__sound_id )
  {
    ++result;
    ++cache_entry;
    if ( (unsigned int)result >= NUM_CACHE_ENTRIES )
    {
      if ( a2 == 1 )
      {
        return -1;
      }
      cache_id = 0;
      while ( v5->__sound_id >= 0 )
      {
        ++cache_id;
        ++v5;
        if ( cache_id >= NUM_CACHE_ENTRIES )
        {
          handle_id = 0;
          v10 = &sm->__handles[0].dw_field_20;
          do
          {
            if ( AIL_sample_status((HSAMPLE)*(v10 - 8)) & 4 )
            {
              v11 = *v10;
              if ( *v10 < 0 || v11 >= NUM_CACHE_ENTRIES )
              {
                return -1;
              }
              *((_DWORD *)&sm->__driver + 3 * (v11 + 0x13)) = gGameTicks;
            }
            ++handle_id;
            v10 += 9;
          }
          while ( handle_id < NUM_HANDLES );
          tick = gGameTicks;
          sample_cache_id = -1;
          scid = 0;
          ptr = &sm->fSampleCache[0].__ticks;
          do
          {
            if ( (unsigned int)*ptr < tick )
            {
              tick = *ptr;
              sample_cache_id = scid;
            }
            ++scid;
            ptr += 3;
          }
          while ( scid < NUM_CACHE_ENTRIES );
          if ( sample_cache_id >= 0 && ISampleManager__LoadSampleCache(sm, sound_id, sample_cache_id) )
          {
            return sample_cache_id;
          }
          return -1;
        }
      }
      if ( !ISampleManager__LoadSampleCache(sm, sound_id, cache_id) )
      {
        return -1;
      }
      return cache_id;
    }
  }
  return result;
}

// Custom implementation of function ISampleManager__PlaySample
DETOUR(0x0046F170, 0x0046F210, _Mod__ISampleManager__PlaySample);

void __thiscall Mod__ISampleManager__PlaySample(ISampleManager *this, HSAMPLE sample, int handle_id, int sound_id, unsigned __int16 loop_count, int pan, int volume)
{
  ISampleManager *sm; // edi
  int cache_id; // eax

  sm = this;
  if ( this->__driver )
  {
    AIL_stop_sample(sample);
    AIL_end_sample(sample);
    AIL_init_sample(sample);
    cache_id = ISampleManager__HandleCache(sm, sound_id, 0);
    if ( cache_id >= 0 && cache_id < NUM_CACHE_ENTRIES && (signed __int16)handle_id < NUM_HANDLES )
    {
      sm->__handles[(signed __int16)handle_id].__sound_id = sound_id;
      *((_DWORD *)&sm->__driver + 9 * ((signed __int16)handle_id + 1)) = cache_id;
      AIL_set_sample_file(sample, sm->fSampleCache[cache_id].filebuffer, 0);
      ISampleManager__setsamplevals(sm, handle_id, sample, pan, volume);
      AIL_set_sample_loop_count(sample, loop_count);
      AIL_start_sample(sample);
    }
  }
}

// Custom implementation of function ISampleManager__PlaySample_0
DETOUR(0x0046F210, 0x0046F321, _Mod__ISampleManager__PlaySample_0);

__int16 __thiscall Mod__ISampleManager__PlaySample_0(ISampleManager *this, int a1, int pan, unsigned int volume, unsigned int a4, unsigned int a5, int a6)
{
  int v7; // edi
  unsigned int v8; // eax
  int v9; // ebp
  HSAMPLE sample; // esi
  unsigned __int8 vol; // [esp+14h] [ebp-4h]

  if ( !this->__driver )
  {
    return -1;
  }
  v7 = a4;
  v8 = a5;
  v9 = -1;
  vol = -1;
  if ( a5 < a4 )
  {
    a5 = a4;
    v8 = a4;
  }
  if ( a4 >= NUM_HANDLES || v8 >= NUM_HANDLES )
  {
    return -1;
  }
  if ( (signed __int16)a4 > (signed __int16)v8 )
  {
LABEL_11:
    if ( a6 != 1 && (v9 & 0x8000u) == 0 && (vol < volume || !a6) )
    {
      ISampleManager__PlaySample(this, this->__handles[(signed __int16)v9].sampleptr, v9, a1, 1u, pan, volume);
      return v9;
    }
    return -1;
  }
  while ( 1 )
  {
    sample = this->__handles[(signed __int16)v7].sampleptr;
    if ( !(AIL_sample_status(this->__handles[(signed __int16)v7].sampleptr) & 4) )
    {
      break;
    }
    if ( AIL_sample_volume(sample) < vol )
    {
      vol = AIL_sample_volume(sample);
      v9 = v7;
    }
    if ( (signed __int16)++v7 > (signed __int16)a5 )
    {
      goto LABEL_11;
    }
  }
  ISampleManager__PlaySample(this, sample, v7, a1, 1u, pan, volume);
  return v7;
}

// Custom implementation of function ISampleManager__setsamplevals
DETOUR(0x0046F330, 0x0046F390, _Mod__ISampleManager__setsamplevals);

void __thiscall Mod__ISampleManager__setsamplevals(ISampleManager *this, int handle_id, HSAMPLE sample, int pan, int volume)
{
  SampleData *handle; // esi

  if ( (signed __int16)handle_id < NUM_HANDLES && (handle_id & 0x8000u) == 0 )
  {
    handle = &this->__handles[(signed __int16)handle_id];
    ISampleManager__SetSampleData(this, handle_id, pan, volume, 0, 22050, 0, 0);
    AIL_set_sample_position(sample, handle->position);
    AIL_set_sample_pan(sample, handle->pan);
    AIL_set_sample_volume(sample, handle->volume);
  }
}

// Custom implementation of function ISampleManager__SetSampleData
DETOUR(0x0046F390, 0x0046F3D7, _Mod__ISampleManager__SetSampleData);

void __thiscall Mod__ISampleManager__SetSampleData(ISampleManager *this, int handle_id, int pan, int volume, int position, int rate, int a7, int a8)
{
  SampleData *handle; // eax

  (void)a7;
  if ( (signed __int16)handle_id < NUM_HANDLES && (handle_id & 0x8000u) == 0 )
  {
    handle = &this->__handles[(signed __int16)handle_id];
    handle->position = position;
    handle->pan = pan;
    handle->rate = rate;
    // New logic start
    // Record ticks the sound was played at
    handle->added_at_ticks = gGameTicks;
    // New logic end
    handle->field_14 = a8;
    handle->volume = volume;
  }
}

// Custom implementation of function ISampleManager__Deinit
DETOUR(0x0046F3E0, 0x0046F3F6, _Mod__ISampleManager__Deinit);

void __thiscall Mod__ISampleManager__Deinit(ISampleManager *this)
{
  ISampleManager *sm; // esi

  sm = this;
  if ( this->__driver )
  {
    ISampleManager__EndSamples(this);
    ISampleManager__Deallocate(sm);
  }
}

// Custom implementation of function ISampleManager__EndSamples
DETOUR(0x0046F400, 0x0046F41D, _Mod__ISampleManager__EndSamples);

void __thiscall Mod__ISampleManager__EndSamples(ISampleManager *this)
{
  ISampleManager *sm; // edi
  int handle_id; // esi

  sm = this;
  if ( this->__driver )
  {
    handle_id = 0;
    do
    {
      ISampleManager__EndSample(sm, handle_id++);
    }
    while ( (unsigned __int16)handle_id < NUM_HANDLES );
  }
}

// Custom implementation of function ISampleManager__EndSample
DETOUR(0x0046F420, 0x0046F449, _Mod__ISampleManager__EndSample);

void __thiscall Mod__ISampleManager__EndSample(ISampleManager *this, int handle_id)
{
  if ( (handle_id & 0x8000u) == 0 && (signed __int16)handle_id < NUM_HANDLES )
  {
    if ( this->__driver )
    {
      AIL_end_sample(this->__handles[(signed __int16)handle_id].sampleptr);
    }
  }
}

// Custom implementation of function ISampleManager__CopyStreamSample
DETOUR(0x0046FCC0, 0x0046FD8E, _Mod__ISampleManager__CopyStreamSample);

int __thiscall Mod__ISampleManager__CopyStreamSample(ISampleManager *this, FILE *file, void *src, void *dest, int size)
{
  char *dst; // ebx
  size_t v6; // eax
  size_t dsize; // esi
  char done; // [esp+13h] [ebp-9h]
  int datasize; // [esp+14h] [ebp-8h]
  ISampleManager *sm; // [esp+18h] [ebp-4h]

  sm = this;
  dst = (char *)dest;
  memset(dest, 0, size);
  done = 0;
  datasize = 0;
  do
  {
    if ( _ReadFile(src, 1u, 8u, file) )
    {
      if ( *((_DWORD *)src + 1) != 0xDEAF )
      {
        return datasize;                        // magic doesn't match
      }
      v6 = 0;
      dsize = *((unsigned __int16 *)src + 1);
      if ( *(unsigned __int16 *)src == (unsigned __int16)dsize )
      {
        _ReadFile(dst, 1u, dsize, file);
      }
      else
      {
        v6 = *(_WORD *)src;
        _ReadFile(src, 1u, v6, file);
        sm->__musicsosdata.lpDest = dst;
        sm->__musicsosdata.lpSource = (char *)src;
        sosCODECDecompressData(&sm->__musicsosdata, dsize);
      }
      dst += dsize;
      datasize += dsize;
      if ( datasize != size )
      {
        continue;
      }
    }
    done = 1;
  }
  while ( !done );
  return datasize;
}

// Custom implementation of function ISampleManager__InitStream
DETOUR(0x0046FD90, 0x0046FF2A, _Mod__ISampleManager__InitStream);

bool __thiscall Mod__ISampleManager__InitStream(ISampleManager *this, char *filename)
{
  ISampleManager *sm; // esi
  FILE *file; // edi
  LPVOID v5; // eax
  HDIGDRIVER v6; // ecx
  char *buffer; // ST00_4
  unsigned int v8; // edi
  int *v9; // ebx

  sm = this;
  if ( !_gSoundOn )
  {
    return 0;
  }
  if ( !this->__driver )
  {
    return 0;
  }
  file = _OpenFile(filename, "rb", gMUSIC_RES_PATH);
  if ( !file )
  {
    // New logic start
    // Allow playing music file also from GAMESFX folder
    char filename_buffer[260];
    sprintf(filename_buffer, "GameSFX\\%s", filename);
    file = _OpenFile(filename_buffer, "rb", gRES_PATH);
    if ( !file )
      return 0;
    // New logic end
  }
  if ( !sm->__musicfilesamplebuffer_1A0 )
  {
    sm->__musicfilesamplebuffer_1A0 = (char *)Alloc(0x18000, 0);
    v5 = Alloc(8192, 0);
    v6 = sm->__driver;
    sm->__musicbuffer_1A4 = v5;
    sm->__streamedsamplehandle_1B0 = AIL_allocate_sample_handle(v6);
  }
  if ( sm->__streamedsamplehandle_1B0 )
  {
    AIL_init_sample(sm->__streamedsamplehandle_1B0);
    AIL_set_sample_type(sm->__streamedsamplehandle_1B0, 1, 1u);
    AIL_set_sample_playback_rate(sm->__streamedsamplehandle_1B0, 22050);
    AIL_set_sample_position(sm->__streamedsamplehandle_1B0, 0);
    AIL_set_sample_pan(sm->__streamedsamplehandle_1B0, 64);
    AIL_set_sample_volume(sm->__streamedsamplehandle_1B0, 64);
    AIL_minimum_sample_buffer_size(sm->__driver, 22050, 1);
  }
  if ( sm->__musicfilehandle_1AC )
  {
    CloseFile(sm->__musicfilehandle_1AC);
  }
  buffer = sm->__musicfilesamplebuffer_1A0;
  sm->__musicfilehandle_1AC = file;
  sm->__musicsampleindex_1A8 = 0;
  _ReadFile(buffer, 1u, 0xCu, file);
  sm->__musicsosdata.wChannels = 1;
  sm->__musicsosdata.wBitSize = 16;
  sosCODECInitStream(&sm->__musicsosdata);
  v8 = 0;
  v9 = sm->__musicfilesamplesizes_1B4;
  do
  {
    *v9 = ISampleManager__CopyStreamSample(
            sm,
            sm->__musicfilehandle_1AC,
            sm->__musicbuffer_1A4,
            &sm->__musicfilesamplebuffer_1A0[v8],
            0x8000);
    v8 += 0x8000;
    ++v9;
  }
  while ( v8 < 0x18000 );
  return 1;
}

// Custom implementation of function ISampleManager__StreamLoop
DETOUR(0x0046FF30, 0x0047006B, _Mod__ISampleManager__StreamLoop);

void __thiscall Mod__ISampleManager__StreamLoop(ISampleManager *this)
{
  int buff_num; // eax

  if ( _gSoundOn )
  {
    if ( this->__driver )
    {
      if ( this->__streamedsamplehandle_1B0 )
      {
        if ( this->__musicfilehandle_1AC )
        {
          buff_num = AIL_sample_buffer_ready(this->__streamedsamplehandle_1B0);
          if ( buff_num != -1 )
          {
            AIL_load_sample_buffer(
              this->__streamedsamplehandle_1B0,
              buff_num,
              &this->__musicfilesamplebuffer_1A0[0x8000 * this->__musicsampleindex_1A8],
              0x8000u);
            this->__musicfilesamplesizes_1B4[this->__musicsampleindex_1A8] = ISampleManager__CopyStreamSample(
                                                                               this,
                                                                               this->__musicfilehandle_1AC,
                                                                               this->__musicbuffer_1A4,
                                                                               &this->__musicfilesamplebuffer_1A0[32768 * this->__musicsampleindex_1A8],
                                                                               32768);
            if ( !this->__musicfilesamplesizes_1B4[this->__musicsampleindex_1A8] )
            {
              // New logic start
              // Do not repeat the same song over and over
              if (RandomMusicEnabled && gGameState == GS_MAINLOOP)
              {
                this->__musicsampleindex_1A8 = (this->__musicsampleindex_1A8 + 1) % 3;
                PlayRandomMusic();
              }
              // New logic end
              SeekFile(this->__musicfilehandle_1AC, 0xC, 0);
              this->__musicfilesamplesizes_1B4[this->__musicsampleindex_1A8] = ISampleManager__CopyStreamSample(
                                                                                 this,
                                                                                 this->__musicfilehandle_1AC,
                                                                                 this->__musicbuffer_1A4,
                                                                                 &this->__musicfilesamplebuffer_1A0[32768 * this->__musicsampleindex_1A8],
                                                                                 32768);
              if ( gGameState == 2 && gGameType == GAME_CAMPAIGN )
              {
                if ( _musicbool_byte_5179D0 )
                {
                  _musicboolbyte_795600 = 1;
                }
              }
            }
            this->__musicsampleindex_1A8 = (this->__musicsampleindex_1A8 + 1) % 3u;
          }
        }
      }
    }
  }
}

// Custom implementation of function ISampleManager__CloseStream
DETOUR(0x00470070, 0x004700AD, _Mod__ISampleManager__CloseStream);

void __thiscall Mod__ISampleManager__CloseStream(ISampleManager *this)
{
  ISampleManager *sm; // esi
  HSAMPLE v2; // eax

  sm = this;
  if ( _gSoundOn )
  {
    if ( this->__driver )
    {
      if ( this->__musicfilehandle_1AC )
      {
        CloseFile(this->__musicfilehandle_1AC);
        v2 = sm->__streamedsamplehandle_1B0;
        sm->__musicfilehandle_1AC = 0;
        AIL_end_sample(v2);
      }
    }
  }
}

// Custom implementation of function ISampleManager__SetStreamVolume
DETOUR(0x004700B0, 0x004700DD, _Mod__ISampleManager__SetStreamVolume);

void __thiscall Mod__ISampleManager__SetStreamVolume(ISampleManager *this, int volume)
{
  if ( _gSoundOn )
  {
    if ( this->__driver )
    {
      if ( this->__musicfilehandle_1AC )
      {
        AIL_set_sample_volume(this->__streamedsamplehandle_1B0, volume);
      }
    }
  }
}

// Custom implementation of function IsSoundPlaying
DETOUR(0x0046F4F0, 0x0046F540, _Mod__IsSoundPlaying);

char Mod__IsSoundPlaying(int sound_id)
{
  ISampleManager *mgr; // eax
  unsigned int i; // esi
  bool bool1; // zf

  if ( !_gSoundOn )
  {
    return 0;
  }
  mgr = _gSampleMgr;
  i = 0;
  while ( 1 )
  {
    bool1 = (AIL_sample_status(mgr->__handles[i].sampleptr) & 4) == 0;
    mgr = _gSampleMgr;
    if ( !bool1 && sound_id == _gSampleMgr->__handles[i].__sound_id )
    {
      break;
    }
    ++i;
    if ( i >= NUM_HANDLES )
    {
      return 0;
    }
  }
  return 1;
}

// Custom implementation of function PlaySoundAt
DETOUR(0x0046F540, 0x0046F696, _Mod__PlaySoundAt);

void Mod__PlaySoundAt(int id, unsigned __int8 xpos, unsigned __int8 ypos)
{
  int xpos_; // ecx MAPDST
  int v4; // edi
  int v5; // ecx
  int v6; // ebp
  int v7; // ebx
  unsigned int v8; // eax
  int v9; // edx
  unsigned int vol; // edx
  int v11; // esi
  int pan; // [esp+10h] [ebp+8h]

  if ( _gSoundOn )
  {
    if ( id >= 0 )
    {
      // New logic start
      // Allow no more than 2 simultaneously playing instances of same sound at same tick, and overally according to maxSameSoundsPlaying rule
      int count_playing = 0;
      int count_playing_same_tick = 0;
      for (int i = 1; i < NUM_HANDLES; i++)
      {
        if ( (AIL_sample_status(_gSampleMgr->__handles[i].sampleptr) & 4) && (id == _gSampleMgr->__handles[i].__sound_id) )
        {
          count_playing++;
          if (_gSampleMgr->__handles[i].added_at_ticks == gGameTicks)
            count_playing_same_tick++;
        }
        if (count_playing == rulesExt__maxSameSoundsPlaying || count_playing_same_tick == 2)
          return;
      }
      // New logic end
      xpos_ = xpos;
      pan = 64;
      v4 = _ViewportWidth / 2 / 32;
      v5 = abs(v4 + _ViewportXPos / 32 - xpos_);
      v6 = _ViewportHeight / 2 / 32;
      v7 = abs(v6 + _ViewportYPos / 32 - ypos);
      if ( v5 > v4 )
      {
        v8 = (unsigned int)(v5 << 6) / gGameMap.width;
        v9 = 64 - v8;
        pan = 64 - v8;
        if ( 64 - v8 < 0x10 )
        {
          pan = 16;
          v9 = 16;
        }
        if ( xpos_ > v4 + _ViewportXPos / 32 )
        {
          pan = 128 - v9;
        }
      }
      vol = 50;
      v11 = v5;
      if ( (unsigned int)v5 <= (unsigned int)v7 )
      {
        v11 = v7;
      }
      if ( v5 > v4 || v7 > v6 )
      {
        if ( v11 >= gGameMap.width || (vol = 35 - (unsigned int)(35 * v11) / gGameMap.width, vol < 5) )
        {
          vol = 5;
        }
      }
      ISampleManager__PlaySample_0(_gSampleMgr, id, pan, gSFXVolume * vol / 100, 1u, NUM_HANDLES-1, 2);
    }
  }
}
