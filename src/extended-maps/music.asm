%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"
%include "utils.inc"

@CLEAR 0x00448CF7, 0x90, 0x00448CF9 ; do not use hardcoded sound track name
@CLEAR 0x00448D25, 0x90, 0x00448D27 ; do not use hardcoded sound track name
@CLEAR 0x00448D31, 0x90, 0x00448D33 ; do not use hardcoded sound track name
@CLEAR 0x00476630, 0x90, 0x00476634 ; Show all tracks in the music player listbox - FindFirst - do not exit if music name is not in string table
@CLEAR 0x004766C6, 0x90, 0x004766CA ; Show all tracks in the music player listbox - FindNext - do not exit if music name is not in string table

hack 0x0046FFE4, 0x0046FFEA ; Do not repeat the same song over and over
    cmp byte[RandomMusicEnabled], 1
    jnz .out
    cmp dword[GameState], GS_MAINLOOP ; Use random songs only in-game, never in the menus
    jnz .out
    
    mov eax, dword[esi+0x1A8]
    xor edx, edx
    inc eax
    mov ecx, 3
    div ecx
    mov dword[esi+0x1A8], edx
    pop esi
    pushad
    call PlayRandomMusic
    popad
    retn

.out:
    mov eax, dword[esi+0x1AC]
    jmp 0x0046FFEA

    
hack 0x004700FC, 0x00470102 ; Sound__PlayMusic() - Save current soundtrack
    mov esi, dword[esp+8] ; FileName
    pushad
    push esi
    call PathGetFileNameWithoutExtension
    add esp, 4
    push eax
    push CurrentSoundTrack
    call strcpy
    add esp, 8
    popad

    test al, al
    jmp hackend

    
hack 0x0047663C, 0x00476647 ; Show all tracks in the music player listbox - FindFirst - use filename as listbox entry
    push eax
    call Data__GetTextId
    add esp, 4
    cmp eax, 0 ; 0 = string does not exist in string table
    jnz .out
    lea eax, [esp+0x1C+6] ; +6 to cut of "music_"
    jmp 0x00476650
    
.out:
    push 1
    jmp hackend

    
hack 0x004766D2, 0x004766DD ; Show all tracks in the music player listbox - FindNext - use filename as listbox entry
    push ecx
    call Data__GetTextId
    add esp, 4
    cmp eax, 0 ; 0 = string does not exist in string table
    jnz .out
    lea eax, [esp+0x1C+6] ; +6 to cut of "music_"
    jmp 0x004766E6
    
.out:
    push 1
    jmp hackend

    
hack 0x00481486, 0x0048148F ; current music track over, use random new track if last one was a custom one
    cmp eax, 0
    jnz .out
    call PlayRandomMusic
    jmp hackend
    
.out:
    push eax
    call Sound__PlayMusic
    add esp, 4
    jmp hackend
