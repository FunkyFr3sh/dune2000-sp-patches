%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"
%include "utils.inc"

; Saving game - Save message data from new memory location and different amount of data
@PATCH 0x0044181A ; SaveGame
    push 4864
    push gMessageData
@ENDPATCH

; Loading game - Load message data into new memory location and different amount of data
@PATCH 0x00441C5D ; LoadGame
    push 4864
    push gMessageData
@ENDPATCH

; Initialize more font palettes
hack 0x00448B6C, 0x00448B71 ; GameLoop
    call Graphlib_font_pal ; Instruction replaced by long jump
    push eax ; Back result of this call
    push 0x004E0760 ; Offset of string "FontCol"
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call Graphlib_font_pal
    call InitFontColors
    pop eax
    pop eax
    jmp hackend
