#include <windows.h>
#include <stdio.h>
#include "dune2000.h"
#include "patch.h"
#include "macros/patch.h"
#include "ini.h"
#include "utils.h"

CLEAR(0x004A3CA4, 0x90, 0x004A3CAA);
CALL(0x004A3CA4, _InitCursor);
CALL(0x00423AF0, _DrawCursor);

bool UseHardwareCursor;
HCURSOR HardwareCursors[40];

int WINAPI InitCursor(BOOL bShow)
{
    UseHardwareCursor =
        D2kIniGetBool("Options", "UseHardwareCursor", UseHardwareCursor);

    if (UseHardwareCursor)
    {
        char *filePath;

        filePath = ".\\data\\cursor0.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor0Data, Cursor0DataLength);
        HardwareCursors[0] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor1.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor1Data, Cursor1DataLength);
        HardwareCursors[1] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor2.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor2Data, Cursor2DataLength);
        HardwareCursors[2] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor3.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor3Data, Cursor3DataLength);
        HardwareCursors[3] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor4.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor4Data, Cursor4DataLength);
        HardwareCursors[4] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor5.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor5Data, Cursor5DataLength);
        HardwareCursors[5] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor6.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor6Data, Cursor6DataLength);
        HardwareCursors[6] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor7.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor7Data, Cursor7DataLength);
        HardwareCursors[7] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor8.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor8Data, Cursor8DataLength);
        HardwareCursors[8] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor9.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor9Data, Cursor9DataLength);
        HardwareCursors[9] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor10.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor10Data, Cursor10DataLength);
        HardwareCursors[10] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor11.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor11Data, Cursor11DataLength);
        HardwareCursors[11] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor12.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor12Data, Cursor12DataLength);
        HardwareCursors[12] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor13.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor13Data, Cursor13DataLength);
        HardwareCursors[13] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor14.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor14Data, Cursor14DataLength);
        HardwareCursors[14] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor15.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor15Data, Cursor15DataLength);
        HardwareCursors[15] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor16.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor16Data, Cursor16DataLength);
        HardwareCursors[16] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor17.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor17Data, Cursor17DataLength);
        HardwareCursors[17] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor18.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor18Data, Cursor18DataLength);
        HardwareCursors[18] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor19.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor19Data, Cursor19DataLength);
        HardwareCursors[19] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor20.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor20Data, Cursor20DataLength);
        HardwareCursors[20] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor21.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor21Data, Cursor21DataLength);
        HardwareCursors[21] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor22.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor22Data, Cursor22DataLength);
        HardwareCursors[22] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor23.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor23Data, Cursor23DataLength);
        HardwareCursors[23] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor24.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor24Data, Cursor24DataLength);
        HardwareCursors[24] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor25.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor25Data, Cursor25DataLength);
        HardwareCursors[25] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor26.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor26Data, Cursor26DataLength);
        HardwareCursors[26] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor27.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor27Data, Cursor27DataLength);
        HardwareCursors[27] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor28.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor28Data, Cursor28DataLength);
        HardwareCursors[28] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor29.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor29Data, Cursor29DataLength);
        HardwareCursors[29] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor30.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor30Data, Cursor30DataLength);
        HardwareCursors[30] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor31.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor31Data, Cursor31DataLength);
        HardwareCursors[31] = LoadCursorFromFileAImp(filePath);

        filePath = ".\\data\\cursor32.ani";
        if (!FileExists(filePath))
            FileWriteAllBytes(filePath, Cursor32Data, Cursor32DataLength);
        HardwareCursors[32] = LoadCursorFromFileAImp(filePath);

        return 0;
    }

    UseHardwareCursor = false;
    return ShowCursor(bShow);
}

void DrawCursor(int *unk1, int x, int y, int *image, int unk2, int unk3, int unk4)
{
    if (UseHardwareCursor)
    {
        static int lastCursorId;

        if (lastCursorId != CurrentCursorId)
        {
            lastCursorId = CurrentCursorId;
            SetCursorImp(HardwareCursors[CurrentCursorId]);
        }
    }
    else
    {
        Image__BlitClipTImage(unk1, x, y, image, unk2, unk3, unk4);
    }
}