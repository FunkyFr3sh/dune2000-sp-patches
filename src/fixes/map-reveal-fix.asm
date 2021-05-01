%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

; game shows an error message when you try to reveal more than 7 cells, this fix allows to reveal the full map when radius param 0 is passed

hack 0x004543C9
    mov eax, dword[esi+4]
    mov ecx, dword[esi]
    cmp dl, 0
    jnz hackend
    
    call 0x0044C480
    jmp 0x004543D9
