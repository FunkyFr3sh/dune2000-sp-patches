%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

hack 0x00441CC5, 0x00441CCC ; Skirmish/SinglePlayer load saved game function
%define saveGamePath esp+0x200+0x18
    lea eax, [saveGamePath]
    pushad
    push eax
    call LoadSavedGame
    add esp, 4
    popad
    
    lea edi, [esp+0x154]
    jmp hackend

    
hack 0x00441549, 0x00441550  ; Skirmish/SinglePlayer save game function
%define saveGamePath esp+0x140+0x30
%define saveGameName esp+0x140
    lea edx, [saveGamePath]
    mov eax, dword[saveGameName]
    pushad
    push eax
    push edx
    call WriteSaveGame
    add esp, 8
    popad

    push esi
    push 1
    lea edx, [esp+0x14]
    jmp hackend

    
hack 0x0047555C, 0x00475562 ; Triggers when the game-play starts
    pushad
    call GamePlayStart
    popad
    
    cmp dword[GameType], ebx
    jmp hackend
