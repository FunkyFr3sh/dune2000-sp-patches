%include "macros/patch.inc"
%include "inc/patch.inc"

; Always reset list of available buildings if rule buildingsAlwaysNeedPrerequisites is set
hack 0x0046BFEA, 0x0046BFF0 ; CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries
  mov al, [rulesExt__buildingsAlwaysNeedPrerequisites]
  xor al, 1
  and al, [esp+13h] ; check if side has construction yard
  jmp hackend
