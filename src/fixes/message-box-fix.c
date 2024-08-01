#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

//MessageBox is hidden behind the window, use MB_TOPMOST to bring it to the front and MB_TASKMODAL to make the cursor visible in window mode

CLEAR(0x00401AF4+5, 0x90, 0x00401AFA);
CALL(0x00401AF4, _FakeMessageBoxA);
CLEAR(0x00403549+5, 0x90, 0x0040354F);
CALL(0x00403549, _FakeMessageBoxA);
CLEAR(0x004035B1+5, 0x90, 0x004035B7);
CALL(0x004035B1, _FakeMessageBoxA);
CLEAR(0x0042B63E+5, 0x90, 0x0042B644);
CALL(0x0042B63E, _FakeMessageBoxA);
CLEAR(0x0042B7A8+5, 0x90, 0x0042B7AE);
CALL(0x0042B7A8, _FakeMessageBoxA);
CLEAR(0x004A29CB+5, 0x90, 0x004A29D1);
CALL(0x004A29CB, _FakeMessageBoxA);
// Replcaed by failure.txt.c
//CLEAR(0x004A314E+5, 0x90, 0x004A3154);
//CALL(0x004A314E, _FakeMessageBoxA);
CLEAR(0x004A3D29+5, 0x90, 0x004A3D2F);
CALL(0x004A3D29, _FakeMessageBoxA);
CLEAR(0x004A3D7D+5, 0x90, 0x004A3D83);
CALL(0x004A3D7D, _FakeMessageBoxA);
CLEAR(0x004A3E7B+5, 0x90, 0x004A3E81);
CALL(0x004A3E7B, _FakeMessageBoxA);

int WINAPI FakeMessageBoxA(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    return MessageBoxA(hWnd, lpText, lpCaption, uType | MB_TASKMODAL | MB_TOPMOST);
}
