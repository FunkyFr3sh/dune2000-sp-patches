%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

; This patch enables mission events in multi-player/skirmish mode

gbool MapScriptExists, false
gbool UseDefaultWinLoseEvents, false
gstring MapScript, "", 50

hack 0x00453F58, 0x00453F60 ; By default the players will lose once all units/buildings were destroyed and maps with .mis files use their own Win/Lose events. This option allows the use of the default logic in scripted maps
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp byte[UseDefaultWinLoseEvents], 1
    jnz .out
    
    jmp hackend
    
.out:
    pop edi
    pop esi
    pop ebp
    pop ebx
    add esp, 0x48
    retn

    
hack 0x00453EEE, 0x00453EF6 ; Check default win/lose events if ActiveEventCount is 0
    test al, al
    jbe .NoActiveEvents
    jmp hackend
    
.NoActiveEvents:
    cmp byte[SpawnerActive], 1
    jnz 0x004540D4
    cmp dword[GameType], GT_SINGLEPLAYER
    jz 0x004540D4

    jmp 0x00453F60 ; default win/lose events starting here
 

hack 0x004752FE ; LoadCustomOnlineMapScript
    cmp byte[SpawnerActive], 1
    jnz .out
    push MapScript
    jmp 0x00475303
    
.out:
    push 0x004E7E64
    jmp 0x00475303


hack 0x0047533D ; LoadCustomSkirmishMapScript
    cmp byte[SpawnerActive], 1
    jnz .out
    push MapScript
    jmp 0x00475342
    
.out:
    push 0x004E7E64
    jmp 0x00475342


hack 0x00453BC9, 0x00453BD1 ; EnableMissionEventsInMP
    add esp, 8
    cmp byte[MapScriptExists], 1
    jz 0x00453BE6
    mov al, byte[NetworkGame]
    jmp 0x00453BD1

    
hack 0x00454446, 0x0045444D ; game over when the mission lose event gets triggered
    mov byte[Lose], 1
    
    cmp byte[SpawnerActive], 1
    jnz hackend
    cmp byte[NetworkGame], 1
    jnz hackend
    mov byte[GameOver], 1
    jmp hackend


hack 0x004543AB ; game over when mission win event gets triggered (this is only needed for spectators)
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp byte[NetworkGame], 1
    jnz .out
    cmp byte[MeIsSpectator], 1
    jnz .out
    mov byte[GameOver], 1

.out:
    mov al, byte[Lose]
    jmp hackend
    