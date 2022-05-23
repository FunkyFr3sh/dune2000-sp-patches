%include "macros/patch.inc"
%include "dune2000.inc"

; Do not target invulnerable carryalls which bring a delivery
hack 0x004A4CA8, 0x004A4CAE ; EnemyUnitInRange
    test byte [esi+8], 40h  ; instruction replaced by the long jump
    jz 0x4A4D02             ; instruction replaced by the long jump
    test dword [esi+8], 8000000h  ; check for invulnerable flag
    jnz 0x4A4D02
    jmp hackend
