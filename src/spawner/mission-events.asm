%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

; This patch enables mission events in multi-player/skirmish mode

gbool MapScriptExists, false
gbool UseDefaultWinLoseEvents, false
gstring MapScript, "", 50


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


hack 0x00454446, 0x0045444D ; game over when the mission lose event gets triggered
    mov byte[gLose], 1
    
    cmp byte[SpawnerActive], 1
    jnz hackend
    cmp byte[NetworkGame], 1
    jnz hackend
    mov byte[_GameOver], 1
    jmp hackend


hack 0x004543AB ; game over when mission win event gets triggered (this is only needed for spectators)
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp byte[NetworkGame], 1
    jnz .out
    cmp byte[MeIsSpectator], 1
    jnz .out
    mov byte[_GameOver], 1

.out:
    mov al, byte[gLose]
    jmp hackend
    