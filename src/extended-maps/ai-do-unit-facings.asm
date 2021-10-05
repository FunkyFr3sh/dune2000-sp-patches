%include "macros/patch.inc"

; Do not face units away from construction yard, if the property DontFaceUnitsAwayFromConYard is set
hack 0x00420C80, 0x00420C85 ; CAI__DoUnitFacings
    cmp byte [ecx + 0x108B], 0 ; Check for DontFaceUnitsAwayFromConYard property
    je .Noskip
    retn 4
.Noskip:    
    ; Instructions replaced by the long jump
    sub esp, 8
    push ebx
    push esi
    jmp hackend
