%include "macros/patch.inc"
%include "inc/patch.inc"

; Always show radar map if rule alwaysShowRadar is set to true
hack 0x0044905D, 0x00449062 ; GameLoop
    add esp, 8 ; Instruction replaced by long jump
    cmp byte [rulesExt__alwaysShowRadar], 0
    jne 0x44906D
    test al, al ; Instruction replaced by long jump
    jmp hackend

; Allow unit move cursor over minimap and clicking to minimap
; Superseded by Mod__HandleGameLoopEvents
;hack 0x00443CD0, 0x00443CD5 ; HandleGameLoopEvents
;    add esp, 8 ; Instruction replaced by long jump
;    cmp byte [rulesExt__alwaysShowRadar], 0
;    jne 0x443CE4
;    test al, al ; Instruction replaced by long jump
;    jmp hackend

; Superseded by Mod__HandleGameLoopEvents
;hack 0x004440C8, 0x004440CD ; HandleGameLoopEvents
;    add esp, 8 ; Instruction replaced by long jump
;    cmp byte [rulesExt__alwaysShowRadar], 0
;    jne 0x4440E0
;    test al, al ; Instruction replaced by long jump
;    jmp hackend

; Superseded by Mod__HandleGameLoopEvents
;hack 0x00445084, 0x00445089 ; HandleGameLoopEvents
;    add esp, 8 ; Instruction replaced by long jump
;    cmp byte [rulesExt__alwaysShowRadar], 0
;    jne 0x44509C
;    test al, al ; Instruction replaced by long jump
;    jmp hackend

; Play S_POWERUP sound instead of S_RADARONLINE sound
hack 0x0044AB6A, 0x0044AB6F ; DoPower
    add esp, 8 ; Instruction replaced by long jump
    cmp byte [rulesExt__alwaysShowRadar], 0
    je .Skip
    xor eax, eax
.Skip:
    test al, al ; Instruction replaced by long jump
    jmp hackend
    
hack 0x0044AB9B, 0x0044ABA0 ; DoPower
    add esp, 8 ; Instruction replaced by long jump
    cmp byte [rulesExt__alwaysShowRadar], 0
    je .Skip2
    xor eax, eax
.Skip2:
    test al, al ; Instruction replaced by long jump
    jmp hackend
