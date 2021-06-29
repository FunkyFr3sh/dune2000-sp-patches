%include "macros/patch.inc"
%include "dune2000.inc"

; Fix AI starting building upgrade with zero credits causing upgrade stuck in HOLD state
hack 0x00421C5A, 0x00421C5F ; CAI::UpgradeBuildings
  call GetSide ; instruction replaced by the long jump
  add esp, 4 ; put the instruction here and remove it from the other place
  mov ecx, [eax+0x2425C] ; get CashReal
  add ecx, [eax+0x24254] ; add SpiceReal
  cmp ecx, 100 ; check if you have at least 100 credits
  jb 0x421CE2 ; skip the logic
  jmp hackend

@CLEAR 0x00421C65, 0x90, 0x00421C68 ; Remove add esp, 4 instruction
