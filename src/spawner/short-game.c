#include <winsock2.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"

CALL(0x00449A8D, _CheckBuildingsLeft);

bool ShortGame = false;
bool MCVDeployed[8];

void CheckBuildingsLeft()
{
    if (ShortGame)
    {
        int playerCount = NetPlayerCount + gNetAIPlayers;
        for (int i = 0; i < playerCount; i++)
        {
            if (_gBuildingsExist[i]) MCVDeployed[i] = true;
            else if (_gUnitsExist[i] && MCVDeployed[i])
            {
                Side side = GetSide(i);
                if (side) CSide__BlowupAll_surrender(side);
                Map__PlayerDefeated(i);
            }
        }
    }
    Mission__CheckEvents();
}
