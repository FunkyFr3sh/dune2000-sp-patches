%include "macros/patch.inc"
%include "dune2000.inc"

; Allow infantry to move on tile where some infantry is already standing (with Alt key)

; Handle mouse click action
hack 0x004456E3, 0x004456E9 ; HandleGameLoopEvents
    jnz 0x445869 ; instruction replaced by the long jump
    cmp byte [0x797E8A], 0 ; Check for Alt pressed
    je hackend
    ; Unit belongs to my side and Ctrl is not pressed and Alt is pressed
    ; Check if any infantry is selected
    push ecx
    call IsAnyInfantrySelected
    pop ecx
    test al, al
    jz hackend
    ; Check if target unit is infantry
    xor edx, edx
    mov dl, [ecx+18h] ; Get unit type
    shl edx, 8
    cmp byte [0x77E261+edx], 0 ; Is infantry?
    je hackend
    ; All criteria met, generate move order
    jmp 0x445D6C

; Handle mouse cursor
hack 0x00443643, 0x00443649 ; HandleGameLoopEvents
    jnz 0x443650 ; instruction replaced by the long jump
    test dl, dl ; instruction replaced by the long jump 
    jnz 0x443650 ; instruction replaced by the long jump
    cmp byte [0x797E8A], 0 ; Check for Alt pressed
    je hackend
    ; Unit belongs to allied side and Ctrl is not pressed and Alt is pressed
    ; Check if target unit is mine
    mov cl, [gSideId]
    cmp [esp+10h], cl
    jne hackend
    ; Check if any infantry is selected
    push eax
    call IsAnyInfantrySelected
    test al, al
    pop eax
    jz hackend
    ; Check if target unit is infantry
    xor edx, edx
    mov dl, [eax+18h] ; Get unit type
    shl edx, 8
    cmp byte [0x77E261+edx], 0 ; Is infantry?
    je hackend
    ; All criteria met, show move cursor
    jmp 0x443684
