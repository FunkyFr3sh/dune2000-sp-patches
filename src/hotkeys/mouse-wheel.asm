%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "hotkeys.inc"
%include "patch.inc"

gbool MouseWheelTriggered, false

hack 0x00444019 ; WheelScrollingUp
    mov  eax,dword [0x516440]
    cmp  byte [MouseWheelTriggered],1
    jnz  0x0044401E
    mov  byte [KeyIsDown2(VK_UP)],0
    mov  byte [MouseWheelTriggered],0
    jmp  0x0044401E


hack 0x00443FD3 ; WheelScrollingDown
    mov  eax,dword [0x516440]
    cmp  byte [MouseWheelTriggered],1
    jnz  0x00443FD8
    mov  byte [KeyIsDown2(VK_DOWN)],0
    mov  byte [MouseWheelTriggered],0
    jmp  0x00443FD8
