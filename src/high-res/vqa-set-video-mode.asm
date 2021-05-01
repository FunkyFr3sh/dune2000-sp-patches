%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"
%include "dune2000.inc"

hack 0x00403687, 0x00403690 ; SetVideoMode() - Try to set a low resolution to get the videos as big as possible
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    mov eax, dword[VQADisplayModeHeight]
    mov edx, dword[VQADisplayModeWidth]
    mov dword[esp+0x18], edx
    jmp hackend
    
.out:
    mov eax, dword[ScreenHeight]
    mov edx, dword[esp+0x18]
    jmp hackend
