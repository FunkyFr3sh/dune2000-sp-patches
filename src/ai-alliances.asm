%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

;This patch enables AI Alliances via the the "a" hotkey in skirmish games

hack 0x00445EB8 ; "A" Key pressed, remove check for NetworkGame
    cmp dword[GameType], GT_SKIRMISH
    jz 0x00445EC1

    mov al, byte[NetworkGame]
    jmp hackend

    
hack 0x00458125, 0x0045812C ; Not sure about this one, seems to check if the player is an AI player
    cmp dword[GameType], GT_SKIRMISH
    jz 0x00458134
    
    mov al, byte[edx*8+0x4F4119]
    jmp hackend
    
    
hack 0x004581F1, 0x004581F9 ; Force the AI players to ally you too
    mov byte[edx*8+eax+Diplomacy], 0
    cmp dword[GameType], GT_SKIRMISH
    jnz hackend
    
    mov byte[eax*8+edx+Diplomacy], 0 ; revert it
    jmp hackend
