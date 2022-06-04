%include "macros/patch.inc"
%include "dune2000.inc"

; Fix infantry rarely remaining invulnerable after released from destroyed building
hack 0x0049D815, 0x0049D81F ; DamageTiles
    mov ecx, [esi+10h] ; instruction replaced by the long jump 
    cmp ecx, 8 ; instruction replaced by the long jump 
    jne .Skip
    ; Work-around: add additional check if infantry stays on tile occupied by building.
    ; If there's not building tile, allow infantry to be damaged even if it's still in state 8 due to a bug
    push esi
    call UnitTileOccupiedByBuilding
    add esp, 4
    dec al ; Flip the bool result of function
.Skip:
    jmp hackend

