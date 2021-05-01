%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"


hack 0x004443B2, 0x004443B9 ; Use the saved id rather than the one matching the current mouse position
    mov dl, byte[MySideID]
    cmp word[ebp+HC_CURRENTLY_PRODUCED_BUILDING_ID], 0x0FFFF
    jz .out
    
    mov ax, word[ebp+HC_CURRENTLY_PRODUCED_BUILDING_ID]

.out:
    push eax
    jmp hackend
