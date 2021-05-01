%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

gbool UseImprovedFPSLimiter, false

hack 0x0042F80E ; Render() - Limit FPS
    cmp byte[UseImprovedFPSLimiter], 1
    jnz .out
    call FPSLimiter
    
.out:
    mov ecx, 0x1B
    jmp hackend


hack 0x004495EF, 0x004495F5 ; MainLoop() - IDirectDrawSurface::Flip - no DDFLIP_WAIT cpu hog
    add esp, 0x10
    mov ecx, dword[EAX]
    
    cmp byte[UseImprovedFPSLimiter], 1
    jnz .out
    push DDFLIP_DONOTWAIT
    jmp hackend
    
.out:
    push DDFLIP_WAIT
    jmp hackend
    
    
hack 0x00492B0D, 0x00492B14 ; DrawMenu() - IDirectDrawSurface::Flip - no DDFLIP_WAIT cpu hog
    add esp, 0x10
    mov ecx, dword[EAX]
    
    cmp byte[UseImprovedFPSLimiter], 1
    jnz .out
    push DDFLIP_DONOTWAIT
    jmp hackend
    
.out:
    push DDFLIP_WAIT
    jmp hackend
