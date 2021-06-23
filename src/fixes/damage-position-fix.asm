%include "macros/patch.inc"

; Fix misplaced exploding barrel damage 
hack 0x0049BD6F, 0x0049BD7E ; UpdateBuilding
    sub eax, 16 ; decrease y-position by 16 (move to center of tile)
    push eax
    mov eax, [esi]
    cdq
    and edx, 0FFFFh
    add eax, edx
    sar eax, 10h
    add eax, 16 ; increase x-position by 16 (move to center of tile)
    jmp hackend

; Fix misplaced grenadier death damage
hack 0x00499E1E, 0x00499E29 ; UpdateUnit 
    mov  eax, [esi+38h]
    cdq
    and  edx, 0FFFFh
    add  eax, edx
    sar  eax, 10h
    push eax ; y-position
    mov  eax, [esi+34h]
    cdq
    and  edx, 0FFFFh
    add  eax, edx
    sar  eax, 10h
    push eax ; x-position
    jmp hackend
    
; Fix misplaced sardaukar death damage
hack 0x0049D14D, 0x0049D152 ; DestroyUnit 
    mov  eax, [esi+38h]
    cdq
    and  edx, 0FFFFh
    add  eax, edx
    sar  eax, 10h
    push eax ; y-position
    mov  eax, [esi+34h]
    cdq
    and  edx, 0FFFFh
    add  eax, edx
    sar  eax, 10h
    push eax ; x-position
    jmp hackend