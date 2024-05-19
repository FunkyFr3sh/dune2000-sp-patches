%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"

;Enable PrePlaced Buildings And Units in multi-player/skirmish games

;remove checks for skirmish/online/lan
@CLEAR 0x00469B7A, 0x90, 0x00469B7F
@CLEAR 0x00469B82, 0x90, 0x00469B98

hack 0x00469D2B, 0x00469D31 ; JumpOutAfterBuildingsAndUnitsWerePlaced
    cmp dword[GameType], GT_SINGLEPLAYER
    jz .out
    mov eax, dword[GameType]
    jmp 0x00469D59
    
.out:
    mov eax, dword[esp+0x30]
    test eax, eax
    jmp 0x00469D31


hack 0x00469CEF, 0x00469CF5 ; DoNotPrePlaceWormsInMP
    mov edi, dword[esp+0x34]
    cmp dword[GameType], GT_SINGLEPLAYER
    jz .out
    push edi
    and edi, 0x000000FF
    shl edi, 8
    cmp byte[edi+0x77E27D], 5
    pop edi
    jz 0x00469D1B
    
.out:
    push 0
    jmp 0x00469CF5
