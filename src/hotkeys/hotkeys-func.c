#include "hotkeys.h"
#include "patch.h"
#include "dune2000.h"

//http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

int SelectAllUnits = 'E';
int Scatter = 'X';
int CenterNextUnit = 'N';
int Repair = 'T';
int Sell = 'Y';
int Guard = 'G';
int CenterBase = 'H';
int Stop = 'S';
int Bookmark1 = VK_F9;
int Bookmark2 = VK_F10;
int Bookmark3 = VK_F11;
int Bookmark4 = VK_F12;
int SidebarUp = VK_UP;
int SidebarDown = VK_DOWN;
int Alliance = 'A';
int Retreat = 'R';
int SelectPrimaryBuilding = 'P';
int OptionsMenu = VK_ESCAPE;
int Team1 = '1';
int Team2 = '2';
int Team3 = '3';
int Team4 = '4';
int Team5 = '5';
int Team6 = '6';
int Team7 = '7';
int Team8 = '8';
int Team9 = '9';
int Team10 = '0';
int CTRL = VK_CONTROL;
int ALT = VK_MENU;
int SHIFT = VK_SHIFT;
int PrivateChat1 = VK_F1;
int PrivateChat2 = VK_F2;
int PrivateChat3 = VK_F3;
int PrivateChat4 = VK_F4;
int PrivateChat5 = VK_F5;
int PublicChat = VK_F6;
int WOLPageReply = VK_F8;
int WOLProposeDraw = 'D';
int CenterSelectedUnits = VK_HOME;
int ScrollLeft = VK_NUMPAD1;
int ScrollDown = VK_NUMPAD2;
int ScrollRight = VK_NUMPAD3;
int ScrollUp = VK_NUMPAD5;
int ToggleLiveStats = VK_TAB;
int PlayRandomSong = VK_OEM_PLUS;

void HandleKeyEvent(int key, bool keyIsDown)
{
    int destKey;

    if (key == SelectAllUnits) destKey = 'E';
    else if (key == Scatter) destKey = 'X';
    else if (key == CenterNextUnit) destKey = 'N';
    else if (key == Repair) destKey = 'T';
    else if (key == Sell) destKey = 'Y';
    else if (key == Guard) destKey = 'G';
    else if (key == CenterBase) destKey = 'H';
    else if (key == Stop) destKey = 'S';
    else if (key == Bookmark1) destKey = VK_F9;
    else if (key == Bookmark2) destKey = VK_F10;
    else if (key == Bookmark3) destKey = VK_F11;
    else if (key == Bookmark4) destKey = VK_F12;
    else if (key == SidebarUp) destKey = VK_UP;
    else if (key == SidebarDown) destKey = VK_DOWN;
    else if (key == Alliance) destKey = 'A';
    else if (key == Retreat) destKey = 'R';
    else if (key == SelectPrimaryBuilding) destKey = 'P';
    else if (key == OptionsMenu) destKey = VK_ESCAPE;
    else if (key == Team1) destKey = '1';
    else if (key == Team2) destKey = '2';
    else if (key == Team3) destKey = '3';
    else if (key == Team4) destKey = '4';
    else if (key == Team5) destKey = '5';
    else if (key == Team6) destKey = '6';
    else if (key == Team7) destKey = '7';
    else if (key == Team8) destKey = '8';
    else if (key == Team9) destKey = '9';
    else if (key == Team10) destKey = '0';
    else if (key == CTRL) destKey = VK_CONTROL;
    else if (key == ALT) destKey = VK_MENU;
    else if (key == SHIFT) destKey = VK_SHIFT;
    else if (key == PrivateChat1) destKey = VK_F1;
    else if (key == PrivateChat2) destKey = VK_F2;
    else if (key == PrivateChat3) destKey = VK_F3;
    else if (key == PrivateChat4) destKey = VK_F4;
    else if (key == PrivateChat5) destKey = VK_F5;
    else if (key == PublicChat) destKey = VK_F6;
    else if (key == WOLPageReply) destKey = VK_F8;
    else if (key == WOLProposeDraw) destKey = 'D';
    else if (key == CenterSelectedUnits) destKey = VK_HOME;
    else if (key == ScrollLeft) destKey = VK_NUMPAD1;
    else if (key == ScrollDown) destKey = VK_NUMPAD2;
    else if (key == ScrollRight) destKey = VK_NUMPAD3;
    else if (key == ScrollUp) destKey = VK_NUMPAD5;
    else destKey = key;
    
    if (SpawnerActive && destKey == WOLPageReply) return;
    
    if (destKey == PlayRandomSong)
    {
        if (!keyIsDown) PlayRandomMusic();
        return;
    }
    
    switch (destKey)
    {
        case VK_MBUTTON:
        case VK_XBUTTON1:
        case VK_XBUTTON2:
            return; // those keys were not supported by the original game so we are not forwarding them if not assigned to any function
        case VK_F1:
        case VK_F2:
        case VK_F3:
        case VK_F4:
        case VK_F5:
        case VK_F6:
        { 
            //rate limit for in game messages to avoid spam
            if (keyIsDown && GameState == GS_MAINLOOP)
            {
                static uint32_t LastTickCount = 0;
                if (!LastTickCount) LastTickCount = fake_timeGetTime();
                else
                {
                    uint32_t tickCount = fake_timeGetTime();
                    int difference = tickCount - LastTickCount;
                    if (difference < 1000) return;
                    else LastTickCount = tickCount;
                }
            }
            break;
        }
    }
    
    if (keyIsDown) KeyIsDown1[destKey] = true;
    KeyIsDown2[destKey] = keyIsDown;
}
