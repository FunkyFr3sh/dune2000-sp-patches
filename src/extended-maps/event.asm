%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"
%include "dune2000.inc"

hack 0x00454471, 0x00454484 ; remove the unknown event error message and allow using new custom events
    push esi
    call ExecuteCustomEvent
    add esp, 4
    jmp hackend
