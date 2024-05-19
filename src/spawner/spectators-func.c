#include <windows.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "macros/patch.h"
#include "stats.h"

CLEAR(0x00428DA7, 0xCC, 0x00428DAD); //do not draw "Mission Failed" text
LJMP(0x00428DA7, 0x00428DE3); //do not draw "Mission Failed" text

bool LiveStatsEnabled = true;
bool MeIsSpectator = false;

void DrawLiveStats(int *image)
{
    if (!SpawnerActive || !MeIsSpectator || !Lose || !LiveStatsEnabled || GameType != GT_LAN) return;
    int y = GameHeight - 250;
    const int textColor = 0;
    const int rowHeight = 20;
    
    int row = 1;
    Graphlib__DrawTextWithBlackShadow(image, "Infantry Owned", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "H. Vehicles Owned", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "L. Vehicles Owned", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "Buildings Owned", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "Units Killed", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "Buildings Destroyed", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "Money Harvested", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "Credits", 3, (rowHeight * row++) + y, 1, textColor);
    Graphlib__DrawTextWithBlackShadow(image, "Starport Deliveries", 3, (rowHeight * row++) + y, 1, textColor);
    
    Graphlib__DrawTextWithBlackShadow(image, "Press 'Tab' to toggle live stats", 3, (rowHeight * ++row) + y, 1, textColor);

    for (int i = 0, column = 0; i < NetPlayerCount; i++)
    {
        if (IsSpectator(i)) continue;
        int x = 112 + (column++ * 60);
        char buffer[16];
        Side side = Side__AsPointer(i);
        row = 0;
        
        Graphlib__DrawTextWithBlackShadow(image, NetPlayerNamesArray[i], x, (rowHeight * row++) + y, 1, textColor);
        
        sprintf(buffer, "%d", GetInfantryOwned(i));
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", GetHeavyVehiclesOwned(i));
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        sprintf(buffer, "%d", GetLightVehiclesOwned(i));
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        int *buildingsOwned = (void *)side + HC_BUILDINGS_OWNED;
        sprintf(buffer, "%d", *buildingsOwned);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        int *unitskilled = (void *)side + HC_UNITS_KILLED;
        sprintf(buffer, "%d", *unitskilled);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        int *buildingsDestroyed = (void *)side + HC_BUILDINGS_DESTROYED;
        sprintf(buffer, "%d", *buildingsDestroyed);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        int *spiceHarvested = (void *)side + HC_SPICE_HARVESTED;
        sprintf(buffer, "%d", *spiceHarvested);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        int *siloCredits = (void *)side + HC_SILO_CREDITS;
        int *credits = (void *)side + HC_CREDITS;
        sprintf(buffer, "%d", *siloCredits + *credits);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        sprintf(buffer, "%d", PlayersUnitsOwned[i].ChoamFrigate);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
    }
}

bool IsSpectator(uint8_t sideId)
{
    for (int i = 0; i < 6; i++)
    {
        if (strlen(NetPlayersExt[i].name) == 0) break; //reached last player
        if (NetPlayersExt[i].house == sideId) return NetPlayersExt[i].isSpectator;
    }
    return false;
}
