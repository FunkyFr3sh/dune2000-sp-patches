#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"

// Add new rules for customized cost and build speed percentage for Easy and Hard difficulty

// Custom implementation of function GetDifficultyCostPercentage
CALL(0x00442BB5, _Mod__GetDifficultyCostPercentage); // w__GetUnitCost
CALL(0x00442BE6, _Mod__GetDifficultyCostPercentage); // GetBuildingCost

int Mod__GetDifficultyCostPercentage(unsigned char side_id)
{
  char difficulty;
  if ( _canQueue_IsMultiplayer )
  {
    difficulty = _OrderData[side_id].c_field_36;
  }
  else
  {
    difficulty = gDifficultyLevel;
    if ( side_id != gSideId )
    {
      difficulty = 1;
    }
  }
  switch (difficulty)
  {
    case 0: return rulesExt__costPercentageEasy;
    case 2: return rulesExt__costPercentageHard;
    default: return 100;
  }
}


// Custom implementation of function GetDifficultyBuildSpeedPercentage
CALL(0x00442A52, _Mod__GetDifficultyBuildSpeedPercentage); // GetUnitBuildSpeedPercentage
CALL(0x00442B91, _Mod__GetDifficultyBuildSpeedPercentage); // GetBuildingBuildSpeedPercentage

int Mod__GetDifficultyBuildSpeedPercentage(unsigned char side_id)
{
  char difficulty;
  if ( _canQueue_IsMultiplayer )
  {
    difficulty = _OrderData[side_id].c_field_36;
  }
  else
  {
    difficulty = gDifficultyLevel;
    if ( side_id != gSideId )
    {
      difficulty = 1;
    }
  }
  switch (difficulty)
  {
    case 0: return rulesExt__buildSpeedPercentageEasy;
    case 2: return rulesExt__buildSpeedPercentageHard;
    default: return 100;
  }
}
