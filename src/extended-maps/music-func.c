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
    if (MusicListCount) Sound__PlayMusic(MusicList[(unsigned int)RandSeed % MusicListCount]); // calling rand() is OOS'ing the game, use seed instead
}

void FakeSound__PlayMusic(char *fileName)
{
    char mapIniPath[256];
    sprintf(mapIniPath, ".\\%s%s", GameType == GT_SINGLEPLAYER ? MissionsResourcePath : MapsResourcePath, PathChangeExtension(MissionMap, ".ini"));
    
    char customFile[256];
    IniGetString("Basic", "Music", GameType == GT_SINGLEPLAYER ? "" : "*", customFile, 256, mapIniPath);
    
    RandomMusicEnabled = strcmp(customFile, "*") == 0;
    if (RandomMusicEnabled) PlayRandomMusic();
    else if (strlen(customFile) > 0) Sound__PlayMusic(customFile);
    else Sound__PlayMusic(fileName);
}

static void FindMusicFiles()
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[256];
    sprintf(searchPath, "%s*.aud", MusicResourcePath);
    hFind = FindFirstFile(searchPath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    for (int i = 0; i < MUSIC_LIST_SIZE; i++)
    {
        MusicListCount = i + 1;
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) strncpy(MusicList[i], ffd.cFileName, 128);
        if (FindNextFile(hFind, &ffd) == 0) break;
    }
    FindClose(hFind);
}
