%include "macros/patch.inc"
%include "dune2000.inc"

; Fix improper mouse cursor (engineer/saboteur/sell) when building has infiltrated flag
; Also do not order engineer/saboteur to enter building when clicking on building with infiltrated flag

; Saboteur
hack 0x0044380E, 0x00443817 ; HandleGameLoopEvents
  and eax, 0x1000200 ; Additional check for infiltrated flag
  cmp eax, 0x200
  jne 0x4438B0
  jmp hackend

hack 0x00445C55, 0x00445C5E ; HandleGameLoopEvents
  and ecx, 0x1000200 ; Additional check for infiltrated flag
  cmp ecx, 0x200
  jne 0x445D99
  jmp hackend

; Engineer
hack 0x00443845, 0x0044384A ; HandleGameLoopEvents
  and eax, 0x1000400 ; Additional check for infiltrated flag
  cmp eax, 0x400
  jne 0x4438B0
  jmp hackend

hack 0x00445C99, 0x00445CA2 ; HandleGameLoopEvents
  and ecx, 0x1000400 ; Additional check for infiltrated flag
  cmp ecx, 0x400
  jne 0x445D99
  jmp hackend

; Sell
hack 0x00443C44, 0x00443C4A ; HandleGameLoopEvents
  jz 0x443D3E ; instruction replaced by the long jump
  mov ecx, [esp+18h] ; Building pointer
  mov eax, [ecx+28h]
  test eax, 0x1000000 ; Additional check for infiltrated flag
  jnz 0x443D3E
  jmp hackend
