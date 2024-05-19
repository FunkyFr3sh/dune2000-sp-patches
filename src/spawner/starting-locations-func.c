#include <stdbool.h>
#include "dune2000.h"
#include "patch.h"

int GetStartingLocation(int house)
{
    for (int i = 0; i < 6; i++)
    {
        if (strlen(NetPlayersExt[i].name) == 0) break; //reached last player
        if (NetPlayersExt[i].house == house) return NetPlayersExt[i].startingLocation;
    }
    return -1;
}

int GetFreeStartingLocation(int currentLocation)
{
    bool locationUsed[8];
    for (int i = 0; i < 8; i++)
    {
        locationUsed[i] = SpawnLocationUsedBoolArray[i];
    }
    for (int i = 0; i < 6; i++)
    {
        int start = NetPlayersExt[i].startingLocation;
        if (start != -1 && strlen(NetPlayersExt[i].name) > 0) locationUsed[start] = true;
    }

    if (!locationUsed[currentLocation]) return currentLocation;
    for (int i = 0; i < 8; i++)
    {
        if (!locationUsed[i]) return i;
    }
    return currentLocation;
}
