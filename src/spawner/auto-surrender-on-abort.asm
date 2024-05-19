%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"

hack 0x0044A173, 0x0044A17A ; Auto Surrender On Abort
    cmp byte[SpawnerActive], 1
    jnz .out
    mov dword[esp+0x80], 1
    jmp 0x0044A17A
    
.out:
    mov dword[esp+0x80], ecx
    jmp 0x0044A17A
