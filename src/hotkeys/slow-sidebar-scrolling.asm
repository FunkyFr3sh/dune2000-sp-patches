%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "hotkeys.inc"

gbool SlowSideBarScrolling, false

hack 0x0044401E, 0x00444023 ; Scroll Sidebar up one by one instead of scrolling multiple items at the same time
    cmp byte[SlowSideBarScrolling], 1
    jnz .out
    mov byte[KeyIsDown2(VK_UP)], 0
    
.out:
    test eax, eax
    jle 0x00444028
    dec eax
    jmp 0x00444023


hack 0x00443FDF, 0x00443FE5 ; Scroll Sidebar down one by one instead of scrolling multiple items at the same time
    cmp byte[SlowSideBarScrolling], 1
    jnz .out
    mov  byte[KeyIsDown2(VK_DOWN)], 0
    
.out:
    mov eax, dword[ebp+0x260B4]
    jmp 0x00443FE5
