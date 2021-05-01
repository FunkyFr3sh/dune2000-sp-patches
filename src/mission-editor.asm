; This hack enables the built in file format converter, the game will start up -> convert -> exit

%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

sstring MisFile, ".\\MisEditor.mis"
sstring TextFile, ".\\MisEditor.txt"

@CLEAR 0x0044FDDD, 0x0044FDE5
@LJMP 0x0044FDDD, 0x004500BE ; read the file and jump out

hack 0x004488AA ; Call Mission__LoadObjectives to load the binary MisFile and afterwards call it again to override the values with the ones from our text file and generate a new binary file
    call Mission__LoadObjectives
    add esp, 4
    mov byte[UseBinaryMapFiles], 0
    push MissionMapData
    call Mission__LoadObjectives
    mov dword[GameState], GS_QUIT
    jmp 0x004488AF
    
hack 0x0044FC89, 0x0044FC8F ; load binary mis file
    push MisFile
    call fopen
    jmp 0x0044FC8F
    
hack 0x0044FA0A, 0x0044FA10 ; load text file
    push TextFile
    call fopen
    jmp 0x0044FA10
    
hack 0x0044FB52, 0x0044FB58 ; write binary mis file
    push MisFile
    call fopen
    jmp 0x0044FB58
    
hack 0x004A3C03, 0x004A3C0B ; force spawner and window mode to be turned on
    mov dword[esp+0x20], 8
    mov byte[JumpToGame], 1
    jmp 0x004A3C0B
    
hack 0x0044F8FA, 0x0044F900 ; jump right to the text file loading code
    mov ebp, 1
    cmp byte[UseBinaryMapFiles], 0
    je 0x0044F9FC
    jmp 0x0044F900
