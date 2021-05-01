#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

void ExecuteCustomEvent(MissionEvent *event)
{
    switch(event->event_type)
    {
        case ET_LEAVE:
        {
            Side side = Side__AsPointer(event->player);
            if (side) Side__BlowupAll(side);
            //Map__PlayerDefeated(event->player);
            break;
        }
        case ET_PLAYSOUND:
        {
            Sound__PlaySample(event->value, 0, 0, 0);
            break;
        }
        case ET_PLAYMUSIC:
        {
            //abusing the units array to pass the file name
            if (strcmp(event->units, "*") == 0) PlayRandomMusic();
            else if (strlen(event->units) > 0) Sound__PlayMusic(event->units);
            break;
        }
    }
}
