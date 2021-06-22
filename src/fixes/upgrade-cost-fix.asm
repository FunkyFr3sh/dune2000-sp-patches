%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

hack 0x0044BDCA, 0x0044BDDD ; HandleTooltips
    mov ebx, [gSideId]
    push ebx ; side_id
    inc ecx
    push ecx ; num_upgrades
    push eax ; building_type
    call GetBuildingCost
    add esp, 12
    mov ecx, eax
    jmp hackend

