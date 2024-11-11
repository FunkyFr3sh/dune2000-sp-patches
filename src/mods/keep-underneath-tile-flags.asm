%include "macros/patch.inc"

; When a building is removed, all tiles occupied by building were set to buildable and
; all solid tiles were set to walkable + driveable + buildable, regardless the tile underneath.
; This patch makes the tile underneath taken into consideration, so for example buildable flag
; is not set when the tile is not buildable and so on. 
hack 0x0044DFD4, 0x0044DFDE ; RemoveBuildingTileDataAndRadar
    movzx edx, word [eax*4+0x517DF2] ; Get backup_tile_index into edx
    mov edx, [edx*4+0x786FF8] ; Get tile attributes for tile into edx
    and edx, 0x8000
    or edx, [eax*4+0x517DF4] ; Add build-on flag into existing tile flags
    jmp hackend

hack 0x0044E07E, 0x0044E084 ; RemoveBuildingTileDataAndRadar
    movzx edx, word [eax*4+0x517DF2] ; Get backup_tile_index into edx
    mov edx, [edx*4+0x786FF8] ; Get tile attributes for tile into edx
    and edx, 0xE000
    or esi, [eax*4+0x517DF4] ; Add walk-on, drive-on and build-on flags into existing tile flags
    jmp hackend

; Remove check for building requiring concrete
@CLEAR 0x0044DFB6, 0x90, 0x0044DFBE ; RemoveBuildingTileDataAndRadar
@CLEAR 0x0044E07C, 0x90, 0x0044E07E ; RemoveBuildingTileDataAndRadar
    
; When an infantry is killed, the occupied-spot tile attribute is reset.
; If a tile underneath has pre-set those attributes (i.e. repulsion field), this patch makes sure
; the attribute will be kept and repulsion field won't get broken.
hack 0x00499EA6, 0x00499EB1 ; UpdateUnit
    movzx ecx, word [eax*4+0x517DF2] ; Get backup_tile_index into ecx
    mov ecx, [ecx*4+0x786FF8] ; Get tile attributes for tile into ecx
    not edx ; edx contained the occupied-spot flag to be removed
    or ecx, edx
    and ecx, [eax*4+0x517DF4] ; Remove occupied-spot flag from existing tile flags
    jmp hackend

hack 0x00499EE6, 0x00499EF1 ; UpdateUnit
    movzx ecx, word [eax*4+0x517DF2] ; Get backup_tile_index into ecx
    mov ecx, [ecx*4+0x786FF8] ; Get tile attributes for tile into ecx
    not edx ; edx contained the occupied-spot flag to be removed
    or ecx, edx
    and ecx, [eax*4+0x517DF4] ; Remove occupied-spot flag from existing tile flags
    jmp hackend
    
; When tile a is damaged, concrete flag is removed.
; Do not remove concrete flag if the tile underneath has pre-set concrete flag (indestructible concrete)
hack 0x0049DD3C, 0x0049DD42 ; DamageTiles
    movzx ebx, word [esi+0x517DF2] ; Get backup_tile_index into ebx
    mov ebx, [ebx*4+0x786FF8] ; Get tile attributes for tile into ebx
    or ebx, 0xFFFFF7FF
    and ebx, [esi+0x517DF4] ; Remove concrete flag from existing tile flags
    mov [esi+0x517DF4], ebx
    jmp hackend
