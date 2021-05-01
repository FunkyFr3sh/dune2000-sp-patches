%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"
%include "ini.inc"
%include "macros/datatypes.inc"

sstring ButtonDebug, "BUTTON Debug"
sstring TextUibKey, "TextUib"
sstring SettingsSection, "Settings"


;New game exit stuff ... exit after score screen
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

