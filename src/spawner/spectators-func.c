#include <windows.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "macros/patch.h"
#include "stats.h"

bool LiveStatsEnabled = true;
bool MeIsSpectator = false;

void DrawLiveStats(TImage *image)
{
    if (!SpawnerActive || !MeIsSpectator || !gLose || !LiveStatsEnabled || gGameType != GAME_NETWORK) return;
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

    for (int i = 0, column = 0; i < gTotalPlayers; i++)
    {
        if (IsSpectator(i)) continue;
        int x = 112 + (column++ * 60);
        char buffer[16];
        CSide *side = GetSide(i);
        row = 0;
        
        Graphlib__DrawTextWithBlackShadow(image, NetPlayerNamesArray[i], x, (rowHeight * row++) + y, 1, textColor);
        
        sprintf(buffer, "%d", GetInfantryOwned(i));
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", GetHeavyVehiclesOwned(i));
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        sprintf(buffer, "%d", GetLightVehiclesOwned(i));
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", side->__BuildingsBuilt);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", side->__UnitsKilled);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", side->__BuildingsKilled);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", side->__SpiceHarvested);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);

        sprintf(buffer, "%d", side->SpiceReal + side->CashReal);
        Graphlib__DrawTextWithBlackShadow(image, buffer, x, (rowHeight * row++) + y, 1, textColor);
        
        sprintf(buffer, "%d", side->__UnitsBuiltPerType[(int)_templates_GroupIDs.Frigate]);
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
