%include "macros/patch.inc"

; Fix music and sound playing always at max volume on score screen

; Remove explicit setting of 100% volume when gGameState == 2 
@CLEAR 0x00470131, 0x90, 0x0047013A ; OpenAudioStream
@CLEAR 0x0046FBDF, 0x90, 0x0046FBE8 ; SoundLoop
@CLEAR 0x0046FB5C, 0x90, 0x0046FB5E ; SoundLoop
