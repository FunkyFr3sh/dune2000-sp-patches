%include "macros/patch.inc"
%include "dune2000.inc"

; Fix the bug when some units are picked before placing order and starport is sold, the credits are returned twice.
hack 0x0046DC89, 0x0046DC8E ; CSide__FindOtherPrimaryBuilding
    xor ecx, ecx
    mov cl, [esi+24252h] ; side_id
    push ecx
    call IsAnyStarportUnitPicked
    test al, al
    jz .Skip
    call ModelStarportCancel
.Skip:
    add esp, 4
    mov ecx, esi
    call CSide__ReturnMoneyFromStarportOrder ; instruction replaced by the long jump
    jmp hackend
