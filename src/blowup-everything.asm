%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"
%include "dune2000.inc"


gfunction BlowUpEverything ; arg1 = sideId - All buildings/units of the chosen player will explode
%define sideId esp+0x0AC
    sub esp, 0x0A8
    mov edx, dword[GameTicks]
    mov al, byte[sideId]
    mov word[esp+0x18], 0
    lea ecx, [esp]
    push ecx
    mov byte[esp+0x27], 0x31
    mov dword[esp+0x0C], edx
    mov byte[esp+0x28], al
    mov word[esp+0x8], 0x3E8
    call Model__ExecuteGameEvent
    add esp, 4
    mov byte[0x797DF8], 1
    add esp, 0x0A8
    retn
