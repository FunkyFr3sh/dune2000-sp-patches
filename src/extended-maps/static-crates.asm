%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"
%include "dune2000.inc"

hack 0x00469A8E, 0x00469A95
    mov eax, [esp+0x10]
    push eax ; y
    push ebx ; x
    push edi ; special_value
    call PlaceStaticCrate
    add esp, 12
    test eax, eax
    jnz 0x469AEA ; skip the original logic
    mov ecx, dword [edi*4+0x6D7DE8] ; instruction which was replaced by long jump
    jmp hackend
