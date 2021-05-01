%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"


hack 0x00483189, 0x0048318F ; Clear the message once sent to prevent people from spamming the same message over and over
    cmp byte[NetMessageString], 0 
    jz hackend

    push eax
    call CUIManager__SendInGameMsgTo
    mov byte[NetMessageString], 0
    jmp hackend
