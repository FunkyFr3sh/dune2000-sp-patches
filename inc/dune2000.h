#include <stdbool.h>
#include <stdint.h>
#include "patch.h"

// This header works with sym.asm which defines the Vanilla symbols
// This header will be split up as it becomes larger

typedef void TImage;

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
extern unsigned char NetUnitCount;
extern unsigned char NetTechLevel;
extern unsigned short NetStartingCredits;
extern unsigned char NetAIPlayers;
extern bool NetCrates;
extern unsigned char NetWorms;
extern char NetPlayerName[];
extern unsigned char NetPlayerSide;
extern unsigned char NetPlayerColor;
extern unsigned char NetPlayerHandicap;
extern char NetIPAddress[];
extern char NetModemPhone[];
extern unsigned char NetSerialComPort;
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
extern CAI_                 gAIArray[];
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

extern EventData            _gEventArray[64];
extern bool                 gUnitsExist[];
extern int                  gTimerValue;
extern bool                 gWin;
extern bool                 gLose;
extern ConditionData        gConditionArray[];
extern bool                 gBuildingsExist[];
extern MiscData             gMiscData;
extern VariableStruct       _gVariables;


extern GroupIDsStruct       _templates_GroupIDs;
extern void *               _RadarMap1;
extern char                 _templates_UnitTypeCount;

extern BuildingAtrbStruct   _templates_buildattribs[100];
extern UnitAtribStruct      _templates_unitattribs[60];
extern void *               _RadarMap2;
extern ExploisonAtrbStruct  _templates_explosionattribs[64];
extern BullAtrbStruct       _templates_bulletattribs[64];
extern bool                 SpawnLocationUsedBoolArray[];
extern unsigned char        gUnitTypeNum;
extern int                  SoundClassObject;
extern TextTableStruct **   gTextTable;
extern int                  CUIManagerObject;

extern int                  GameType;


extern bool                 BitsPerPixelChanged;


extern unsigned char        MySideID;
extern unsigned char        gSideId;
extern unsigned char        gDiplomacy[8][8];
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

index           ModelAddUnit(unsigned char side, unsigned char type, unsigned char add_at_x, unsigned char add_at_y, unsigned char move_to_x, unsigned char move_to_y, int pixel_offset_x, int pixel_offset_y);

signed __int16  ModelAddExplosion(int side, unsigned __int8 explosionType, unsigned __int16 x, unsigned __int16 y, int a5, int a6, char a7, int a8, int a9);
int             Model__GenerateUnitMoveOrder(int,int,int);
int             Model__GenerateUnitAttackUnitOrder(int,int,int);
void            Model__ResetVars();
void            Model__ExecuteGameEvent(GameEvent event);
// Setup
void            Setup__LoadUIBBFile();

void            SetPixelOnRadar8(unsigned __int8 x, unsigned __int8 y, char color);
void            SetPixelOnRadar16(unsigned __int8 x, unsigned __int8 y, __int16 color);

// CSide
void __thiscall CSide__update_list_of_available_buildings_and_units(CSide *side);
uint8_t __thiscall CSide__MyVersionOfUnit(CSide *this, char unit, bool bool1);
void __thiscall CSide_add_cash_drip(CSide *this, int a2);
void __thiscall CSide__BlowupAll_surrender(CSide *side);
void            CSide__SurrenderAbort(bool unknown);
void __thiscall CSide__reset_enemy(CSide *this, char a2);
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

void            MakeUnitsStealthInRange(unsigned __int8 x, unsigned __int8 y, eSideType side);
int             RevealTilesSeenByBuildingsAndUnits(eSideType side);
bool            Unit_49F5F0(Unit *unit);


CSide *         GetSide(int sideId);
bool            FindNearestFreeTile(unsigned char *x, unsigned char *y, unsigned char a3);
void            cinit();

