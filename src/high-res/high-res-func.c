#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

CALL(0x004486F6, _InitUIBB);

bool HighResPatchEnabled = false;
int HighResAddedWidth;
int HighResAddedHeight;
int HighResUIAlignX;
int HighResUIAlignY;
bool HighResUilCenterExcept = false;
bool HighResControlCenterExcept = false;
bool HighResControlFirstTimeCenter = true;

void InitHighRes()
{
    GameWidth = D2kIniGetInt("Options", "GameWidth", 640);
    GameHeight = D2kIniGetInt("Options", "GameHeight", 400);

    if (GameWidth != 640 || GameHeight != 400)
    {
        HighResPatchEnabled = true;
        ScreenWidth = GameWidth;
        ScreenHeight = GameHeight;
        HighResAddedWidth = GameWidth - 640;
        HighResAddedHeight = GameHeight - 400;
        HighResUIAlignX = HighResAddedWidth > 0 ? (HighResAddedWidth / 2) : 0;
        HighResUIAlignY = HighResAddedHeight > 0 ? (HighResAddedHeight / 2) : 0;
        CreditsUIPosX = 390 + HighResAddedWidth;
        PowerBarUIPosX = 485 + HighResAddedWidth;
        PowerBarUIPosY = 172 + HighResAddedHeight;
        SideBarPanelRightUIPosX = 563 + HighResAddedWidth;
        SideBarPanelLeftUIPosX = 501 + HighResAddedWidth;
        SideBarIconCount = (GameHeight - 212) / SideBarIconHeight;
        MiniMapUIPosX = 497 + HighResAddedWidth;
        if (GameWidth % 32 != 0){
            SideBarWidth = 160 + (GameWidth%32);
        }
        SetVQADisplayModeRes();
    }
    cinit();
}

void SetBattleFieldSize()
{
    int mapWidth = 32 * gGameMapWidth;
    int mapHeight = 32 * gGameMapHeight;
    int maxWidth = GameWidth - SideBarWidth;
    int maxHeight = GameHeight - _OptionsBarHeight;
    
    BattleFieldWidth = mapWidth < maxWidth ? mapWidth : maxWidth;
    BattleFieldHeight = mapHeight < maxHeight ? mapHeight : maxHeight;
}

void InitUIBB()
{
    static bool UIBBLoaded = false;
    if (HighResPatchEnabled && !UIBBLoaded)
    {
        char customUIBB16Path[256];
        sprintf(customUIBB16Path, ".\\%sUIBB_%dx%d.R16", ResourcePath, GameWidth, GameHeight);
        if (!FileExists(customUIBB16Path)) GenerateUIBB_r16(customUIBB16Path);
        if (FileExists(customUIBB16Path)) sprintf(uibb_r16FileName, "UIBB_%dx%d.R16", GameWidth, GameHeight);

        char customUIBB8Path[256];
        sprintf(customUIBB8Path, ".\\%sUIBB_%dx%d.R8", ResourcePath, GameWidth, GameHeight);
        if (!FileExists(customUIBB8Path)) GenerateUIBB_r8(customUIBB8Path);
        if (FileExists(customUIBB8Path)) sprintf(uibb_r8FileName, "UIBB_%dx%d.R8", GameWidth, GameHeight);
        UIBBLoaded = true;
    }
    Setup__LoadUIBBFile();
}

void UilCenterExceptCheck(char *fileName)
{
    HighResUilCenterExcept = !strcmpi(fileName, "GAMEMSG.uid") || !strcmpi(fileName, "GAMEMSG.UIL") ||
                             !strcmpi(fileName, "GAMEPAGE.uid") || !strcmpi(fileName, "GAMEPAGE.UIL");
}

void ControlCenterExceptCheck(char *controlName)
{
    HighResControlFirstTimeCenter = true;
    HighResControlCenterExcept = !strcmpi(controlName, "BUTTON serialModem") || StringStartsWith(controlName, "LISTBOX");
    if (!strcmpi(controlName, "BUTTON serialModem")) HighResControlFirstTimeCenter = false;
}

void DrawEmptySideBarIcons(int *unk, int *image)
{
    int y = SideBarPanelsPosY + (SideBarIconHeight * 4);
    for (int i = SideBarIconCount - 4; i-- > 0;)
    {
        Image__BlitClipTImage(unk, SideBarPanelRightUIPosX, y + (i * SideBarIconHeight), image, 0, 0, 0);
        Image__BlitClipTImage(unk, SideBarPanelLeftUIPosX, y + (i * SideBarIconHeight), image, 0, 0, 0);
    }
}
