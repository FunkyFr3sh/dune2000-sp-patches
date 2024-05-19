%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

cextern RandomClassLog
cextern RandomGameTick

%assign RandomClassLogLength 4096

sint RandomClassLogIndex, 0

hack 0x004B1810 ; rand()
    mov eax, dword[RandomClassLogIndex]
    
    cmp eax, RandomClassLogLength
    jb .inc
    
    xor eax, eax
    mov dword[RandomClassLogIndex], eax
    jmp .noInc
    
.inc:
    inc dword[RandomClassLogIndex]

.noInc:    
    mov ecx, dword[esp]
    mov dword[eax*4+RandomClassLog], ecx
    
    mov ecx, dword[GameTicks]
    mov dword[eax*4+RandomGameTick], ecx
    
.out:
    mov eax, dword[RandSeed]
    jmp hackend
