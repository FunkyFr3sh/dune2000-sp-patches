%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

sstring debugTxtPath, "Dune2000Debug.txt"
sstring fileAccess, "a"
sint debugTxt, 0

hack 0x004A4587
    lea ecx, [esp+0x4]
    
    pushad
    push ecx
    
    push fileAccess
    push debugTxtPath
    call fopen
    add esp, 8
    cmp eax, 0
    jz .out
    mov dword[debugTxt], eax
    
    push eax
    call fprintf
    add esp, 8
    
    push dword[debugTxt]
    call fclose
    add esp, 4
    
.out:
    popad
    
    push ecx
    jmp hackend
