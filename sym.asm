;;
;; This file exists to define the locations of everything we know about the
;; vanilla exe.
;;
;; The idea is by separating knowledge about the vanilla exe from changes to it,
;; we make the code more clean. Also it means we don't need to hard-code the
;; symbols separately for C, C++, asm, etc.
;;
;; Symbols are defined with `setcglob` for C (remove leading underscore)
;;                     and  `setxglob` for C++ and C
;;

%include "macros/setsym.inc"
%include "macros/extern.inc"

setcglob 0x004B2810, start

;; ### Variables ###

;Network settings
setcglob 0x4E3AEC, SendRate
setcglob 0x6B9460, ReconnectTimeOutToPlayer
setcglob 0x6B9644, InitialConnectTimeOut

setcglob 0x6B93E8, UnknownNetVar1 
setcglob 0x4E3AE8, UnknownNetVar2
setcglob 0x4E3B1C, UnknownNetVar3
setcglob 0x4E3AF0, UnknownNetVar4
setcglob 0x4E3AF4, UnknownNetVar5

setcglob 0x007984C4, NetworkGame ;bool
setcglob 0x004E3B10, NetworkType ;short
setcglob 0x004F2898, NetPlayers

;Multiplayer settings
setcglob 0x004E3B00, gNetUnitCount
setcglob 0x004E3B04, gNetTechLevel
setcglob 0x004E3B08, gNetStartingCredits
setcglob 0x004E3B0C, gNetAIPlayers
setcglob 0x006B9834, gNetCrates
setcglob 0x006B9838, gNetWorms
setcglob 0x006B93F8, gNetPlayerName
setcglob 0x006B9840, gNetPlayerSide
setcglob 0x006B9844, gNetPlayerColor
setcglob 0x006B9848, gNetPlayerHandicap
setcglob 0x007975D0, gNetIPAddress
setcglob 0x00797960, gNetModemPhone
setcglob 0x004E8BF4, gNetSerialComPort

setcglob 0x00797638, NetMap
setcglob 0x004F1018, LoginName

setcglob 0x006B87D0, NetPlayersStartingCredits
setcglob 0x006B87C8, NetPlayersTechLevel
setcglob 0x00798630, NetPlayerNamesArray

setcglob 0x006B922D, NetPlayersColor ;Actually there is more than just the colors, not sure what... struct size 60byte, first byte = color
setcglob 0x006B922E, NetPlayersHandicap ;Same struct as the colors, byte 2

setcglob 0x006F9840, MissionMap

;Game settings
setcglob 0x004E7ACC, FirstTimePlay
setcglob 0x004EB030, ScrollRate
setcglob 0x004E3B18, GameSpeed
setcglob 0x004EB01C, GameBitsPerPixel
setcglob 0x004E5FC4, gSFXVolume
setcglob 0x004E5FC8, MusicVolume
setcglob 0x005178C4, DebugMode
setcglob 0x005178C8, DebugModeBlocks
setcglob 0x004eb008, FullScreen
setcglob 0x00797e30, ForceNoCD
setcglob 0x004EB004, MoviesEnabled ; -M = false
setcglob 0x004EB00C, SoundsEnabled ; -S = false
setcglob 0x005178A4, JumpToWOLLogin ; -N = true
setcglob 0x007984B4, Force8Bit ; -8 = true
setcglob 0x007984B8, Force16Bit ; -6 = true
setcglob 0x004EB010, VideoBackBuffer ; -F = false
setcglob 0x007984E4, Force480Height ; -G = true

;WOL
setcglob 0x004DB9D4, AllowFindUser
setcglob 0x004DB9D8, AllowPageUser
setcglob 0x004F3D00, LanguageFilter
setcglob 0x004F3D04, ShowAllGames
setcglob 0x004F3CF0, Microphone

setcglob 0x004E8BEC, WOLMaxPlayers
setcglob 0x004F3D3C, WOLTournamentGame
setcglob 0x004F3D40, WOLPrivateGame
setcglob 0x004F3D38, WOLGameId
setcglob 0x004F3D30, GameStartTickCount
setcglob 0x004F3D34, GameEndTickCount

;Screen display stuff
setcglob 0x004eb020, ScreenWidth
setcglob 0x004eb024, ScreenHeight
setcglob 0x004EB028, GameWidth
setcglob 0x004EB02C, GameHeight
setcglob 0x004E4214, CreditsUIPosX
setcglob 0x004E420C, PowerBarUIPosX
setcglob 0x004E4210, PowerBarUIPosY
setcglob 0x004E4204, SideBarPanelRightUIPosX
setcglob 0x004E41FC, SideBarPanelLeftUIPosX
setcglob 0x004E4200, SideBarPanelsPosY
setcglob 0x004E41EC, SideBarIconCount
setcglob 0x004E41F8, SideBarIconHeight
setcglob 0x004E41E4, MiniMapUIPosX
setcglob 0x004E41E0, SideBarWidth
setcglob 0x005162F4, SideBarLDownArrowVisible 
setcglob 0x005162DC, SideBarLUpArrowVisible 
setcglob 0x00516324, SideBarRDownArrowVisible 
setcglob 0x005163B4, SideBarRUpArrowVisible 
setcglob 0x005163FC, SideBarButtonMainMenuVisible
setcglob 0x00516414, SideBarButtonMainMenu2Visible
setcglob 0x0051642C, SideBarButtonPurchaseVisible
setcglob 0x00786D48, BattleFieldWidth
setcglob 0x006DC538, BattleFieldHeight
setcglob 0x004E41DC, _OptionsBarHeight
setcglob 0x00798538, BattleFieldPosX
setcglob 0x0079853C, BattleFieldPosY
setcglob 0x00798540, CurrentCursorId

;DisableDebugFileFormats {
setcglob 0x004EB014, DisableDebugFileFormats  ; bool
;Colours.bin -> colours.txt
;vars.bin -> vars.dat
;font.bin -> font.map
;font.fnt -> font.bmp
;mouse.r16/mouse.r8 -> mouse48.bmp
;circles.bin -> SCircles.bmp
;templates.bin -> many different .bmp files...
; ...and much more...
; }
setcglob 0x004EB000, UseBinaryMapFiles

;rules
setcglob 0x006B8818, harvestUnloadDelay
setcglob 0x006B881C, harvestBlobValue
setcglob 0x006B8820, harvestLoadSpiceDelay
setcglob 0x006B8824, starportUpdateDelay
setcglob 0x006B8828, starportStockIncreaseDelay
setcglob 0x006B882C, starportStockIncreaseProb
setcglob 0x006B8830, starportCostVariationPercent
setcglob 0x006B8834, starportFrigateDelay
setcglob 0x006B8838, refineryExplosionOffsetX
setcglob 0x006B883C, refineryExplosionOffsetY
setcglob 0x006B8840, HarvesterDriveDistance
setcglob 0x006B8844, RepairDriveDistance
setcglob 0x006B8848, BuildingRepairValue
setcglob 0x006B884C, UnitRepairValue
setcglob 0x006B8850, SinglePlayerDelay
setcglob 0x006B8854, NumberOfFremen
setcglob 0x006B8855, SandWormAppetite
setcglob 0x006B8856, SandWormInitialSleep
setcglob 0x006B8857, SandWormFedSleep
setcglob 0x006B8858, SandWormShotSleep
setcglob 0x006B8859, NumberOfCrates
setcglob 0x006B885A, CratesPerPlayer
setcglob 0x006B885B, DevastatorExplodeDelay
setcglob 0x006B885C, IgnoreDistance
setcglob 0x006B8860, CrateCash
setcglob 0x006B8864, ShowWarnings
setcglob 0x006B8865, DeathHandAccuracy

;others
setcglob 0x004C93E8, _InfantryCellspotPosOffsets2
setcglob 0x004D7028, _InfantryCellspotPosOffsets
setcglob 0x004DB9D8, gAllowPageUser
setcglob 0x004DB9E0, GameEndState
setcglob 0x004DE610, MissionNumber
setcglob 0x004DEDB8, CreditsScreenWidth
setcglob 0x004DEDB4, CreditsScreenTop
setcglob 0x004DEDBC, CreditsScreenBottom
setcglob 0x004DFB08, gGameState
setcglob 0x004E3AE8, _firgcrap_dword_4E3AE8
setcglob 0x004E3B14, MyCurrentFrameRate
setcglob 0x004E3E48, _healthbar_heights
setcglob 0x004E41E4, _RadarLocationX
setcglob 0x004E41E8, _RadarLocationY
setcglob 0x004E41EC, _SideBarIconCount
setcglob 0x004E41F0, _SidebarIconWidth
setcglob 0x004E41F4, _SideBarIconHeight1
setcglob 0x004E41F8, _SideBarIconHeight2
setcglob 0x004E41FC, _SidebarStrip1XPos
setcglob 0x004E4200, _SidebarStrip1YPos
setcglob 0x004E4204, _SidebarStrip2XPos
setcglob 0x004E4208, _SidebarStrip2YPos
setcglob 0x004E420C, _PowerBarUIPosX
setcglob 0x004E4210, _PowerBarUIPosY
setcglob 0x004E4214, _CreditsTextXPos
setcglob 0x004E8BF0, gDifficultyLevel
setcglob 0x004E9520, _SandTileIDs
setcglob 0x004E9530, _UnitAnimTypeFrames
setcglob 0x004EB008, _gFullscreen_DebugModes_pathfinddebug
setcglob 0x004EB00C, _gSoundOn
setcglob 0x004EB014, _ReadBinaries
setcglob 0x004EB018, gBitsPerPixel
setcglob 0x004EB028, _ScreenClipWidth
setcglob 0x004EB02C, _ScreenClipHeight
setcglob 0x004EB030, gScrollSize
setcglob 0x004EB048, _gMousePos
setcglob 0x004EB048, MousePositionX
setcglob 0x004EB04C, MousePositionY
setcglob 0x004ED870, RandSeed
setcglob 0x004DFB04, _bool_shroud_4DFB04
setcglob 0x004F0F18, _Nullstring
setcglob 0x004F1D60, gPageUser
setcglob 0x004F3D24, _DrawOffered1
setcglob 0x004F3D28, _DrawOffered2
setcglob 0x004F3D3C, gTournamentGame
setcglob 0x004F4118, _gAIArray
setcglob 0x00502ED8, _crater_draw_offsets_y
setcglob 0x00503F88, _crater_draw_offsets_x
;setcglob 0x00504030, _gMessageData ; Replaced by mod
setcglob 0x00512D58, _selectedmaybe_dw_BlitVehicles_512D58
setcglob 0x00513488, ResourcePath
setcglob 0x00513488, gRES_PATH
setcglob 0x00513520, MoviesResourcePath
setcglob 0x005135B8, gMUSIC_RES_PATH
setcglob 0x00513650, gMISSIONS_RES_PATH
setcglob 0x005136E8, gMAPS_RES_PATH
setcglob 0x00513A20, _FontBinData
setcglob 0x00513B20, _FontData
setcglob 0x00515B60, _FontPals
setcglob 0x00515BB8, gHighScores
setcglob 0x005162B4, _GreenColor8
setcglob 0x005162B8, _GreyColor16
setcglob 0x005162BC, _RedColor8
setcglob 0x005162C0, _YellowColor8
setcglob 0x005162C8, _SidebarButtons
setcglob 0x00516430, _TacticalData
setcglob 0x00516530, _radarcolor16_impassable
setcglob 0x00516538, _YellowColor16
setcglob 0x0051653C, _RedColor16
setcglob 0x005165B0, _UnitGroupKeyState
setcglob 0x005165BC, _GreyColor8
setcglob 0x005165C0, _radarcolor16_driveon
setcglob 0x005165C8, _AttribFileName
setcglob 0x00516690, _GreenColor16
setcglob 0x005166A0, _BloxFileName
;setcglob 0x00516770, _TileTooltips ; Replaced by mod
setcglob 0x005173F0, _radarcolor16_sand
setcglob 0x005173F4, gGameTicks
setcglob 0x005173F8, _NeutralUnitText
setcglob 0x005173FC, _radarcolor8_drive_on
setcglob 0x00517400, _bool_missionfailed_517400
setcglob 0x00517408, _UnitGroupTextIds
setcglob 0x005174F8, _colormask3
setcglob 0x005174FC, _tooltipcolor_yellow
setcglob 0x00517500, _cheatstates
setcglob 0x00517508, _Tooltipdata
setcglob 0x005175C8, _TooltipBackupImage
setcglob 0x005175D0, _tooltipcolor_gray
setcglob 0x005175D4, _NeutralStructureText
setcglob 0x005175D8, _IRValuesBackup
setcglob 0x005175E0, _UnrevealedText
setcglob 0x005175E8, _BuildingGroupTextIds
setcglob 0x00517778, _EnemyUnitText
setcglob 0x00517780, _radarcolor8_spice
setcglob 0x00517784, _EnemyStructureText
setcglob 0x0051778C, _radarcolor8_sand
setcglob 0x00517790, _radarcolor8_impassable
setcglob 0x00517898, _radarcolor16_spice
setcglob 0x0051789C, _StoredMouseCursorIndex
setcglob 0x005178AC, _blitflag
setcglob 0x005178B4, PointerToMainImage
setcglob 0x005178B4, gBackBuf
setcglob 0x005178C4, _DebugPrintStuff
setcglob 0x005178C8, _DebugTiles
setcglob 0x005178D8, _ScreenShakes
setcglob 0x005179D0, _musicbool_byte_5179D0
setcglob 0x005179D8, _circle_13x13grid
setcglob 0x00517A88, _circle_11x11grid
setcglob 0x00517B08, _circle_9x9grid
setcglob 0x00517B60, _circle_7x7grid
setcglob 0x00517B98, gCrates
setcglob 0x00517D00, _circle_15x15grid
setcglob 0x00517DE8, gGameMap
setcglob 0x00517DE8, gGameMapWidth
setcglob 0x00517DEC, gGameMapHeight
setcglob 0x00547DF0, _CellNumbersWidthSpan
setcglob 0x00547FF0, _ptr_circles
setcglob 0x00548010, _mapvisstate_548010
setcglob 0x00548018, _circle_5x5grid
setcglob 0x00548038, _circle_3x3grid
setcglob 0x00548044, _circle_1x1grid
setcglob 0x00548048, _NumCratesAllowed
setcglob 0x006B63C8, _GameOver
setcglob 0x006B63CC, _gEventCount
setcglob 0x006B7050, _somebool_6B7050
;setcglob 0x006B7058, _gEventArray ; Replaced by mod
setcglob 0x006B8258, _persideflags_6B8258
setcglob 0x006B8260, _gConditionCount
setcglob 0x006B8268, _gUnitsExist
setcglob 0x006B8270, _gTimerValue
setcglob 0x006B8274, gWin
setcglob 0x006B8278, gLose
;setcglob 0x006B8280, _gConditionArray ; Replaced by mod
setcglob 0x006B87C0, _gBuildingsExist
setcglob 0x006B87C8, _gMiscData
setcglob 0x006B8818, _gVariables
setcglob 0x006B8868, _TechPosdata
setcglob 0x006B91F8, _OrderData
setcglob 0x006B9518, NetMessageString
setcglob 0x006B9864, LimitedModelRate
setcglob 0x006D5FD0, _templates_AnimationArtFrames
setcglob 0x006D60D8, _templates_GroupIDs
setcglob 0x006D6130, _SideBarPowerImages
setcglob 0x006D7BA0, _RadarMap1
setcglob 0x006D7BA8, _SpawnLocations
setcglob 0x006D7BE8, _images_crater
setcglob 0x006D7DE8, _tiledata
setcglob 0x006D8D88, _images_blobs
setcglob 0x006D8D98, _RadarHouseImages
setcglob 0x006D9350, _palettes_6D9350
setcglob 0x006D9B50, _templates_UnitGroupCount
;setcglob 0x006D9B58, _images_tiles ; Replaced by mod
setcglob 0x006DA7D8, _templates_UnitGroupNameList
setcglob 0x006DB390, _templates_BulletNameList
setcglob 0x006DC018, _images_unstealts
setcglob 0x006DC468, _image_spanner
setcglob 0x006DC470, MissionMapData
setcglob 0x006DC538, _ViewportHeight
setcglob 0x006DC540, _templates_buildattribs
setcglob 0x006E2DF0, _images_small_numbers
setcglob 0x006E2E20, gExplosionElements
setcglob 0x006E5750, _templates_Explosiondata_AnimationArtFlags
setcglob 0x006E5850, _templates_BuildingGroupCount
setcglob 0x006E5854, _image_placement_marker_buildable_concrete
setcglob 0x006E5880, dataimage
setcglob 0x006E5A60, _templates_BuildingArtDirections
setcglob 0x006E5C40, gBuildingElements
setcglob 0x006E9840, _sinValues
setcglob 0x006F9840, _MapName
setcglob 0x006F9908, _templates_BuildingNameList
setcglob 0x00704A98, _cosValues
setcglob 0x00714A98, _templates_BuildupArtFrames
setcglob 0x00716B00, _BuildingAnimationImages
setcglob 0x0071ACA0, gUnitElements
setcglob 0x0077D3A0, _SmokeDataStructs
setcglob 0x0077E250, _templates_unitattribs
setcglob 0x00781E54, _RadarMap2
setcglob 0x00781E58, _templates_explosionattribs
setcglob 0x00782058, _images_miscIcon
setcglob 0x00782068, _templates_bulletattribs
setcglob 0x00782768, _BlankRadarImage
setcglob 0x00782AA0, _FreeSpawnLocations
setcglob 0x00782AA8, _BuildupAnimationImages
setcglob 0x00786C48, _ColoursBinData
setcglob 0x00786D48, _ViewportWidth
setcglob 0x00786D50, _images_stealths
setcglob 0x00786D90, _ArmourNames
setcglob 0x00786FE8, _image_placement_marker_buildable
setcglob 0x00786FEC, _SpawnLocationCount
setcglob 0x00786FF0, _radarcolor8_sidecolor
;setcglob 0x00786FF8, _TileBitflags ; Replaced by mod
setcglob 0x00787E08, _templates_BuildingGroupNameList
setcglob 0x00789198, _images_healthbars
setcglob 0x007891B0, _templates_UnitNameList
setcglob 0x0078FB28, _image_selection_bl
setcglob 0x0078FB2C, _image_selection_br
setcglob 0x0078FB30, _WarheadNames
setcglob 0x00790550, _WarheadData
setcglob 0x007907A8, _gBuildingElementsDamaged
setcglob 0x007943A8, _templates_ExplosionNameList
setcglob 0x00795128, _speed_values
setcglob 0x007951A8, _images_crate
setcglob 0x007951C8, _image_selection_tl
setcglob 0x00795338, _image_selection_tr
setcglob 0x00795340, _templates_ProjectileArtDirections
setcglob 0x00795440, _image_placement_marker_nonbuildable
setcglob 0x007954B0, _templates_BuildingAnimationFrames
setcglob 0x00795518, _radarcolor16_sidecolor
setcglob 0x00795538, gUnitTypeNum
setcglob 0x0079553C, gBuildingTypeNum
setcglob 0x00795540, gBulletTypeNum
setcglob 0x00795544, gExplosionTypeNum
setcglob 0x00795560, _CreditsTextYPos
setcglob 0x00795570, _tileset_resource_buffer
setcglob 0x00795600, _musicboolbyte_795600
setcglob 0x00795608, _gSampleMgr
setcglob 0x0079560C, _samplemanunused
setcglob 0x00795610, _SampleManagerInitDone
setcglob 0x00795618, sample_filebuffer
setcglob 0x00795628, sample_buffer
setcglob 0x007975A8, gTextTable
setcglob 0x007975B8, gSampleTable
setcglob 0x007975BC, _sampletablecount
setcglob 0x00797590, _Mouse_bool_797590
setcglob 0x0079759C, gUIMgr
setcglob 0x007978F8, gSendingToPlayer
setcglob 0x00797A98, CurrentSoundTrack
setcglob 0x00797E18, dword_797E18
setcglob 0x00797E1C, gRestartGame
setcglob 0x00797E34, gGameType
setcglob 0x00797E78, KeyIsDown2
setcglob 0x00797E78, _KeyboardKeyState
setcglob 0x00798384, _colormask1
setcglob 0x00798388, _MouseClickCoords
setcglob 0x00798390, KeyIsDown1
setcglob 0x00798390, _KeyboardKeyDown
setcglob 0x00798498, _mouse_dword_798498
setcglob 0x007984C0, gTotalPlayers
setcglob 0x007984C4, _IsMultiplayer
setcglob 0x007984D0, _DebugOn_Pathfind_WinLose_DebugNewGame
setcglob 0x007984D4, BitsPerPixelChanged
setcglob 0x007984D8, MenuDraw_FromTopPixel		;       int MenuDraw::FromTopPixel
setcglob 0x007984DC, _palette_ptr_7984DC
setcglob 0x007984E0, _PalettePtr
setcglob 0x00798510, WindowActivated
setcglob 0x00798520, _MouseLeftState
setcglob 0x00798524, _MouseLeftDown
setcglob 0x00798528, _MouseLeftUp
setcglob 0x0079852C, _MouseRightState
setcglob 0x00798530, _MouseRightDown
setcglob 0x00798534, _MouseRightUp
setcglob 0x00798538, _ViewportXPos
setcglob 0x0079853C, _ViewportYPos
setcglob 0x00798540, _MouseCursorID
setcglob 0x00798544, MySideID
setcglob 0x00798544, gSideId
setcglob 0x00798628, _SidesToProcess
setcglob 0x00798630, _NetPlayerNamesArray
setcglob 0x00798830, _gDiplomacy
setcglob 0x00798870, gSideArray
setcglob 0x008CD4F0, _IRValues
setcglob 0x008CD51C, OSMajorVersion
setcglob 0x008CD520, OSMinorVersion

;Side specifics
setcglob 0x007BCACC, AtreidesMoney
setcglob 0x007BCACC+(158096*1), HarkonnenMoney
setcglob 0x007BCACC+(158096*2), OrdosMoney
setcglob 0x007BCACC+(158096*3), EmperorMoney
setcglob 0x007BCACC+(158096*4), FremenMoney
setcglob 0x007BCACC+(158096*5), SmugglersMoney
setcglob 0x007BCACC+(158096*6), MercenariesMoney
setcglob 0x007BCACC+(158096*7), SandwormMoney

;; ### Functions ###

setcglob 0x005A3BB0, WinMain

; drawing
setcglob 0x004308f0, Image__DrawLineClipTImage		;       Image__DrawLineClipTImage(int,int,int,int,int,int,int)
setcglob 0x004326b0, Image__BlitTImage_4326B0			;       Image__BlitTImage_4326B0
setcglob 0x0042eae0, Image__GetBitmapData			;       Image__GetBitmapData(char,int,int,int)
setcglob 0x0042ebe0, Image__LoadBitmap				;       Image__LoadBitmap(char,int,long,int,int,int,int)
setcglob 0x0042ef90, Image__CreateTImage			;       Image__CreateTImage(int width, int height, int, char *FilePath)
setcglob 0x0042f1d0, Image__CreateTImageWithTarga	;       Image__CreateTImageWithTarga(char *FilePath,int,int,int,int,int)
setcglob 0x0042f430, Image__CreateSurfaceWithBitmap	;       Image__CreateSurfaceWithBitmap(char,char,int,long,int,int,int,char,int,int)
setcglob 0x00430860, Image__ClearTImage				;       Image__ClearTImage(int,int,int)
setcglob 0x0042e8b0, Image__GetPaletteFromBitmap	;       Image__GetPaletteFromBitmap(lpFilename,int)
setcglob 0x0042e660, Image__LoadTarga					;       Image__LoadTarga(char *FilePath, int,int,int,int width, int heigh)
setcglob 0x0042e810, Image__LoadTarga2				;       Image__LoadTarga(char *FilePath,int,int)
setcglob 0x004305F0, Image__BlitClipTImage
setcglob 0x00491330, Load_Image
setcglob 0x00427690, Display__DrawStarportSideBarMenu
setcglob 0x004278A0, Display__DrawUpgradeSideBarMenu 
setcglob 0x00426E80, Display__DrawNormalSideBarMenu
setcglob 0x00492A70, DrawMenu

; Others
setcglob 0x00402FF0, IsLocalIp
setcglob 0x00404860, sosCODECInitStream
setcglob 0x004048C0, sosCODECDecompressData
setcglob 0x0040D2D0, WOL__SendGameResultsToServer
setcglob 0x00411E70, IsCurrentlyShown
setcglob 0x00417C40, WOL__StartGuestINetGame
setcglob 0x00417E50, WOL__StartHostINetGame
setcglob 0x00418140, SetgAllowPageUser
setcglob 0x00418480, IsOnlineGame
setcglob 0x004184A0, WOL__OpenWebsite				; (char *URL)
;AI
setcglob 0x0041CE50, CAI__RecalculateSideToAttack
setcglob 0x0041EF30, CAI__PlaceBuiltBuilding
setcglob 0x00420480, CAI__FindGroupForDelivery
setcglob 0x00423830, AIDebugPrint
;Others
setcglob 0x004239B0, BlitWhereTooltipWas
setcglob 0x004239F0, BlitWhereMouseWas
setcglob 0x00423A20, BlitMouse
setcglob 0x00423B40, BlitTooltip
setcglob 0x00423C50, BlitBandBox
setcglob 0x00423D20, BlitTiles
setcglob 0x00423FB0, BlitShroud
setcglob 0x00424140, DebugFillSomeRect
setcglob 0x004241E0, DebugBlitTileGrid
setcglob 0x004243E0, DebugBlitPath
setcglob 0x004245E0, DebugBlitFlaggedTiles
setcglob 0x004249B0, DebugPrintOnScreen
setcglob 0x004254A0, BlitCredits
setcglob 0x00425560, BlitPlayerCredits
setcglob 0x004255C0, InitMessageData
setcglob 0x004255E0, BlitMessages
setcglob 0x00425720, QueueMessage
setcglob 0x00425830, FreeMessageSlot
setcglob 0x00425860, BlitPowerbar
setcglob 0x004259F0, BlitColourFadeIn
setcglob 0x00425C60, BlitRadarScore
setcglob 0x00425FA0, BlitRadarHouseImage
setcglob 0x00426000, BlitRadar
setcglob 0x004263B0, UpdateIconImage
setcglob 0x00426E80, BlitUIIcons
setcglob 0x00427690, BlitUIStarportIcons
setcglob 0x004278A0, BlitUIUpgradeIcons
setcglob 0x00427CC0, BlitSidebarButtons
setcglob 0x00427D50, BlitBuildingFoundation
setcglob 0x00428070, BlitBullets
setcglob 0x00428220, BlitCursorPuffAnimations
setcglob 0x004282F0, BlitExplosions
setcglob 0x004285C0, HandleBuildingPlacement
setcglob 0x004288D0, HandleConcretePlacement
setcglob 0x00428C30, GetOwnershipStatusOfCell
setcglob 0x00428CA0, BlitGame
setcglob 0x00428E30, BlitObjects
setcglob 0x0042A5D0, BlitFlyingUnits
setcglob 0x0042AA90, AddToDisplayList
setcglob 0x0042ACE0, InitLensData
setcglob 0x0042ADA0, SetLensImage
setcglob 0x0042AF00, BlitInfantryBehindObjects
setcglob 0x0042B590, DebugFatal ; Game_ShowErrorAndExit
setcglob 0x00492EB0, AbortGame
setcglob 0x0042B730, ReportFileError
setcglob 0x0042BA50, _OpenFile
setcglob 0x0042BB40, CloseFile
setcglob 0x0042BB60, _ReadFile
setcglob 0x0042BB80, _WriteFile
setcglob 0x0042BBA0, SeekFile
setcglob 0x0042BC60, CreateGlobe
; Graphlib
setcglob 0x0042D450, BlitHorizontalLineRGB
setcglob 0x0042D660, BlitBeveledRectRGB
setcglob 0x0042DC00, BlitFontChars
setcglob 0x0042DD50, Graphlib__DrawRightAlignedText
setcglob 0x0042DD90, Graphlib__DrawTextWithBlackShadow
setcglob 0x0042DFB0, GetStringPixelWidth
setcglob 0x0042E1A0, GetFontHeight
setcglob 0x0042E1C0, Graphlib_font_pal
setcglob 0x0042E420, GetFontPaletteHandle
setcglob 0x0042E450, Graphlib__LoadFontFile
setcglob 0x0042F160, FreeTImage
setcglob 0x0042F1D0, CreateTImageWithTGA
setcglob 0x0042F3D0, GetColor16bit
setcglob 0x0042F430, CreateSurfaceWithBMP
setcglob 0x0042F800, BlitDirectDrawSurface
setcglob 0x0042FAC0, BlitTImage
setcglob 0x004305F0, BlitClipTImage1
setcglob 0x00430770, BlitClipTImage2
setcglob 0x00430860, ClearTImage
setcglob 0x004308F0, DrawLineClipTImage
setcglob 0x00430D10, DrawRectClipTImage
setcglob 0x00430D90, TImage_430D90_convert
setcglob 0x004310C0, BlitTImage_Flags_2
setcglob 0x004310F0, BlitTImage_Flags_2_1
setcglob 0x00431560, BlitTImage_1
setcglob 0x004318A0, ReduceWholePaletteByHalf
setcglob 0x004318F0, image_palette_4318F0
setcglob 0x004319B0, image_palette_4319B0
setcglob 0x00431AA0, image_palette_431AA0
setcglob 0x00431B50, image_palette_431B50
setcglob 0x00431C40, GetColor8bit
setcglob 0x00431D50, image_palette_431D50
setcglob 0x00431E40, BlitFontChar
setcglob 0x00431F50, BlitFontChar_0
setcglob 0x00432060, BlitFontChar_1
setcglob 0x004322D0, BlitTImageCopy
setcglob 0x004326B0, BlitClipTImageColoured
setcglob 0x00432D80, BlitTImage_4
setcglob 0x00433340, BlitTImage_Flags_4
setcglob 0x004337D0, BlitTImage_Flags_40
setcglob 0x00433C50, BlitTImage_Flag_80
setcglob 0x004340E0, BlitTImage_Flags_10
setcglob 0x00434620, BlitTImage_Shadow
setcglob 0x00434970, SharePalette
setcglob 0x004349A0, BlitTImage_Flag_200
setcglob 0x00434EA0, BlitTImage_11
setcglob 0x004352C0, AdjustPalette
setcglob 0x004353B0, BlitTImage_12
; Other
setcglob 0x00442910, IsBuildingWithBehaviorBuilt
setcglob 0x004429B0, GetDifficultyCostPercentage
setcglob 0x00442A00, GetUnitBuildSpeedPercentage
setcglob 0x00442B10, GetBuildingBuildSpeedPercentage
setcglob 0x00442BB0, GetUnitCost
setcglob 0x00442BE0, GetBuildingCost
setcglob 0x00443040, HandleSidebarButton
setcglob 0x004430D0, CanUnitBeBuilt
setcglob 0x004431D0, CanSideUpgradeBuildingGroup
setcglob 0x00443250, CheckIfMCVCanBeDeployedOn
setcglob 0x004432F0, MoreProductionBuildingsOfSameGroupExist
setcglob 0x00443370, IsAnyStarportUnitPicked
setcglob 0x00448460, GetRandomValue
setcglob 0x0044B170, SetUnitGroup
setcglob 0x0044B1D0, SetBuildingState17andFlags20000
setcglob 0x0044B1F0, SetBuildingAsPrimary
setcglob 0x0044C420, SetMouseCursor
; Map
setcglob 0x0044C480, RevealMap
setcglob 0x0044C4F0, Map__PlayerDefeated
setcglob 0x0044C520, Map__SetDeadOrder
setcglob 0x0044C6D0, LoadMapData
setcglob 0x0044CD10, UpdateShroudInRegion
setcglob 0x0044D500, FindNearestActiveThumper
setcglob 0x0044D630, FindSandwormsTarget
setcglob 0x0044D760, GetSpiceTileToHarvest
setcglob 0x0044D970, spice_44D970
setcglob 0x0044DA90, FindFreeSpotForInfantry
setcglob 0x0044DAC0, GetBuildingOnConcreteCount
setcglob 0x0044DEE0, RemoveBuildingStuff
setcglob 0x0044E120, RemoveUnitTileData
setcglob 0x0044E3B0, RevealCircle
setcglob 0x0044E790, GetFreeCrateIndex
setcglob 0x0044E8E0, PlaceCrate
setcglob 0x0044E9D0, GetCrateFromMap
setcglob 0x0044EC50, UpdateSpiceInRegion
setcglob 0x0044EF10, SpiceMound
setcglob 0x0044F110, RecycleCrate
setcglob 0x0044F440, GetMapVisState
; Memory
setcglob 0x0044F4D0, Alloc
setcglob 0x0044F630, Free
; Mission
setcglob 0x0044F8E0, Mission__LoadObjectives
setcglob 0x00453B90, Mission__CheckEvents
setcglob 0x004544E0, Mission__LoadVarsFile
setcglob 0x00454E00, Mission__LoadTechposFile
; Model
setcglob 0x00455290, EvaluateIfBuildingsOrUnitsExistForSide
setcglob 0x00455480, UncloakRemainingStealthUnit
setcglob 0x00455510, ModelBuildUnitPick
setcglob 0x00455870, ModelAddUnit
setcglob 0x00455FC0, ModelBuildBuildingPick
setcglob 0x00456210, ModelAddConcrete
setcglob 0x004563B0, ModelAddBuilding
setcglob 0x00456A30, ModelAddBullet
setcglob 0x00456D50, AddCursorPuffAnimationToQueue
setcglob 0x00456DF0, ModelAddExplosion
setcglob 0x004572F0, ModelStarportCancel
setcglob 0x00459450, GenerateUnitMoveOrder
setcglob 0x00459540, GenerateDockWithRefineryOrder
setcglob 0x004595E0, GenerateRepairSelectedUnitsOrder
setcglob 0x00459690, GenerateRepairSingleUnitOrder
setcglob 0x004596F0, GenerateUnitAttackUnitOrder
setcglob 0x00459800, GenerateUnitAttackBuildingOrder
setcglob 0x004598D0, GenerateUnitAttackTileOrder
setcglob 0x00459980, GenerateUnitGuardOrder
setcglob 0x00459A20, GenerateUnitScatterOrder
setcglob 0x00459AB0, GenerateUnitRetreatOrder
setcglob 0x00459B50, GenerateAllyOrder
setcglob 0x00459C40, GenerateStopOrder
setcglob 0x00459D20, GenerateBuildingAttackUnitOrder
setcglob 0x00459DE0, GenerateBuildingAttackBuildingOrder
setcglob 0x00459EA0, GenerateBuildingSetPrimaryOrder
setcglob 0x00459F30, GenerateBuildBuildingPickOrder
setcglob 0x00459F80, GenerateBuildBuildingCancelOrder
setcglob 0x00459FD0, GenerateBuildPlaceBuildingOrder
setcglob 0x0045A0B0, GenerateBuildUnitPickOrder
setcglob 0x0045A100, GenerateBuildUnitCancelOrder
setcglob 0x0045A150, GenerateBuildingRepairOrder
setcglob 0x0045A1A0, GenerateBuildingSellOrder
setcglob 0x0045A200, GenerateStarportPickOrder
setcglob 0x0045A260, GenerateStarportUnpickOrder
setcglob 0x0045A2B0, GenerateStarportPurchaseOrder
setcglob 0x0045A310, GenerateStarportCancelOrder
setcglob 0x0045A350, GenerateUpgradePickOrder
setcglob 0x0045A3A0, GenerateUpgradeCancelOrder
setcglob 0x0045A3F0, GenerateUnitDeployOrder
setcglob 0x0045A440, GenerateSpecialWeaponOrder
setcglob 0x0045B7E0, Model__ResetVars
setcglob 0x0045BF00, Orderdata_add
setcglob 0x0045D730, UpdateParticles
setcglob 0x0045D990, CanUnitUseSquare
setcglob 0x0045DE30, GetNextSquareInDirection
setcglob 0x0045DEE0, GetFreeAdjacentTile
setcglob 0x0045DFE0, GetFacing
setcglob 0x0045E020, GetFacing_and_252
; Setup
setcglob 0x00460550, SetTImageToResource
setcglob 0x00460C80, Setup__LoadUIBBFile
setcglob 0x00460EC0, Setup__LoadUI_LanguageFile
setcglob 0x00462010, SetPixelOnRadar8
setcglob 0x004620B0, SetPixelOnRadar16
setcglob 0x00462140, SetupRadarMap
setcglob 0x00462D10, LoadTileset
setcglob 0x00466140, Setup__LoadGraphicsFile
; CSide
setcglob 0x0046B990, CSide__Init
setcglob 0x0046BD20, CSide__InitCash
setcglob 0x0046BD40, CSide__GetNextFreeObject
setcglob 0x0046BD60, CSide__SetupObject
setcglob 0x0046BDB0, CSide__RemoveObject
setcglob 0x0046BE50, CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries
setcglob 0x0046C450, CSide__MyVersionOfBuilding
setcglob 0x0046C570, CSide__MyVersionOfUnit
setcglob 0x0046C640, CSide__ResetBuildingAndUnitIcons
setcglob 0x0046C680, CSide__AddBuildingIcon
setcglob 0x0046C6D0, CSide__AddUnitIcon
setcglob 0x0046C720, CSide__SpendMoneyIfSufficient
setcglob 0x0046C790, CSide__AddSpice
setcglob 0x0046C7B0, CSide__AddCash
setcglob 0x0046C7D0, CSide__UpdateCashDrip
setcglob 0x0046CA00, CSide__CalculatePower
setcglob 0x0046CB00, CSide__DeselectAllUnits
setcglob 0x0046CB20, CSide__DeselectAllBuildings
setcglob 0x0046CB40, CSide__SelectUnitsByGroupId
setcglob 0x0046CBD0, CSide__CenterViewportOnFirstUnitFromGroupId
setcglob 0x0046CCA0, CSide__GetQueuePos
setcglob 0x0046CD00, CSide__AddToQueue
setcglob 0x0046CE00, CSide__46CE00
setcglob 0x0046CE70, CSide__46CE70
setcglob 0x0046CF10, CSide__46CF10_HKEY_BattleFieldPos
setcglob 0x0046D180, CSide__ProcessPickupQueue
setcglob 0x0046D2E0, ChangeUnitOwner
setcglob 0x0046D6D0, CaptureBuilding
setcglob 0x0046D9A0, CSide__FindOtherPrimaryBuilding
setcglob 0x0046DDC0, CSide__UpdateStarportStockAndCosts
setcglob 0x0046DEF0, CSide__ProcessDeliveries
setcglob 0x0046E3F0, CSide__FindBestBasePosition
setcglob 0x0046E6B0, CSide__BlowupAll_surrender
setcglob 0x0046E740, FindFirstBuildingByGroup
setcglob 0x0046E7A0, CSide__InitStarportAndUpgradeIcons
setcglob 0x0046E9C0, CSide__LetAITakeOver
setcglob 0x0046EA50, GenerateSurrenderOrder
setcglob 0x0046EAC0, CSide__ReturnMoneyFromStarportOrder
setcglob 0x0046EAF0, CSide__ResetStarportOrderCost
setcglob 0x0046EB00, CSide__ResetEnemyForSide
; Sound
setcglob 0x0046EB60, LoadSoundEffects
setcglob 0x0046ECC0, FindSoundEffectByFilename
setcglob 0x0046ED00, LoadSoundEffect
setcglob 0x0046EDA0, ISampleManager__Init
setcglob 0x0046EEE0, ISampleManager__Allocate
setcglob 0x0046EF40, ISampleManager__Deallocate
setcglob 0x0046EF90, ISampleManager__LoadSampleCache
setcglob 0x0046F080, ISampleManager__HandleCache
setcglob 0x0046F170, ISampleManager__PlaySample
setcglob 0x0046F210, ISampleManager__PlaySample_0
setcglob 0x0046F330, ISampleManager__setsamplevals
setcglob 0x0046F390, ISampleManager__SetSampleData
setcglob 0x0046F3E0, ISampleManager__Deinit
setcglob 0x0046F400, ISampleManager__EndSamples
setcglob 0x0046F420, ISampleManager__EndSample
setcglob 0x0046F450, InitSampleManager
setcglob 0x0046F490, DeinitSampleManager
setcglob 0x0046F4F0, IsSoundPlaying
setcglob 0x0046F540, PlaySoundAt
setcglob 0x0046F6A0, audio_priority_init_46F6A0
setcglob 0x0046F6C0, audio_priority_46F6C0
setcglob 0x0046F6E0, audio_46F6E0
setcglob 0x0046F700, is_audio_priority_46F700
setcglob 0x0046F770, audio_priority_46F770
setcglob 0x0046F7D0, getqueuedsoundid
setcglob 0x0046F820, QueueAudioToPlay
setcglob 0x0046F900, PlayMentatSound
setcglob 0x0046F980, init_audio
setcglob 0x0046F9A0, CopySample
setcglob 0x0046FAA0, SoundLoop
setcglob 0x0046FCC0, ISampleManager__CopyStreamSample
setcglob 0x0046FD90, ISampleManager__InitStream
setcglob 0x0046FF30, ISampleManager__StreamLoop
setcglob 0x00470070, ISampleManager__CloseStream
setcglob 0x004700B0, ISampleManager__SetStreamVolume
setcglob 0x004700E0, OpenAudioStream
setcglob 0x00470170, CloseAudioStream
; CUIManager
setcglob 0x00470E60, CUIManager__ReplaceWithOne_470E60
setcglob 0x004734E0, CUIManager__JumpToMenu 				;(char *menu)
setcglob 0x00473840, CUIManager__invoke_473840
setcglob 0x004752C0, CUIManager__LoadMission
setcglob 0x00478C20, CUIManager__LoadDune2000Cfg
setcglob 0x00478FA0, CUIManager__SaveDune2000Cfg
setcglob 0x00479E80, CUIManager__StartGame
setcglob 0x0047BAE0, CUIManager__SendInGameMsgTo
setcglob 0x00480F30, CUIManager__CreateWigNet
setcglob 0x00482690, CUIManager__GetCD					 ;       Get_CD::Get_CD(void)
; Data
setcglob 0x0048DC10, GetTextID			; (char *key, int unk)
setcglob 0x0048DCC0, GetTextString			; (int id, int unk)
setcglob 0x0048DD90, GetSoundTableID
setcglob 0x0048DE10, GetSampleFilename
; Other
setcglob 0x004936A0, GetNextSquare
setcglob 0x00493EB0, TurnUnitInDirection
setcglob 0x00493F40, TurnUnitBarrelInDirection
setcglob 0x00493FD0, TurnBuildingTurret
setcglob 0x00494050, SetUnitAnimationType
setcglob 0x004940D0, AnimateInfantryUnit
setcglob 0x00494210, Unit_movement_494210
setcglob 0x00494260, MoveUnit
setcglob 0x00494CD0, HandleDeathHand
setcglob 0x00494FD0, Unit_movement_bullet_494FD0
setcglob 0x004954A0, UnitSetSpeed
setcglob 0x004954B0, TurnUnit
setcglob 0x00495560, MoveUnitSimple
setcglob 0x00495B90, UnitShootTarget
setcglob 0x00496030, BuildingShootTarget
setcglob 0x00496480, UnitAttack
setcglob 0x00496790, BuildingAttack
setcglob 0x00496B00, UnitAttackUnit
setcglob 0x00496D10, UnitAttackBuilding
setcglob 0x00496FF0, UnitAttackTile
setcglob 0x00497090, BuildingAttackTile
setcglob 0x004970F0, CheckDistance
setcglob 0x00497140, CanBuildingAttackTile
setcglob 0x00497270, CanUnitAttackTile
setcglob 0x00497390, GetBuildingInRange
setcglob 0x00497490, IsBuildingInRange
setcglob 0x00497530, IsBuildingInRange_0
setcglob 0x00497550, CanUnitAttackBuilding
setcglob 0x004975D0, BuildingAttackUnit
setcglob 0x004976B0, BuildingAttackBuilding
setcglob 0x004977A0, ReleaseInfantryFromBuilding
setcglob 0x004979A0, GetRandomAdjacentTile
setcglob 0x00497AF0, KickUnitsOutOfTile
setcglob 0x00497B90, UpdateUnit
setcglob 0x0049B7F0, UpdateBuilding
setcglob 0x0049BE60, UpdateBullet
setcglob 0x0049CA10, DestroyBuilding
setcglob 0x0049CDE0, DestroyUnit
setcglob 0x0049D170, ClosestBuildingTile
setcglob 0x0049D280, UnitTileOccupiedByBuilding
setcglob 0x0049D2E0, DamageTiles
setcglob 0x0049DD80, UpdateExplosion
setcglob 0x0049DEB0, UnitAdjustState
setcglob 0x0049E120, SetBuildingState
setcglob 0x0049E140, Unit_49E140
setcglob 0x0049E160, facing_49E160
setcglob 0x0049E290, tile_driveon_49E290
setcglob 0x0049E3E0, UnitDeliver
setcglob 0x0049E6B0, GetFlyingUnitSpawnPositionAndFacing
setcglob 0x0049E770, LaunchOrnithopters
setcglob 0x0049E860, LaunchDeathHand
setcglob 0x0049E9D0, NewBestBullet
setcglob 0x0049EBF0, PickupCrate
setcglob 0x0049F1B0, MakeUnitsStealthInRange
setcglob 0x0049F240, PlayUnitResponse
setcglob 0x0049F310, SetUnitToFlicker
setcglob 0x0049F320, SetBuildingToFlicker
setcglob 0x0049F330, UnmarkTileWithFlyingUnit
setcglob 0x0049F370, MarkTileWithFlyingUnit
setcglob 0x0049F3B0, GetSpeed
setcglob 0x0049F4A0, CanUnitAcceptOrders
setcglob 0x0049F4D0, RevealTilesSeenByBuildingsAndUnits
setcglob 0x0049F580, UnitCloakStart
setcglob 0x0049F5C0, UnitUncloakStart
setcglob 0x0049F5F0, UnitIsCloaked
setcglob 0x0049F610, UnitHandleCloaking
setcglob 0x0049F750, ObjectsAbleToRevealStealthUnitsAreNearby
setcglob 0x0049F870, NeutralBecomeHostile
setcglob 0x0049F8F0, CrushUnit
setcglob 0x0049FA50, TryCrushUnit
setcglob 0x0049FB40, CanDeviatedUnitRevert
setcglob 0x004A31D0, GameWindowProcedure
setcglob 0x004A39E0, Parse_Command_Line
setcglob 0x004A4660, GetSide
setcglob 0x004A46B0, GetUnit
setcglob 0x004A4740, GetBuilding
setcglob 0x004A47F0, map_4A47F0
setcglob 0x004A4900, EnemyUnitInRange
setcglob 0x004A4FD0, EnemyBuildingInRange
setcglob 0x004A5200, UnitOccupiesTile
setcglob 0x004A5270, GetUnitOnTile
setcglob 0x004A54A0, GetNextUnitOnTile
setcglob 0x004A5570, BuildingOccupiesTile
setcglob 0x004A5640, GetBuildingOnTile_0
setcglob 0x004A56F0, GetBuildingOnTile_1
setcglob 0x004A5790, DeselectAllForAllSides
setcglob 0x004A57C0, IsAnyUnitSelected
setcglob 0x004A57F0, IsAnyUnitWithBehaviorSelected
setcglob 0x004A5830, IsAnyBuildingSelected
setcglob 0x004A5860, GetSelectedBuildingWithBarrel
setcglob 0x004A58B0, IsAnyArmedUnitSelected
setcglob 0x004A58F0, IsAnyArmedUnitSelectedExceptThis
setcglob 0x004A5940, IsAnyVehicleSelected
setcglob 0x004A5980, IsAnyInfantrySelected
setcglob 0x004A59C0, AllSelectedUnitsHaveBehavior
setcglob 0x004A5A10, IsAnyCrusherSelected
setcglob 0x004A5A50, SelectUnit
setcglob 0x004A5AB0, SelectBuilding
setcglob 0x004A5AE0, SelectAllUnitsInArea
setcglob 0x004A5C00, CenterViewportOnSelectedUnits
setcglob 0x004A5CD0, CenterViewportOnSelectedBuildings
setcglob 0x004A5DE0, SelectNextUnit
setcglob 0x004A5E80, GetRefineryIndex
setcglob 0x004A5F80, GetNearestBuildingWithBehavior
setcglob 0x004A60E0, GetNearestFreeTileForUnit
setcglob 0x004B1890, cinit
setcglob 0x004BCD80, _filelength
setcglob 0x004BCE20, __fileno

; winapi
setcglob 0x008CF5EC, _imp__LoadLibraryA
setcglob 0x008CF584, _imp__GetProcAddress
setcglob 0x008CF598, _imp__GetStdHandle
setcglob 0x008CF4E4, _imp__WriteFile
setcglob 0x008CF4F8, _imp__OutputDebugStringA
setcglob 0x008CF498, _imp__CreatePolygonRgn
setcglob 0x008CF510, _imp__GetTickCount
setcglob 0x008CF714, _imp__recvfrom
setcglob 0x008CF718, _imp__sendto
setcglob 0x008CF704, _imp__htonl
setcglob 0x008CF738, _imp__htons
setcglob 0x008CF6FC, _imp__gethostbyname
setcglob 0x008CF734, _imp__socket
setcglob 0x008CF500, _imp__GetLastError
setcglob 0x008CF508, _imp__GetModuleHandleA
setcglob 0x008CF6E8, _imp__timeGetTime
setcglob 0x008CF69C, _imp__DefWindowProcA
setcglob 0x008CF544, _imp__FindFirstFileA
setcglob 0x008CF540, _imp__FindClose
setcglob 0x008CF554, _imp__FindNextFileA
setcglob 0x008CF4E8, _imp__Sleep
setcglob 0x008CF54C, _imp__GetModuleFileNameA
setcglob 0x008CF6F4, _imp__timeBeginPeriod
setcglob 0x008CF6F0, _imp__timeEndPeriod
setcglob 0x008CF698, _imp__MessageBoxA  
setcglob 0x008CF6C0, _imp__ShowCursor
setcglob 0x008CF61C, _imp__GetCurrentProcess
setcglob 0x008CF578, _imp__ExitProcess

; clib
setcglob 0x004B15B0, vsprintf
setcglob 0x004B0670, sprintf
setcglob 0x004B2B60, strlen
setcglob 0x004b1700, tolower
setcglob 0x004b2ad0, strcmp
setcglob 0x004b0e30, malloc
setcglob 0x004ba640, calloc
setcglob 0x004b29e0, strcpy
setcglob 0x004b0ee0, free
setcglob 0x004b07a0, atoi
setcglob 0x004b08b0, strtok
setcglob 0x004b0fd0, fopen
setcglob 0x004b0ff0, fclose
setcglob 0x004b1070, fread
setcglob 0x004b11b0, fwrite
setcglob 0x004b29f0, strcat
setcglob 0x004b13a0, strncmp
setcglob 0x004bcb20, strcmpi
setcglob 0x004b1b20, memcpy
setcglob 0x004b2be0, memset
setcglob 0x004B07B0, strncpy
setcglob 0x004B1810, rand
setcglob 0x004B1300, fseek
setcglob 0x004B6A80, ftell
setcglob 0x004BD490, strrchr
setcglob 0x004B2030, fprintf
setcglob 0x004BC5B0, stat

; imports
;Address  Ordinal Name                               Library 
;-------  ------- ----                               ------- 
;008CF464         RegCloseKey                        ADVAPI32
;008CF468         RegOpenKeyExA                      ADVAPI32
;008CF46C         RegQueryValueExA                   ADVAPI32
;008CF470         RegSetValueExA                     ADVAPI32
;008CF474         RegOpenKeyA                        ADVAPI32
;008CF47C         DirectDrawCreate                   DDRAW   
;008CF484 4       __imp_DirectPlayLobbyCreateA       DPLAYX  
;008CF488 1       __imp_DirectPlayCreate             DPLAYX  
;008CF490         DirectSoundCreate                  DSOUND  
;008CF498         CreatePolygonRgn                   GDI32   
;008CF49C         GetStockObject                     GDI32   
;008CF4A0         PtInRegion                         GDI32   
;008CF4A4         TextOutA                           GDI32   
;008CF4A8         DeleteObject                       GDI32   
;008CF4AC         GetDeviceCaps                      GDI32   
;008CF4B4         FileTimeToLocalFileTime            KERNEL32
;008CF4B8         HeapCreate                         KERNEL32
;008CF4BC         VirtualFree                        KERNEL32
;008CF4C0         LeaveCriticalSection               KERNEL32
;008CF4C4         EnterCriticalSection               KERNEL32
;008CF4C8         InitializeCriticalSection          KERNEL32
;008CF4CC         DeleteCriticalSection              KERNEL32
;008CF4D0         InterlockedDecrement               KERNEL32
;008CF4D4         InterlockedIncrement               KERNEL32
;008CF4D8         GetCommandLineA                    KERNEL32
;008CF4DC         GetVersion                         KERNEL32
;008CF4E0         GetStartupInfoA                    KERNEL32
;008CF4E4         WriteFile                          KERNEL32
;008CF4E8         Sleep                              KERNEL32
;008CF4EC         MultiByteToWideChar                KERNEL32
;008CF4F0         WideCharToMultiByte                KERNEL32
;008CF4F4         lstrlenA                           KERNEL32
;008CF4F8         OutputDebugStringA                 KERNEL32
;008CF4FC         CloseHandle                        KERNEL32
;008CF500         GetLastError                       KERNEL32
;008CF504         CreateFileA                        KERNEL32
;008CF508         GetModuleHandleA                   KERNEL32
;008CF50C         HeapDestroy                        KERNEL32
;008CF510         GetTickCount                       KERNEL32
;008CF514         SetEvent                           KERNEL32
;008CF518         ResetEvent                         KERNEL32
;008CF51C         WaitForMultipleObjects             KERNEL32
;008CF520         CreateEventA                       KERNEL32
;008CF524         GetExitCodeProcess                 KERNEL32
;008CF528         CreateProcessA                     KERNEL32
;008CF52C         CreateDirectoryA                   KERNEL32
;008CF530         MoveFileA                          KERNEL32
;008CF534         DeleteFileA                        KERNEL32
;008CF538         HeapFree                           KERNEL32
;008CF53C         GetProcessHeap                     KERNEL32
;008CF540         FindClose                          KERNEL32
;008CF544         FindFirstFileA                     KERNEL32
;008CF548         GetDriveTypeA                      KERNEL32
;008CF54C         GetModuleFileNameA                 KERNEL32
;008CF550         RtlUnwind                          KERNEL32
;008CF554         FindNextFileA                      KERNEL32
;008CF558         GetVolumeInformationA              KERNEL32
;008CF55C         GetVersionExA                      KERNEL32
;008CF560         GlobalFree                         KERNEL32
;008CF564         GlobalUnlock                       KERNEL32
;008CF568         GlobalHandle                       KERNEL32
;008CF56C         GlobalLock                         KERNEL32
;008CF570         GlobalAlloc                        KERNEL32
;008CF574         GlobalMemoryStatus                 KERNEL32
;008CF578         ExitProcess                        KERNEL32
;008CF57C         VirtualAlloc                       KERNEL32
;008CF580         GetLogicalDriveStringsA            KERNEL32
;008CF584         GetProcAddress                     KERNEL32
;008CF588         ReadFile                           KERNEL32
;008CF58C         SetFilePointer                     KERNEL32
;008CF590         LCMapStringA                       KERNEL32
;008CF594         LCMapStringW                       KERNEL32
;008CF598         GetStdHandle                       KERNEL32
;008CF59C         GetLocalTime                       KERNEL32
;008CF5A0         FlushFileBuffers                   KERNEL32
;008CF5A4         UnhandledExceptionFilter           KERNEL32
;008CF5A8         FreeEnvironmentStringsA            KERNEL32
;008CF5AC         FreeEnvironmentStringsW            KERNEL32
;008CF5B0         GetEnvironmentStrings              KERNEL32
;008CF5B4         GetEnvironmentStringsW             KERNEL32
;008CF5B8         GetCPInfo                          KERNEL32
;008CF5BC         GetACP                             KERNEL32
;008CF5C0         GetOEMCP                           KERNEL32
;008CF5C4         SetHandleCount                     KERNEL32
;008CF5C8         GetFileType                        KERNEL32
;008CF5CC         GetStringTypeA                     KERNEL32
;008CF5D0         GetStringTypeW                     KERNEL32
;008CF5D4         SetUnhandledExceptionFilter        KERNEL32
;008CF5D8         IsBadReadPtr                       KERNEL32
;008CF5DC         IsBadWritePtr                      KERNEL32
;008CF5E0         IsBadCodePtr                       KERNEL32
;008CF5E4         SetStdHandle                       KERNEL32
;008CF5E8         GetTimeZoneInformation             KERNEL32
;008CF5EC         LoadLibraryA                       KERNEL32
;008CF5F0         SetEndOfFile                       KERNEL32
;008CF5F4         CompareStringA                     KERNEL32
;008CF5F8         CompareStringW                     KERNEL32
;008CF5FC         SetEnvironmentVariableA            KERNEL32
;008CF600         GetFullPathNameA                   KERNEL32
;008CF604         GetCurrentDirectoryA               KERNEL32
;008CF608         FileTimeToSystemTime               KERNEL32
;008CF60C         TerminateProcess                   KERNEL32
;008CF610         HeapAlloc                          KERNEL32
;008CF614         HeapReAlloc                        KERNEL32
;008CF618         GetDiskFreeSpaceA                  KERNEL32
;008CF620         SetVolumeLabelA                    KERNEL32
;008CF62C         FindExecutableA                    SHELL32 
;008CF634         PostMessageA                       USER32  
;008CF638         GetDlgItem                         USER32  
;008CF63C         SetFocus                           USER32  
;008CF640         EndDialog                          USER32  
;008CF644         EndPaint                           USER32  
;008CF648         SendDlgItemMessageA                USER32  
;008CF64C         DialogBoxParamA                    USER32  
;008CF650         PtInRect                           USER32  
;008CF654         SetCursorPos                       USER32  
;008CF658         IntersectRect                      USER32  
;008CF65C         SetRect                            USER32  
;008CF660         wsprintfA                          USER32  
;008CF664         SetForegroundWindow                USER32  
;008CF668         GetUpdateRect                      USER32  
;008CF66C         FillRect                           USER32  
;008CF670         ShowWindow                         USER32  
;008CF674         DispatchMessageA                   USER32  
;008CF678         PeekMessageA                       USER32  
;008CF67C         TranslateMessage                   USER32  
;008CF680         LoadStringA                        USER32  
;008CF684         SendMessageA                       USER32  
;008CF688         KillTimer                          USER32  
;008CF68C         SetTimer                           USER32  
;008CF690         GetWindowDC                        USER32  
;008CF694         ReleaseDC                          USER32  
;008CF69C         DefWindowProcA                     USER32  
;008CF6A0         GetClassNameA                      USER32  
;008CF6A4         GetWindowRect                      USER32  
;008CF6A8         EnumWindows                        USER32  
;008CF6AC         CloseWindow                        USER32  
;008CF6B0         InvalidateRect                     USER32  
;008CF6B4         GetDesktopWindow                   USER32  
;008CF6B8         UpdateWindow                       USER32  
;008CF6BC         RegisterClassA                     USER32  
;008CF6C0         ShowCursor                         USER32  
;008CF6C4         CreateWindowExA                    USER32  
;008CF6C8         LoadIconA                          USER32  
;008CF6CC         WaitForInputIdle                   USER32  
;008CF6D0         FindWindowA                        USER32  
;008CF6D4         GetTopWindow                       USER32  
;008CF6D8         BeginPaint                         USER32  
;008CF6E0         timeSetEvent                       WINMM   
setcglob 0x008CF6E4, _imp__waveOutGetNumDevs@0
;008CF6E8         timeGetTime                        WINMM   
;008CF6EC         timeKillEvent                      WINMM     
;008CF6FC 52      __imp_gethostbyname                WSOCK32 
;008CF700 2       __imp_bind                         WSOCK32 
;008CF704 8       __imp_htonl                        WSOCK32 
;008CF708 57      __imp_gethostname                  WSOCK32 
;008CF70C 21      __imp_setsockopt                   WSOCK32 
;008CF710 7       __imp_getsockopt                   WSOCK32 
;008CF714 17      __imp_recvfrom                     WSOCK32 
;008CF718 20      __imp_sendto                       WSOCK32 
;008CF71C 14      __imp_ntohl                        WSOCK32 
;008CF720 116     __imp_WSACleanup                   WSOCK32 
;008CF724 115     __imp_WSAStartup                   WSOCK32 
;008CF728 101     __imp_WSAAsyncSelect               WSOCK32 
;008CF72C 108     __imp_WSACancelAsyncRequest        WSOCK32 
;008CF730 3       __imp_closesocket                  WSOCK32 
;008CF734 23      __imp_socket                       WSOCK32 
;008CF738 9       __imp_htons                        WSOCK32 
;008CF73C 15      __imp_ntohs                        WSOCK32 
setcglob 0x008CF744, _imp__AIL_set_sample_position@8
setcglob 0x008CF748, _imp__AIL_set_sample_pan@8
setcglob 0x008CF74C, _imp__AIL_set_sample_volume@8
setcglob 0x008CF750, _imp__AIL_stop_sample@4
setcglob 0x008CF754, _imp__AIL_end_sample@4
setcglob 0x008CF758, _imp__AIL_set_sample_file@12
setcglob 0x008CF75C, _imp__AIL_set_preference@8
setcglob 0x008CF760, _imp__AIL_last_error@0
setcglob 0x008CF764, _imp__AIL_start_sample@4
setcglob 0x008CF768, _imp__AIL_waveOutOpen@16
setcglob 0x008CF76C, _imp__AIL_startup@0
setcglob 0x008CF770, _imp__AIL_allocate_sample_handle@4
setcglob 0x008CF774, _imp__AIL_init_sample@4
setcglob 0x008CF778, _imp__AIL_sample_status@4
setcglob 0x008CF77C, _imp__AIL_sample_volume@4
setcglob 0x008CF780, _imp__AIL_load_sample_buffer@16
setcglob 0x008CF784, _imp__AIL_sample_buffer_ready@4
setcglob 0x008CF788, _imp__AIL_set_sample_playback_rate@8
setcglob 0x008CF78C, _imp__AIL_set_sample_type@12
setcglob 0x008CF790, _imp__AIL_minimum_sample_buffer_size@12
setcglob 0x008CF794, _imp__AIL_shutdown@0
setcglob 0x008CF798, _imp__AIL_set_sample_loop_count@8
;008CF7A0         OleInitialize                      ole32   
;008CF7A4         CoCreateInstance                   ole32   
;008CF7A8         OleUninitialize                    ole32   

;; ### C Compiler stuff ###

cglobal __chkstk_ms
cglobal dummy

__chkstk_ms:
  ret

dummy:
  ret
