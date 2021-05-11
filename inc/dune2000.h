#include <stdbool.h>
#include <stdint.h>

// This header works with sym.asm which defines the Vanilla symbols
// This header will be split up as it becomes larger

// ### types ###

typedef void *Side;

// ### Structs ###

typedef struct MissionEvent
{
    uint32_t map_pos_x;
    uint32_t map_pos_y;
    uint32_t value;      // Flag value, Message unknown value
    uint8_t num_conditions;
    uint8_t event_type;
    uint8_t num_units;      // Also Reveal Map radius
    uint8_t player;         // Also Set Flag flag number
    uint8_t allegiance_target;
    uint8_t allegiance_type;
    uint8_t deploy_action;
    char condition_index[14];
    char condition_not[14];
    char units[21];
    uint32_t message_index;
}MissionEvent;

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

enum EventTypes
{
    ET_REINFORCE,
    ET_STARPORT_DELIVERY,
    ET_ALLEGIANCE,
    ET_LEAVE,
    ET_BESERK,
    ET_PLAYSOUND,
    ET_SETBUILDRATE,
    ET_SETATTACKBUILDINGRATE,
    ET_SETCASH,
    ET_SETTECH,
    ET_WIN,
    ET_LOSE,
    ET_BLOXFILE,
    ET_ATTRIBFILE,
    ET_REVEAL,
    ET_SHOWTIMER,
    ET_HIDETIMER,
    ET_SHOWMESSAGE,
    ET_ADDUNITS,
    ET_SETFLAG,
    ET_UNUSED,
    ET_PLAYMUSIC
};

enum EventConditions
{
    EC_BUILDINGEXISTS,
    EC_UNITEXISTS,
    EC_INTERVAL,
    EC_TIMER,
    EC_CASUALTIES,
    EC_BASEDESTROYED,
    EC_UNITSDESTROYED,
    EC_REVEALED,
    EC_HARVESTED,
    EC_FLAG
};

enum CrateTypes
{
    CT_CASH,
    CT_EXPLODE,
    CT_REVEAL,
    CT_NOMAP,
    CT_UNIT,
    CT_STEALTH,
    CT_UNSUPPORTED6,
    CT_UNSUPPORTED7,
    CT_SPICEBLOOM,
    CT_SPICEBLOOM2,
    CT_SPICEBLOOM3
};

enum CrateImages
{
    CI_BLUE_CRATE,
    CI_RED_CRATE,
    CI_GREEN_CRATE,
    CI_PURPLE_CRATE,
    CI_INVISIBLE,
    CI_SMALL_BLOOM,
    CI_MEDIUM_BLOOM,
    CI_LARGE_BLOOM
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
extern unsigned char NetPlayerHandicap;
extern unsigned int AtreidesMoney;
extern int MousePositionX;
extern int MousePositionY;
extern unsigned char MySideID;
extern bool Win;
extern bool Lose;
extern short DifficultyLevel;
extern int GameType;
extern int GameState;
extern int OSMajorVersion;
extern int OSMinorVersion;
extern int RandSeed;
extern int CUIManagerObject;
extern unsigned char MissionNumber;
extern int SoundClassObject;
extern char NetworkGame;
extern char MusicResourcePath[];
extern char MapsResourcePath[];
extern char MissionsResourcePath[];
extern char ResourcePath[];
extern char MoviesResourcePath[];
extern unsigned int GameTicks;
extern bool SpawnLocationUsedBoolArray[];
extern int gGameMapWidth;
extern int gGameMapHeight;
extern bool BitsPerPixelChanged;
extern bool gUnitsExist[];
extern bool gBuildingsExist[];

// ### Functions ###
void Graphlib__TextOnScreen(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void Graphlib__DrawRightAlignedText(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void Graphlib__DrawTextWithBlackShadow(int *image, char *text, int x, int y, int unk, int color_unk);
void Graphlib__LoadFontFile();
int Model__GenerateUnitMoveOrder(int,int,int);
int Model__GenerateUnitAttackUnitOrder(int,int,int);
void Model__ResetVars();
void CUIManager__LoadDune2000Cfg();
void CUIManager__SaveDune2000Cfg();
void CUIManager__JumpToMenu(char *menu);
void CUIManager__CreateWigNet();
void CUIManager__GetCD(char *arg);
void __thiscall CUIManager__StartGame(int cUIManagerObject);
void __thiscall CUIManager__LoadMission(int cUIManagerObject, char *map);
void WOL__StartGuestINetGame();
void WOL__StartHostINetGame();
void WOL__OpenWebsite(char *URL);
void *Side__AsPointer(int sideId);
void Side__SurrenderAbort(bool unknown);
void __thiscall Side__BlowupAll(Side side);
void Sound__PlayMusic(char *fileName);
void Sound__PlaySample(int id, int unknown1, int unknown2, int unknown3);
void __thiscall Sound__SetMusicVolume(int soundClassObject, int volume);
void __thiscall Sound__LoadMusicFile(int this, char *fileName);
void cinit();
void Load_Image(char *fileName, int unk);
void Image__BlitClipTImage(int *unk1, int x, int y, int *image, int unk2, int unk3, int unk4);
bool __stdcall IsLocalIp(char *Ip);
bool IsCurrentlyShown(char *menu);
void Mission__LoadVarsFile();
void Mission__CheckEvents();
char *Data__GetTextString(int stringId, bool showError);
void Setup__LoadUIBBFile();
void *Memory__HeapAllocWrapper(size_t size, char *debugString);
//void Model__ExecuteGameEvent(GameEvent event);
void Map__PlayerDefeated(uint8_t sideId);
void Game__ShowErrorAndExit(char *caption, char *format, ...);
void PlaceCrate(int x, int y, int timing, enum CrateTypes type, enum CrateImages image, int respawn_count);
