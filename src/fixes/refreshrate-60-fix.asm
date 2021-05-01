%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"

; pass 60 on DirectDraw::SetDisplayMode
@SET 0x004A4175, { push 0x3c }
