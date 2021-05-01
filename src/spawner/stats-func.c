#include <windows.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "stats.h"
#include "macros/patch.h"

char StatsDmp[] = ".\\stats.dmp";
UnitTracker PlayersUnitsOwned[8];
BuildingTracker PlayersBuildingsOwned[8];

static char StatsDmpBuffer[1024 * 20]; 
static unsigned short StatsDmpLength = 0;

static void WriteBool(char *id, bool value);
static void WriteString(char *id, char *value);
static void WriteUInt32(char *id, unsigned int value);
static void WriteUInt32Array(char *id, const void *arrayPtr, unsigned short length);
static void StatsAppend(const void *data, size_t num);
static void WritePlayerCredits();
static void WriteUnitsOwned();
static void WriteBuildingsOwned();

void WriteStatsDmp(const void *buffer, int length)
{
    FILE *file = fopen(StatsDmp, "wb");
    if (file)
    {
        StatsDmpLength = (unsigned short)length - 2;
        memcpy(StatsDmpBuffer, buffer + 2, length - 2);
        
        WriteString("ACCN", NetPlayerName);
        WriteUInt32("TICK", GameTicks);
        WriteUInt32("UNIT", (unsigned int)NetUnitCount);
        WriteUInt32("NUMP", NetPlayerCount);
        WriteUInt32("AIPL", NetAIPlayers);
        WriteUInt32("ENDS", SpawnerGameEndState);
        WritePlayerCredits();
        WriteUnitsOwned();
        WriteBuildingsOwned();
        
        unsigned short len = htons(StatsDmpLength + 2);
        fwrite(&len, 1, 2, file);
        fwrite(StatsDmpBuffer, 1, StatsDmpLength, file);
        fclose(file);
    }
}

void StatsClear() // We have to call this when a game was restarted in skirmish mode
{
    memset(&PlayersUnitsOwned, 0, sizeof(PlayersUnitsOwned));
    memset(&PlayersBuildingsOwned, 0, sizeof(PlayersBuildingsOwned));
}

int GetInfantryOwned(int house)
{
    return PlayersUnitsOwned[house].LightInfantry + PlayersUnitsOwned[house].Trooper + PlayersUnitsOwned[house].Engineer + 
           PlayersUnitsOwned[house].ThumperInfantry + PlayersUnitsOwned[house].Sardaukar + 
           PlayersUnitsOwned[house].StealthFremen + PlayersUnitsOwned[house].Fremen + PlayersUnitsOwned[house].Saboteur +
           PlayersUnitsOwned[house].Sandworm + PlayersUnitsOwned[house].Grenadier + PlayersUnitsOwned[house].SardaukarMP;
}

int GetHeavyVehiclesOwned(int house)
{
    return PlayersUnitsOwned[house].Harvester + PlayersUnitsOwned[house].CombatTankAtreides + PlayersUnitsOwned[house].CombatTankHarkonnen +
           PlayersUnitsOwned[house].CombatTankOrdos + PlayersUnitsOwned[house].MCV + PlayersUnitsOwned[house].MissileTank + 
           PlayersUnitsOwned[house].Deviator + PlayersUnitsOwned[house].SiegeTank + PlayersUnitsOwned[house].SonicTank + 
           PlayersUnitsOwned[house].Devastator;
}

int GetLightVehiclesOwned(int house)
{
    return PlayersUnitsOwned[house].Trike + PlayersUnitsOwned[house].Raider + PlayersUnitsOwned[house].Quad + 
           PlayersUnitsOwned[house].StealthRaider;
}

int GetAirUnitsOwned(int house)
{
    return PlayersUnitsOwned[house].DeathHandRocket + PlayersUnitsOwned[house].ChoamFrigate + PlayersUnitsOwned[house].Carryall + 
           PlayersUnitsOwned[house].Carryall2 + PlayersUnitsOwned[house].Ornithopter;
}

static void WritePlayerCredits()
{
    int playerCount = NetPlayerCount + NetAIPlayers;
    for (int i = 0; i < playerCount; i++)
    {
        char id[5];
        sprintf(id, "CRD%d", i);
        Side side = Side__AsPointer(i);
        uint32_t *siloCredits = (void *)side + HC_SILO_CREDITS;
        uint32_t *credits = (void *)side + HC_CREDITS;
        WriteUInt32(id, *siloCredits + *credits);
    }
}

static void WriteUnitsOwned()
{
    int playerCount = NetPlayerCount + NetAIPlayers;
    for (int i = 0; i < playerCount; i++)
    {
        char id[5];
        sprintf(id, "UNB%d", i);
        WriteUInt32Array(id, &PlayersUnitsOwned[i], sizeof(UnitTracker));
    }
}

static void WriteBuildingsOwned()
{
    int playerCount = NetPlayerCount + NetAIPlayers;
    for (int i = 0; i < playerCount; i++)
    {
        char id[5];
        sprintf(id, "BLB%d", i);
        WriteUInt32Array(id, &PlayersBuildingsOwned[i], sizeof(BuildingTracker));
    }
}

static void WriteBool(char *id, bool value)
{
    StatsAppend(id, 4);
    
    unsigned short dataType = htons(2);
    StatsAppend(&dataType, 2);
        
    unsigned short length = htons(1);
    StatsAppend(&length, 2);
    
    StatsAppend(&value, 1);
    StatsDmpLength += 3;
}

static void WriteString(char *id, char *value)
{
    StatsAppend(id, 4);
    
    unsigned short dataType = htons(7);
    StatsAppend(&dataType, 2);
        
    unsigned short strlength = strlen(value) + 1;
    unsigned short length = htons(strlength);
    StatsAppend(&length, 2);
    
    StatsAppend(value, strlength);
    while ((StatsDmpLength + 2) % 4) StatsDmpLength++;
}

static void WriteUInt32(char *id, unsigned int value)
{
    StatsAppend(id, 4);
    
    unsigned short dataType = htons(6);
    StatsAppend(&dataType, 2);
        
    unsigned short length = htons(4);
    StatsAppend(&length, 2);
    
    value = htonl(value);
    StatsAppend(&value, 4);
}

static void WriteUInt32Array(char *id, const void *arrayPtr, unsigned short length)
{
    StatsAppend(id, 4);
    
    unsigned short dataType = htons(20);
    StatsAppend(&dataType, 2);
        
    unsigned short len = htons(length);
    StatsAppend(&len, 2);
    
    for (int i = 0; i < length; i = i + 4)
    {
        unsigned int *data = (void *)arrayPtr + i;
        unsigned int tempData = htonl(*data);
        StatsAppend(&tempData, 4);
    }
}

static void StatsAppend(const void *data, size_t num)
{
    memcpy(StatsDmpBuffer + StatsDmpLength, data, num);
    StatsDmpLength += num;
}
