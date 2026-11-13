#include "macros/patch.h"
#include "dune2000.h"

// Add possibility to play custom mentat sounds for extra houses other than Atr, Hark, Ord

// Custom implementation of function PlayMentatSound
DETOUR(0x0046F900, 0x0046F973, _Mod__PlayMentatSound);

void Mod__PlayMentatSound(int at_table, int or_table, int hark_table, char state, int time, int priority)
{
  char house_letter = '0';
  switch(_IRValues[(unsigned char)gSideId])
  {
    case 0: QueueAudioToPlay(at_table, state, time, priority); return;
    case 1: QueueAudioToPlay(hark_table, state, time, priority); return;
    case 2: QueueAudioToPlay(or_table, state, time, priority); return;
    case 3: house_letter = 'E'; break;
    case 4: house_letter = 'F'; break;
    case 5: house_letter = 'G'; break;
    case 6: house_letter = 'M'; break;
    case 7: house_letter = 'W'; break;
  }
  char sample_name[32];
  strcpy(sample_name, gSampleTable[at_table]->key);
  sample_name[0] = house_letter;
  int sample_id = GetSoundTableID(sample_name);
  QueueAudioToPlay(sample_id?sample_id:or_table, state, time, priority);
}
