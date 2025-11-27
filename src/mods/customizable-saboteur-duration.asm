%include "macros/patch.inc"
%include "dune2000.inc"

; Customizable Saboteur cloaking capacity
hack 0x0041D40D, 0x0041D413 ; CAI__MonitorGroups
    xor eax, eax
    mov al, [esi+18h] ; Get unit type
    shl eax, 8
    mov al, [_templates_unitattribs + eax + 0xC1] ; Get StorageCapacity
    test al, al
    jnz .nonzero
    mov al, 160
.nonzero:
    cmp byte [esi+29h], al
    jb 0x0041D41F ; Instruction replaced by the long jump
    jmp hackend
