%include "macros/patch.inc"
%include "inc/patch.inc"

; Guard mode radius can be customized by map rule guardModeRadius
hack 0x00498CE0, 0x00498CE5 ; UpdateUnit
    mov eax, [rulesExt__guardModeRadius]
    push eax
    jmp hackend
