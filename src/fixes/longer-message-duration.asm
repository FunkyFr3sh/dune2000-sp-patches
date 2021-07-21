%include "macros/patch.inc"

; Change the default message duration from 200 to 400 ticks (also for built-in messages)
@PATCH 0x004257F1 ; QueueMessage
    dd 400
@ENDPATCH
