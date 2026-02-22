%include "macros/extern.inc"

; This header is used for patches
; This header will be split up as it becomes larger

; ### Constants ###

;ddraw
%assign DDFLIP_DONOTWAIT 0x00000020
%assign DDFLIP_WAIT 0x00000001

; ### Variables ###

cextern SpawnerMaxAhead
cextern SpawnerActive
cextern StartWithMCV
cextern ListenPort
cextern InitHouses
cextern MeIsSpectator

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

; ### Extended data replacing original data ###

cextern _gEventArray
cextern _gConditionArray
cextern gMessageData
cextern _TileBitflags

; ### Extra rules ###

cextern rulesExt__buildingsAlwaysNeedPrerequisites
cextern rulesExt__alwaysShowRadar
cextern rulesExt__uncloakRemainingStealthUnit
cextern rulesExt__showNeutralBecomeHostileMsg

; ### Functions ###

cextern InitSpawner
cextern HandleKeyEvent
cextern LoadRulesFromMap
cextern SetDisableMaxWindowedMode
cextern InitDxWnd
cextern SetupAIPlayerColors
cextern IsSpectator
cextern DrawLiveStats
cextern WriteStatsDmp
cextern PlayRandomMusic
cextern FPSLimiter
cextern ExecuteCustomEvent
cextern InitMissionScript
cextern LoadSavedGame
cextern WriteSaveGame
cextern GamePlayStart
cextern fake_GetTickCount
cextern HandleSpecialValue
cextern HitCrate
cextern Mod__PickupCrate
cextern InitFontColors
cextern DebugUnitCrash
cextern ExecuteEventHook
cextern HandleSpiceBloomCrate
cextern GAME_SET_RES_PATH
