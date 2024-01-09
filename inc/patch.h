#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

// This header is used for patches
// This header will be split up as it becomes larger

typedef HCURSOR(WINAPI* LOADCURSORFROMFILEAPROC)(LPCSTR);
typedef HCURSOR(WINAPI* SETCURSORPROC)(HCURSOR);

// ### Variables ###

extern bool SpawnerActive;
extern bool StartWithMCV;
extern bool LiveStatsEnabled;
extern char MapScript[];
extern char MapName[];
extern bool UseDefaultWinLoseEvents;
extern uint32_t SpawnerGameEndState;
extern unsigned char gOldSideId;
extern uint32_t MapScrollLockTicks;

extern const char Cursor0Data[];
extern const unsigned int Cursor0DataLength;
extern const char Cursor1Data[];
extern const unsigned int Cursor1DataLength;
extern const char Cursor2Data[];
extern const unsigned int Cursor2DataLength;
extern const char Cursor3Data[];
extern const unsigned int Cursor3DataLength;
extern const char Cursor4Data[];
extern const unsigned int Cursor4DataLength;
extern const char Cursor5Data[];
extern const unsigned int Cursor5DataLength;
extern const char Cursor6Data[];
extern const unsigned int Cursor6DataLength;
extern const char Cursor7Data[];
extern const unsigned int Cursor7DataLength;
extern const char Cursor8Data[];
extern const unsigned int Cursor8DataLength;
extern const char Cursor9Data[];
extern const unsigned int Cursor9DataLength;
extern const char Cursor10Data[];
extern const unsigned int Cursor10DataLength;
extern const char Cursor11Data[];
extern const unsigned int Cursor11DataLength;
extern const char Cursor12Data[];
extern const unsigned int Cursor12DataLength;
extern const char Cursor13Data[];
extern const unsigned int Cursor13DataLength;
extern const char Cursor14Data[];
extern const unsigned int Cursor14DataLength;
extern const char Cursor15Data[];
extern const unsigned int Cursor15DataLength;
extern const char Cursor16Data[];
extern const unsigned int Cursor16DataLength;
extern const char Cursor17Data[];
extern const unsigned int Cursor17DataLength;
extern const char Cursor18Data[];
extern const unsigned int Cursor18DataLength;
extern const char Cursor19Data[];
extern const unsigned int Cursor19DataLength;
extern const char Cursor20Data[];
extern const unsigned int Cursor20DataLength;
extern const char Cursor21Data[];
extern const unsigned int Cursor21DataLength;
extern const char Cursor22Data[];
extern const unsigned int Cursor22DataLength;
extern const char Cursor23Data[];
extern const unsigned int Cursor23DataLength;
extern const char Cursor24Data[];
extern const unsigned int Cursor24DataLength;
extern const char Cursor25Data[];
extern const unsigned int Cursor25DataLength;
extern const char Cursor26Data[];
extern const unsigned int Cursor26DataLength;
extern const char Cursor27Data[];
extern const unsigned int Cursor27DataLength;
extern const char Cursor28Data[];
extern const unsigned int Cursor28DataLength;
extern const char Cursor29Data[];
extern const unsigned int Cursor29DataLength;
extern const char Cursor30Data[];
extern const unsigned int Cursor30DataLength;
extern const char Cursor31Data[];
extern const unsigned int Cursor31DataLength;
extern const char Cursor32Data[];
extern const unsigned int Cursor32DataLength;


extern int HighResAddedWidth;
extern int HighResAddedHeight;
extern int HighResUIAlignX;
extern int HighResUIAlignY;
extern bool HighResPatchEnabled;
extern bool HighResCenterUI;
extern char uibb_r16FileName[];
extern char uibb_r8FileName[];

extern bool JumpToSkirmish;
extern bool JumpToLAN;
extern bool JumpToGame;

extern bool DisableMaxWindowedMode;
extern char GameLanguage[];
extern bool MouseWheelUp;
extern bool MouseWheelDown;
extern bool UseImprovedFPSLimiter;
extern bool CutsceneChangeResolution;
extern bool SingleProcessorAffinity;
extern bool UseHardwareCursor;

extern HCURSOR HardwareCursors[40];

extern LOADCURSORFROMFILEAPROC LoadCursorFromFileImpA;
extern SETCURSORPROC SetCursorImp;

// ### Functions ###

void PlayRandomMusic();
void HandleKeyEvent(int key, bool keyIsDown);
void SaveDune2000Ini();
void GenerateUIBB_r16(char *UIBBR16Name);
void GenerateUIBB_r8(char *UIBBR8Name);
void SetVQADisplayModeRes();
void BlowUpEverything(uint8_t sideId);
void InitMissionScript();
void StatsClear();
void SetSingleProcessorAffinity();
DWORD WINAPI fake_timeGetTime();
DWORD WINAPI fake_GetTickCount();
