%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"
%include "dune2000.inc"

; Overwrite the code which takes care of blowing up spice blooms when shot
hack 0x0049D326, 0x0049D394 ; DamageTiles
    mov eax, [esp+1Ch]
    and eax, 0FFh
    push eax ; crate_index
    call HitCrate
    add esp, 4
    jmp hackend

; Overwrite the code which takes care of exploding large spice bloom by itself after timer has expired
hack 0x0044EAE8, 0x0044EAF4 ; ProcessCrates
    mov edx, esi
    sub edx, 2 ; Pointer to crate
    push 0 ; side_id
    push 0 ; unit
    push 10 ; crate_type
    push edx ; crate
    call HandleSpiceBloomCrate
    add esp, 16
    jmp hackend
