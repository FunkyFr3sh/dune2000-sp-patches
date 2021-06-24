%include "macros/patch.inc"

; Fix MCV-deploy check is not considering crates
@PATCH 0x004432AB ; CheckIfMCVCanBeDeployedOn
    test eax, 0x13F8
@ENDPATCH

@PATCH 0x004230AB ; CAI::CanIDeployMCVHere
    and ebx, 0xB000
@ENDPATCH
