%include "macros/patch.inc"
%include "macros/extern.inc"
%include "dune2000.inc"
%include "patch.inc"

hack 0x004A3C7B, 0x004A3C85 ; FixWin8CreateWindowBug
    push dword[ScreenHeight]
    push dword[ScreenWidth]
    jmp 0x004A3C85


hack 0x004A3DB5, 0x004A3DBB ; CallSetDisableMaxWindowedMode
    pushad
    call SetDisableMaxWindowedMode
    popad
    push ebx
    push 0x007984F4
    jmp hackend
