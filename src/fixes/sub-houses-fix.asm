%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "inc/utils.inc"

sstring EmptyString, ""

; On the score screen there's a check to see if you have a > 2 side, jump past it!
@CLEAR 0x0043DA29, 0x90, 0x0043DA41 ; SubHousesScoreFix

; Fix Enemy house not valid error on score screen
hack 0x0043D86F, 0x0043D876 ; CreateScore
    mov al, [ecx * 4 + 0x797C01]
    cmp al, 2
    jbe .Skip
    mov al, 2
.Skip:
    jmp hackend

; Skip the error message "Unknown side %d for losing movie" when losing a mission playing as side > 2
@CLEAR 0x0043C111, 0x90, 0x0043C124 ; LoadMovie

; Fix the error "CreateGlobe()", "Do not recognise gSideId %d" - reset Side ID to 2 if it's higher than 2 before calling CreateGlobe
hack 0x00436162, 0x00436167 ; CInterface::CInterface
    xor eax, eax
    mov al, [gSideId]
    cmp eax, 2
    jbe .Skip
    mov byte [gSideId], 2
.Skip:
    call CreateGlobe
    jmp hackend

; For playing a unit response, there's another check to see if your side is > 2, if so we should default it to 2 (Ordos)
hack 0x0049F25C, 0x0049F26E ; SubHousesPlayUnitResponseFix
    mov ebx,0x00000002
    jmp hackend

; Fix wrong sound played when clicking on unit with repair cursor and playing as Emperor or higher
hack 0x00445303, 0x00445309 ; HandleGameLoopEvents
    mov dl,[_IRValues + eax]
    cmp dl,2
    jbe .Skip
    mov dl,2
.Skip:
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
    
; Fix in-game briefing lockup: Default to IGA0MIS.UIL file if respective UIL file does not exist
sstring DefaultBriefingUilFileName, "IGA0MIS.UIL"
sstring BriefingUilFilePathFormat, ".\\%sUI_DATA\\%s"
sbyte BriefingUilFilePath, 0, 256
hack 0x00435F36, 0x00435F3C ; CInterface::CInterface
    mov eax, [ebp+654h] ; instruction replaced by the long jump
    ;sprintf(BriefingUilFilePath, ".\\%sUI_DATA\\%s", ResourcePath, eax);
    push eax
    push ResourcePath
    push BriefingUilFilePathFormat
    push BriefingUilFilePath
    call sprintf
    add esp, 16
    push BriefingUilFilePath
    call FileExists
    add esp, 4
    test eax, eax
    jz .dontexists
    mov eax, [ebp+654h]
    jmp hackend
.dontexists:
    mov eax, [ebp+654h]
    push DefaultBriefingUilFileName
    push eax
    call strcpy
    pop eax
    add esp, 4
    jmp hackend

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
