#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

// This header works with sym.asm which defines the Vanilla symbols
// This header will be split up as it becomes larger

typedef char eBuildingGroupType;
typedef unsigned char eSideType;
typedef int32_t _DWORD;
typedef int16_t _WORD;
typedef uint8_t _BYTE;
typedef int BOOL;

typedef struct dwXYStruct
{
  int X;
  int Y;
} dwXYStruct;

#include "dune2000/side.h"
#include "dune2000/ai.h"
#include "dune2000/templates.h"
#include "dune2000/map.h"
#include "dune2000/crates.h"
#include "dune2000/mission.h"
#include "dune2000/text.h"
#include "dune2000/vars.h"
#include "dune2000/order.h"
#include "dune2000/graphlib.h"
#include "dune2000/sound.h"
#include "dune2000/uimgr.h"
#include "dune2000/vk.h"

typedef struct NetPlayer
{
    char name[20];
    int unk;
    char color;
    char side;
    char handicap;
    char unk2;
    unsigned int ip;
    int unk3;
    int unk4;
}NetPlayer;

typedef struct TechPosEntry
{
  char UnitType_Atreides;
  char UnitType_Harkonnen;
  char UnitType_Ordos;
  char PosX;
  char PosY;
} TechPosEntry;

typedef struct TacticalStruct
{
  int __SidebarMode;
  int __PrevSidebarMode;
  int __SidebarButtonMode;
  char __RadarScoreView;
  char __CanPlace;
  char field_E;
  char field_F;
  int __Strip1ScrollPos1;
  int __Strip2ScrollPos1;
  int __Strip1ScrollPos2;
  int __Strip2ScrollPos2;
  int __BandboxStartX;
  int __BandboxStartY;
  bool __DraggingBandbox;
  POINT __Bookmarks[4];
  int __radarfade;
  char __isLowOnFunds;
  char __RadarOnline;
  char field_52;
  char field_53;
} TacticalStruct;

typedef struct TooltipStruct
{
  char __LongString[80];
  POINT __ScreenPosition;
  POINT __LastPosition;
  char __IsShown;
  unsigned int __Timer;
  RECT __UnusedRect;
  RECT __TotalRect;
  RECT __LongStringRect;
  char __ShortString[12];
  RECT __ShortStringRect;
  int __Cost;
  int __Color;
  TImage *__BackupImage;
} TooltipStruct;

typedef struct SidebarButtonData
{
  TImage *__images[3];
  int __xpos;
  int __ypos;
  char __status;
  char __prevstatus;
} SidebarButtonData;

typedef struct HighScoreStruct
{
  char c_field_0_state;
  int __rank;
  char __name[200];
  int __side;
  char __missionumber;
  int __score;
} HighScoreStruct;

// ### Constants ###

enum eSideType
{
  SIDE_ATREIDES = 0x0,
  SIDE_HARKONNEN = 0x1,
  SIDE_ORDOS = 0x2,
  SIDE_EMPEROR = 0x3,
  SIDE_FREMEN = 0x4,
  SIDE_SMUGGLER = 0x5,
  SIDE_MERCENARY = 0x6,
  SIDE_OTHER = 0x7,
  SIDE_COUNT = 0x8,
  SIDE_NONE = 0xFF,
};

enum GameEndStates
{
    GES_ENDEDNORMALLY,
    GES_ISURRENDERED,
    GES_OPPONENTSURRENDERED,
    GES_OUTOFSYNC,
    GES_CONNECTIONLOST,
    GES_WASHGAME,
    GES_DRAWGAME,
    GES_UNKNOWNENDSTATE
};

typedef enum eGameType
{
  GAME_CAMPAIGN = 0x0,
  GAME_SKIRMISH = 0x1,
  GAME_NETWORK = 0x2,
  GAME_SERIAL = 0x3,
  GAME_MODEM = 0x4,
  GAME_INTERNET = 0x5,
} eGameType;

enum GameStates
{
    GS_INTRO,
    GS_BRIEFING,
    GS_MAINLOOP,
    GS_LOAD,
    GS_SAVE,
    GS_QUIT
};

enum NetworkTypes
{
    NT_NONE,
    NT_UNKNOWN,
    NT_UDP,
    NT_SERIAL,
    NT_IPXDIRECTPLAY
};

enum CursorTypes
{
  CURSOR_POINTER = 0,
  CURSOR_MOVE = 1,
  CURSOR_ATTACK = 2,
  CURSOR_CANT_MOVE = 3,
  CURSOR_ENTER = 4,
  CURSOR_OVER_UNIT = 5,
  CURSOR_MOVE_RADAR = 6,
  CURSOR_CANT_SELL = 7,
  CURSOR_CANT_REPAIR = 8,
  CURSOR_SPECIAL = 9,
  CURSOR_SELL = 10,
  CURSOR_REPAIR = 11,
  CURSOR_DEPLOY = 12,
  CURSOR_CANT_DEPLOY = 13,
  CURSOR_SCROLL_N = 14,
  CURSOR_SCROLL_NE = 15,
  CURSOR_SCROLL_E = 16,
  CURSOR_SCROLL_SE = 17,
  CURSOR_SCROLL_S = 18,
  CURSOR_SCROLL_SW = 19,
  CURSOR_SCROLL_W = 20,
  CURSOR_SCROLL_NW = 21,
  CURSOR_CANT_SCROLL_N = 22,
  CURSOR_CANT_SCROLL_NE = 23,
  CURSOR_CANT_SCROLL_E = 24,
  CURSOR_CANT_SCROLL_SE = 25,
  CURSOR_CANT_SCROLL_S = 26,
  CURSOR_CANT_SCROLL_SW = 27,
  CURSOR_CANT_SCROLL_W = 28,
  CURSOR_CANT_SCROLL_NW = 29,
  CURSOR_DEATHHAND = 30,
  CURSOR_DEMOLISH = 31,
  CURSOR_MOVE_MOUNTAIN = 32,
  CURSORS_MAX = 33,
};

enum eSidebarButton
{
  SIDEBARBUTTON_UPARROW1 = 0,
  SIDEBARBUTTON_DOWNARROW1 = 1,
  SIDEBARBUTTON_UPARROW2 = 2,
  SIDEBARBUTTON_DOWNARROW2 = 3,
  SIDEBARBUTTON_REPAIR = 4,
  SIDEBARBUTTON_SELL = 5,
  SIDEBARBUTTON_MAPTOGGLE = 6,
  SIDEBARBUTTON_GUARD = 7,
  SIDEBARBUTTON_RETREAT = 8,
  SIDEBARBUTTON_UPGRADE = 9,
  SIDEBARBUTTON_STARPORT = 10,
  SIDEBARBUTTON_OPTIONS = 11,
  SIDEBARBUTTON_MAIN1 = 12,
  SIDEBARBUTTON_MAIN2 = 13,
  SIDEBARBUTTON_PURCHASE = 14,
};

// Side (HouseClass)
#define HC_CREDITS 0x2425C
#define HC_SILO_CREDITS 0x24254
#define HC_SPICE_HARVESTED 0x2477C
#define HC_BUILDINGS_DESTROYED 0x24C94
#define HC_UNITS_KILLED 0x24C90
#define HC_BUILDINGS_OWNED 0x24784

// ### Variables ###

//Network settings
extern unsigned int SendRate;
extern unsigned int ReconnectTimeOutToPlayer;
extern unsigned int InitialConnectTimeOut;
extern unsigned int UnknownNetVar1;
extern unsigned int UnknownNetVar2;
extern unsigned int UnknownNetVar3;
extern unsigned int UnknownNetVar4;
extern unsigned int UnknownNetVar5;
extern int NetworkType;

//Multiplayer settings
extern char NetworkGame;
extern unsigned char gNetUnitCount;
extern unsigned char gNetTechLevel;
extern unsigned short gNetStartingCredits;
extern unsigned char gNetAIPlayers;
extern bool gNetCrates;
extern unsigned char gNetWorms;
extern char gNetPlayerName[];
extern unsigned char gNetPlayerSide;
extern unsigned char gNetPlayerColor;
extern unsigned char gNetPlayerHandicap;
extern char gNetIPAddress[];
extern char gNetModemPhone[];
extern unsigned char gNetSerialComPort;
extern char NetMap[];
extern char LoginName[];
// extern int gTotalPlayers;
extern unsigned int NetPlayersStartingCredits[];
extern char NetPlayersTechLevel[];
extern char NetPlayersColor[]; //Actually there is more than just the colors, not sure what... struct size 60byte, first byte = color
extern char MissionMap[];
extern struct NetPlayer NetPlayers[];
extern char NetPlayerNamesArray[6][60];

//Game settings
extern bool FirstTimePlay;
extern unsigned int ScrollRate;
extern unsigned short GameSpeed;
extern unsigned int GameBitsPerPixel;
extern unsigned int SFXVolume;
extern unsigned int MusicVolume;
extern bool MoviesEnabled;
extern bool SoundsEnabled;
extern bool JumpToWOLLogin;
extern bool VideoBackBuffer;
extern bool DebugMode;
extern bool ForceNoCD;
extern bool FullScreen;
extern bool Force480Height;

//WOL
extern bool AllowFindUser;
extern bool AllowPageUser;
extern bool LanguageFilter;
extern bool ShowAllGames;
extern bool Microphone;
extern unsigned char WOLMaxPlayers;
extern bool WOLTournamentGame;
extern bool WOLPrivateGame;
extern int WOLGameId;
extern int GameStartTickCount;
extern int GameEndTickCount;
extern int GameEndState;

//Screen display stuff
extern int ScreenWidth;
extern int ScreenHeight;
extern int GameWidth;
extern int GameHeight;
extern int CreditsUIPosX;
extern int PowerBarUIPosX;
extern int PowerBarUIPosY;
extern int SideBarPanelRightUIPosX;
extern int SideBarPanelLeftUIPosX;
extern int SideBarPanelsPosY;
extern int SideBarIconCount;
extern int SideBarIconHeight;
extern int MiniMapUIPosX;
extern int SideBarWidth;
extern int BattleFieldWidth;
extern int BattleFieldHeight;
extern int _OptionsBarHeight;
extern int CurrentCursorId;

//Others
extern int                  _InfantryCellspotPosOffsets2[10];
extern int                  _InfantryCellspotPosOffsets[10];
extern bool                 gAllowPageUser;

extern unsigned char        MissionNumber;



extern int                  GameState;
extern int                  _firgcrap_dword_4E3AE8;

extern int                  _RadarLocationX;
extern int                  _RadarLocationY;
extern int                  _SideBarIconCount;
extern int                  _SidebarIconWidth;
extern int                  _SideBarIconHeight1;
extern int                  _SideBarIconHeight2;
extern int                  _SidebarStrip1XPos;
extern int                  _SidebarStrip1YPos;
extern int                  _SidebarStrip2XPos;
extern int                  _SidebarStrip2YPos;
extern int                  _PowerBarUIPosX;
extern int                  _PowerBarUIPosY;
extern int                  _CreditsTextXPos;
extern short                gDifficultyLevel;
extern short                _SandTileIDs[8];
extern POINT                _UnitAnimTypeFrames[16];
extern char                 _gFullscreen_DebugModes_pathfinddebug;
extern int                  gBitsPerPixel;
extern int                  _ScreenClipWidth;
extern int                  _ScreenClipHeight;
extern int                  gScrollSize;
extern POINT                _gMousePos;
extern int                  MousePositionX;
extern int                  MousePositionY;
extern int                  RandSeed;
extern char                 _bool_shroud_4DFB04;
extern char                 _Nullstring;
extern char                 gPageUser[56];
extern char                 _DrawOffered1;
extern char                 _DrawOffered2;
extern char                 gTournamentGame;
extern CAI_                 _gAIArray[];
// extern MessageData          _gMessageData; // Replaced by mod
extern char                 ResourcePath[];
extern char                 gRES_PATH[152];
extern char                 MoviesResourcePath[];
extern char                 MusicResourcePath[];
extern char                 MissionsResourcePath[];
extern char                 MapsResourcePath[];
extern char                 _FontBinData[256];
extern FontHeader           _FontData[8];
extern int *                _FontPals[16];
extern HighScoreStruct      gHighScores[6];
extern char                 _GreenColor8;
extern short                _GreyColor16;
extern char                 _RedColor8;
extern char                 _YellowColor8;
extern SidebarButtonData    _SidebarButtons[15];
extern TacticalStruct       _TacticalData;
extern short                _radarcolor16_impassable;
extern short                _YellowColor16;
extern short                _RedColor16;
extern char                 _UnitGroupKeyState[10];
extern char                 _GreyColor8;
extern short                _radarcolor16_driveon;
extern short                _GreenColor16;
extern char                 _BloxFileName[200];
extern int                  _TileTooltips[800];
extern short                _radarcolor16_sand;
extern unsigned int         gGameTicks;
extern unsigned int         _NeutralUnitText;
extern char                 _radarcolor8_drive_on;
extern char                 _bool_missionfailed_517400;
extern int                  _UnitGroupTextIds[60];
extern short                _colormask3;
extern int                  _tooltipcolor_yellow;
extern char                 _cheatstates[8];
extern TooltipStruct        _Tooltipdata;
extern TImage *             _TooltipBackupImage;
extern int                  _tooltipcolor_gray;
extern unsigned int         _NeutralStructureText;
extern unsigned int         _UnrevealedText;
extern int                  _BuildingGroupTextIds[100];
extern unsigned int         _EnemyUnitText;
extern char                 _radarcolor8_spice;
extern unsigned int         _EnemyStructureText;
extern char                 _radarcolor8_sand;
extern char                 _radarcolor8_impassable;
extern short                _radarcolor16_spice;
extern int                  _StoredMouseCursorIndex;
extern char                 _blitflag;

extern TImage               *gBackBuf;
extern short                _ScreenShakes;
extern CrateStruct          gCrates[30];
extern GameMapStruct        gGameMap;
extern int                  gGameMapWidth;
extern int                  gGameMapHeight;
extern int                  _CellNumbersWidthSpan[128];
extern char *               _ptr_circle_1x1grid;
extern int                  _mapvisstate_548010;
extern char                 _NumCratesAllowed;
extern char                 _GameOver;
extern int                  _gEventCount; // Extended from char to int (mod)
extern char                 _somebool_6B7050;
// extern EventData            _gEventArray[64]; // Replaced by mod
extern char                 _persideflags_6B8258[8];
extern int                  _gConditionCount; // Extended from char to int (mod)
extern bool                 _gUnitsExist[8];
extern int                  _gTimerValue;
extern bool                 gWin;
extern bool                 gLose;
// extern ConditionData        _gConditionArray[48]; // Replaced by mod
extern bool                 _gBuildingsExist[8];
extern MiscData             _gMiscData;
extern VariableStruct       _gVariables;
extern TechPosEntry         _TechPosdata[10][10];
extern OrderStruct          _OrderData[8];


extern int                  _templates_AnimationArtFrames[64];
extern GroupIDsStruct       _templates_GroupIDs;
extern TImage *             _SideBarPowerImages[4];
extern TImage *             _RadarMap1;
extern POINT                _SpawnLocations[8];
extern int                  _tiledata[1000];
extern TImage *             _RadarHouseImages[3];
extern char                 _templates_UnitGroupCount;
extern char                 _templates_UnitGroupNameList[60][50];
extern char                 _templates_BulletNameList[64][50];

extern int                  _ViewportHeight;
extern BuildingAtrbStruct   _templates_buildattribs[100];
extern int                  _templates_Explosiondata_AnimationArtFlags[64];
extern char                 _templates_BuildingGroupCount;
extern TImage *             _image_placement_marker_buildable_concrete;
extern TImage *             dataimage[480];
extern TImage *             gBuildingElements[120][32];
extern int                  _sinValues[16384];
extern char                 _MapName[200];
extern char                 _templates_BuildingNameList[100][450];
extern int                  _cosValues[16384];
extern char                 _templates_BuildupArtFrames[100];
extern SmokeDataStruct      _SmokeDataStructs[100];
extern UnitAtribStruct      _templates_unitattribs[60];
extern TImage *             _RadarMap2;
extern ExploisonAtrbStruct  _templates_explosionattribs[64];
extern BullAtrbStruct       _templates_bulletattribs[64];
extern TImage *             _BlankRadarImage;
extern char                 _FreeSpawnLocations[8];
extern uint16_t             _ColoursBinData[128];
extern int                  _ViewportWidth;
extern char                 _ArmourNames[12][50];
extern TImage *             _image_placement_marker_buildable;
extern char                 _SpawnLocationCount;
extern char                 _radarcolor8_sidecolor[8];
extern unsigned int         _TileBitflags[800];
extern char                 _templates_BuildingGroupNameList[100][50];
extern char                 _templates_UnitNameList[60][450];
extern char                 _WarheadNames[30][50];
extern WarheadStruct        _WarheadData[30];
extern char                 _templates_ExplosionNameList[64][50];
extern float                _speed_values[8][4];
extern int                  _templates_ProjectileArtDirections[64];
extern TImage *             _image_placement_marker_nonbuildable;
extern char                 _templates_BuildingAnimationFrames[100];
extern uint16_t             _radarcolor16_sidecolor[8];
extern unsigned char        gUnitTypeNum;
extern unsigned char        gBuildingTypeNum;
extern unsigned char        gBulletTypeNum;
extern unsigned char        gExplosionTypeNum;
extern int                  _CreditsTextYPos;
extern int                  SoundClassObject;
extern ISampleManager *     _gSampleMgr;
extern TextTableStruct **   gTextTable;
extern SampleTableStruct ** gSampleTable;
extern int                  _sampletablecount;
extern char                 _Mouse_bool_797590;
extern CUIManager *         gUIMgr;
extern char                 gSendingToPlayer[104];

extern _DWORD               dword_797E18;
extern bool                 gRestartGame;
extern eGameType            gGameType;

extern char                 _KeyboardKeyState[256];
extern int                  _colormask1;
extern POINT                _MouseClickCoords;

extern char                 _KeyboardKeyDown[256];
extern POINT                _mouse_dword_798498;
extern unsigned char        gTotalPlayers;
extern char                 _IsMultiplayer;
extern char                 _DebugOn_Pathfind_WinLose_DebugNewGame;
extern bool                 BitsPerPixelChanged;

extern unsigned char *      _PalettePtr;

extern int                  _MouseLeftState;
extern int                  _MouseLeftDown;
extern int                  _MouseLeftUp;
extern int                  _MouseRightState;
extern int                  _MouseRightDown;
extern int                  _MouseRightUp;
extern int                  _ViewportXPos;
extern int                  _ViewportYPos;
extern int                  _MouseCursorID;
extern unsigned char        MySideID;
extern unsigned char        gSideId;
extern char                 _SidesToProcess;
extern char                 _NetPlayerNamesArray[8][60];
extern char                 _gDiplomacy[8][8];
extern CSide                gSideArray[8];
extern unsigned char        _IRValues[8];
extern int                  OSMajorVersion;
extern int                  OSMinorVersion;

// ### Functions ###

// drawing
void            Load_Image(char *fileName, int unk);
void            Image__BlitClipTImage(int *unk1, int x, int y, int *image, int unk2, int unk3, int unk4);

// Others
bool __stdcall  IsLocalIp(char *Ip);

bool            IsCurrentlyShown(char *menu);
void            WOL__StartGuestINetGame();
void            WOL__StartHostINetGame();
void            SetgAllowPageUser(unsigned char a1);
BOOL            IsOnlineGame();
void            WOL__OpenWebsite(char *URL);
// AI
eSideType __thiscall CAI__RecalculateSideToAttack(CAI_ *this);
void __thiscall CAI__PlaceBuiltBuilding(CAI_ *this);
char __thiscall CAI__FindGroupForDelivery(CAI_ *this, char a2);
void            AIDebugPrint(char *str);
// Others
void            QueueMessage(const char *message, int type);
void            FreeMessageSlot();
void            GetOwnershipStatusOfCell(int x, int y, char side, _BYTE *flags);
void            DebugFatal(char *caption, char *format, ...);
void            AbortGame();
FILE *          _OpenFile(char * filename, char *mode, char *path);
void            CloseFile(FILE *Stream);
size_t          _ReadFile(void *buffer, size_t size, size_t count, FILE *file);
size_t          _WriteFile(void *buffer, size_t size, size_t count, FILE *file);

// Graphlib
void            BlitHorizontalLineRGB(TImage *img, int x, int y, int length, int color);
void            BlitBeveledRectRGB(TImage *img, RECT *rect, int fill, int hightlight, int shadow);
void            BlitFontChars(TImage *img, char *string, int x, int y, unsigned char font, int color1, int color2);
void            Graphlib__DrawRightAlignedText(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void            Graphlib__DrawTextWithBlackShadow(TImage *image, char *text, int x, int y, int unk, int color_unk);
int             GetStringPixelWidth(const char *string, unsigned char font);
char            GetFontHeight(unsigned char font);

void *          GetFontPaletteHandle(unsigned char a1);
void            Graphlib__LoadFontFile();
uint16_t        GetColor16bit(int colormask, int color);
void            BlitTImage(TImage *lpTITo, int toX, int toY, TImage *lpTIFrom, RECT *rFrom, bool trans, int a7);
// Image
void            BlitClipTImage1(TImage *lpTITo, int toX, int toY, TImage *lpTIFrom, RECT *rect, bool trans, int a7);
void            BlitClipTImage2(TImage *lpTITo, RECT *rect, int toX, int toY, TImage *lpTIFrom, bool trans, int a7);
void            ClearTImage(TImage *a1, int color, int unusable);
void            DrawRectClipTImage(TImage *img, RECT *clip, int x1, int y1, int x2, int y2, short color);
char            GetColor8bit(unsigned char red, unsigned char green, unsigned char blue, unsigned char *palette, char bool1, char bool2, char bool3);
void            BlitFontChar_0(TImage *dest, int x, int y, TImage *src, _WORD *pal);
// Other
char            IsBuildingWithBehaviorBuilt(unsigned char a1, BuildingBehaviorType a2);
int             GetDifficultyCostPercentage(eSideType side_id);
unsigned int    GetUnitBuildSpeedPercentage(unsigned char unit_type, unsigned char side_id);
unsigned int    GetBuildingBuildSpeedPercentage(unsigned char side_id);
unsigned int    GetUnitCost(int type, eSideType side);
unsigned int    GetBuildingCost(int building_type, int num_upgrades, eSideType side_id);
char            HandleSidebarButton(int idx, bool pressed_down);
bool            CanUnitBeBuilt(unsigned char side_id, unsigned char unitType, char bool1);
bool            CanSideUpgradeBuildingGroup(eSideType side_id, eBuildingGroupType building_group);
char            CheckIfMCVCanBeDeployedOn(int xpos, int ypos);
char            MoreProductionBuildingsOfSameGroupExist(int buildingtype);
char            IsAnyStarportUnitPicked(unsigned char side_id);
int             GetRandomValue(char *, int);
void            SetUnitGroup(char group_id);
void            SetBuildingState17andFlags20000(Building *a1);
void            SetBuildingAsPrimary(eSideType side_id, int building_index);
void            SetMouseCursor(int cursor);
// Map
int             RevealMap();
void            Map__PlayerDefeated(uint8_t sideId);

void            LoadMapData(const CHAR *ArgList, char a2);
char            UpdateShroudInRegion(RECT *rect, unsigned char width, unsigned char height);
bool            FindSandwormsTarget(unsigned char blockfromx, unsigned char blockfromy, _BYTE *targetx, _BYTE *targety);
bool            ValidateSandwormsTarget(unsigned char a1, unsigned char a2, unsigned char *a3, unsigned char *a4);
char            GetSpiceTileToHarvest(Unit *unit, eSideType side, _BYTE *x, _BYTE *y);
bool            spice_44D970(unsigned char *x, unsigned char *y);
unsigned int    FindFreeSpotForInfantry(TileFlags tile_flags);
void            GetBuildingOnConcreteCount(char side_id, unsigned char building_type, unsigned char x, unsigned char y, unsigned int *buildTileCount1, unsigned int *concreteTileCount1);
void            RemoveBuildingStuff(int building_type, int x, int y, eSideType side_id);
void            RemoveUnitTileData(Unit *a1);
void            RevealCircle(int x, int y, int size);
char            GetFreeCrateIndex();
void            PlaceCrate(int x, int y, int timing, eCrateType type, eCrateImage image, int respawn_count);
eCrateType      GetCrateFromMap(int x, int y);
void            UpdateSpiceInRegion(RECT *a1);
void            SpiceMound(unsigned char xpos, unsigned char ypos, int range);
void            RecycleCrate(unsigned char index);
int             GetMapVisState();
// Memory
void *          Memory__HeapAllocWrapper(size_t size, char *debugString);
// Mission

void            Mission__CheckEvents();
void            Mission__LoadVarsFile();

// Model
bool            EvaluateIfBuildingsOrUnitsExistForSide(eSideType side_id, char buildings_or_units);


index           ModelAddUnit(unsigned char side, unsigned char type, unsigned char add_at_x, unsigned char add_at_y, unsigned char move_to_x, unsigned char move_to_y, int pixel_offset_x, int pixel_offset_y);

void            ModelAddConcrete(eSideType side_id, char building_type, unsigned char xpos, int ypos, int a5, int tilebitmask);
signed short    ModelAddBuilding(eSideType side_id, char building_type, unsigned char x, unsigned char y, int initialsetup, bool captured, bool captured2);
signed short    ModelAddBullet(unsigned char side_id, unsigned char bulletype, int a3, short firer, unsigned short source_x, unsigned short source_y, unsigned short target_x, unsigned short target_y, short homing_index, char homing_side);
void            AddCursorPuffAnimationToQueue(int x, int y);
signed short    ModelAddExplosion(unsigned char side_id, unsigned char explosionType, unsigned short x, unsigned short y, int z, int extraFlags, char a7, int a8, int a9);

void            GenerateUnitMoveOrder(char side_id, unsigned char x, unsigned char y);
void            GenerateDockWithRefineryOrder(char side_id, short refinery_index);
void            GenerateRepairSelectedUnitsOrder(char side_id, short repair_pad_index);
void            GenerateRepairSingleUnitOrder(char side_id, short unit_index);
void            GenerateUnitAttackUnitOrder(eSideType side_id, eSideType target_side_id, short target_unit_index);
void            GenerateUnitAttackBuildingOrder(char side_id, char target_side_id, short target_building_index);
void            GenerateUnitAttackTileOrder(char side_id, short x, short y);
void            GenerateUnitGuardOrder(char side_id);
void            GenerateUnitScatterOrder(char side_id);
void            GenerateUnitRetreatOrder(char side_id, unsigned char x, unsigned char y);
void            GenerateAllyOrder(unsigned char side_id);
void            GenerateStopOrder(char side_id);
void            GenerateBuildingAttackUnitOrder(eSideType attacker_side_id, eSideType target_side_id, short target_unit_index);
void            GenerateBuildingAttackBuildingOrder(char side_id, char target_side_id, short target_building_index);
void            GenerateBuildingSetPrimaryOrder(char side_id);
void            GenerateBuildBuildingPickOrder(char side_id, char building_type);
void            GenerateBuildBuildingCancelOrder(char side_id, char building_type);
void            GenerateBuildPlaceBuildingOrder(char side_id, char building_type, char x, char y);
void            GenerateBuildUnitPickOrder(char side_id, char unit_type);
void            GenerateBuildUnitCancelOrder(char side_id, char unit_type);
void            GenerateBuildingRepairOrder(char side_id, short building_index);
void            GenerateBuildingSellOrder(char side_id, short building_index);
void            GenerateStarportPickOrder(eSideType side_id, char unit_type);
void            GenerateStarportUnpickOrder(char side_id, char unit_type);
void            GenerateStarportPurchaseOrder(char side_id);
void            GenerateStarportCancelOrder(char side_id);
void            GenerateUpgradePickOrder(char side_id, eBuildingGroupType building_group);
void            GenerateUpgradeCancelOrder(char side_id, char building_group);
void            GenerateUnitDeployOrder(char side_id, short unit_index);
void            GenerateSpecialWeaponOrder(char side_id, char unit_behavior, char x, char y);
void            Model__ResetVars();
void            Orderdata_add(Orderdata *a1);
void            UpdateParticles(unsigned short x, unsigned short y);
bool            CanUnitUseSquare(dwXYStruct x, Unit *unit, eSideType side_id, char route_mode);
dwXYStruct      GetNextSquareInDirection(int x, int y, char facing);
char            GetFreeAdjacentTile(int x, int y, Unit *unit, int side_id, dwXYStruct *result_point, char ArgList);
char            GetFacing(int x1, int y1, int x2, int y2);
char            GetFacing_and_252(int x1, int y1, int x2, int y2);
// Setup
void            Setup__LoadUIBBFile();

void            SetPixelOnRadar8(unsigned char x, unsigned char y, char color);
void            SetPixelOnRadar16(unsigned char x, unsigned char y, short color);
void            SetupRadarMap();

// CSide
void __thiscall CSide__Init(CSide *this, unsigned char side_id, char house_id);
void __thiscall CSide__InitCash(CSide *this);
void *__thiscall CSide__GetNextFreeObject(CSide *this);
void __thiscall CSide__SetupObject(CSide *this, Unit *a1, eObjectType a2);
void __thiscall CSide__RemoveObject(CSide *this, Unit *obj);
void __thiscall CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(CSide *side);
char __thiscall CSide__MyVersionOfBuilding(CSide *this, char building_type, bool bool1);
uint8_t __thiscall CSide__MyVersionOfUnit(CSide *this, char unit, bool bool1);
int __thiscall CSide__ResetBuildingAndUnitIcons(CSide *this);
int __thiscall CSide__AddBuildingIcon(CSide *this, unsigned char icon);
int __thiscall CSide__AddUnitIcon(CSide *this, unsigned char a2);
char __thiscall CSide__SpendMoneyIfSufficient(CSide *this, int cost, char *source);
char __thiscall CSide__AddSpice(CSide *this, int spice);
void __thiscall CSide__AddCash(CSide *this, int a2);
void __thiscall CSide__UpdateCashDrip(CSide *this);
void __thiscall CSide__CalculatePower(CSide *this);
void __thiscall CSide__DeselectAllUnits(CSide *this);
void __thiscall CSide__DeselectAllBuildings(CSide *this);
char __thiscall CSide__SelectUnitsByGroupId(CSide *this, char group_id, char deselect_others);
void __thiscall CSide__CenterViewportOnFirstUnitFromGroupId(CSide *this, char group_id, int *viewport_x_ptr, int *viewport_y_ptr);
int8_t __thiscall CSide__GetQueuePos(CSide *this, Unit *unit);
bool __thiscall CSide__AddToQueue(CSide *this, Unit *unit, short unit_index, unsigned char queue_pos, char a5, int state);
int8_t __thiscall CSide__46CE00(CSide *this, int a2);
int8_t __thiscall CSide__46CE70(CSide *this, int a1, unsigned char a3);
int *__thiscall CSide__46CF10_HKEY_BattleFieldPos(CSide *this, int *x, int *y, char a4);
void __thiscall CSide__ProcessPickupQueue(CSide *this);
Unit *          ChangeUnitOwner(eSideType source_side_id, eSideType target_side_id, short source_unit_index, char bool1);
char            CaptureBuilding(eSideType source_side_id, eSideType target_side_id, short source_building_index);
void __thiscall CSide__FindOtherPrimaryBuilding(CSide *this, short building_type, short removed_building_index);
void __thiscall CSide__UpdateStarportStockAndCosts(CSide *this);
void __thiscall CSide__ProcessDeliveries(CSide *a1);
char __thiscall CSide__FindBestBasePosition(CSide *this, _BYTE *x, _BYTE *y);
void __thiscall CSide__BlowupAll_surrender(CSide *side);
int16_t         FindFirstBuildingByGroup(eBuildingGroupType building_group, unsigned char side_id, short skip_building_index);
void __thiscall CSide__InitStarportAndUpgradeIcons(CSide *this);
void __thiscall CSide__LetAITakeOver(CSide *this);
void            GenerateSurrenderOrder(bool unknown);
char __thiscall CSide__ReturnMoneyFromStarportOrder(CSide *this);
void __thiscall CSide__ResetStarportOrderCost(CSide *this);
void __thiscall CSide__ResetEnemyForSide(CSide *this, char a2);
// Sound
void __thiscall ISampleManager__EndSample(ISampleManager *this, int handle_id);
char            IsSoundPlaying(int sound_id);
void            PlaySoundAt(int id, unsigned char xpos, unsigned char ypos);
void            QueueAudioToPlay(int id, char state, int time, int priority);
void            PlayMentatSound(int at_table, int or_table, int hark_table, char state, int time, int priority);
void __thiscall Sound__LoadMusicFile(int this, char *fileName);
void __thiscall Sound__SetMusicVolume(int soundClassObject, int volume);
void            Sound__PlayMusic(char *fileName);
// CUIManager
void __thiscall CUIManager__ReplaceWithOne_470E60(CUIManager *this, char *name, TImage *image);
void            CUIManager__JumpToMenu(char *menu);
unsigned int __thiscall CUIManager__invoke_473840(CUIManager *this, int a2, const char *a3, const char *a4);
void __thiscall CUIManager__LoadMission(CUIManager * cUIManagerObject, char *map);
void            CUIManager__LoadDune2000Cfg();
void            CUIManager__SaveDune2000Cfg();
void __thiscall CUIManager__StartGame(CUIManager * cUIManagerObject);

void            CUIManager__CreateWigNet();
void            CUIManager__GetCD(char *arg);
// Data
int             GetTextID(char *String1);
char *          GetTextString(int stringId, bool showError);
int             GetSoundTableID(const char *key);
// Other
void            GetNextSquare(Unit *unit, eSideType side, char flags);
bool            TurnUnitInDirection(Unit *unit, unsigned char target_facing);
void            TurnUnitBarrelInDirection(Unit *unit, char facing);
void            TurnBuildingTurret(Building *building, unsigned char taget_facing);
void            SetUnitAnimationType(Unit *unit, signed int anim_type);
void            AnimateInfantryUnit(Unit *unit);
void            Unit_movement_494210(Unit *unit);
char            MoveUnit(Unit *unit, eSideType side, short index);
char            HandleDeathHand(Unit *unit, eSideType side_id, short myIndex);
char            Unit_movement_bullet_494FD0(Unit *unit, unsigned char side_id, unsigned short myIndex, bool speed_bool);
bool            UnitSetSpeed(Unit *a1, int a2);
char            TurnUnit(unsigned char face_what, Unit *unit);
char            MoveUnitSimple(Unit *a3, int a4, char arg8, char argC);
void            UnitShootTarget(Unit *unit, eSideType side, unsigned short index, int x, int y, eSideType enemySideId, short enemyIndex, char a8);
void            BuildingShootTarget(Building *building, eSideType side, unsigned short index, int x, int y, eSideType e_side, unsigned short e_index, int arg1C);
char            UnitAttack(Unit *unit, eSideType side, unsigned short index, eSideType *e_side, unsigned short *e_index);
char            BuildingAttack(Building *bld, eSideType side_id, int a3, unsigned char *a4, _WORD *a5);
char            UnitAttackUnit(Unit *unit, eSideType side, short target);
char            UnitAttackBuilding(Unit *unit, eSideType side, short index);
char            UnitAttackTile(Unit *unit, eSideType side, short index);
char            BuildingAttackTile(Building *building, eSideType side_id, unsigned short building_index);
bool            CheckDistance(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short distance);
char            CanBuildingAttackTile(Building *building, char x, char y, char a4);
bool            CanUnitAttackTile(Unit *unit, int a2, int a3, int a4);
int             GetBuildingInRange(int a1, int a2, Building *building, char *a4, char *a5);
char            IsBuildingInRange(Building *a5, Building *a3, int *a4, int *argC);
char            IsBuildingInRange_0(Building *a1, Building *a2);
bool            CanUnitAttackBuilding(Unit *unit, Building *building, int *tile_x, int *tile_y);
char            BuildingAttackUnit(Building *building, unsigned char side_id, unsigned short building_index);
char            BuildingAttackBuilding(Building *building, unsigned char side_id, unsigned short building_index);
void            ReleaseInfantryFromBuilding(Building *building, eSideType side_id);
bool            GetRandomAdjacentTile(unsigned char *x_ptr, unsigned char *y_ptr);
bool            KickUnitsOutOfTile(Unit *unit, eSideType a2, int x, int y);
char            UpdateUnit(Unit *arg0, eSideType side_id, short myIndex);
bool            UpdateBuilding(Building *bld, int side_id, short building_index);
char            UpdateBullet(Bullet *bul, eSideType side_id);
void            DestroyBuilding(int side, int objIndex, char a3);
void            DestroyUnit(eSideType side, short index);
void            ClosestBuildingTile(Building *building, int xpos, int ypos, int *result_x, int *result_y);
char            UnitTileOccupiedByBuilding(Unit *unit);
char            DamageTiles(unsigned int xpos, unsigned int ypos, unsigned int zpos, unsigned char bulletType, unsigned char firer_side_id, short firer_index, char deviator);
bool            UpdateExplosion(Explosion *arg0, eSideType a1);
bool            UnitAdjustState(Unit *unit, eUnitState state);
bool            SetBuildingState(Building *bld, eBuildingState state);
bool            Unit_49E140(Unit *unit);
int             facing_49E160(int x, int y);
char            tile_driveon_49E290(eSideType side, unsigned char *x, unsigned char *y);
char            UnitDeliver(Unit *unit, int side_id);
char            GetFlyingUnitSpawnPositionAndFacing(_WORD *x, _WORD *y, char offset);
void            LaunchOrnithopters(unsigned char side_id, int xoff, int yoff, char target_x, char target_y);
void            LaunchDeathHand(eSideType side, char target_x, char target_y);
char            NewBestBullet(char primary_weapon, char type, unsigned int check, eSideType side, unsigned short enemyIndex, char armormode);
bool            PickupCrate(Unit *unit, unsigned char side_id);
void            MakeUnitsStealthInRange(unsigned char x, unsigned char y, eSideType side);
void            PlayUnitResponse(char a1);
void            SetUnitToFlicker(Unit *unit);
void            SetBuildingToFlicker(Building *bld);
char            UnmarkTileWithFlyingUnit(unsigned char x, unsigned char y);
char            MarkTileWithFlyingUnit(unsigned char x, unsigned char y);
int             GetSpeed(Unit *a1);
bool            CanUnitAcceptOrders(Unit *unit, int allow_awaiting_pickup_state);
int             RevealTilesSeenByBuildingsAndUnits(eSideType side);
char            UnitCloakStart(Unit *unit);
char            UnitUncloakStart(Unit *unit);
bool            UnitIsCloaked(Unit *unit);
void            UnitHandleCloaking(Unit *unit, eSideType side);
bool            ObjectsAbleToRevealStealthUnitsAreNearby(unsigned char x, unsigned char y, eSideType side, unsigned char radius);
void            NeutralBecomeHostile(eSideType side1, eSideType side2);
void            CrushUnit(Unit *unit, int side_id, int myIndex, int crusher_side, short crusher_index);
char            TryCrushUnit(Unit *unit, int side_id, int myIndex);
char            CanDeviatedUnitRevert(Unit *unit);


CSide *         GetSide(int sideId);
Unit *          GetUnit(eSideType side, index objIndex);
Building *      GetBuilding(eSideType side, index objIndex);
char            map_4A47F0(unsigned int x, unsigned int y, Building *bld, int *tile_x_ptr, int *tile_y_ptr);
char            EnemyUnitInRange(int x, int y, eSideType side_id, int a4, int *targ_x_ptr, int *targ_y_ptr, _BYTE *targ_side_id_ptr, short *targ_index_ptr, char prim_bullet, signed char sec_bullet, char a11, char a12, char a13);
char            EnemyBuildingInRange(int x, int y, eSideType side_id, _DWORD *targ_x_ptr, _DWORD *targ_y_ptr, char *targ_side_id_ptr, short *targ_index_ptr, char bullet1, char bullet2, bool isai);
bool            UnitOccupiesTile(Unit *unit, char x, char y);
Unit *          GetUnitOnTile(unsigned int x, unsigned int y, eSideType *side_id, _WORD *index, bool exclude_cloaked);
Unit *          GetNextUnitOnTile(unsigned int x, unsigned int y, unsigned int side, _WORD *unit_index);
bool            BuildingOccupiesTile(Building *building, unsigned char x, unsigned char y);
bool            GetBuildingOnTile_0(int x, int y, Building **building_ptr, eSideType *side_id, _WORD *index);
bool            GetBuildingOnTile_1(int x, int y, eSideType *side_id_ptr, int a3);
void            DeselectAllForAllSides();
char            IsAnyUnitSelected();
char            IsAnyUnitWithBehaviorSelected(UnitBehaviorType behavior);
char            IsAnyBuildingSelected();
Building *      GetSelectedBuildingWithBarrel();
char            IsAnyArmedUnitSelected();
char            IsAnyArmedUnitSelectedExceptThis(Unit *a1);
char            IsAnyVehicleSelected();
char            IsAnyInfantrySelected();
char            AllSelectedUnitsHaveBehavior(UnitBehaviorType behavior);
char            IsAnyCrusherSelected();
void            SelectUnit(unsigned char side_id, unsigned short unit_index, bool toggle_selection);
void            SelectBuilding(eSideType side_id, unsigned short objIndex);
bool            SelectAllUnitsInArea(int min_x, int min_y, int max_x, int max_y);
bool            CenterViewportOnSelectedUnits(eSideType side, int *x, int *y);
bool            CenterViewportOnSelectedBuildings(eSideType side, int *x, int *y);
void            SelectNextUnit();
int16_t         GetRefineryIndex(eSideType side_id);
Building *      GetNearestBuildingWithBehavior(unsigned char x, unsigned char y, eSideType side_id, BuildingBehaviorType behavior, _BYTE *exit_x1, _BYTE *exit_y1);
bool            GetNearestFreeTileForUnit(unsigned char *x, unsigned char *y, unsigned char a3);
void            cinit();

