#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"

// Add new rules for customized cost and build speed percentage for Easy and Hard difficulty

// Custom implementation of function GetDifficultyCostPercentage
DETOUR(0x004429B0, 0x004429FC, _Mod__GetDifficultyCostPercentage);
int Mod__GetDifficultyCostPercentage(unsigned char side_id)
{
  char difficulty;
  if ( _IsMultiplayer )
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
DETOUR(0x00442960, 0x004429AB, _Mod__GetDifficultyBuildSpeedPercentage);
int Mod__GetDifficultyBuildSpeedPercentage(unsigned char side_id)
{
  char difficulty;
  if ( _IsMultiplayer )
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
