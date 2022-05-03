%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "inc/patch.inc"

; Increase sound size limit to 512 kilobytes
@PATCH 0x0046EEFA ; ISampleManager__Allocate
    dd 0x80000
@ENDPATCH

@PATCH 0x0046EF16 ; ISampleManager__Allocate
    dd 0x80000
@ENDPATCH

@PATCH 0x0046EF29 ; ISampleManager__Allocate
    dd 0x80000
@ENDPATCH

@PATCH 0x0046F005 ; ISampleManager__LoadSampleCache
    dd 0x80000
@ENDPATCH

@PATCH 0x0046F00D ; ISampleManager__LoadSampleCache
    dd 0x80000
@ENDPATCH
