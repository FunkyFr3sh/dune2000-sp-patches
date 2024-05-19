#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "hotkeys.h"
#include "ini.h"

CALL(0x00470A68, _LoadDune2000Ini);

CLEAR(0x00470917, 0x90, 0x00470931); //do not sleep 10seconds if dune2000.cfg does not exist

LPCTSTR dune2000Ini = ".\\dune2000.ini";

void SaveDune2000Ini()
{
    D2kIniSetInt("MultiPlayer", "UnitCount", NetUnitCount);
    D2kIniSetInt("MultiPlayer", "TechLevel", NetTechLevel);
    D2kIniSetInt("MultiPlayer", "StartingCredits", NetStartingCredits);
    D2kIniSetInt("MultiPlayer", "AIPlayers", NetAIPlayers);
    D2kIniSetBool("MultiPlayer", "Crates", NetCrates);
    D2kIniSetInt("MultiPlayer", "Worms", NetWorms);
    D2kIniSetString("MultiPlayer", "PlayerName", NetPlayerName);
    D2kIniSetInt("MultiPlayer", "PlayerSide", NetPlayerSide);
    D2kIniSetInt("MultiPlayer", "PlayerColor", NetPlayerColor);
    D2kIniSetInt("MultiPlayer", "PlayerHandicap", NetPlayerHandicap);
    D2kIniSetString("MultiPlayer", "IPAddress", NetIPAddress);
    D2kIniSetString("MultiPlayer", "ModemPhone", NetModemPhone);
    D2kIniSetInt("MultiPlayer", "SerialComPort", NetSerialComPort);

    D2kIniSetBool("Options", "FirstTimePlay", FirstTimePlay);
    D2kIniSetInt("Options", "ScrollRate", ScrollRate);
    D2kIniSetInt("Options", "GameSpeed", GameSpeed);
    D2kIniSetInt("Options", "GameBitsPerPixel", GameBitsPerPixel);
    D2kIniSetInt("Options", "SFXVolume", SFXVolume);
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
    
    D2kIniSetInt("Hotkeys", "SelectAllUnits", SelectAllUnits);
    D2kIniSetInt("Hotkeys", "Scatter", Scatter);
    D2kIniSetInt("Hotkeys", "CenterNextUnit", CenterNextUnit);
    D2kIniSetInt("Hotkeys", "Repair", Repair);
    D2kIniSetInt("Hotkeys", "Sell", Sell);
    D2kIniSetInt("Hotkeys", "Guard", Guard);
    D2kIniSetInt("Hotkeys", "CenterBase", CenterBase);
    D2kIniSetInt("Hotkeys", "Stop", Stop);
    D2kIniSetInt("Hotkeys", "Bookmark1", Bookmark1);
    D2kIniSetInt("Hotkeys", "Bookmark2", Bookmark2);
    D2kIniSetInt("Hotkeys", "Bookmark3", Bookmark3);
    D2kIniSetInt("Hotkeys", "Bookmark4", Bookmark4);
    D2kIniSetInt("Hotkeys", "SidebarUp", SidebarUp);
    D2kIniSetInt("Hotkeys", "SidebarDown", SidebarDown);
    D2kIniSetInt("Hotkeys", "Alliance", Alliance);
    D2kIniSetInt("Hotkeys", "Retreat", Retreat);
    D2kIniSetInt("Hotkeys", "SelectPrimaryBuilding", SelectPrimaryBuilding);
    D2kIniSetInt("Hotkeys", "OptionsMenu", OptionsMenu);
    D2kIniSetInt("Hotkeys", "Team1", Team1);
    D2kIniSetInt("Hotkeys", "Team2", Team2);
    D2kIniSetInt("Hotkeys", "Team3", Team3);
    D2kIniSetInt("Hotkeys", "Team4", Team4);
    D2kIniSetInt("Hotkeys", "Team5", Team5);
    D2kIniSetInt("Hotkeys", "Team6", Team6);
    D2kIniSetInt("Hotkeys", "Team7", Team7);
    D2kIniSetInt("Hotkeys", "Team8", Team8);
    D2kIniSetInt("Hotkeys", "Team9", Team9);
    D2kIniSetInt("Hotkeys", "Team10", Team10);
    D2kIniSetInt("Hotkeys", "CTRL", CTRL);
    D2kIniSetInt("Hotkeys", "ALT", ALT);
    D2kIniSetInt("Hotkeys", "SHIFT", SHIFT);
    D2kIniSetInt("Hotkeys", "PrivateChat1", PrivateChat1);
    D2kIniSetInt("Hotkeys", "PrivateChat2", PrivateChat2);
    D2kIniSetInt("Hotkeys", "PrivateChat3", PrivateChat3);
    D2kIniSetInt("Hotkeys", "PrivateChat4", PrivateChat4);
    D2kIniSetInt("Hotkeys", "PrivateChat5", PrivateChat5);
    D2kIniSetInt("Hotkeys", "PublicChat", PublicChat);
    D2kIniSetInt("Hotkeys", "WOLPageReply", WOLPageReply);
    D2kIniSetInt("Hotkeys", "WOLProposeDraw", WOLProposeDraw);
    D2kIniSetInt("Hotkeys", "CenterSelectedUnits", CenterSelectedUnits);
    D2kIniSetInt("Hotkeys", "ScrollLeft", ScrollLeft);
    D2kIniSetInt("Hotkeys", "ScrollDown", ScrollDown);
    D2kIniSetInt("Hotkeys", "ScrollRight", ScrollRight);
    D2kIniSetInt("Hotkeys", "ScrollUp", ScrollUp);
    D2kIniSetInt("Hotkeys", "ToggleLiveStats", ToggleLiveStats);
    D2kIniSetInt("Hotkeys", "PlayRandomSong", PlayRandomSong);
}

void LoadDune2000Ini()
{
    NetUnitCount = D2kIniGetInt("MultiPlayer", "UnitCount", 10);
    NetTechLevel = D2kIniGetInt("MultiPlayer", "TechLevel", 7);
    NetStartingCredits = D2kIniGetInt("MultiPlayer", "StartingCredits", 7000);
    NetAIPlayers = D2kIniGetInt("MultiPlayer", "AIPlayers", 0);
    NetCrates = D2kIniGetBool("MultiPlayer", "Crates", false);
    NetWorms = D2kIniGetInt("MultiPlayer", "Worms", 0);
    D2kIniGetString("MultiPlayer", "PlayerName", "NONAME", NetPlayerName, 100);
    NetPlayerSide = D2kIniGetInt("MultiPlayer", "PlayerSide", 0);
    NetPlayerColor = D2kIniGetInt("MultiPlayer", "PlayerColor", 0);
    NetPlayerHandicap = D2kIniGetInt("MultiPlayer", "PlayerHandicap", 0);
    D2kIniGetString("MultiPlayer", "IPAddress", "", NetIPAddress, 100);
    D2kIniGetString("MultiPlayer", "ModemPhone", "", NetModemPhone, 100);
    NetSerialComPort = D2kIniGetInt("MultiPlayer", "SerialComPort", 1);

    FirstTimePlay = D2kIniGetBool("Options", "FirstTimePlay", true);
    ScrollRate = D2kIniGetInt("Options", "ScrollRate", 20);
    GameSpeed = D2kIniGetInt("Options", "GameSpeed", 75);
    GameBitsPerPixel = D2kIniGetInt("Options", "GameBitsPerPixel", 16);
    if (GameBitsPerPixel != 16)
    {
        GameBitsPerPixel = 8;
        BitsPerPixelChanged = true;
    }
    SFXVolume = D2kIniGetInt("Options", "SFXVolume", 100);
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
    
    SelectAllUnits = D2kIniGetInt("Hotkeys", "SelectAllUnits", SelectAllUnits);
    Scatter = D2kIniGetInt("Hotkeys", "Scatter", Scatter);
    CenterNextUnit = D2kIniGetInt("Hotkeys", "CenterNextUnit", CenterNextUnit);
    Repair = D2kIniGetInt("Hotkeys", "Repair", Repair);
    Sell = D2kIniGetInt("Hotkeys", "Sell", Sell);
    Guard = D2kIniGetInt("Hotkeys", "Guard", Guard);
    CenterBase = D2kIniGetInt("Hotkeys", "CenterBase", CenterBase);
    Stop = D2kIniGetInt("Hotkeys", "Stop", Stop);
    Bookmark1 = D2kIniGetInt("Hotkeys", "Bookmark1", Bookmark1);
    Bookmark2 = D2kIniGetInt("Hotkeys", "Bookmark2", Bookmark2);
    Bookmark3 = D2kIniGetInt("Hotkeys", "Bookmark3", Bookmark3);
    Bookmark4 = D2kIniGetInt("Hotkeys", "Bookmark4", Bookmark4);
    SidebarUp = D2kIniGetInt("Hotkeys", "SidebarUp", SidebarUp);
    SidebarDown = D2kIniGetInt("Hotkeys", "SidebarDown", SidebarDown);
    Alliance = D2kIniGetInt("Hotkeys", "Alliance", Alliance);
    Retreat = D2kIniGetInt("Hotkeys", "Retreat", Retreat);
    SelectPrimaryBuilding = D2kIniGetInt("Hotkeys", "SelectPrimaryBuilding", SelectPrimaryBuilding);
    OptionsMenu = D2kIniGetInt("Hotkeys", "OptionsMenu", OptionsMenu);
    Team1 = D2kIniGetInt("Hotkeys", "Team1", Team1);
    Team2 = D2kIniGetInt("Hotkeys", "Team2", Team2);
    Team3 = D2kIniGetInt("Hotkeys", "Team3", Team3);
    Team4 = D2kIniGetInt("Hotkeys", "Team4", Team4);
    Team5 = D2kIniGetInt("Hotkeys", "Team5", Team5);
    Team6 = D2kIniGetInt("Hotkeys", "Team6", Team6);
    Team7 = D2kIniGetInt("Hotkeys", "Team7", Team7);
    Team8 = D2kIniGetInt("Hotkeys", "Team8", Team8);
    Team9 = D2kIniGetInt("Hotkeys", "Team9", Team9);
    Team10 = D2kIniGetInt("Hotkeys", "Team10", Team10);
    CTRL = D2kIniGetInt("Hotkeys", "CTRL", CTRL);
    ALT = D2kIniGetInt("Hotkeys", "ALT", ALT);
    SHIFT = D2kIniGetInt("Hotkeys", "SHIFT", SHIFT);
    PrivateChat1 = D2kIniGetInt("Hotkeys", "PrivateChat1", PrivateChat1);
    PrivateChat2 = D2kIniGetInt("Hotkeys", "PrivateChat2", PrivateChat2);
    PrivateChat3 = D2kIniGetInt("Hotkeys", "PrivateChat3", PrivateChat3);
    PrivateChat4 = D2kIniGetInt("Hotkeys", "PrivateChat4", PrivateChat4);
    PrivateChat5 = D2kIniGetInt("Hotkeys", "PrivateChat5", PrivateChat5);
    PublicChat = D2kIniGetInt("Hotkeys", "PublicChat", PublicChat);
    WOLPageReply = D2kIniGetInt("Hotkeys", "WOLPageReply", WOLPageReply);
    WOLProposeDraw = D2kIniGetInt("Hotkeys", "WOLProposeDraw", WOLProposeDraw);
    CenterSelectedUnits = D2kIniGetInt("Hotkeys", "CenterSelectedUnits", CenterSelectedUnits);
    ScrollLeft = D2kIniGetInt("Hotkeys", "ScrollLeft", ScrollLeft);
    ScrollDown = D2kIniGetInt("Hotkeys", "ScrollDown", ScrollDown);
    ScrollRight = D2kIniGetInt("Hotkeys", "ScrollRight", ScrollRight);
    ScrollUp = D2kIniGetInt("Hotkeys", "ScrollUp", ScrollUp);
    ToggleLiveStats = D2kIniGetInt("Hotkeys", "ToggleLiveStats", ToggleLiveStats);
    PlayRandomSong = D2kIniGetInt("Hotkeys", "PlayRandomSong", PlayRandomSong);
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