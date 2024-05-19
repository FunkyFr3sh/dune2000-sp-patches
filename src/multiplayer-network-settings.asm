%include "macros/patch.inc"
%include "dune2000.inc"

%assign WOLGameVersion 3

hack 0x0045A983, 0x0045A9C1 ; LAN_Settings
    mov  dword [InitialConnectTimeOut],15000
    mov  dword [ReconnectTimeOutToPlayer],6
    mov  dword [SendRate],150
    mov  dword [UnknownNetVar1],180
    mov  dword [UnknownNetVar2],40 ; Order Delay?
    mov  dword [UnknownNetVar3],300
    mov  dword [UnknownNetVar4],1500
    mov  dword [UnknownNetVar5],0x40066666
    jmp  0x0045AA2C ; not %$end on purpose!

    
hack 0x0045A9C1, 0x0045A9FA ; WOL_Settings
    mov  dword [InitialConnectTimeOut],15000
    mov  dword [ReconnectTimeOutToPlayer],30
    mov  dword [SendRate],300
    mov  dword [UnknownNetVar1],300
    mov  dword [UnknownNetVar2],75 ; Order Delay?
    mov  dword [UnknownNetVar3],1000
    mov  dword [UnknownNetVar4],1500
    mov  dword [UnknownNetVar5],0x40000000
    jmp  0x0045AA2C ; not %$end on purpose!


@CLEAR 0x0041A28C, 0x90, 0x0041A293

hack 0x00413EBE ; CustomWOLGameVersion1
    cmp  eax, WOLGameVersion
    je   0x00413F0A
    jmp  0x00413EC3


hack 0x0041A28C ; CustomWOLGameVersion2
    or   ecx,0xFFFFFFFF
    xor  eax,eax
    push WOLGameVersion
    jmp  0x0041A293
