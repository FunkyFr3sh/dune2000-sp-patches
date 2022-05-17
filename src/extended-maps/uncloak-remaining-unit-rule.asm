%include "macros/patch.inc"
%include "inc/dune2000.inc"
%include "inc/patch.inc"

; Add rule uncloakRemainingStealthUnit to toggle this functionality
hack 0x00449A92, 0x00449A97 ; GameLoop
    cmp byte [rulesExt__uncloakRemainingStealthUnit], 0
    je hackend
    call UncloakRemainingStealthUnit
    jmp hackend
