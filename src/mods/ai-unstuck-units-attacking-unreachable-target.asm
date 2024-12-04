%include "macros/patch.inc"

; Unstuck AI units ordered to attack unreachable target
hack 0x00493C89, 0x00493C8F ; GetNextSquare
    mov [esi+48h], dl ; Instruction replaced by long jump
    mov [esi+49h], cl ; Instruction replaced by long jump
    mov edi, [esp+30h] ; get side_id
    push eax
    push edx
    mov eax, 7608
    mul edi
    add eax, 0x4F4119 ; get address of _gAIArray[side_id].__IsAI
    mov cl, [eax]
    pop edx
    pop eax
    test cl, cl ; check if AI is enabled on the side
    jz hackend
    mov dword [esi+10h], 2 ; Set unit state to 2 (Idle)
    mov byte [esi+28h], 0xFF ; Set GroupID to -1
    jmp hackend
