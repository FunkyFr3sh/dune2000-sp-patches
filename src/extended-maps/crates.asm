%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"
%include "dune2000.inc"

; In function DamageTiles - overwrite the code which takes care of blowing up spice blooms when shot
hack 0x0049D326, 0x0049D394
    mov eax, [esp+1Ch]
    and eax, 0FFh
    push eax ; crate_index
    call HitCrate
    add esp, 4
    jmp hackend
