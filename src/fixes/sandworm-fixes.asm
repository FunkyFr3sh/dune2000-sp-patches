%include "macros/patch.inc"

; Fix units keeping firing on allied sandworm even after it was killed and put on sleep
; Superseded by Mod__UpdateUnit
;hack 0x00497E15 ; UpdateUnit
;    cmp [eax+10h], edi ; instruction replaced by the long jump
;    jz 0x497E44 ; instruction replaced by the long jump
;    cmp dword [eax+10h], 29 ; If the target unit state is 29 (Sleeping), always reset enemy index
;    jz 0x497E44
;    jmp hackend

; Make sleeping sandworm not taking damage - prevents turning neutral to hostile
hack 0x0049D826 ; DamageTiles
    cmp ecx, 23 ; instruction replaced by the long jump
    jnz .Skip ; instruction replaced by the long jump
    xor al, al
.Skip:
    cmp ecx, 29 ; If the unit state is 29 (Sleeping), do not take damage
    jnz 0x49D82D
    jmp hackend
