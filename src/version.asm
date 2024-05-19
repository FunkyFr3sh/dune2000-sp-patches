%include "macros/patch.inc"
%include "macros/datatypes.inc"

sstring GameVersion, "V 1.06p rev.43 - FunkyFr3sh"

hack 0x0048BE05 ;use custom version string and align x
    sub ecx, 120 ;x
    mov ebx, GameVersion
    push ecx
    push ebx
    jmp 0x0048BE0A

    
hack 0x0048BDDD, 0x0048BDE8 ; change the color
    mov edx, 63768 ;light green
    mov dword[esp+0x14], eax
    jmp 0x0048BDE8

    
hack 0x0048BDEE ;align y
    mov edx, dword[esp+0x24]
    add edx, eax
    sub edx, 80 ;y
    jmp 0x0048BDF4
