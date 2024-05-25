%include "macros/patch.inc"
%include "dune2000.inc"
%include "macros/datatypes.inc"

gbool StartWithMCV, true

hack 0x0046A04A ; do not spawn the mcv as starting unit
    cmp byte[StartWithMCV], 1
    jnz hackend

    call 0x00455870 ; model_add_unit()
    jmp hackend
