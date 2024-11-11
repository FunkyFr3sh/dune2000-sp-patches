%include "macros/patch.inc"

; Preserve original tiles beneath spice, concrete and building skirt tiles
; (This will free up hardcoded sand and rock tiles in tileset for any other purpose)

; Store back-up tile index into GameMapTileStruct.back_up_tile_index during loading of map
hack 0x0044C83A, 0x0044C842 ; LoadMapData
    mov [edx*4+0x517DF0], ax ; instruction which was replaced by long jump
    mov [edx*4+0x517DF2], ax ; extra store into GameMapTileStruct.back_up_tile_index
    jmp hackend

; Restore back-up tile index when concrete is destroyed
@PATCH 0x0049DD4D ; DamageTiles
    mov cx, [esi+0x517DF2]
    mov [esi+0x517DF0], cx
    nop
@ENDPATCH

; Restore back-up tile index when building skirt is removed
@PATCH 0x0044F402 ; RemoveSkirt
    mov ax, [edx*4+0x517DF2]
    mov [edx*4+0x517DF0], ax
@ENDPATCH

; Restore back-up tile index when all spice on tile is harvested
@PATCH 0x004994D6 ; UpdateUnit
    mov ax, [edx*4+0x517DF2]
    mov [edx*4+0x517DF0], ax
@ENDPATCH
