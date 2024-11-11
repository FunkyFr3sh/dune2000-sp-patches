%include "macros/patch.inc"
%include "inc/dune2000.inc"
%include "inc/patch.inc"

; Add rule showNeutralBecomeHostileMsg to enable of disable showing of "NeutralBecomeHostile" message
hack 0x0049F898, 0x0049F8A0 ; NeutralBecomeHostile
    cmp dl, [gSideId] ; Instruction replaced by long jump
    jnz 0x49F8DF ; Instruction replaced by long jump
    cmp byte [rulesExt__showNeutralBecomeHostileMsg], 0
    je 0x49F8DF
    jmp hackend
