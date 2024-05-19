%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

hack 0x00469FD0, 0x00469FD6 ; OverrideStartingLocation
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jz .out
    pushad
    xor eax, eax
    mov al, byte[esp+0x24+(8*4)]
    push eax
    call GetStartingLocation
    add esp, 4
    cmp eax, -1
    jz .randomLoc
    mov byte[esp+0x44+(8*4)], al
    jmp .notRandom
    
.randomLoc:
    xor eax, eax
    mov al, byte[esp+0x44+(8*4)]
    push eax
    call GetFreeStartingLocation
    add esp, 4
    mov byte[esp+0x44+(8*4)], al

.notRandom:
    popad
    
.out:
    mov eax, dword[esp+0x44]
    xor edx, edx
    jmp 0x00469FD6
