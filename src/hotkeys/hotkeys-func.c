#include "hotkeys.h"
#include "patch.h"
#include "dune2000.h"
#include "../event-system/event-core.h"

//http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

int Hotkey_SelectAllUnits =           'E';
int Hotkey_SelectAllUnitsOfSameType = 'W';
int Hotkey_Scatter =                  'X';
int Hotkey_CenterNextUnit =           'N';
int Hotkey_Repair =                   'T';
int Hotkey_Sell =                     'Y';
int Hotkey_Guard =                    'G';
int Hotkey_CenterBase =               'H';
int Hotkey_Stop =                     'S';
int Hotkey_Deploy =                   'D';
int Hotkey_BuildOrPlaceBuilding =     'B';
int Hotkey_Bookmark[4] =              {VK_F9, VK_F10, VK_F11, VK_F12};
int Hotkey_SidebarUp =                VK_UP;
int Hotkey_SidebarDown =              VK_DOWN;
int Hotkey_Alliance =                 'A';
int Hotkey_Retreat =                  'R';
int Hotkey_SelectPrimaryBuilding =    'P';
int Hotkey_OptionsMenu =              VK_ESCAPE;
int Hotkey_Team[10] =                 {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int Hotkey_CTRL =                     VK_CONTROL;
int Hotkey_ALT =                      VK_MENU;
int Hotkey_SHIFT =                    VK_SHIFT;
int Hotkey_PrivateChat[5] =           {VK_F1, VK_F2, VK_F3, VK_F4, VK_F5};
int Hotkey_PublicChat =               VK_F6;
int Hotkey_WOLPageReply =             VK_F8;
int Hotkey_WOLProposeDraw =           'O';
int Hotkey_CenterSelectedUnits =      VK_HOME;
int Hotkey_ScrollLeft =               VK_NUMPAD1;
int Hotkey_ScrollDown =               VK_NUMPAD2;
int Hotkey_ScrollRight =              VK_NUMPAD3;
int Hotkey_ScrollUp =                 VK_NUMPAD5;
int Hotkey_ToggleLiveStats =          VK_TAB;
int Hotkey_PlayRandomSong =           VK_OEM_PLUS;

void HandleKeyEvent(int key, bool keyIsDown)
{
  if (gGameState == GS_MAINLOOP && !gUIMgr->dw_field_110_index && key != Hotkey_OptionsMenu)
  {
    if (keyIsDown)
    {
      if (ExecuteEventHook(HOOK_KEYBOARD_MOUSE_DOWN, 2, 0, key, 0, 0, 0))
        return;
    }
    else
    {
      if (ExecuteEventHook(HOOK_KEYBOARD_MOUSE_UP, 2, 0, key, 0, 0, 0))
      {
        if (key == VK_LBUTTON)
        {
          _MouseLeftState = 0;
          _TacticalData.__DraggingBandbox = 0;
        }
        if (key == VK_RBUTTON)
          _MouseRightState = 0;
        _KeyboardKeyState[key] = 0;
        return;
      }
    }
  }

  if (key == Hotkey_OptionsMenu && !keyIsDown && IsCurrentlyShown("MP_TIMEOUT"))
  {
    ForceQuickExit = true;
    gGameState = GS_QUIT;
    return;
  }

  if (key == Hotkey_OptionsMenu && keyIsDown)
    _KeyboardKeyDown[VK_ESCAPE] = true;

  if (key == Hotkey_PrivateChat[0] || key == Hotkey_PrivateChat[1] || key == Hotkey_PrivateChat[2] || key == Hotkey_PrivateChat[3] || key == Hotkey_PrivateChat[4] || key == Hotkey_PublicChat)
  {
    //rate limit for in game messages to avoid spam
    if (keyIsDown && gGameState == GS_MAINLOOP)
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
  }

  if (keyIsDown) _KeyboardKeyDown[key] = true;
  _KeyboardKeyState[key] = keyIsDown;
  if (key == VK_LBUTTON)
  {
    _MouseLeftState = keyIsDown;
    if (keyIsDown)
      _MouseLeftDown = 1;
    else
      _MouseLeftUp = 1;
  }
  if (key == VK_RBUTTON)
  {
    _MouseRightState = keyIsDown;
    if (keyIsDown)
      _MouseRightDown = 1;
    else
      _MouseRightUp = 1;
  }
}
