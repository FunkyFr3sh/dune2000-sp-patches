%include "macros/patch.inc"
%include "hotkeys.inc"
%include "patch.inc"

hack 0x004A33A4, 0x4A33B0 ; WM_KeyDown
    pushad
    push 1
    push esi
    call HandleKeyEvent
    add esp, 8
    popad
    jmp hackend

hack 0x004A3468, 0x4A346F ; WM_KeyUp
    pushad
    push 0
    push eax
    call HandleKeyEvent
    add esp, 8
    popad
    jmp hackend
