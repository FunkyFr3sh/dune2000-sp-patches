%include "macros/patch.inc"
%include "patch.inc"

; Add more debug information to investigate "unit->pos.steps > 255" error
hack 0x00494757, 0x00494769 ; MoveUnit
    push esi
    call DebugUnitCrash
    add esp, 4
    jmp hackend
