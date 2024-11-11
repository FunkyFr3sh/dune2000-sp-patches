%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "inc/patch.inc"

hack 0x004995A5, 0x004995AC ; do not remove the spice
    cmp byte[rulesExt__InfiniteSpice], 1
    jz hackend
    mov dword[ecx*4+0x517DF4], eax
    jmp hackend
