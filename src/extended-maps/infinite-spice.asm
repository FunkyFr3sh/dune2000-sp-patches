%include "macros/patch.inc"
%include "macros/datatypes.inc"

gbool InfiniteSpice, false

hack 0x004995A5, 0x004995AC ; do not remove the spice
    cmp byte[InfiniteSpice], 1
    jz hackend
    mov dword[ecx*4+0x517DF4], eax
    jmp hackend
