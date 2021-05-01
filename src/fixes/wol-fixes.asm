%include "macros/patch.inc"
%include "dune2000.inc"

;Fix for the HelpURLError, caused by broken WOL installations
@CLEAR 0x0040C72A, 0x90, 0x0040C740
@CLEAR 0x0040C77A, 0x90, 0x0040C790

;Do -NOT- show the registration dialog if there are no accounts found in the registry
@CLEAR 0x0047841A, 0x90, 0x00478424

hack 0x0043BD7B, 0x0043BD81 ; WOL_6Player
    add esp,0x8AC
    mov byte[WOLMaxPlayers],0x6
    jmp hackend
