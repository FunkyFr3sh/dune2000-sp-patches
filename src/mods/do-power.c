#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "rules.h"

// Custom implementation of function DoPower
DETOUR(0x0044AAE0, 0x0044ABD9, _Mod__DoPower);

void Mod__DoPower()
{
  unsigned int side_id; // esi
  CSide *side; // eax
  CSide *my_side; // eax MAPDST
  unsigned int prev_power; // eax
  int sound_id; // eax MAPDST

  side_id = 0;
  do
  {
    side = GetSide(side_id);
    // New logic start
    // Debug features
    if (DebugFeatures & DEBUGFEATURE_UNLIMITED_POWER && side_id == gSideId)
      side->__PowerPercent = 200;
    else
      CSide__CalculatePower(side);
    // New logic end
    ++side_id;
  }
  while ( side_id < 8 );
  my_side = GetSide(gSideId);
  prev_power = my_side->__PowerPercentPrev;
  if ( prev_power != 0xFFFF )
  {
    if ( my_side->__PowerPercent < 100u && prev_power >= 100 )
    {
      sound_id = GetSoundTableID("S_POWERDOWN");
      QueueAudioToPlay(sound_id, 0, 0, 0);
    }
    if ( my_side->__PowerPercent >= 100u )
    {
      if ( (unsigned)my_side->__PowerPercentPrev < 100u )
      {
        // New logic start
        // Always show radar map if rule alwaysShowRadar is set to true
        // Play S_POWERUP sound instead of S_RADARONLINE sound
        if ( !IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_OUTPOST) || rulesExt__alwaysShowRadar )
        // New logic end
        {
          sound_id = GetSoundTableID("S_POWERUP");
          QueueAudioToPlay(sound_id, 0, 0, 0);
          return;
        }
LABEL_13:
        sound_id = GetSoundTableID("S_RADARONLINE");
        QueueAudioToPlay(sound_id, 0, 0, 0);
        _TacticalData.__RadarOnline = 1;
        return;
      }
      // New logic start
      // Always show radar map if rule alwaysShowRadar is set to true
      // Do not play S_RADARONLINE sound when radar is built
      if ( IsBuildingWithBehaviorBuilt(gSideId, BuildingBehavior_OUTPOST) && !rulesExt__alwaysShowRadar )
      // New logic end
      {
        if ( _TacticalData.__RadarOnline )
        {
          return;
        }
        goto LABEL_13;
      }
      _TacticalData.__RadarOnline = 0;
    }
  }
}
