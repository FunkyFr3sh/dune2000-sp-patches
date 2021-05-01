%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"

sstring EmptyString, ""

; On the score screen there's a check to see if you have a > 2 side, jump past it!
@CLEAR 0x0043DA29, 0x90, 0x0043DA41 ; SubHousesScoreFix

; For playing a unit response, there's another check to see if your side is > 2, if so we should default it to 2 (Ordos)
hack 0x0049F25C, 0x0049F26E ; SubHousesPlayUnitResponseFix
    mov ebx,0x00000002
    jmp hackend

; When interacting with the sidebar (i.e. building a unit), this only works if side is <= 2, so disable the if side == 2 check, so sides that are not 0 or 1 will use 2 (Ordos)
@CLEAR 0x0046F919, 0x90, 0x0046F91C ; SubHousesSidebarResponseFix

hack 0x00435F1A, 0x00435F20 ; LoadCustomInGameBriefingMenuFiles
    cmp byte[MySideID], 0
    jz .Atreides
    cmp byte[MySideID], 1
    jz .Harkonnen
    cmp byte[MySideID], 2
    jz .Ordos
    cmp byte[MySideID], 3
    jz .Emperor
    cmp byte[MySideID], 4
    jz .Fremen
    cmp byte[MySideID], 5
    jz .Smugglers
    cmp byte[MySideID], 6
    jz .Mercenaries
    
.Atreides:
    mov bl, 'A'
    jmp .out
    
.Harkonnen:
    mov bl, 'H'
    jmp .out
    
.Ordos:
    mov bl, 'O'
    jmp .out
    
.Emperor:
    mov bl, 'E'
    jmp .out
    
.Fremen:
    mov bl, 'F'
    jmp .out
    
.Smugglers:
    mov bl, 'S'
    jmp .out
    
.Mercenaries:
    mov bl, 'M'
    
.out:
    test cl, cl
    je 0x00435F44
    xor ecx, ecx
    jmp 0x00435F20
    

@CLEAR 0x0048DE1E, 0x90, 0x0048DE33 ; Don't exit if sound does not exist
@PATCH 0x0048DE1E
    pop esi
    mov eax, EmptyString
    retn
@ENDPATCH

@CLEAR_INT 0x00449CED, 0x00449CFF
@SJMP 0x00449CED, 0x00449D26 ; Special case for mission 9 (the final mission), do not play the video and go to the score screen instead

@CLEAR_INT 0x00481722, 0x00481735 ; Every mission loads its own music track, sub houses don't have a music track saved and the game would crash without this fix
@PATCH 0x00481722
    mov al, 0
    pop esi
    retn
@ENDPATCH
