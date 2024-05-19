%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"

hack 0x0044A814, 0x0044A819; OverrideAIPlayerColors
    pushad
    call SetupAIPlayerColors
    popad
    
    mov al, byte[NetCrates]
    jmp hackend


hack 0x0041B445 ; RemoveAINetCheck
    mov eax, dword[GameType]
    cmp byte[SpawnerActive], 1
    jnz 0x0041B44A
    jmp 0x0041B457


hack  0x0041B3B9, 0x0041B3C3 ; HardAIWithLowDifficultySetting
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jnz .out
    jmp 0x0041B3C3
    
.out:
    cmp word[DifficultyLevel], 2
    jne 0x0041B3E6
    jmp 0x0041B3C3


hack 0x0049E529, 0x0049E533 ; HardAIWithLowDifficultySetting2
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jnz .out
    jmp 0x0049E555
    
.out:
    cmp word[DifficultyLevel], 0
    jne 0x0049E555
    jmp 0x0049E533


hack 0x004429DA, 0x004429E5 ;Use handicap 0 setting for unit price - skirmish
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jnz .out
    cmp cl, al
    mov al, byte[DifficultyLevel]
    jz 0x004429E5
    mov al, 0
    jmp 0x004429E5

.out:
    cmp cl, al
    mov al, byte[DifficultyLevel]
    jz 0x004429E5
    mov al, 1
    jmp 0x004429E5
    
    
hack 0x0044298A, 0x00442995 ;Use handicap 0 setting for production speed - skirmish
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jnz .out
    cmp cl, al
    mov al, byte[DifficultyLevel]
    jz 0x00442995
    mov al, 0
    jmp 0x00442995

.out:
    cmp cl, al
    mov al, byte[DifficultyLevel]
    jz 0x00442995
    mov al, 1
    jmp 0x00442995
