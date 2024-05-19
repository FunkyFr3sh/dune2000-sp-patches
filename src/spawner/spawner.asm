%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"
%include "ini.inc"
%include "macros/datatypes.inc"

sstring ButtonDebug, "BUTTON Debug"
sstring TextUibKey, "TextUib"
sstring SettingsSection, "Settings"
sstring SpawnerMapsResourcePath, "data\maps\"

hack 0x0042BA0B, 0x0042BA13 ; force map resource path when the spawner is active
    call 0x0042BBE0
    add esp, 0x0C
    
    cmp byte[SpawnerActive], 1
    jnz hackend
    pushad
    push SpawnerMapsResourcePath
    push MapsResourcePath
    call strcpy
    add esp, 8
    popad
    jmp 0x0042BA3F


hack 0x0045A942, 0x0045A94C ; Spawner_Settings
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SKIRMISH
    jz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jz .out
    mov dword [InitialConnectTimeOut],25000
    mov dword [ReconnectTimeOutToPlayer],15
    
    push eax
    mov eax, dword [SpawnerMaxAhead]
    mov dword [SendRate], eax ; 100=100ms, 125=250ms, 150=300ms, 175=400ms, 200=560ms
    pop eax
    
    mov dword [UnknownNetVar1],180
    mov dword [UnknownNetVar2],40
    mov dword [UnknownNetVar3],300
    mov dword [UnknownNetVar4],1500
    mov dword [UnknownNetVar5],0x40066666
    jmp 0x0045AA2C
    
.out:
    mov dword [InitialConnectTimeOut], 0x0EA60
    cmp eax, 3
    jmp 0x0045A94C

;Old Game exit stuff ... wont show score screen
%if 0
hack 0x0044A2CC, 0x0044A2D3 ; ExitAfterOnlineGame
    cmp byte[SpawnerActive], 1
    jnz .out
    mov dword[GameState], GS_QUIT
    jmp 0x0044A42C
    
.out:
    cmp dword[GameType], GT_WOL
    jmp 0x0044A2D3
%endif

;New game exit stuff ... exit after score screen
%if 1
hack 0x00482850, 0x00482856 ; ExitAfterScore
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameState], GS_BRIEFING
    jnz .out
    mov dword[GameType], GT_SKIRMISH
    mov dword[GameState], GS_QUIT
    
.out:
    sub esp, 0x0D0
    jmp 0x00482856


hack 0x00473E51 ; ExitIfNoScoreShown
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameState], GS_BRIEFING
    jnz .out
    mov dword[GameType], GT_SKIRMISH
    mov dword[GameState], GS_QUIT
    jmp 0x00474085
    
.out:
    push 0x004DCE30
    jmp 0x00473E56
    

hack 0x00473DF7 ; ExitIfNoScoreShownSP
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameState], GS_BRIEFING
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jnz .out
    mov dword[GameState], GS_QUIT
    jmp 0x00474085
    
.out:
    push 0x004DBDCC
    jmp 0x00473DFC

hack 0x0044A440, 0x0044A447 ; ExitIfLoseSP2 ; works also for skirmish
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jnz .out
    cmp byte[MySideID], 2
    ja .GameExit
    cmp byte[Lose], 1
    jnz .out
    
.GameExit:
    mov dword[GameState], GS_QUIT
    
.out:
    cmp dword[GameState], GS_BRIEFING
    jmp 0x0044A447
%endif

hack 0x00407F6B ; CustomListenPort
    cmp byte[SpawnerActive], 1
    jnz .out
    push dword[ListenPort]
    jmp 0x00407F70
    
.out:
    push 0x4D2
    jmp 0x00407F70

hack 0x0047564E, 0x00475655 ; DoNotDecideSideIDbyMapNameInSinglePlayer
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jnz .out
    cmp byte[MySideID], 0
    jz 0x00475669
    cmp byte[MySideID], 1
    jz 0x0047567B
    cmp byte[MySideID], 2
    jz 0x0047568E
    jmp 0x004756AC
    
.out:
    movsx ecx, byte[ebp]
    add ecx, -0x41
    jmp 0x00475655


hack 0x00475646, 0x0047564B ; DoNotDecideMissionNumberbyMapNameInSinglePlayer
    cmp byte[MissionNumber], 9 ; Original missions will never go above 9, can only be a custom mission
    ja 0x0047564B

    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jnz .out
    jmp 0x0047564B
    
.out:
    mov byte[MissionNumber], AL
    jmp 0x0047564B

;removes the debug button gruntlord added as a special feature for SP
hack 0x00439241, 0x00439246 ; RemoveDebugMenuButton
    lea ecx, [esp+0x10]
    cmp byte[SpawnerActive], 1
    jnz .out
    cmp dword[GameType], GT_SINGLEPLAYER
    jz .out
    pushad
    push ButtonDebug
    push ecx
    call strcmpi
    add esp, 8
    cmp al, 0
    popad
    jz 0x00439562
    
.out:
    push ebp
    jmp 0x00439246


hack 0x004486AE ; CallInitHouses
    pushad
    call InitHouses
    popad
    push 0x004DF02C
    jmp 0x004486B3

hack 0x00481E23, 0x00481E29 ; Load custom string table for add-on campaigns
    jne 0x00481F76
    cmp byte[SpawnerActive], 1
    jnz 0x00481E29
    pushad
    push 256
    push TextUib
    push TextUib
    push TextUibKey
    push SettingsSection
    call SpawnIniGetString
    add esp, 20
    popad
    jmp 0x00481E29

;clean up game the StartINetGame functions, keep only the stuff we need for the spawner (this prolly breaks WOL, but who cares!)
;Guest function
@CLEAR 0x00417C57, 0x90, 0x00417C7D
@CLEAR 0x00417C8E, 0x90, 0x00417C99
@CLEAR 0x00417C9B, 0x90, 0x00417C9F
@CLEAR 0x00417CBB, 0x90, 0x00417CC0
@CLEAR 0x00417CC2, 0x90, 0x00417CCA
@CLEAR 0x00417CEE, 0x90, 0x00417DD0

;host function
@CLEAR 0x00417E60, 0x90, 0x00417E7C
@CLEAR 0x00417E7D, 0x90, 0x00417E7F
@CLEAR 0x00417E80, 0x90, 0x00417E9E
@CLEAR 0x00417EA4, 0x90, 0x00417EA5
@CLEAR 0x00417EAC, 0x90, 0x00417EBA
@CLEAR 0x00417ED8, 0x90, 0x00417EDD
@CLEAR 0x00417EDF, 0x90, 0x00417EE3
@CLEAR 0x00417F07, 0x90, 0x00417F62