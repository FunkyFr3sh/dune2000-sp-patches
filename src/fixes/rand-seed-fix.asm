%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"


hack 0x0044A888 ; init seed with tickcount
    call [_imp__GetTickCount]
    mov dword[RandSeed], eax
    mov ecx, 0x30
    jmp hackend
