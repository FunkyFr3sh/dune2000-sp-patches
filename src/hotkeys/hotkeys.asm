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

hack 0x004A362D, 0x4A3637 ; WM_LBUTTONUP
    pushad
    push 0
    push 1
    call HandleKeyEvent
    add esp, 8
    popad
    jmp hackend

@CLEAR 0x004A3639, 0x90, 0x004A3643 ; Windows_Procedure

hack 0x004A35B9, 0x4A35BF ; WM_LBUTTONDOWN, WM_LBUTTONDBLCLK
    pushad
    push 1
    push 1
    call HandleKeyEvent
    add esp, 8
    popad
    jmp hackend

@CLEAR 0x004A35C1, 0x90, 0x004A35C7 ; Windows_Procedure

hack 0x004A3711, 0x4A3725 ; WM_RBUTTONUP
    pushad
    push 0
    push 2
    call HandleKeyEvent
    add esp, 8
    popad
    jmp hackend

hack 0x004A36E3, 0x4A36EF ; WM_RBUTTONDOWN, WM_RBUTTONDBLCLK
    pushad
    push 1
    push 2
    call HandleKeyEvent
    add esp, 8
    popad
    jmp hackend
