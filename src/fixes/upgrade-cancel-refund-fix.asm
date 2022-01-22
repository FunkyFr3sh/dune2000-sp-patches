%include "macros/patch.inc"

; Fix returning wrong amount of credits from cancelled second and third building upgrade
@CLEAR 0x004574C1, 0x90, 0x004574F0 ; Remove call of GetBuildingCost function and computation of cost to return

@PATCH 0x004574C1 ; ModelUpgradeCancel
    mov ecx, [esi+26604h] ; __BuildingUpgradeQueue.dw_field_C_credits
@ENDPATCH
