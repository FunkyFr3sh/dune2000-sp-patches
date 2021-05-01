%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

hack 0x00454388, 0x0045438F ; fix for the SetCash mission event. It modified the StartingCredits which are not used during the game, the fix is using the Credits in the house class now instead
    push edx
    push ecx
    call Side__AsPointer
    add esp, 4
    pop edx
    mov dword[eax+HC_CREDITS], edx
    mov dword[eax+HC_SILO_CREDITS], 0
    jmp hackend
