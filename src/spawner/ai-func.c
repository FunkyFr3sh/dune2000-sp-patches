#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"

CLEAR(0x00450006, 0x90, 0x0045006A);
CALL(0x00450006, _SetupAIPlayerCredits);
CALL(0x00469DE4, _SetupAIPlayerSides);

AIPlayer AIPlayers[8];

void InitAIPlayers()
{
    for (int i = 0; i < 8; i++)
    {
        char multiX[10];
        sprintf(multiX, "Multi%d", i+1);
            
        int difficulty = SpawnIniGetInt("HouseHandicaps", multiX, -1);
        switch (difficulty)
        {
            case 0: //hard
                AIPlayers[i].startingCredits = 40000;
                break;
            case 2: //easy
                AIPlayers[i].startingCredits = 4000;
                break;
            case 1: //Normal
            default:
                AIPlayers[i].startingCredits = NetStartingCredits;
                break;
        }
            
        AIPlayers[i].color = SpawnIniGetInt("HouseColors", multiX, -1);
        AIPlayers[i].side = SpawnIniGetInt("HouseCountries", multiX, -1);
    }
}

void SetupAIPlayerCredits()
{
    for (int i = 0; i < 8; i++)
    {
        NetPlayersTechLevel[i] = NetTechLevel;
        
        if (GameType == GT_SKIRMISH && i != MySideID && !SpawnerActive) //Original Code
        {
            switch(DifficultyLevel)
            {
                case 0:
                    NetPlayersStartingCredits[i] = 7000;
                    break;
                case 1:
                    NetPlayersStartingCredits[i] = 10500;
                    break;
                case 2:
                    NetPlayersStartingCredits[i] = 14000;
                    break;
                default:
                    NetPlayersStartingCredits[i] = NetStartingCredits;
                    break;
            }
        } 
        else if (i >= NetPlayerCount && SpawnerActive) //Setup starting credits based on spawner handicap setting (Only for AI Players)
            NetPlayersStartingCredits[i] = AIPlayers[i].startingCredits;
        else NetPlayersStartingCredits[i] = NetStartingCredits;
    }
}

int SetupAIPlayerSides()
{
    static int currentAI = 0;
    int side = rand();
    if (!SpawnerActive) return side;
    
    if (currentAI == 0) currentAI = NetPlayerCount; //ai players are added after the humans, take the highest human house+1 to get the first ai house
    if (AIPlayers[currentAI].side != -1) side = AIPlayers[currentAI].side;
    
    if (currentAI - (NetPlayerCount - 1) == NetAIPlayers) currentAI = 0; //set it back to 0 once we reached the last ai player
    else currentAI++;
    return side;
}

void SetupAIPlayerColors()
{
    if (!SpawnerActive) return;
    
    for (int i = 0; i < 8; i++)
    {
        if (AIPlayers[i].color != -1) NetPlayersColor[i*0x3c] = AIPlayers[i].color;
    }
}
