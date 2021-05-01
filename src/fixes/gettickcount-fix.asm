%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "patch.inc"


@CLEAR 0x0040C469, 0x90, 0x0040C46F
@SET 0x0040C469, { mov esi, fake_GetTickCount }

@CLEAR 0x0040D0C9, 0x90, 0x0040D0CF
@SET 0x0040D0C9, { mov esi, fake_GetTickCount }

@CLEAR 0x0040EC49, 0x90, 0x0040EC4F
@SET 0x0040EC49, { mov esi, fake_GetTickCount }

@CLEAR 0x00419E32, 0x90, 0x00419E38
@SET 0x00419E32, { mov esi, fake_GetTickCount }

@CLEAR 0x0043D790, 0x90, 0x0043D796
@SET 0x0043D790, { mov esi, fake_GetTickCount }

@CLEAR 0x0043F0A4, 0x90, 0x0043F0AA
@SET 0x0043F0A4, { mov esi, fake_GetTickCount }

@CLEAR 0x00472490, 0x90, 0x00472496
@SET 0x00472490, { mov esi, fake_GetTickCount }

@CLEAR 0x0047295F, 0x90, 0x00472965
@SET 0x0047295F, { mov edi, fake_GetTickCount }

@CLEAR 0x0047B0DC, 0x90, 0x0047B0E2
@SET 0x0047B0DC, { mov esi, fake_GetTickCount }

@CLEAR 0x0048C454, 0x90, 0x0048C45A
@SET 0x0048C454, { mov ebx, fake_GetTickCount }

@CLEAR 0x0048D2B2, 0x90, 0x0048D2B8
@SET 0x0048D2B2, { mov ebp, fake_GetTickCount }

@CLEAR 0x00492CE0, 0x90, 0x00492CE5
@SET 0x00492CE0, { mov edi, fake_GetTickCount }
