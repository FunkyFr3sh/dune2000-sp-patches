%include "macros/patch.inc"
%include "dune2000.inc"

; Fix AI getting stuck in infinite loop building and cancelling concrete when it's about to rebuild or build a new building
@PATCH 0x0041F28B ; CAI_PlaceBuiltBuilding
    db 0xE8 ; Modify target of conditional jump to include instruction ai->c_field_1F = 1;
@ENDPATCH

@PATCH 0x0041F420 ; CAI_PlaceBuiltBuilding
    db 0x38 ; Modify target of conditional jump to include call to Generate37Order_BuildingCancel before the function returns
    db 0x00 ; So when the concrete could not be placed in this run it's cancelled and makes place for building of actual building in the next run
@ENDPATCH
