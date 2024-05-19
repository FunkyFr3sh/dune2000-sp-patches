%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

gbool DisableEngineer, false
gbool DisableTurrets, false
gbool NoCarryall, false

hack 0x0046950A, 0x00469510 ; OptionDisableUnitsAndBuildings
    cmp byte[SpawnerActive], 1
    jnz .out
    
    cmp byte[NoCarryall], 1
    jnz .turret
    mov byte[0x0077E250+0x1227], 1 
    mov byte[0x0077E250+0x1327], 1 
    mov byte[0x0077E250+0x1427], 0 
    mov byte[0x006DC540+0x5DA0-0x3c00], 0xE8
    mov byte[0x006DC540+0x5DA1-0x3c00], 0x03
    mov byte[0x006DC540+0x5E9D-0x3c00], 0x06 
    mov byte[0x006DC540+0x5EAC-0x3c00], 0xE8
    mov byte[0x006DC540+0x5EAD-0x3c00], 0x03
    mov byte[0x006DC540+0x5FA9-0x3c00], 0x06
    mov byte[0x006DC540+0x5FB8-0x3c00], 0xE8
    mov byte[0x006DC540+0x5FB9-0x3c00], 0x03
    
.turret:
    cmp byte[DisableTurrets], 1
    jnz .engineer
    mov byte[0x006DC540+0x1824], 0xFF
    mov byte[0x006DC540+0x1930], 0xFF
    mov byte[0x006DC540+0x1a3c], 0xFF
    mov byte[0x006DC540+0x1e6c], 0xFF
    mov byte[0x006DC540+0x1f78], 0xFF
    mov byte[0x006DC540+0x2084], 0xFF
    
.engineer:
    cmp byte[DisableEngineer], 1
    jnz .out
    mov byte[0x0077E474], 0xFF
    
.out:
    push esi
    push 0x700
    jmp 0x00469510
