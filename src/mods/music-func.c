#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "ini.h"
#include "utils.h"

CALL(0x00448CB6, _FakeSound__PlayMusic); //SinglePlayer
CALL(0x00448D02, _FakeSound__PlayMusic); //MultiPlayer Skirmish

bool RandomMusicEnabled = false;

#define MUSIC_LIST_SIZE 128
static char MusicList[MUSIC_LIST_SIZE][128];
static int MusicListCount = 0;

static void FindMusicFiles();

void PlayRandomMusic()
{
    if (!MusicListCount) FindMusicFiles();
    if (MusicListCount) OpenAudioStream(MusicList[(unsigned int)RandSeed % MusicListCount]); // calling rand() is OOS'ing the game, use seed instead
}

void FakeSound__PlayMusic(char *fileName)
{
    char mapIniPath[256];
    sprintf(mapIniPath, ".\\%s%s", gGameType == GAME_CAMPAIGN ? gMISSIONS_RES_PATH : gMAPS_RES_PATH, PathChangeExtension(MissionMap, ".ini"));
    
    char customFile[256];
    IniGetString("Basic", "Music", gGameType == GAME_CAMPAIGN ? "" : "*", customFile, 256, mapIniPath);
    
    RandomMusicEnabled = strcmp(customFile, "*") == 0;
    if (RandomMusicEnabled) PlayRandomMusic();
    else if (strlen(customFile) > 0) OpenAudioStream(customFile);
    else OpenAudioStream(fileName);
}

static void FindMusicFiles()
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[256];
    sprintf(searchPath, "%s*.aud", gMUSIC_RES_PATH);
    hFind = FindFirstFile(searchPath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
      return;

    do
    {
      if (MusicListCount == MUSIC_LIST_SIZE)
        break;
      if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmpi(ffd.cFileName, "SCORE.AUD") && strcmpi(ffd.cFileName, "OPTIONS.AUD"))
        strncpy(MusicList[MusicListCount++], ffd.cFileName, 128);
    }
    while(FindNextFile(hFind, &ffd));

    FindClose(hFind);
}
