#include <windows.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "stats.h"
#include "macros/patch.h"
#include "../event-system/event-core.h"

char StatsDmp[] = ".\\stats.dmp";
char ProfilerDmp[] = ".\\event_profiler.txt";

static char StatsDmpBuffer[1024 * 20]; 
static unsigned short StatsDmpLength = 0;

static void WriteBool(char *id, bool value);
static void WriteString(char *id, char *value);
static void WriteUInt32(char *id, unsigned int value);
static void WriteUInt32Array(char *id, const void *arrayPtr, unsigned short length);
static void StatsAppend(const void *data, size_t num);
static void WritePlayerCredits();
static void WriteSpectators();
static void WriteUnitsOwned();
static void WriteBuildingsOwned();

void WriteStatsDmp(const void *buffer, int length)
{
    FILE *file = fopen(StatsDmp, "wb");
    if (file)
    {
        StatsDmpLength = (unsigned short)length - 2;
        memcpy(StatsDmpBuffer, buffer + 2, length - 2);
        
        WriteString("ACCN", gNetPlayerName);
        WriteUInt32("TICK", gGameTicks);
        WriteUInt32("UNIT", (unsigned int)gNetUnitCount);
        WriteUInt32("NUMP", gTotalPlayers);
        WriteUInt32("AIPL", gNetAIPlayers);
        WriteUInt32("ENDS", SpawnerGameEndState);
        WritePlayerCredits();
        WriteSpectators();
        WriteUnitsOwned();
        WriteBuildingsOwned();
        
        unsigned short len = htons(StatsDmpLength + 2);
        fwrite(&len, 1, 2, file);
        fwrite(StatsDmpBuffer, 1, StatsDmpLength, file);
        fclose(file);
    }

    if (DebugFeatures & DEBUGFEATURE_ENABLE_EVENT_PROFILING)
    {
      file = fopen(ProfilerDmp, "w");
      if (file)
      {
        fprintf(file, "Ticks = %d\n\n", gGameTicks);
        fprintf(file, "  ID    execs   filter\n");
        fprintf(file, "----------------------\n");
        for (int i = 0; i <= MAX_EVENTS; i++)
          fprintf(file, "%4d %8d %8d\n", i, profiler_executed_events[i], profiler_filter_check[i]);
        fclose(file);
      }
    }
}

int GetInfantryOwned(int side_id)
{
  CSide *side = GetSide(side_id);
  int result = 0;
  for (int i = 0; i < gUnitTypeNum; i++)
    if (_templates_unitattribs[i].__IsInfantry)
      result += side->__UnitsBuiltPerType[i];
  return result;
}

int GetHeavyVehiclesOwned(int side_id)
{
  CSide *side = GetSide(side_id);
  int result = 0;
  for (int i = 0; i < gUnitTypeNum; i++)
  {
    int behavior = _templates_unitattribs[i].__Behavior;
    if (!_templates_unitattribs[i].__IsInfantry && _templates_unitattribs[i].__CanCrush &&
        behavior != UnitBehavior_SANDWORM && behavior != UnitBehavior_CARRYALL && behavior != UnitBehavior_FRIGATE && behavior != UnitBehavior_ORNITHOPTER && behavior != UnitBehavior_DEATH_HAND)
      result += side->__UnitsBuiltPerType[i];
  }
  return result;
}

int GetLightVehiclesOwned(int side_id)
{
  CSide *side = GetSide(side_id);
  int result = 0;
  for (int i = 0; i < gUnitTypeNum; i++)
  {
    int behavior = _templates_unitattribs[i].__Behavior;
    if (!_templates_unitattribs[i].__IsInfantry && !_templates_unitattribs[i].__CanCrush &&
        behavior != UnitBehavior_SANDWORM && behavior != UnitBehavior_CARRYALL && behavior != UnitBehavior_FRIGATE && behavior != UnitBehavior_ORNITHOPTER && behavior != UnitBehavior_DEATH_HAND)
      result += side->__UnitsBuiltPerType[i];
  }
  return result;
}

int GetAirUnitsOwned(int side_id)
{
  CSide *side = GetSide(side_id);
  int result = 0;
  for (int i = 0; i < gUnitTypeNum; i++)
  {
    int behavior = _templates_unitattribs[i].__Behavior;
    if (behavior == UnitBehavior_CARRYALL || behavior == UnitBehavior_FRIGATE || behavior == UnitBehavior_ORNITHOPTER || behavior == UnitBehavior_DEATH_HAND)
      result += side->__UnitsBuiltPerType[i];
  }
  return result;
}

static void WritePlayerCredits()
{
    int playerCount = gTotalPlayers + gNetAIPlayers;
    for (int i = 0; i < playerCount; i++)
    {
        char id[5];
        sprintf(id, "CRD%d", i);
        CSide *side = GetSide(i);
        WriteUInt32(id, side->SpiceReal + side->CashReal);
    }
}

static void WriteSpectators()
{
    for (int i = 0; i < gTotalPlayers; i++)
    {
        char id[5];
        sprintf(id, "SPC%d", NetPlayersExt[i].house);
        WriteBool(id, NetPlayersExt[i].isSpectator);
    }
}

static void WriteUnitsOwned()
{
    int playerCount = gTotalPlayers + gNetAIPlayers;
    for (int i = 0; i < playerCount; i++)
    {
        char id[5];
        sprintf(id, "UNB%d", i);
        CSide *side = GetSide(i);
        WriteUInt32Array(id, &side->__UnitsBuiltPerType, sizeof(side->__UnitsBuiltPerType));
    }
}

static void WriteBuildingsOwned()
{
    int playerCount = gTotalPlayers + gNetAIPlayers;
    for (int i = 0; i < playerCount; i++)
    {
        char id[5];
        sprintf(id, "BLB%d", i);
        CSide *side = GetSide(i);
        WriteUInt32Array(id, &side->__BuildingsBuiltPerType, sizeof(side->__BuildingsBuiltPerType));
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
