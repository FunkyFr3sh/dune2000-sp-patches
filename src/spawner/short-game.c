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
        int playerCount = NetPlayerCount + NetAIPlayers;
        for (int i = 0; i < playerCount; i++)
        {
            if (gBuildingsExist[i]) MCVDeployed[i] = true;
            else if (gUnitsExist[i] && MCVDeployed[i])
            {
                Side side = Side__AsPointer(i);
                if (side) Side__BlowupAll(side);
                Map__PlayerDefeated(i);
            }
        }
    }
    Mission__CheckEvents();
}
