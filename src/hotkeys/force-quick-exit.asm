%include "macros/patch.inc"
%include "macros/datatypes.inc"

gbool ForceQuickExit, false

hack 0x0044A1BD ; Exit instantly (do not wait 10seconds) - This is currently only used when ESC was pressed while the reconnect screen is showing
    cmp byte[ForceQuickExit], 1
    jz 0x0044A1DE
    
    call 0x0045A4D0
    jmp hackend
