%include "macros/patch.inc"

; Exploding barrel-type building should not make damage when it is sold or captured
hack 0x0049BD4C, 0x0049BD51 ; UpdateBuilding
    mov eax, [esi+28h] ; get building flags
    test eax, 0x1000004 ; check if selling or infiltrated flag is set
    jnz 0x49BD87 ; skip call of DamageTiles
    mov al, [0x6D60EE] ; instruction replaced by the long jump  
    jmp hackend
