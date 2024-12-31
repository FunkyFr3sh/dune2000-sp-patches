%include "macros/patch.inc"

; Increase number of sound channels from 6 to 15 and increase sound cache size from 16 to 40
; Rest is in sample-manager.c

; Change AIL_set_preference parameter from 8 to 16 so that we can allocate 16 channels
@PATCH 0x0046F464 ; InitSampleManager
    db 16
@ENDPATCH

; Increase size of ISampleManager struct by size of additional handles and cachec entries
@PATCH 0x0046F470 ; InitSampleManager
    dd 0x460
@ENDPATCH

; Make it so additional sound channels are shown in DebugPrintOnScreen
@PATCH 0x004253C5 ; DebugPrintOnScreen
    dd 550
@ENDPATCH
