#include <winsock2.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "macros/patch.h"
#include "ini.h"
#include "utils.h"

CALL(0x00407ED2, _FakeLocalIpCheck);

unsigned int SpawnerMaxAhead = 150;
bool SpawnerActive = false;
LPCTSTR spawnIni = ".\\spawn.ini";
unsigned int ListenPort = DEFAULT_PORT;
NetPlayerExt NetPlayersExt[6];
extern SOCKET SendSocket;

static int MyIndex;

static void InitTunnel();
static void InitHolePunching();
#define GetInt(a, b, c) SpawnIniGetInt(a, b, c)
#define GetBool(a, b, c) SpawnIniGetBool(a, b, c)
#define GetString(a, b, c, d, e) SpawnIniGetString(a, b, c, d, e)

void InitSpawner()
{
    SpawnerActive = true;
    RandSeed = SpawnIniGetInt("Settings", "Seed", 1);

    GameSpeed = SpawnIniGetInt("Settings", "GameSpeed", GameSpeed);
    SpawnIniGetString("Settings", "Scenario", "2play1", NetMap, 50);
    strcpy(MissionMap, NetMap);
    SpawnIniGetString("Settings", "ScenarioName", NetMap, MapName, 60);
    WOLGameId = SpawnIniGetInt("Settings", "GameID", 0);
    P2Pheader = (WOLGameId << 8) | (WOLGameId >> (sizeof(WOLGameId) * CHAR_BIT - 8));

#ifdef WWDEBUG
    DebugMode = SpawnIniGetBool("Settings", "DebugMode", true);
#endif

    int mySideID = SpawnIniGetInt("Settings", "MySideID", -1);
    bool singlePlayer = (mySideID != -1);
    if (singlePlayer) // ### Single-Player Settings ###
    {
        MissionNumber = SpawnIniGetInt("Settings", "MissionNumber", 0);
        MySideID = mySideID;
        DifficultyLevel = SpawnIniGetInt("Settings", "DifficultyLevel", 1);
        // stats hack
        NetPlayerCount = 1;
        NetAIPlayers = 7;
    }
    else // ### Multi-Player and Skirmish Settings ###
    {
        NetUnitCount = SpawnIniGetInt("Settings", "UnitCount", 10);
        if (NetUnitCount > 10)
            NetUnitCount = 10;
        NetTechLevel = SpawnIniGetInt("Settings", "TechLevel", 7);
        NetStartingCredits = SpawnIniGetInt("Settings", "Credits", 7000);
        if (NetStartingCredits == 0)
            NetStartingCredits = 1;
        if (NetStartingCredits > 40000)
            NetStartingCredits = 40000;
        NetAIPlayers = SpawnIniGetInt("Settings", "AIPlayers", 1);
        NetCrates = SpawnIniGetBool("Settings", "Crates", false);
        NetWorms = SpawnIniGetInt("Settings", "Worms", 0);
        SpawnIniGetString("Settings", "Name", "NONAME", NetPlayerName, 20);
        NetPlayerSide = SpawnIniGetInt("Settings", "Side", 0);
        NetPlayerColor = SpawnIniGetInt("Settings", "Color", 0);
        NetPlayerHandicap = SpawnIniGetInt("Settings", "Handicap", 0);
        SpawnerMaxAhead = SpawnIniGetInt("Settings", "MaxAhead", SpawnerMaxAhead);

        // ### New Features ###
        InitMissionScript();

        DisableEngineer = SpawnIniGetBool("Settings", "DisableEngineer", DisableEngineer);
        DisableTurrets = SpawnIniGetBool("Settings", "DisableTurrets", DisableTurrets);
        NoCarryall = SpawnIniGetBool("Settings", "NoCarryall", NoCarryall);
        ShortGame = SpawnIniGetBool("Settings", "ShortGame", ShortGame);

        // ### Add the Players ###
        // we gotta make sure all players are stored in the same order and everyone knows the fake ip of each other
        MyIndex = SpawnIniGetInt("Settings", "MyIndex", 0);

        strcpy(LoginName, NetPlayerName);
        strcpy(NetPlayers[MyIndex].name, NetPlayerName);
        NetPlayers[MyIndex].unk = 0xFFFFFFFF;
        NetPlayers[MyIndex].color = NetPlayerColor;
        NetPlayers[MyIndex].side = NetPlayerSide;
        NetPlayers[MyIndex].handicap = NetPlayerHandicap;
        NetPlayers[MyIndex].ip = MyIndex + 1;
        NetPlayers[MyIndex].unk4 = 0x00000101;

        strcpy(NetPlayersExt[MyIndex].name, NetPlayerName);
        MeIsSpectator = SpawnIniGetBool("Settings", "IsSpectator", false);
        NetPlayersExt[MyIndex].isSpectator = MeIsSpectator;
        NetPlayersExt[MyIndex].startingLocation = SpawnIniGetInt("Settings", "StartingLocation", -1);
        NetPlayersExt[MyIndex].team = SpawnIniGetInt("Settings", "Team", -1);

        NetPlayerCount = 1;

        for (int i = 0; i < 6; i++)
        {
            if (strlen(NetPlayers[i].name))
                continue;

            char otherX[10];
            sprintf(otherX, "Other%d", NetPlayerCount);

            NetPlayers[i].unk = 0xFFFFFFFF;
            NetPlayers[i].unk4 = 0x00000101;
            NetPlayers[i].color = 9;
            NetPlayers[i].handicap = 1;

            SpawnIniGetString(otherX, "Name", "", NetPlayers[i].name, 20);
            if (strlen(NetPlayers[i].name))
            {
                NetPlayerCount++;
                NetPlayers[i].color = SpawnIniGetInt(otherX, "Color", 0);
                NetPlayers[i].side = SpawnIniGetInt(otherX, "Side", 0);
                NetPlayers[i].handicap = SpawnIniGetInt(otherX, "Handicap", 0);

                strcpy(NetPlayersExt[i].name, NetPlayers[i].name);
                NetPlayersExt[i].isSpectator = SpawnIniGetBool(otherX, "IsSpectator", false);
                NetPlayersExt[i].startingLocation = SpawnIniGetInt(otherX, "StartingLocation", -1);
                NetPlayersExt[i].team = SpawnIniGetInt(otherX, "Team", -1);

                char ip[20];
                SpawnIniGetString(otherX, "IP", "", ip, 20);
                AddressList[i].ip = StringToIntIp(ip);
                NetPlayers[i].ip = i + 1;

#ifndef WWDEBUG
                SpawnIniSetString(otherX, "IP", "");
#endif

                int port = SpawnIniGetInt(otherX, "Port", DEFAULT_PORT);
                AddressList[i].port = htons(port);
            }
        }
        InitAIPlayers();
    }

    // ### Fire it up! ###
    if (singlePlayer) // Single-Player
    {
        GameState = GS_MAINLOOP;
        CUIManager__LoadMission(gUIMgr, NetMap);
    }
    else if (NetPlayerCount > 1) // Multi-Player
    {
        ListenPort = SpawnIniGetInt("Settings", "Port", DEFAULT_PORT);
        TunnelId = htons(ListenPort);

        int tunnelPort = SpawnIniGetInt("Tunnel", "Port", 0);
        TunnelPort = htons(tunnelPort);
        if (TunnelPort)
            ListenPort = 0;

        char ip[20];
        SpawnIniGetString("Tunnel", "IP", "", ip, 20);
        TunnelIp = StringToIntIp(ip);

        SpawnIniSetString("Tunnel", "IP", "");

        char tempMap[128];
        sprintf(tempMap, ".\\%s%s.map", MapsResourcePath, NetMap);
        NetKey = GetCRC32(tempMap) + (uint16_t)WOLGameId;

        NetworkType = NT_UDP;
        GameType = GT_LAN;
        CUIManager__CreateWigNet();

        bool IsHost = SpawnIniGetBool("Settings", "Host", true);
        if (IsHost)
        {
            WOL__StartHostINetGame();
            if (TunnelPort)
                InitTunnel();
            else
                InitHolePunching();
        }
        else
            WOL__StartGuestINetGame();
    }
    else // Skirmish
    {
        DifficultyLevel = NetPlayerHandicap;

        NetworkType = NT_NONE;
        GameType = GT_SKIRMISH;
        CUIManager__StartGame(gUIMgr);
    }
}

void InitMissionScript()
{
    char mapScript[128];
    sprintf(mapScript, ".\\%s_%s.mis", MapsResourcePath, PathChangeExtension(MissionMap, ""));
    if (FileExists(mapScript))
    {
        MapScriptExists = true;
        strcpy(MapScript, PathChangeExtension(MissionMap, ""));
    }
    else
    {
        MapScriptExists = false;
        strcpy(MapScript, "spawn");
    }
}

void InitHouses()
{
    for (int i = 0; i < 6; i++)
        for (int x = 0; x < 6; x++)
        {
            if (strlen(NetPlayerNamesArray[i]) && strcmp(NetPlayerNamesArray[i], NetPlayersExt[x].name) == 0)
                NetPlayersExt[x].house = i;
        }
}

bool __stdcall FakeLocalIpCheck(char *Ip)
{
    if (!SpawnerActive)
        return IsLocalIp(Ip);

    char fakeIp[20];
    sprintf(fakeIp, "%d.0.0.0", MyIndex + 1);

    return (strcmp(Ip, fakeIp) == 0);
}

static void InitTunnel()
{
    static char TunnelBuf[20];
    static struct sockaddr_in TunnelAddr;
    TunnelAddr.sin_family = AF_INET;
    TunnelAddr.sin_port = TunnelPort;
    TunnelAddr.sin_addr.s_addr = TunnelIp;
    int len = sizeof(TunnelBuf);
    TunnelBuf[0] = (TunnelId >> (8 * 0)) & 0xff;
    TunnelBuf[1] = (TunnelId >> (8 * 1)) & 0xff;

    for (int i = 0; i < 5; i++)
        sendto(SendSocket, TunnelBuf, len, 0, (struct sockaddr *)&TunnelAddr, 16);
}

static void InitHolePunching()
{
    static struct sockaddr_in PlayerAddr;
    char buffer[1];
    for (int i = 0; i < 10; i++) // sending 10, but actually 1 should be enough
        for (int x = 0; x < 8; x++)
        {
            if (AddressList[x].ip)
            {
                PlayerAddr.sin_family = AF_INET;
                PlayerAddr.sin_addr.s_addr = AddressList[x].ip;
                PlayerAddr.sin_port = AddressList[x].port;
                sendto(SendSocket, buffer, 0, 0, (struct sockaddr *)&PlayerAddr, 16);
            }
        }
}

// ### Write ###
bool SpawnIniSetString(LPCTSTR section, LPCTSTR key, LPCTSTR value)
{
    return IniSetString(section, key, value, spawnIni);
}

bool SpawnIniSetBool(LPCTSTR section, LPCTSTR key, bool value)
{
    return IniSetBool(section, key, value, spawnIni);
}

bool SpawnIniSetInt(LPCTSTR section, LPCTSTR key, int value)
{
    return IniSetInt(section, key, value, spawnIni);
}

// ### Read ###
int SpawnIniGetString(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue, char *out, int length)
{
    return IniGetString(section, key, defaultValue, out, length, spawnIni);
}

bool SpawnIniGetBool(LPCTSTR section, LPCTSTR key, bool defaultValue)
{
    return IniGetBool(section, key, defaultValue, spawnIni);
}

int SpawnIniGetInt(LPCTSTR section, LPCTSTR key, int defaultValue)
{
    return IniGetInt(section, key, defaultValue, spawnIni);
}