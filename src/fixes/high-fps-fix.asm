%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

;Do not use FrameRate as LimitedModelRate if value above 60 to make sure the game speed stays smooth

hack 0x0045C4B1, 0x0045C4B7
    cmp ax, 60
    jbe .out
    mov ax, 60
    
.out:
    mov word[LimitedModelRate], ax
    jmp 0x0045C4B7
    
    
hack 0x0045CFFB, 0x0045D001
    cmp ax, 60
    jbe .out
    mov ax, 60
    
.out:
    mov word[LimitedModelRate], ax
    jmp 0x0045D001
