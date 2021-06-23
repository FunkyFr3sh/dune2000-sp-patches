%include "macros/patch.inc"
%include "dune2000.inc"

; Fix missing devastator self-destruct sound
hack 0x0049B3E9, 0x0049B3F1 ; UpdateUnit
    call ModelAddExplosion ; instruction replaced by the long jump
    add esp, 24h ; instruction replaced by the long jump  
    mov  eax, [esi+38h]
    cdq
    and  edx, 0FFFFh
    add  eax, edx
    sar  eax, 15h
    push eax ; y-position
    mov  eax, [esi+34h]
    cdq
    and  edx, 0FFFFh
    add  eax, edx
    sar  eax, 15h
    push eax ; x-position    
    xor eax, eax
    mov al, [0x6D612C]
    mov ecx, [0x781E5C + 8 * eax] ; get the sound ID  
    push ecx ; sound ID
    call PlaySoundAt
    add esp, 12
    jmp hackend

