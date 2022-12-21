%include "macros/patch.inc"
%include "dune2000.inc"

; Fix orders not being processed after AI on side 0 is deactivated
hack 0x0045D14A, 0x0045D150 ; ProcessOrderQueue
    test al, al ; instruction replaced by the long jump 
    jz 0x45D150 ; instruction replaced by the long jump
    cmp byte [_IsMultiplayer],0 ; Do not set ai_order = 1 if it's not multiplayer game
    je 0x45D150    
    mov bl, 1 ; instruction replaced by the long jump 
    jmp hackend
