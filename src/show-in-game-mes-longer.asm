%include "macros/patch.inc"

hack 0x004256FD, 0x00425704 ; ShowChatMessagesLonger
    mov edx, dword[ecx*4+0x50403C]
    add edx, 200
    jmp 0x00425704
