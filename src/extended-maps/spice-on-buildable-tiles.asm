%include "macros/patch.inc"

; Spice works properly in tilesets where spice and buildings can be placed on same tiles (i.e. Red Alert, Warcraft 2...)
; (Buildings cannot be placed on spice, but can be placed on tiles where spice was harvested)

; Do not hard-change tile attributes to constant value for special values 1 and 2, but only set respective spice flag
hack 0x00469836, 0x00469847 ; setupmapstuff
    mov ecx, [eax+0x517DF4]
    or ecx, 0x200000
    mov [eax+0x517DF4], ecx
    jmp hackend

hack 0x00469852, 0x00469865 ; setupmapstuff
    mov ecx, [eax+0x517DF4]
    or ecx, 0x400000
    mov [eax+0x517DF4], ecx
    jmp hackend
    
; Display red building placement marker on tiles with spice
@PATCH 0x00428B3C ; HandleConcretePlacement
    test eax, 0x7013F8
@ENDPATCH

@PATCH 0x0042881C ; HandleBuildingPlacement
    test eax, 0x7013F8
@ENDPATCH

; Do not allow placing concrete on tiles with spice
@PATCH 0x004562B3 ; AddConcrete
    test ecx, 0x7013F8
@ENDPATCH

; Do not let AI place buildings on tiles with spice
@PATCH 0x00421243 ; w_CAI::CanAIPlaceBuilding
    test eax, 0x7013F8
@ENDPATCH

@PATCH 0x00421BF2 ; w_CAI::FindAlternativePlace
    test dword [ebp*4+0x517DF4], 0x7013F8
@ENDPATCH

; Do not let MCV deploy on tiles with spice AND crate
; This merges the deploy-mcv-on-crate-fix
@PATCH 0x004432AB ; CheckIfMCVCanBeDeployedOn
    test eax, 0x7013F8
@ENDPATCH

@PATCH 0x004230AB ; CAI::CanIDeployMCVHere
    and ebx, 0x70B000
@ENDPATCH

; Let spice mound target spice projectiles only into tiles without building or concrete
hack 0x0044F022, 0x0044F033 ; SpiceMound
    mov eax, [ecx*4+0x517DF4]
    and eax, 0x10810
    cmp eax, 0x10000
    jnz 0x44F0CF
    jmp hackend


