%include "macros/patch.inc"
%include "dune2000.inc"

; Fix AI not always ignoring units with flag11 ("Delivering/No AI")

; Do not assign any group ID to units with this flag
hack 0x0041E710, 0x0041E716 ; w_CAI::DoRepairUnits
  jbe 0x41E7DF ; instruction replaced by the long jump
  test dword [esi+8], 0x840 ; test unit flags
  jnz 0x41E7DF
  jmp hackend

; Do not send units with this flag to do defence
hack 0x004233A3, 0x004233A8 ; w_CAI::SendNearestUnit
  mov al, [esi+28h] ; instruction replaced by the long jump
  cmp al, cl ; instruction replaced by the long jump
  jnz 0x4233EF
  test dword [esi+8], 0x840 ; test unit flags
  jmp hackend
