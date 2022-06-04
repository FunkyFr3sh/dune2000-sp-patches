%include "macros/patch.inc"

; Make top row of map buildable
; Remove special checks for Y = 0 which prevent placing buildings there
@CLEAR 0x004562D1, 0x90, 0x004562D9 ; ModelAddConcrete

; Superseded by Mod__HandleBuildingPlacement
;@CLEAR 0x0042867A, 0x90, 0x0042868F ; HandleBuildingPlacement
;@CLEAR 0x00428830, 0x90, 0x00428838 ; HandleBuildingPlacement

@CLEAR 0x00428992, 0x90, 0x004289A7 ; HandleConcretePlacement
@CLEAR 0x00428B5D, 0x90, 0x00428B63 ; HandleConcretePlacement

@CLEAR 0x00428C47, 0x00, 0x00428C48 ; getownershipstatusofcell

@CLEAR 0x0044512E, 0x00, 0x0044512F ; HandleGameLoopEvents
