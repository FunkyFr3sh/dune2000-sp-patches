#include <stdbool.h>
#include <stdint.h>
#include "patch.h"

// This header works with sym.asm which defines the Vanilla symbols
// This header will be split up as it becomes larger

typedef void TImage;
typedef char eBuildingGroupType;
typedef int32_t _DWORD;
typedef int16_t _WORD;
typedef uint8_t _BYTE;

#include "dune2000/side.h"
#include "dune2000/ai.h"
#include "dune2000/templates.h"
#include "dune2000/map.h"
#include "dune2000/crates.h"
#include "dune2000/mission.h"
#include "dune2000/text.h"
#include "dune2000/vars.h"

typedef struct GameEvent // 168 byte
{
    int unknown1;
    int unknown2;
    int ticks;
    
    //cellX 30
    //ticks 8
    //cellY 28
    //Order 35
    //unknown1 24
    //type 38
    //sideId 36
}GameEvent;

typedef struct TechPosEntry
{
  char UnitType_Atreides;
  char UnitType_Harkonnen;
  char UnitType_Ordos;
  char PosX;
  char PosY;
} TechPosEntry;

// ### Constants ###

enum Sides
{
    SD_ATREIDES,
    SD_HARKONNEN,
    SD_ORDOS,
    SD_EMPEROR,
    SD_FREMEN,
    SD_SMUGGLERS,
    SD_MERCENARIES,
    SD_SANDWORM
};

typedef enum eSideType
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
} eSideType;

enum Colors
{
    CL_BLUE,
    CL_RED,
    CL_TEAL,
    CL_PURPLE,
    CL_GRAY,
    CL_BROWN,
    CL_GOLD,
    CL_LIGHTBROWN
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

enum GameTypes
{
    GT_SINGLEPLAYER,
    GT_SKIRMISH,
    GT_LAN,
    GT_SERIAL,
    GT_MODEM,
    GT_WOL
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

// Side (HouseClass)
#define HC_SIDEID 0x24252
#define HC_CREDITS 0x2425C
#define HC_SILO_CREDITS 0x24254
#define HC_SPICE_HARVESTED 0x2477C
#define HC_BUILDINGS_DESTROYED 0x24C94
#define HC_UNITS_KILLED 0x24C90
#define HC_BUILDINGS_OWNED 0x24784
//not sure about this one
#define HC_BUILDING_PRODUCTION_AVAILABLE 0x2652D
//0x5A00 = 100%
#define HC_BUILDING_PRODUCTION_PROGRESS 0x2651E
#define HC_HARKONNEN_PALACE_EXISTS 0x260AA
#define HC_CURRENTLY_PRODUCED_BUILDING_ID 0x2651C

// Special Weapons
#define SW_SABOTEUR 4
#define SW_AIRSTRIKE 9
#define SW_DEATH_HAND_ROCKET 10
#define SW_FREMEN 12

// ### Variables ###

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
extern int NetPlayerCount;
extern unsigned int NetPlayersStartingCredits[];
extern char NetPlayersTechLevel[];
extern char NetPlayersColor[]; //Actually there is more than just the colors, not sure what... struct size 60byte, first byte = color
extern char MissionMap[];
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
extern int GameEndTickCount;

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
extern int OptionsBarHeight;
extern int CurrentCursorId;

//Others

extern unsigned char MissionNumber;



extern int                  GameState;

extern short                DifficultyLevel;
extern int                  gBitsPerPixel;
extern int                  MousePositionX;
extern int                  MousePositionY;
extern int                  RandSeed;
extern CAI_                 _gAIArray[];
extern MessageData          _gMessageData;
extern char                 ResourcePath[];
extern char                 MoviesResourcePath[];
extern char                 MusicResourcePath[];
extern char                 MissionsResourcePath[];
extern char                 MapsResourcePath[];
extern unsigned int         gGameTicks;
extern char                 _cheatstates[8];
extern char                 _radarcolor_byte_517780_spicecolor;
extern short                _radarcolor_word_517898_spicecolor;

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
extern int                  gTimerValue;
extern bool                 gWin;
extern bool                 gLose;
// extern ConditionData        _gConditionArray[48]; // Replaced by mod
extern bool                 _gBuildingsExist[8];
extern MiscData             _gMiscData;
extern VariableStruct       _gVariables;
extern TechPosEntry         _TechPosdata[10][10];


extern GroupIDsStruct       _templates_GroupIDs;
extern void *               _RadarMap1;
extern POINT                _SpawnLocations[8];
extern int                  _tiledata[1000];
extern char                 _templates_UnitTypeCount;

extern int                  _ViewportHeight;
extern BuildingAtrbStruct   _templates_buildattribs[100];
extern int                  _sinValues[16384];
extern char                 _MapName[200];
extern int                  _cosValues[16384];
extern UnitAtribStruct      _templates_unitattribs[60];
extern void *               _RadarMap2;
extern ExploisonAtrbStruct  _templates_explosionattribs[64];
extern BullAtrbStruct       _templates_bulletattribs[64];
extern char                 _FreeSpawnLocations[8];
extern int                  _ViewportWidth;
extern char                 _SpawnLocationCount;
extern unsigned int         _TileBitflags[800];
extern unsigned char        gUnitTypeNum;
extern unsigned char        gBuildingTypeNum;
extern unsigned char        gBulletTypeNum;
extern unsigned char        gExplosionTypeNum;
extern int                  SoundClassObject;
extern TextTableStruct **   gTextTable;
extern int                  CUIManagerObject;

extern int                  GameType;
extern eGameType            gGameType;

extern unsigned char        gTotalPlayers;
extern char                 _canQueue_IsMultiplayer;
extern bool                 BitsPerPixelChanged;


extern int                  _ViewportXPos;
extern int                  _ViewportYPos;
extern unsigned char        MySideID;
extern unsigned char        gSideId;
extern char                 _gDiplomacy[8][8];
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
void            WOL__OpenWebsite(char *URL);
void            QueueMessage(const char *message, int type);
void            DebugFatal(char *caption, char *format, ...);

// Graphlib
void            Graphlib__TextOnScreen(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void            Graphlib__DrawRightAlignedText(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void            Graphlib__DrawTextWithBlackShadow(int *image, char *text, int x, int y, int unk, int color_unk);
void            Graphlib__LoadFontFile();
// Image
void            BlitClipTImage1(TImage *lpTITo, int toX, int toY, TImage *lpTIFrom, RECT *rect, bool trans, int a7);
void            ClearTImage(TImage *a1, int color, int unusable);
// Other
unsigned int    w__GetUnitCost(int type, eSideType side);
unsigned int    GetBuildingCost(int building_type, int num_upgrades, eSideType side_id);
int             GetRandomValue(char *, int);
void            SetBuildingAsPrimary(eSideType side_id, int building_index);
// Map
int             RevealMap();
void            Map__PlayerDefeated(uint8_t sideId);

void            RevealCircle(int x, int y, int size);
char            GetFreeCrateIndex();
void            PlaceCrate(int x, int y, int timing, eCrateType type, eCrateImage image, int respawn_count);
eCrateType      GetCrateFromMap(int x, int y);
void            UpdateSpiceInRegion(RECT *a1);
void            SpiceMound(unsigned __int8 xpos, unsigned __int8 ypos, int range);
void            RecycleCrate(unsigned __int8 index);
int             GetMapVisState();
// Memory
void *          Memory__HeapAllocWrapper(size_t size, char *debugString);
// Mission

void            Mission__CheckEvents();
void            Mission__LoadVarsFile();

// Model
bool            EvaluateIfBuildingsOrUnitsExistForSide(eSideType side_id, char buildings_or_units);

index           ModelAddUnit(unsigned char side, unsigned char type, unsigned char add_at_x, unsigned char add_at_y, unsigned char move_to_x, unsigned char move_to_y, int pixel_offset_x, int pixel_offset_y);

void            ModelAddConcrete(eSideType side_id, char building_type, unsigned __int8 xpos, int ypos, int a5, int tilebitmask);
signed __int16  ModelAddBuilding(eSideType side_id, char building_type, unsigned __int8 x, unsigned __int8 y, int initialsetup, bool captured, bool captured2);
signed __int16  ModelAddBullet(unsigned __int8 side_id, unsigned __int8 bulletype, int a3, __int16 firer, unsigned __int16 source_x, unsigned __int16 source_y, unsigned __int16 target_x, unsigned __int16 target_y, __int16 homing_index, char homing_side);
signed __int16  ModelAddExplosion(int side_id, unsigned __int8 explosionType, unsigned __int16 x, unsigned __int16 y, int a5, int a6, char a7, int a8, int a9);
void            GenerateUnitMoveOrder(char side_id, unsigned __int8 x, unsigned __int8 y);
void            GenerateDockWithRefineryOrder(char side_id, __int16 refinery_index);
void            GenerateRepairSelectedUnitsOrder(char side_id, __int16 repair_pad_index);
void            GenerateRepairSingleUnitOrder(char side_id, __int16 unit_index);
void            GenerateUnitAttackUnitOrder(eSideType side_id, eSideType target_side_id, __int16 target_unit_index);
void            GenerateUnitAttackBuildingOrder(char side_id, char target_side_id, __int16 target_building_index);
void            GenerateUnitAttackTileOrder(char side_id, __int16 x, __int16 y);
void            GenerateUnitGuardOrder(char side_id);
void            GenerateUnitScatterOrder(char side_id);
void            GenerateUnitRetreatOrder(char side_id, unsigned __int8 x, unsigned __int8 y);
void            GenerateStopOrder(char side_id);
void            GenerateBuildingAttackUnitOrder(eSideType attacker_side_id, eSideType target_side_id, __int16 target_unit_index);
void            GenerateBuildingAttackBuildingOrder(char side_id, char target_side_id, __int16 target_building_index);
void            GenerateBuildingSetPrimaryOrder(char side_id);
void            GenerateBuildBuildingPickOrder(char side_id, char building_type);
void            GenerateBuildBuildingCancelOrder(char side_id, char building_type);
void            GenerateBuildPlaceBuildingOrder(char side_id, char building_type, char x, char y);
void            GenerateBuildUnitPickOrder(char side_id, char unit_type);
void            GenerateBuildUnitCancelOrder(char side_id, char unit_type);
void            GenerateBuildingRepairOrder(char side_id, __int16 building_index);
void            GenerateBuildingSellOrder(char side_id, __int16 building_index);
void            GenerateStarportPickOrder(eSideType side_id, char unit_type);
void            GenerateStarportUnpickOrder(char side_id, char unit_type);
void            GenerateStarportPurchaseOrder(char side_id);
void            GenerateStarportCancelOrder(char side_id);
void            GenerateUpgradePickOrder(char side_id, eBuildingGroupType building_group);
void            GenerateUpgradeCancelOrder(char side_id, char building_group);
void            GenerateUnitDeployOrder(char side_id, __int16 unit_index);
void            GenerateSpecialWeaponOrder(char side_id, char unit_behavior, char x, char y);
void            Model__ResetVars();
void            Model__ExecuteGameEvent(GameEvent event);
// Setup
void            Setup__LoadUIBBFile();

void            SetPixelOnRadar8(unsigned __int8 x, unsigned __int8 y, char color);
void            SetPixelOnRadar16(unsigned __int8 x, unsigned __int8 y, __int16 color);
void            radarmap();

// CSide
void __thiscall CSide__UpdateBuildingAndUnitIconsAndBaseBoundaries(CSide *side);
char __thiscall CSide__MyVersionOfBuilding(CSide *this, char building_type, bool bool1);
uint8_t __thiscall CSide__MyVersionOfUnit(CSide *this, char unit, bool bool1);
void __thiscall CSide__AddCash(CSide *this, int a2);
int8_t __thiscall CSide_46CCA0_get_queue_pos(CSide *this, Unit *unit);
bool __thiscall CSide__AddToQueue(CSide *this, Unit *unit, __int16 unit_index, unsigned __int8 queue_pos, char a5, int state);
Unit *          ChangeUnitOwner(eSideType source_side_id, eSideType target_side_id, __int16 source_unit_index, char bool1);
char            CaptureBuilding(eSideType source_side_id, eSideType target_side_id, __int16 source_building_index);
char __thiscall CSide__FindBestBasePosition(CSide *this, _BYTE *x, _BYTE *y);
void __thiscall CSide__BlowupAll_surrender(CSide *side);
void            GenerateSurrenderOrder(bool unknown);
void __thiscall CSide__ResetEnemyForSide(CSide *this, char a2);
// Sound
void            PlaySoundAt(int id, unsigned __int8 xpos, unsigned __int8 ypos);
void            Sound__PlaySample(int id, char state, int time, int priority);
void __thiscall Sound__LoadMusicFile(int this, char *fileName);
void __thiscall Sound__SetMusicVolume(int soundClassObject, int volume);
void            Sound__PlayMusic(char *fileName);
// CUIManager
void            CUIManager__JumpToMenu(char *menu);
void __thiscall CUIManager__LoadMission(int cUIManagerObject, char *map);
void            CUIManager__LoadDune2000Cfg();
void            CUIManager__SaveDune2000Cfg();
void __thiscall CUIManager__StartGame(int cUIManagerObject);

void            CUIManager__CreateWigNet();
void            CUIManager__GetCD(char *arg);
// Data

char *          Data__GetTextString(int stringId, bool showError);
int             Data__GetSoundTableID(const char *key);
// Other
bool            GetRandomAdjacentTile(unsigned __int8 *x_ptr, unsigned __int8 *y_ptr);
void            DestroyBuilding(int side, int objIndex, char a3);
void            DestroyUnit(eSideType side, __int16 index);
char            DamageTiles(unsigned int xpos, unsigned int ypos, unsigned int a3, unsigned __int8 bulletType, int ai_side, __int16 ai_index, char a7);

bool            UnitAdjustState(Unit *unit, eUnitState state);
void            MakeUnitsStealthInRange(unsigned __int8 x, unsigned __int8 y, eSideType side);
int             RevealTilesSeenByBuildingsAndUnits(eSideType side);
bool            Unit_49F5F0(Unit *unit);


CSide *         GetSide(int sideId);
Unit *          GetUnit(eSideType side, index objIndex);
Building *      GetBuilding(eSideType side, index objIndex);
Unit *          GetUnitOnTile(unsigned int x, unsigned int y, eSideType *side, _WORD *index, bool bool1);
Unit *          GetNextUnitOnTile(unsigned int x, unsigned int y, unsigned int side, _WORD *unit_index);
bool            GetBuildingOnTile_0(int x, int y, Building **building_ptr, eSideType *side_id, _WORD *index);
bool            FindNearestFreeTile(unsigned char *x, unsigned char *y, unsigned char a3);
void            cinit();

