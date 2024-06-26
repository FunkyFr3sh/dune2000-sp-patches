%include "macros/extern.inc"

; This header works with sym.asm which defines the Vanilla symbols
; This header will be split up as it becomes larger

; ### Constants ###

%assign GES_ENDEDNORMALLY 0
%assign GES_ISURRENDERED 1
%assign GES_OPPONENTSURRENDERED 2
%assign GES_OUTOFSYNC 3
%assign GES_CONNECTIONLOST 4
%assign GES_WASHGAME 5
%assign GES_DRAWGAME 6
%assign GES_UNKNOWNENDSTATE 7

%assign GT_SINGLEPLAYER 0
%assign GT_SKIRMISH 1
%assign GT_LAN 2
%assign GT_SERIAL 3
%assign GT_MODEM 4
%assign GT_WOL 5

%assign GS_INTRO 0
%assign GS_BRIEFING 1
%assign GS_MAINLOOP 2
%assign GS_LOAD 3
%assign GS_SAVE 4
%assign GS_QUIT 5

%assign NT_NONE 0
%assign NT_UNKNOWN 1 
%assign NT_UDP 2 
%assign NT_SERIAL 3
%assign NT_IPXDIRECTPLAY 4

; ### Variables ###

;Network settings
cextern SendRate
cextern ReconnectTimeOutToPlayer
cextern InitialConnectTimeOut
cextern UnknownNetVar1
cextern UnknownNetVar2
cextern UnknownNetVar3
cextern UnknownNetVar4
cextern UnknownNetVar5
cextern NetworkGame
cextern NetworkType
cextern NetPlayerCount

;Multiplayer settings
cextern gNetUnitCount
cextern gNetTechLevel
cextern gNetStartingCredits
cextern gNetAIPlayers
cextern gNetCrates
cextern gNetWorms
cextern gNetPlayerName
cextern gNetPlayerSide
cextern gNetPlayerColor
cextern gNetPlayerHandicap
cextern gNetIPAddress
cextern gNetModemPhone
cextern gNetSerialComPort
cextern NetMap
cextern NetPlayerNamesArray

;Game settings
cextern FirstTimePlay
cextern ScrollRate
cextern GameSpeed
cextern GameBitsPerPixel
cextern SFXVolume
cextern MusicVolume
cextern MoviesEnabled
cextern SoundsEnabled
cextern JumpToWOLLogin
cextern VideoBackBuffer
cextern DebugMode
cextern ForceNoCD
cextern FullScreen
cextern Force480Height

;WOL
cextern AllowFindUser
cextern AllowPageUser
cextern LanguageFilter
cextern ShowAllGames
cextern Microphone
cextern WOLMaxPlayers
cextern WOLTournamentGame
cextern WOLPrivateGame
cextern GameEndState

;Screen display stuff
cextern ScreenWidth
cextern ScreenHeight
cextern GameWidth
cextern GameHeight
cextern CreditsUIPosX
cextern PowerBarUIPosX
cextern SideBarPanelRightUIPosX
cextern SideBarPanelLeftUIPosX
cextern SideBarIconCount
cextern MiniMapUIPosX
cextern PointerToMainImage
cextern _OptionsBarHeight
cextern BattleFieldWidth
cextern BattleFieldHeight
cextern BattleFieldPosX
cextern BattleFieldPosY

;Others
cextern MousePositionX
cextern MousePositionY
cextern MySideID
cextern gSideId
cextern gWin
cextern gLose
cextern gDifficultyLevel
cextern GameType
cextern GameState
cextern AtreidesMoney
cextern MissionNumber
cextern GameEndTickCount
cextern UseBinaryMapFiles
cextern MissionMapData
cextern MissionMap
cextern LimitedModelRate
cextern MapsResourcePath
cextern ResourcePath
cextern MoviesResourcePath
cextern gGameMapWidth
cextern gGameMapHeight
cextern CurrentSoundTrack
cextern _gBuildingsExist
cextern _gUnitsExist
cextern NetMessageString
cextern CreditsScreenWidth
cextern CreditsScreenTop
cextern CreditsScreenBottom
cextern gGameTicks
cextern _GameOver
cextern RandSeed
cextern _IsMultiplayer
cextern _gDiplomacy
cextern SoundClassObject
cextern WindowActivated
cextern _IRValues


; ### Functions ###
cextern BlitFontChars
cextern Orderdata_add
cextern CUIManager__LoadDune2000Cfg
cextern CUIManager__SaveDune2000Cfg
cextern CUIManager__JumpToMenu
cextern CUIManager__CreateWigNet
cextern CUIManager__StartGame
cextern CUIManager__SendInGameMsgTo
cextern Map__SetDeadOrder
cextern Map__PlayerDefeated
cextern cinit
cextern WOL__SendGameResultsToServer
cextern Mission__LoadObjectives
cextern DrawMenu
cextern GetTextID
cextern GetSide
cextern Sound__PlayMusic
cextern Sound__SetMusicVolume
cextern GetBuildingCost
cextern CanSideUpgradeBuildingGroup
cextern IsAnyStarportUnitPicked
cextern UncloakRemainingStealthUnit
cextern ModelAddExplosion
cextern ModelStarportCancel
cextern PlaySoundAt
cextern CreateGlobe
cextern UnitTileOccupiedByBuilding
cextern IsAnyInfantrySelected
cextern CSide__ReturnMoneyFromStarportOrder
cextern _ReadFile
cextern Graphlib_font_pal

;imports
cextern _imp__GetTickCount
cextern _imp__Sleep
cextern _imp__timeBeginPeriod

;clib
cextern memset
cextern fopen
cextern strcmpi
cextern sprintf
cextern strcpy
cextern fprintf
cextern fclose
