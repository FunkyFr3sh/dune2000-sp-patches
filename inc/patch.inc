%include "macros/extern.inc"

; This header is used for patches
; This header will be split up as it becomes larger

; ### Constants ###

;ddraw
%assign DDFLIP_DONOTWAIT 0x00000020
%assign DDFLIP_WAIT 0x00000001

;others
%assign UnitTrackerSize 120
%assign BuildingTrackerSize 248

; ### Variables ###

cextern SpawnerActive
cextern StartWithMCV
cextern PlayersUnitsOwned
cextern PlayersBuildingsOwned

cextern HighResAddedWidth
cextern HighResAddedHeight
cextern HighResUIAlignX
cextern HighResUIAlignY
cextern HighResPatchEnabled
cextern HighResCenterUI
cextern HighResUilCenterExcept
cextern HighResControlCenterExcept
cextern HighResControlFirstTimeCenter
cextern UilCenterExceptCheck
cextern ControlCenterExceptCheck
cextern DrawEmptySideBarIcons
cextern VQADisplayModeWidth
cextern VQADisplayModeHeight

cextern DisableMaxWindowedMode
cextern InfiniteSpice
cextern GameLanguage
cextern TextUib
cextern RandomMusicEnabled
cextern nextGameTick

; ### Functions ###

cextern InitSpawner
cextern HandleKeyEvent
cextern LoadRulesFromMap
cextern SetDisableMaxWindowedMode
cextern DrawLiveStats
cextern WriteStatsDmp
cextern PlayRandomMusic
cextern SetBattleFieldSize
cextern FPSLimiter
cextern LoadSavedGame
cextern WriteSaveGame
cextern GamePlayStart
cextern fake_GetTickCount
cextern HandleSpecialValue
