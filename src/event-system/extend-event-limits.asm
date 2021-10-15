%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"

; Loading MIS file - Load event and condition arrays into new memory location
@PATCH 0x0044FD39 ; LoadMission
    dd _gEventArray
@ENDPATCH

@PATCH 0x0044FD4E ; LoadMission
    dd _gConditionArray
@ENDPATCH

; Loading MIS file - Load extra data from MIS file
hack 0x0044FDC7, 0x0044FDCC ; LoadMission
    call ReadFile ; Instruction replaced by the long jump
    ; Read extended _gEventCount
    add esp, 10h
    push esi
    push 1
    push 2
    push 0x6B63CC
    call ReadFile
    ; Read extended _gConditionCount
    add esp, 10h
    push esi
    push 1
    push 2
    push 0x6B8260
    call ReadFile
    ; Read extended part of _gConditionArray
    add esp, 10h
    push esi
    push (256 - 48)
    push 28
    push _gConditionArray + (48 * 28)
    call ReadFile
    ; Read extended part of _gEventArray
    add esp, 10h
    push esi
    push (1024 - 64)
    push 72
    push _gEventArray + (64 * 72)
    call ReadFile
    jmp hackend

; Saving game - Save event and condition arrays from new memory location and different amount of data
@PATCH 0x004416D0 ; SaveGame
    push 28 * 256
    push _gConditionArray
@ENDPATCH

@PATCH 0x004416E5 ; SaveGame
    push 72 * 1024
    push _gEventArray
@ENDPATCH

@PATCH 0x004416FB ; SaveGame
    db 2
@ENDPATCH

@PATCH 0x0044170D ; SaveGame
    db 2
@ENDPATCH

; Loading game - Load event and condition arrays into new memory location and different amount of data
@PATCH 0x00441B13 ; LoadGame
    push 28 * 256
    push _gConditionArray
@ENDPATCH

@PATCH 0x00441B28 ; LoadGame
    push 72 * 1024
    push _gEventArray
@ENDPATCH

@PATCH 0x00441B3E ; LoadGame
    db 2
@ENDPATCH

@PATCH 0x00441B50 ; LoadGame
    db 2
@ENDPATCH
