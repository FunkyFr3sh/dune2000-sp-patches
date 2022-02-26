%include "macros/patch.inc"
%include "dune2000.inc"

; When tech is lowered by event, running building upgrades which are no longer possible should get cancelled
hack 0x00446B63, 0x00446B6C ; DoIconBuild
    push eax
    push ecx
    push edx
    push ebp ; building_group
    push ebx ; side_id
    call CanSideUpgradeBuildingGroup
    add esp, 8
    pop edx
    pop ecx
    test al, al
    pop eax
    jmp hackend
