%include "macros/patch.inc"

; Fix units with less than one third health moving faster than units with less than half health 
@PATCH 0x004946D9 ; MoveUnit
    fmul dword [0x004D7024]
@ENDPATCH

@PATCH 0x0049470A ; MoveUnit
    fmul dword [0x004D7020]
@ENDPATCH
