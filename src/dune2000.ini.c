#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "hotkeys.h"
#include "ini.h"

CLEAR(0x00470917, 0x90, 0x00470931); //do not sleep 10seconds if dune2000.cfg does not exist

LPCTSTR dune2000Ini = ".\\dune2000.ini";

DETOUR(0x00478C20, 0x00478F9B, _LoadDune2000Ini);
void SaveDune2000Ini()
{
    D2kIniSetInt("MultiPlayer", "UnitCount", gNetUnitCount);
    D2kIniSetInt("MultiPlayer", "TechLevel", gNetTechLevel);
    D2kIniSetInt("MultiPlayer", "StartingCredits", gNetStartingCredits);
    D2kIniSetInt("MultiPlayer", "AIPlayers", gNetAIPlayers);
    D2kIniSetBool("MultiPlayer", "Crates", gNetCrates);
    D2kIniSetInt("MultiPlayer", "Worms", gNetWorms);
    D2kIniSetString("MultiPlayer", "PlayerName", gNetPlayerName);
    D2kIniSetInt("MultiPlayer", "PlayerSide", gNetPlayerSide);
    D2kIniSetInt("MultiPlayer", "PlayerColor", gNetPlayerColor);
    D2kIniSetInt("MultiPlayer", "PlayerHandicap", gNetPlayerHandicap);
    D2kIniSetString("MultiPlayer", "IPAddress", gNetIPAddress);
    D2kIniSetString("MultiPlayer", "ModemPhone", gNetModemPhone);
    D2kIniSetInt("MultiPlayer", "SerialComPort", gNetSerialComPort);

    D2kIniSetBool("Options", "FirstTimePlay", FirstTimePlay);
    D2kIniSetInt("Options", "ScrollRate", ScrollRate);
    D2kIniSetInt("Options", "GameSpeed", GameSpeed);
    D2kIniSetInt("Options", "GameBitsPerPixel", GameBitsPerPixel);
    D2kIniSetInt("Options", "SFXVolume", gSFXVolume);
    D2kIniSetInt("Options", "MusicVolume", MusicVolume);
    D2kIniSetBool("Options", "MoviesEnabled", MoviesEnabled);
    D2kIniSetBool("Options", "SoundsEnabled", SoundsEnabled);
    //D2kIniSetBool("Options", "VideoBackBuffer", VideoBackBuffer);
    D2kIniSetBool("Options", "ForceNoCD", ForceNoCD);
    if (!SpawnerActive) D2kIniSetInt("Options", "GameWidth", GameWidth);
    if (!SpawnerActive) D2kIniSetInt("Options", "GameHeight", GameHeight);
    D2kIniSetBool("Options", "SlowSideBarScrolling", SlowSideBarScrolling);
    D2kIniSetString("Options", "Language", GameLanguage);
    D2kIniSetBool("Options", "DisableMaxWindowedMode", DisableMaxWindowedMode);
    //D2kIniSetBool("Options", "UseImprovedFPSLimiter", UseImprovedFPSLimiter);
    D2kIniSetBool("Options", "SingleProcessorAffinity", SingleProcessorAffinity);
    D2kIniSetBool("Options", "CutsceneChangeResolution", CutsceneChangeResolution);
    D2kIniSetBool("Options", "UseHardwareCursor", UseHardwareCursor);
    
    D2kIniSetBool("WOL", "Microphone", Microphone);
    D2kIniSetBool("WOL", "AllowFindUser", AllowFindUser);
    D2kIniSetBool("WOL", "AllowPageUser", AllowPageUser);
    D2kIniSetBool("WOL", "LanguageFilter", LanguageFilter);
    D2kIniSetBool("WOL", "ShowAllGames", ShowAllGames);
    
    D2kIniSetInt("Hotkeys", "SelectAllUnits",           Hotkey_SelectAllUnits);
    D2kIniSetInt("Hotkeys", "SelectAllUnitsOfSameType", Hotkey_SelectAllUnitsOfSameType);
    D2kIniSetInt("Hotkeys", "Scatter",                  Hotkey_Scatter);
    D2kIniSetInt("Hotkeys", "CenterNextUnit",           Hotkey_CenterNextUnit);
    D2kIniSetInt("Hotkeys", "Repair",                   Hotkey_Repair);
    D2kIniSetInt("Hotkeys", "Sell",                     Hotkey_Sell);
    D2kIniSetInt("Hotkeys", "Guard",                    Hotkey_Guard);
    D2kIniSetInt("Hotkeys", "CenterBase",               Hotkey_CenterBase);
    D2kIniSetInt("Hotkeys", "Stop",                     Hotkey_Stop);
    D2kIniSetInt("Hotkeys", "Deploy",                   Hotkey_Deploy);
    D2kIniSetInt("Hotkeys", "BuildOrPlaceBuilding",     Hotkey_BuildOrPlaceBuilding);
    D2kIniSetInt("Hotkeys", "Bookmark1",                Hotkey_Bookmark[0]);
    D2kIniSetInt("Hotkeys", "Bookmark2",                Hotkey_Bookmark[1]);
    D2kIniSetInt("Hotkeys", "Bookmark3",                Hotkey_Bookmark[2]);
    D2kIniSetInt("Hotkeys", "Bookmark4",                Hotkey_Bookmark[3]);
    D2kIniSetInt("Hotkeys", "SidebarUp",                Hotkey_SidebarUp);
    D2kIniSetInt("Hotkeys", "SidebarDown",              Hotkey_SidebarDown);
    D2kIniSetInt("Hotkeys", "Alliance",                 Hotkey_Alliance);
    D2kIniSetInt("Hotkeys", "Retreat",                  Hotkey_Retreat);
    D2kIniSetInt("Hotkeys", "SelectPrimaryBuilding",    Hotkey_SelectPrimaryBuilding);
    D2kIniSetInt("Hotkeys", "OptionsMenu",              Hotkey_OptionsMenu);
    D2kIniSetInt("Hotkeys", "Team1",                    Hotkey_Team[1]);
    D2kIniSetInt("Hotkeys", "Team2",                    Hotkey_Team[2]);
    D2kIniSetInt("Hotkeys", "Team3",                    Hotkey_Team[3]);
    D2kIniSetInt("Hotkeys", "Team4",                    Hotkey_Team[4]);
    D2kIniSetInt("Hotkeys", "Team5",                    Hotkey_Team[5]);
    D2kIniSetInt("Hotkeys", "Team6",                    Hotkey_Team[6]);
    D2kIniSetInt("Hotkeys", "Team7",                    Hotkey_Team[7]);
    D2kIniSetInt("Hotkeys", "Team8",                    Hotkey_Team[8]);
    D2kIniSetInt("Hotkeys", "Team9",                    Hotkey_Team[9]);
    D2kIniSetInt("Hotkeys", "Team10",                   Hotkey_Team[0]);
    D2kIniSetInt("Hotkeys", "CTRL",                     Hotkey_CTRL);
    D2kIniSetInt("Hotkeys", "ALT",                      Hotkey_ALT);
    D2kIniSetInt("Hotkeys", "SHIFT",                    Hotkey_SHIFT);
    D2kIniSetInt("Hotkeys", "PrivateChat1",             Hotkey_PrivateChat[0]);
    D2kIniSetInt("Hotkeys", "PrivateChat2",             Hotkey_PrivateChat[1]);
    D2kIniSetInt("Hotkeys", "PrivateChat3",             Hotkey_PrivateChat[2]);
    D2kIniSetInt("Hotkeys", "PrivateChat4",             Hotkey_PrivateChat[3]);
    D2kIniSetInt("Hotkeys", "PrivateChat5",             Hotkey_PrivateChat[4]);
    D2kIniSetInt("Hotkeys", "PublicChat",               Hotkey_PublicChat);
    D2kIniSetInt("Hotkeys", "WOLPageReply",             Hotkey_WOLPageReply);
    D2kIniSetInt("Hotkeys", "WOLProposeDraw",           Hotkey_WOLProposeDraw);
    D2kIniSetInt("Hotkeys", "CenterSelectedUnits",      Hotkey_CenterSelectedUnits);
    D2kIniSetInt("Hotkeys", "ScrollLeft",               Hotkey_ScrollLeft);
    D2kIniSetInt("Hotkeys", "ScrollDown",               Hotkey_ScrollDown);
    D2kIniSetInt("Hotkeys", "ScrollRight",              Hotkey_ScrollRight);
    D2kIniSetInt("Hotkeys", "ScrollUp",                 Hotkey_ScrollUp);
    D2kIniSetInt("Hotkeys", "ToggleLiveStats",          Hotkey_ToggleLiveStats);
    D2kIniSetInt("Hotkeys", "PlayRandomSong",           Hotkey_PlayRandomSong);
}

void LoadDune2000Ini()
{
    gNetUnitCount = D2kIniGetInt("MultiPlayer", "UnitCount", 10);
    gNetTechLevel = D2kIniGetInt("MultiPlayer", "TechLevel", 7);
    gNetStartingCredits = D2kIniGetInt("MultiPlayer", "StartingCredits", 7000);
    gNetAIPlayers = D2kIniGetInt("MultiPlayer", "AIPlayers", 0);
    gNetCrates = D2kIniGetBool("MultiPlayer", "Crates", false);
    gNetWorms = D2kIniGetInt("MultiPlayer", "Worms", 0);
    D2kIniGetString("MultiPlayer", "PlayerName", "NONAME", gNetPlayerName, 100);
    gNetPlayerSide = D2kIniGetInt("MultiPlayer", "PlayerSide", 0);
    gNetPlayerColor = D2kIniGetInt("MultiPlayer", "PlayerColor", 0);
    gNetPlayerHandicap = D2kIniGetInt("MultiPlayer", "PlayerHandicap", 0);
    D2kIniGetString("MultiPlayer", "IPAddress", "", gNetIPAddress, 100);
    D2kIniGetString("MultiPlayer", "ModemPhone", "", gNetModemPhone, 100);
    gNetSerialComPort = D2kIniGetInt("MultiPlayer", "SerialComPort", 1);

    FirstTimePlay = D2kIniGetBool("Options", "FirstTimePlay", true);
    ScrollRate = D2kIniGetInt("Options", "ScrollRate", 20);
    GameSpeed = D2kIniGetInt("Options", "GameSpeed", 75);
    GameBitsPerPixel = D2kIniGetInt("Options", "GameBitsPerPixel", 16);
    if (GameBitsPerPixel != 16)
    {
        GameBitsPerPixel = 8;
        BitsPerPixelChanged = true;
    }
    gSFXVolume = D2kIniGetInt("Options", "SFXVolume", 100);
    MusicVolume = D2kIniGetInt("Options", "MusicVolume", 100);
    MoviesEnabled = D2kIniGetBool("Options", "MoviesEnabled", true);
    SoundsEnabled = D2kIniGetBool("Options", "SoundsEnabled", true);
    SlowSideBarScrolling = D2kIniGetBool("Options", "SlowSideBarScrolling", SlowSideBarScrolling);
    //if (VideoBackBuffer) VideoBackBuffer = D2kIniGetBool("Options", "VideoBackBuffer", true);
    
    if (!VideoBackBuffer)
        UseImprovedFPSLimiter = true;
    
    //UseImprovedFPSLimiter = D2kIniGetBool("Options", "UseImprovedFPSLimiter", UseImprovedFPSLimiter);
    if (UseImprovedFPSLimiter) timeBeginPeriod(4);
    SingleProcessorAffinity = D2kIniGetBool("Options", "SingleProcessorAffinity", SingleProcessorAffinity);
    if (SingleProcessorAffinity) SetSingleProcessorAffinity();
    
    //UseHardwareCursor = D2kIniGetBool("Options", "UseHardwareCursor", UseHardwareCursor);

    AllowFindUser = D2kIniGetBool("WOL", "AllowFindUser", true);
    AllowPageUser = D2kIniGetBool("WOL", "AllowPageUser", true);
    LanguageFilter = D2kIniGetBool("WOL", "LanguageFilter", false);
    ShowAllGames = D2kIniGetBool("WOL", "ShowAllGames", false);
    Microphone = D2kIniGetBool("WOL", "Microphone", false);
    
    Hotkey_SelectAllUnits =           D2kIniGetInt("Hotkeys", "SelectAllUnits",           Hotkey_SelectAllUnits);
    Hotkey_SelectAllUnitsOfSameType = D2kIniGetInt("Hotkeys", "SelectAllUnitsOfSameType", Hotkey_SelectAllUnitsOfSameType);
    Hotkey_Scatter =                  D2kIniGetInt("Hotkeys", "Scatter",                  Hotkey_Scatter);
    Hotkey_CenterNextUnit =           D2kIniGetInt("Hotkeys", "CenterNextUnit",           Hotkey_CenterNextUnit);
    Hotkey_Repair =                   D2kIniGetInt("Hotkeys", "Repair",                   Hotkey_Repair);
    Hotkey_Sell =                     D2kIniGetInt("Hotkeys", "Sell",                     Hotkey_Sell);
    Hotkey_Guard =                    D2kIniGetInt("Hotkeys", "Guard",                    Hotkey_Guard);
    Hotkey_CenterBase =               D2kIniGetInt("Hotkeys", "CenterBase",               Hotkey_CenterBase);
    Hotkey_Stop =                     D2kIniGetInt("Hotkeys", "Stop",                     Hotkey_Stop);
    Hotkey_Deploy =                   D2kIniGetInt("Hotkeys", "Deploy",                   Hotkey_Deploy);
    Hotkey_BuildOrPlaceBuilding =     D2kIniGetInt("Hotkeys", "BuildOrPlaceBuilding",     Hotkey_BuildOrPlaceBuilding);
    Hotkey_Bookmark[0] =              D2kIniGetInt("Hotkeys", "Bookmark1",                Hotkey_Bookmark[0]);
    Hotkey_Bookmark[1] =              D2kIniGetInt("Hotkeys", "Bookmark2",                Hotkey_Bookmark[1]);
    Hotkey_Bookmark[2] =              D2kIniGetInt("Hotkeys", "Bookmark3",                Hotkey_Bookmark[2]);
    Hotkey_Bookmark[3] =              D2kIniGetInt("Hotkeys", "Bookmark4",                Hotkey_Bookmark[3]);
    Hotkey_SidebarUp =                D2kIniGetInt("Hotkeys", "SidebarUp",                Hotkey_SidebarUp);
    Hotkey_SidebarDown =              D2kIniGetInt("Hotkeys", "SidebarDown",              Hotkey_SidebarDown);
    Hotkey_Alliance =                 D2kIniGetInt("Hotkeys", "Alliance",                 Hotkey_Alliance);
    Hotkey_Retreat =                  D2kIniGetInt("Hotkeys", "Retreat",                  Hotkey_Retreat);
    Hotkey_SelectPrimaryBuilding =    D2kIniGetInt("Hotkeys", "SelectPrimaryBuilding",    Hotkey_SelectPrimaryBuilding);
    Hotkey_OptionsMenu =              D2kIniGetInt("Hotkeys", "OptionsMenu",              Hotkey_OptionsMenu);
    Hotkey_Team[1] =                  D2kIniGetInt("Hotkeys", "Team1",                    Hotkey_Team[1]);
    Hotkey_Team[2] =                  D2kIniGetInt("Hotkeys", "Team2",                    Hotkey_Team[2]);
    Hotkey_Team[3] =                  D2kIniGetInt("Hotkeys", "Team3",                    Hotkey_Team[3]);
    Hotkey_Team[4] =                  D2kIniGetInt("Hotkeys", "Team4",                    Hotkey_Team[4]);
    Hotkey_Team[5] =                  D2kIniGetInt("Hotkeys", "Team5",                    Hotkey_Team[5]);
    Hotkey_Team[6] =                  D2kIniGetInt("Hotkeys", "Team6",                    Hotkey_Team[6]);
    Hotkey_Team[7] =                  D2kIniGetInt("Hotkeys", "Team7",                    Hotkey_Team[7]);
    Hotkey_Team[8] =                  D2kIniGetInt("Hotkeys", "Team8",                    Hotkey_Team[8]);
    Hotkey_Team[9] =                  D2kIniGetInt("Hotkeys", "Team9",                    Hotkey_Team[9]);
    Hotkey_Team[0] =                  D2kIniGetInt("Hotkeys", "Team10",                   Hotkey_Team[0]);
    Hotkey_CTRL =                     D2kIniGetInt("Hotkeys", "CTRL",                     Hotkey_CTRL);
    Hotkey_ALT =                      D2kIniGetInt("Hotkeys", "ALT",                      Hotkey_ALT);
    Hotkey_SHIFT =                    D2kIniGetInt("Hotkeys", "SHIFT",                    Hotkey_SHIFT);
    Hotkey_PrivateChat[0] =           D2kIniGetInt("Hotkeys", "PrivateChat1",             Hotkey_PrivateChat[0]);
    Hotkey_PrivateChat[1] =           D2kIniGetInt("Hotkeys", "PrivateChat2",             Hotkey_PrivateChat[1]);
    Hotkey_PrivateChat[2] =           D2kIniGetInt("Hotkeys", "PrivateChat3",             Hotkey_PrivateChat[2]);
    Hotkey_PrivateChat[3] =           D2kIniGetInt("Hotkeys", "PrivateChat4",             Hotkey_PrivateChat[3]);
    Hotkey_PrivateChat[4] =           D2kIniGetInt("Hotkeys", "PrivateChat5",             Hotkey_PrivateChat[4]);
    Hotkey_PublicChat =               D2kIniGetInt("Hotkeys", "PublicChat",               Hotkey_PublicChat);
    Hotkey_WOLPageReply =             D2kIniGetInt("Hotkeys", "WOLPageReply",             Hotkey_WOLPageReply);
    Hotkey_WOLProposeDraw =           D2kIniGetInt("Hotkeys", "WOLProposeDraw",           Hotkey_WOLProposeDraw);
    Hotkey_CenterSelectedUnits =      D2kIniGetInt("Hotkeys", "CenterSelectedUnits",      Hotkey_CenterSelectedUnits);
    Hotkey_ScrollLeft =               D2kIniGetInt("Hotkeys", "ScrollLeft",               Hotkey_ScrollLeft);
    Hotkey_ScrollDown =               D2kIniGetInt("Hotkeys", "ScrollDown",               Hotkey_ScrollDown);
    Hotkey_ScrollRight =              D2kIniGetInt("Hotkeys", "ScrollRight",              Hotkey_ScrollRight);
    Hotkey_ScrollUp =                 D2kIniGetInt("Hotkeys", "ScrollUp",                 Hotkey_ScrollUp);
    Hotkey_ToggleLiveStats =          D2kIniGetInt("Hotkeys", "ToggleLiveStats",          Hotkey_ToggleLiveStats);
    Hotkey_PlayRandomSong =           D2kIniGetInt("Hotkeys", "PlayRandomSong",           Hotkey_PlayRandomSong);
}


// ### Write ###
bool D2kIniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value)
{
    return IniSetString(section, key, value, dune2000Ini);
}

bool D2kIniSetBool(LPCTSTR section, LPCTSTR key, bool value)
{
    return IniSetBool(section, key, value, dune2000Ini);
}

bool D2kIniSetInt(LPCTSTR section, LPCTSTR key, int value)
{
    return IniSetInt(section, key, value, dune2000Ini);
}


// ### Read ###
int D2kIniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length)
{
    return IniGetString(section, key, defaultValue, out, length, dune2000Ini);
}

bool D2kIniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue)
{
    return IniGetBool(section, key, defaultValue, dune2000Ini);
}

int D2kIniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue)
{
    return IniGetInt(section, key, defaultValue, dune2000Ini);
}
