%include "macros/extern.inc"

; ### Write ###
cextern IniSetString
cextern IniSetBool
cextern IniSetInt

;dune2000.ini
cextern D2kIniSetString
cextern D2kIniSetBool
cextern D2kIniSetInt

;spawn.ini
cextern SpawnIniSetString
cextern SpawnIniSetBool
cextern SpawnIniSetInt


; ### Read ###
cextern IniGetString
cextern IniGetBool
cextern IniGetInt

;dune2000.ini
cextern D2kIniGetString
cextern D2kIniGetBool
cextern D2kIniGetInt

;spawn.ini
cextern SpawnIniGetString
cextern SpawnIniGetBool
cextern SpawnIniGetInt
