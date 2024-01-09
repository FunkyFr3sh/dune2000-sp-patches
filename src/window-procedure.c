#include <windowsx.h>
#include "dune2000.h"
#include "patch.h"
#include "macros/patch.h"
#include "hotkeys.h"

SETDWORD(0x004a3c12, _WindowProcedure);

LRESULT CALLBACK GameWindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool MouseWheelUp = false;
bool MouseWheelDown = false;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETCURSOR:
        {
            if (UseHardwareCursor && SetCursorImp && LOWORD(lParam) == HTCLIENT)
            {
                SetCursorImp(HardwareCursors[CurrentCursorId]);
                return TRUE;
            }

            break;
        }
        case WM_MOUSEWHEEL:
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (zDelta > 0) //MouseWheel Up
            {
                MouseWheelUp = true;
            }
            else //MouseWheel Down
            {
                MouseWheelDown = true;
            }
            break;
        }
        case WM_MBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        {
            HandleKeyEvent(VK_MBUTTON, true);
            break;
        }
        case WM_MBUTTONUP:
        {
            HandleKeyEvent(VK_MBUTTON, false);
            break;
        }
        case WM_XBUTTONDOWN:
        {
            int fwKeys = GET_KEYSTATE_WPARAM(wParam);
            if (fwKeys & MK_XBUTTON1) HandleKeyEvent(VK_XBUTTON1, true);
            if (fwKeys & MK_XBUTTON2) HandleKeyEvent(VK_XBUTTON2, true);
            break;
        }
        case WM_XBUTTONUP:
        {
            int fwKeys = GET_KEYSTATE_WPARAM(wParam);
            if (!(fwKeys & MK_XBUTTON1)) HandleKeyEvent(VK_XBUTTON1, false);
            if (!(fwKeys & MK_XBUTTON2)) HandleKeyEvent(VK_XBUTTON2, false);
            break;
        }
    }
    
    return GameWindowProcedure(hwnd, uMsg, wParam, lParam);
}
