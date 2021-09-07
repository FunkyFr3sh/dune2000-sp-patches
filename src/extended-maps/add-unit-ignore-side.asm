%include "macros/patch.inc"

; If unit is being added on a tile occupied by a building, the game checks if the unit is added 
; on building owned by same side. This patch removes this check so you can add unit on building
; owned by different side.
@CLEAR 0x00455904, 0x90, 0x0045590A ; ModelAddUnit
