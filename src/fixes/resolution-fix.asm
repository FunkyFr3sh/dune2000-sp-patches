%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"

; allow any resolution
@LJMP 0x004A3E69, 0x004A3E90
