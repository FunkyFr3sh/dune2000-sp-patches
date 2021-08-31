%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "inc/dune2000.inc"

gbyte gOldSideId, 0

; Store the original side ID upon starting a map
hack 0x00469790, 0x00469795 ; setupmapstuff
    mov al, byte [gSideId]
    mov byte [gOldSideId], al    
    mov eax, 17C8h ; Instruction replaced by long jump
    jmp hackend

; Store the original side ID during loading saved game
hack 0x00441BBB, 0x00441BC1 ; LoadGame
    mov al, byte [gSideId]
    mov byte [gOldSideId], al
    add esp, 10h ; Instructions replaced by long jump
    push esi
    push 1
    jmp hackend

; Restore the original side ID upon restart
hack 0x0044C472, 0x0044C479 ; flag_restart
    mov byte [0x797E1C], 1 ; Instruction replaced by long jump
    mov al, byte [gOldSideId]
    mov byte [gSideId], al
    jmp hackend
