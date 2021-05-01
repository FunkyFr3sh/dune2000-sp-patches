%include "macros/patch.inc"
%include "dune2000.inc"
%include "patch.inc"
%include "macros/datatypes.inc"

gbool JumpToSkirmish, false
gbool JumpToLAN, false
gbool JumpToGame, false

sstring SkirmishMenu, "SKIRMISH"
sstring WOLLoginMenu, "INETLOGON"
sstring LANMenu, "IPXLOBBY"

hack 0x0044269F, 0x004426A7 ; Jump To Menu On Start 1
    cmp byte[JumpToGame], 1
    je 0x004426A7
    cmp byte[JumpToWOLLogin], 1
    je 0x004426A7
    cmp byte[JumpToSkirmish], 1
    je 0x004426A7
    cmp byte[JumpToLAN], 1
    je 0x004426A7
    jmp 0x00442703


hack 0x004426D4, 0x004426F0 ; Jump To Menu On Start 2
    cmp byte[JumpToGame], 1
    jnz .lan
    pushad
    call InitSpawner
    popad
    jmp .out2
    
.lan:
    cmp byte[JumpToLAN], 1
    jnz .skirmish
    mov word[NetworkType], NT_IPXDIRECTPLAY
    mov dword[GameType], GT_LAN
    call CUIManager__CreateWigNet
    push LANMenu
    jmp .out
    
.skirmish:
    cmp byte[JumpToSkirmish], 1
    jnz .wol
    mov word[NetworkType], NT_NONE
    mov dword[GameType], GT_SKIRMISH
    push SkirmishMenu
    jmp .out
    
.wol:
    mov word[NetworkType], NT_UDP
    call CUIManager__CreateWigNet
    push WOLLoginMenu
    
.out:
    mov ecx, dword[0x79759C]
    call CUIManager__JumpToMenu
    
.out2:
    mov byte[JumpToSkirmish], 0
    mov byte[JumpToWOLLogin], 0
    mov byte[JumpToLAN], 0
    mov byte[JumpToGame], 0
    jmp 0x004426F0
