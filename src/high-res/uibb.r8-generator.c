#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "utils.h"

void GenerateUIBB_r8(char *UIBBR8Name)
{
    char UIBBR8Path[256];
    sprintf(UIBBR8Path, ".\\%sUIBB.R8", ResourcePath);
    FILE *UIBBR8 = fopen(UIBBR8Path, "rb"); 
    if (UIBBR8)
    {
        int p; 
        int k;
        int FileSizeUIBBR8 = 640*400; //original UIBB.R8
        unsigned char * OriginalUIBB;
        OriginalUIBB = (unsigned char*) calloc (FileSizeUIBBR8, sizeof(unsigned char));
        fread(OriginalUIBB, sizeof(unsigned char), FileSizeUIBBR8, UIBBR8);
        fclose(UIBBR8);
        unsigned char * HighResUIBB;
        HighResUIBB = (unsigned char*) calloc (GameWidth*GameHeight, sizeof(unsigned char));
        
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
        unsigned char * TopBar;
        TopBar = (unsigned char*) calloc (20*305, sizeof(unsigned char));
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
        unsigned char * BuildBlock;
        BuildBlock = (unsigned char*) calloc (159*160, sizeof(unsigned char));
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
                BuildBlock[p+j] = 7; // purple colour
                BuildBlock[k+j] = 7; //
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
        
        //Output the Custom High Res UIBB.R8 file
        FILE *UIBBR8 = fopen(UIBBR8Name, "wb"); //(over)write it
        if (UIBBR8)
        {
            fwrite(HighResUIBB, sizeof(unsigned char), GameWidth*GameHeight, UIBBR8);
            fclose(UIBBR8);
        }
        free(OriginalUIBB);
        free(HighResUIBB);
        free(BuildBlock);
        free(TopBar);
    }
}
