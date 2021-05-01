%include "macros/patch.inc"
%include "hotkeys.inc"
%include "patch.inc"

;Allows scrolling the map with hotkeys, normally this feature is only available in debug window mode and the hotkeys are hardcoded by default

;remove checks for debug window mode
@CLEAR 0x00443D3E, 0x90, 0x00443D44
@CLEAR 0x00443D49, 0x90, 0x00443D4D
@CLEAR 0x00443E26, 0x90, 0x00443E2C 
@CLEAR 0x00443E37, 0x90, 0x00443E3B
@CLEAR 0x00443E6A, 0x90, 0x00443E6E
@CLEAR 0x00443DC3, 0x90, 0x00443DC7

;map scroll down default hotkey
@SET 0x00443D4F, dd 0x00797eda
;map scroll up default hotkey
@SET 0x00443DC9, dd 0x00797edd
;map scroll right default hotkey
@SET 0x00443E3D, dd 0x00797edb
;map scroll left default hotkey
@SET 0x00443E70, dd 0x00797ed9
