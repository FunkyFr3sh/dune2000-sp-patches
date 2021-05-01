%include "macros/extern.inc"

; This header works with sym.asm which defines the Vanilla symbols
; This header will be split up as it becomes larger

; ### Constants ###

%assign SD_ATREIDES 0
%assign SD_HARKONNEN 1
%assign SD_ORDOS 2
%assign SD_EMPEROR 3
%assign SD_FREMEN 4
%assign SD_SMUGGLERS 5
%assign SD_MERCENARIES 6
%assign SD_SANDWORM 7

%assign CL_BLUE 0
%assign CL_RED 1
%assign CL_TEAL 2
%assign CL_PURPLE 3
%assign CL_GRAY 4
%assign CL_BROWN 5
%assign CL_GOLD 6
%assign CL_LIGHTBROWN 7

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

; Side (HouseClass)
%assign HC_SIDEID 0x24252
%assign HC_CREDITS 0x2425C
%assign HC_SILO_CREDITS 0x24254
%assign HC_SPICE_HARVESTED 0x2477C
%assign HC_BUILDINGS_DESTROYED 0x24C94
%assign HC_UNITS_KILLED 0x24C90
%assign HC_BUILDINGS_OWNED 0x24784
;not sure about this one
%assign HC_BUILDING_PRODUCTION_AVAILABLE 0x2652D
;0x5A00 = 100%
%assign HC_BUILDING_PRODUCTION_PROGRESS 0x2651E
%assign HC_CURRENTLY_PRODUCED_BUILDING_ID 0x2651C

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
cextern NetUnitCount
cextern NetTechLevel
cextern NetStartingCredits
cextern NetAIPlayers
cextern NetCrates
cextern NetWorms
cextern NetPlayerName
cextern NetPlayerSide
cextern NetPlayerColor
cextern NetPlayerHandicap
cextern NetIPAddress
cextern NetModemPhone
cextern NetSerialComPort
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
cextern OptionsBarHeight
cextern BattleFieldWidth
cextern BattleFieldHeight
cextern BattleFieldPosX
cextern BattleFieldPosY

;Others
cextern MousePositionX
cextern MousePositionY
cextern MySideID
cextern Win
cextern Lose
cextern DifficultyLevel
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
cextern gBuildingsExist
cextern gUnitsExist
cextern NetMessageString
cextern CreditsScreenWidth
cextern CreditsScreenTop
cextern CreditsScreenBottom
cextern GameTicks
cextern GameOver
cextern RandSeed
cextern Diplomacy
cextern SoundClassObject
cextern WindowActivated


; ### Functions ###
cextern Graphlib__TextOnScreen
cextern Model__GenerateUnitMoveOrder
cextern Model__GenerateUnitAttackUnitOrder
cextern Model__ExecuteGameEvent
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
cextern Data__GetTextId
cextern Side__AsPointer
cextern Sound__PlayMusic
cextern Sound__SetMusicVolume

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
