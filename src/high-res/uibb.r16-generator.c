#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

void GenerateUIBB_r16(char *UIBBR16Name)
{
    char UIBBR16Path[256];
    sprintf(UIBBR16Path, ".\\%sUIBB.R16", ResourcePath);
    FILE *UIBBR16 = fopen(UIBBR16Path, "rb"); 
    if (UIBBR16)
    {
        int p; 
        int k;
        int FileSizeUIBBR16 = 640*400; //original UIBB.R16
        unsigned short * OriginalUIBB;
        OriginalUIBB = (unsigned short*) calloc (FileSizeUIBBR16, sizeof(unsigned short));
        fread(OriginalUIBB, sizeof(unsigned short), FileSizeUIBBR16, UIBBR16);
        fclose(UIBBR16);
        unsigned short * HighResUIBB;
        HighResUIBB = (unsigned short*) calloc (GameWidth*GameHeight, sizeof(unsigned short));
        
        //####################### CORNER ####################### 
        // Copy original into new corner
        p = (GameWidth-640);
        k = 0;
        for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 640; ++j)
            {
                HighResUIBB[p+j] = OriginalUIBB[k+j];
            }
            p = p + GameWidth;
            k = k + 640;
        }	
        // Add black to block dude when using adjusted SideBarWidth
        p = GameWidth*20 + GameWidth-160-480;
        for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 480; ++j)
            {
                HighResUIBB[p+j] = 0x00;
            }
            p = p + GameWidth;
        }
        
        //####################### TOP BAR ####################### 
        //Top bar copy
        unsigned short * TopBar;
        TopBar = (unsigned short*) calloc (20*305, sizeof(unsigned short));
        p = 0;
        k = 71; //jump over options button
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 305; ++j)
            {
                TopBar[p+j] = OriginalUIBB[k+j];
            }			
            p = p + 305;
            k = k + 640;
        }		
        //Top bar pasting
        p = 0;
        k = 0;
        for (int i = 0; i < 20; ++i)
        {
            int tj = 0;
            for (int j = 0; j < GameWidth-640+71; ++j) //+71 to remove 'old' options button
            {
                if (j!=0 && j%305==0) ++tj;
                HighResUIBB[p+j] = TopBar[k+j-tj*305];
            }
            p = p + GameWidth;
            k = k + 305;
        }
        
        //####################### BUILDING BLOCK ####################### 
        //Building block fix I (overwrite old power arrow)
        p = 268*640 + 485;
        k = 268*640 + 485 - 8*640; //copy 6x6 pixels from 8 pixels higher
        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                OriginalUIBB[p+j] = OriginalUIBB[k+j];
            }
            p = p + 640;
            k = k + 640;
        }			
        //Building block copy
        unsigned short * BuildBlock;
        BuildBlock = (unsigned short*) calloc (159*160, sizeof(unsigned short));
        p = 0;
        k = 183*640 + 640 - 160;
        for (int i = 0; i < 159; ++i)
        {
            for (int j = 0; j < 160; ++j)
            {
                BuildBlock[p+j] = OriginalUIBB[k+j];
            }			
            p = p + 160;
            k = k + 640;
        }				
        //Building block fix II (overwrite old build icons)
        p = 21;
        k = 83;
        for (int i = 0; i < 159; ++i)
        {
            for (int j = 0; j < 60; ++j)
            {
                BuildBlock[p+j] = 3074; //'\x02\x0c', purple colour
                BuildBlock[k+j] = 3074;
            }			
            p = p + 160;
            k = k + 160;
        }			
        //Building block pasting
        p = 183*GameWidth + GameWidth - 160;
        k = 0;
        for (int i = 0; i < GameHeight-183; ++i)
        {
            for (int j = 0; j < 160; ++j)
            {
                HighResUIBB[p+j] = BuildBlock[k+j];
            }
            p = p + GameWidth;
            k = k + 160;
            if ( k >= 159*160){ k = 0; }
        }
        
        //####################### BOTTOM PIECE ####################### 
        p = (GameHeight-29)*GameWidth + GameWidth - 160;
        k = (400-29)*640 + 640 - 160;
        for (int i = 0; i < 29; ++i)
        {
            for (int j = 0; j < 160; ++j)
            {
                HighResUIBB[p+j] = OriginalUIBB[k+j];
            }
            p = p + GameWidth;
            k = k + 640;
        }
        
        //Output the Custom High Res UIBB.R16 file
        FILE *UIBBR16 = fopen(UIBBR16Name, "wb"); //(over)write it
        if (UIBBR16)
        {
            fwrite(HighResUIBB, sizeof(unsigned short), GameWidth*GameHeight, UIBBR16);
            fclose(UIBBR16);
        }
        free(OriginalUIBB);
        free(HighResUIBB);
        free(BuildBlock);
        free(TopBar);
    }
}
