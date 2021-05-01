#include <windows.h>
#include <stdint.h>
#include "patch.h"

#define FRAMES_PER_SECOND 60

void FPSLimiter()
{
    static uint32_t nextGameTick;
    if (!nextGameTick)
    {
        nextGameTick = fake_timeGetTime();
        return;
    }
    nextGameTick += 1000 / FRAMES_PER_SECOND;
    uint32_t tickCount = fake_timeGetTime();
    int sleepTime = nextGameTick - tickCount;
    if (sleepTime <= 0 || sleepTime > 1000 / FRAMES_PER_SECOND) nextGameTick = tickCount;
    else Sleep(sleepTime);
}
