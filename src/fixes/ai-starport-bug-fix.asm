%include "macros/patch.inc"

; Fix AI losing ability to order units from starport after it tries to order a unit with low cash
@PATCH 0x00421DCD ; CAI::BuyUnitsFromStarport
    db 0x7e ; There was bug in calculating cash caused by unsigned comparison, when it should compare in signed. This patch changes instruction JBE to JLE
@ENDPATCH
