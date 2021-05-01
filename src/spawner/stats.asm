%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

gstring MapName, "", 60
gint SpawnerGameEndState, 0

sstring ComputerPlayer, "Computer"
sstring UnknownSide, "Unknown"

@CLEAR 0x0040D94B, 0x90, 0x0040D950 ;do not quit if unknown game end state

hack 0x0044A0C1, 0x0044A0C6 ; CallWOL__SendGameResultsToServer
    cmp byte[SpawnerActive], 1
    jnz .out
    pushad

    cmp dword[SpawnerGameEndState], GES_ISURRENDERED
    jnz .notSurrendered
    mov al, byte[MySideID]
    push eax
    call Map__PlayerDefeated
    add esp, 4
    
.notSurrendered:
    call WOL__SendGameResultsToServer
    popad
    
.out:
    call 0x0045B830
    jmp hackend


hack 0x0040DB75, 0x0040DB7A ;CallWriteStatsDmp
    pushad
    push ecx
    push esi
    call WriteStatsDmp
    add esp, 8
    popad
    cmp byte[SpawnerActive], 1
    jz 0x0040DBE0
    mov eax, dword[0x4F40E8]
    jmp hackend


hack 0x0040D828, 0x0040D82F ; UseSpawnIniMapNameIfMapNotInStringTable
    lea edx, [esp+0x0F4]
    cmp byte[SpawnerActive], 1
    jnz 0x0040D82F
    cmp byte[eax], 0
    jnz 0x0040D82F
    mov eax, MapName
    jmp 0x0040D82F


@CLEAR 0x0040D530, 0x90, 0x0040D533
hack 0x0040D4F7, 0x0040D529 ; UseEnglishStringsForSides
    push eax
    xor eax, eax
    mov al, byte[NetPlayerCount]
    cmp ebx, eax
    pop eax
    jb .IsNotAiPlayer
    mov eax, UnknownSide
    jmp 0x0040D529

.IsNotAiPlayer:
    cmp al, 2
    jnz .harkonnen
    mov eax, 0x004DC514 ; ordos
    jmp 0x0040D529
    
.harkonnen:
    cmp al, 1
    jnz .atreides
    mov eax, 0x004DC508 ; harkonnen
    jmp 0x0040D529
    
.atreides:
    mov eax, 0x004DC4FC ; atreides
    jmp 0x0040D529


hack 0x0040DAFD, 0x0040DB02 ; CallSetGameEndTickCount
    cmp byte[SpawnerActive], 1
    jnz .out
    pushad
    call [_imp__GetTickCount]
    mov dword[GameEndTickCount], eax
    popad
    
.out:
    mov eax, dword[0x4F3D34]
    jmp hackend


hack 0x0040DAE9, 0x0040DAEF ; WriteStatsForAiPlayers1
    cmp byte[SpawnerActive], 1
    jnz .out
    mov cl, byte[NetPlayerCount]
    add cl, byte[NetAIPlayers]
    jmp 0x0040DAEF
    
.out:
    mov cl, byte[NetPlayerCount]
    jmp 0x0040DAEF


hack 0x0040D763, 0x0040D769 ; WriteStatsForAiPlayers2
    cmp byte[SpawnerActive], 1
    jnz .out
    mov cl, byte[NetPlayerCount]
    add cl, byte[NetAIPlayers]
    jmp 0x0040D769
    
.out:
    mov cl, byte[NetPlayerCount]
    jmp 0x0040D769

    
hack 0x0040D9DD, 0x0040D9E3 ; write FinishingPlace for AI players
    mov cl, byte[NetPlayerCount]

    cmp byte[SpawnerActive], 1
    jnz hackend
    add cl, byte[NetAIPlayers]
    jmp hackend


hack 0x0040D565, 0x0040D56B ; FixPlayerNames
    cmp byte[SpawnerActive], 1
    jnz .out
    xor ecx, ecx
    mov cl, byte[NetPlayerCount]
    
    cmp ebx, ecx
    jge .IsAiPlayer
    cmp dword[GameType], GT_SKIRMISH
    jnz .out
    mov ecx, NetPlayerName
    jmp 0x0040D56B
    
.IsAiPlayer:
    mov ecx, ComputerPlayer
    jmp 0x0040D56B
    
.out:
    lea ecx, [ebp+NetPlayerNamesArray]
    jmp 0x0040D56B


hack 0x0040D8A0 ; UseSpawnerGameEndState
    cmp byte[SpawnerActive], 1
    jnz .out
    mov eax, dword[SpawnerGameEndState]
    jmp 0x0040D8A5
    
.out:
    mov eax, dword[GameEndState]
    jmp 0x0040D8A5


hack 0x00492E64 ; ISurrendered
    cmp dword[SpawnerGameEndState], GES_ENDEDNORMALLY
    jnz .out
    mov dword[SpawnerGameEndState], GES_ISURRENDERED
    
.out:
    mov eax, dword[GameType]
    jmp 0x00492E69


hack 0x00492EF4 ; IAborted
    cmp dword[SpawnerGameEndState], GES_ENDEDNORMALLY
    jnz .out
    mov dword[SpawnerGameEndState], GES_ISURRENDERED
    
.out:
    mov eax, dword[GameType]
    jmp 0x00492EF9


hack 0x0045AAF9, 0x0045AAFF ; OutOfSync
    cmp dword[SpawnerGameEndState], GES_ENDEDNORMALLY
    jnz .out
    mov dword[SpawnerGameEndState], GES_OUTOFSYNC
    
.out:
    mov dword[GameEndState], ebx
    jmp 0x0045AAFF


hack 0x0045CEA9, 0x0045CEB0 ; Opponent Surrendered
    cmp dword[SpawnerGameEndState], GES_ENDEDNORMALLY
    jnz .out
    mov dword[SpawnerGameEndState], GES_OPPONENTSURRENDERED
    
.out:
    cmp dword[GameType], GT_WOL
    jmp 0x0045CEB0


hack 0x00458D2D, 0x00458D34 ; OpponentSurrendered2
    cmp dword[SpawnerGameEndState], GES_ENDEDNORMALLY
    jnz .out
    mov dword[SpawnerGameEndState], GES_OPPONENTSURRENDERED
    
.out:
    cmp dword[GameType], GT_WOL
    jmp 0x00458D34


hack 0x0045CD36 ; ConnectionLost
    cmp dword[SpawnerGameEndState], GES_ENDEDNORMALLY
    jnz .out
    mov dword[SpawnerGameEndState], GES_CONNECTIONLOST
    
.out:
    mov eax, dword[GameType]
    jmp 0x0045CD3B



hack 0x00455938, 0x0045593E ; SaveUnitOwnedStats
%define house esp+0x18
%define unitId esp+0x10

    cmp byte[SpawnerActive], 1
    jnz .out
    mov ecx, dword[house]
    mov eax, dword[unitId]
    cmp cl, 8
    jge .out
    cmp al, 30
    jge .out
    
    pushad
    and ecx, 0x000000FF
    imul ecx, UnitTrackerSize
    
    and eax, 0x000000FF
    imul eax, 4
    
    inc dword[PlayersUnitsOwned+eax+ecx]
    popad
    
.out:
    mov edx, dword[edi+0x24780]
    jmp 0x0045593E


hack 0x004563E5, 0x004563EF ; SaveBuildingsOwnedStats
%define house esp+0x38
%define buildingId esp+0x3C

    cmp byte[SpawnerActive], 1
    jnz .out
    mov ecx, dword[house]
    mov eax, dword[buildingId]
    cmp cl, 8
    jge .out
    cmp al, 62
    jge .out
    
    pushad
    and ecx, 0x000000FF
    imul ecx, BuildingTrackerSize
    
    and eax, 0x000000FF
    imul eax, 4
    
    inc dword[PlayersBuildingsOwned+eax+ecx]
    popad

.out:
    mov ecx, dword[buildingId]
    mov ebp, dword[edi+0x24784]
    jmp 0x004563EF
    
    
hack 0x004412E6, 0x004412EC ; single player stats fix
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jnz .out
    mov dl, 1
    
.out:
    mov byte[esp+0x13], dl
    xor cl, cl
    jmp hackend
