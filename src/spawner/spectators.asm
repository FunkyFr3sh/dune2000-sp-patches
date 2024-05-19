%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"
%include "macros/datatypes.inc"
%include "macros/extern.inc"

sstring RadarStringFormat, "%d %d"

hack 0x0044904F, 0x00449055 ; DrawMiniMapForSpectators
    mov cl, byte[0x00798544]
    cmp byte[SpawnerActive], 1
    jnz 0x00449055
    cmp byte[Lose], 1
    jnz 0x00449055
    jmp 0x0044906D


hack 0x00456843, 0x00456849 ; ShareBuildingsViewWithSpectators
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp byte[Lose], 1
    jnz .out
    jmp 0x0045684D
    
.out:
    mov cl, byte[MySideID]
    jmp 0x00456849


hack 0x0044D02F, 0x0044D035 ; ShareUnitsViewWithSpectators
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp byte[Lose], 1
    jnz .out
    jmp 0x0044D035
    
.out:
    cmp byte[edi+eax], 0
    jne 0x0044D0B3
    jmp 0x0044D035


hack 0x004440BA, 0x004440C0 ; ClickableMiniMapForSpectators
    mov dl, byte[0x00798544]
    cmp byte[SpawnerActive], 1
    jnz 0x004440C0
    cmp byte[Lose], 1
    jnz 0x004440C0
    jmp 0x004440E0


hack 0x00425F20, 0x00425F2E ; ShowPlayerCreditsForSpectators
    add esp, 0x0C
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jz .ShowCredits
    cmp byte[Lose], 1
    jnz .out
    cmp byte[MeIsSpectator], 1
    jnz .out
    
.ShowCredits:
    mov edx, dword[esi+0x24C94]
    mov ecx, dword[esi+0x24C90]
    add edx, ecx
    push edx
    mov ecx, dword[esi+0x24254]
    mov edx, dword[esi+0x2425C]
    add ecx, edx
    push ecx
    push RadarStringFormat
    push 0x00503FC8
    call _sprintf
    add esp, 0x10
    
.out:
    mov eax, dword[0x4DDF38]
    mov ecx, dword[0x6D8D98]
    jmp 0x00425F2E


hack 0x00469ECD, 0x00469ED6 ; SkipSpawningStartingUnitsForSpectators
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jz .out
    pushad
    
.CheckCurrentPlayer:
    xor eax, eax
    mov al, byte[esp+0x24+(8*4)]
    push eax
    call IsSpectator
    add esp, 4
    cmp al, 1
    jnz .IsNotSpectator
    add byte[esp+0x24+(8*4)], 1 ;raise the house, skip spawning units and check the next house
    jmp .CheckCurrentPlayer
    
.IsNotSpectator:
    mov al, byte[esp+0x2F+(8*4)]
    cmp al, byte[esp+0x24+(8*4)]
    popad
    jz 0x0046A271
    
.out:
    test al, al
    jne 0x00469EFC
    push 0x1628
    jmp 0x00469ED6


hack 0x004494B8 ; draw live stats
    pushad
    push dword[PointerToMainImage]
    call DrawLiveStats
    add esp, 4
    popad

    push 0x004E0620
    jmp 0x004494BD
    

hack 0x00469BB1 ;do not pre place buildings/units for spectators
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jz .out
    pushad
    push edx
    call IsSpectator
    add esp, 4
    cmp al, 1
    popad
    jz 0x00469D1B
    
.out:
    test eax, 0x80000000
    jmp hackend

    
hack 0x0044FC53; set bool Lose to true on game start
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jz .out
    cmp byte[MeIsSpectator], 1
    jnz .out
    mov byte[Lose], 1
    
.out:
    push 0x004E2B4C
    jmp hackend
