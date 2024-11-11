%include "macros/patch.inc"
%include "patch.inc"

; Crates can be picked up by a carryall dropping a unit on them
hack 0x00498865, 0x0049886B ; UpdateUnit
    and ebx, 0FFh ; instruction replaced by the long jump
    push ecx
    push edx
    push ebx ; side_id
    push edi ; unit
    call Mod__PickupCrate
    add esp, 8
    pop edx
    pop ecx
    jmp hackend

; Superseded by Mod__UnitDeliver
;hack 0x0049E5A9, 0x0049E5AF ; UnitDeliver
;    mov al, [edi+54h] ; instruction replaced by the long jump
;    mov [esi+54h], al ; instruction replaced by the long jump
;    mov eax, [esp+28h]
;    push ecx
;    push edx
;    push eax ; side_id
;    push esi ; unit
;    call Mod__PickupCrate
;    add esp, 8
;    pop edx
;    pop ecx
;    jmp hackend
