%include "macros/patch.inc"

; Make all tiles count tile damage
; Remove check for Walk-on attribute so that tile damage is counted for any tile
@CLEAR 0x0049DCA7, 0x90, 0x0049DCB0 ; DamageTiles
