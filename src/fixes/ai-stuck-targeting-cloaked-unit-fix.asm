%include "macros/patch.inc"

; Fix AI units getting stuck on targeting a cloaked unit (when doing harvester protection)
@PATCH 0x004237EB ; CAI__ValidateTargetUnitOfGroup
    dd 0x400144 ; Add flag UFLAGS_4_CLOAKED to check for flags on which the group's target unit is cleared
@ENDPATCH
