#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

CALL(0x0044A8F3, _InitLanguage);
SETDWORD(0x0042E59F+1, _FontBin);
SETDWORD(0x0042E461+1, _FontBin);
SETDWORD(0x00470860+1, _FontFnt);
SETDWORD(0x00448B48+1, _FontColFnt);
SETDWORD(0x00481E29+1, _TextUib);
SETDWORD(0x0046EB80+1, _GameSFX);
SETDWORD(0x0046ED19+1, _GameSFX);
SETDWORD(0x0046FAFD+1, _GameSFXWav);

char GameLanguage[256];
char TextUib[256] = "text.uib";

static char FontBin[256] = "font";
static char FontFnt[256] = "font";
static char FontColFnt[256] = "FontCol";
static char GameSFX[256] = "GameSFX\\%s";
static char GameSFXWav[256] = "GameSFX\\%s.wav";

void InitLanguage()
{
    D2kIniGetString("Options", "Language", "", GameLanguage, 256);
    
    char textUibPath[256];
    sprintf(textUibPath, ".\\%s%stext%s.uib", ResourcePath, "UI_DATA\\", GameLanguage);
    if (FileExists(textUibPath)) sprintf(TextUib, "text%s.uib", GameLanguage);
    
    char fontBinPath[256];
    sprintf(fontBinPath, ".\\%s%sfont%s.bin", ResourcePath, "bin\\", GameLanguage);
    if (FileExists(fontBinPath)) sprintf(FontBin, "font%s", GameLanguage);
    
    char fontFntPath[256];
    sprintf(fontFntPath, ".\\%sfont%s.fnt", ResourcePath, GameLanguage);
    if (FileExists(fontFntPath)) sprintf(FontFnt, "font%s", GameLanguage);
    
    char fontColFntPath[256];
    sprintf(fontColFntPath, ".\\%sFontCol%s.fnt", ResourcePath, GameLanguage);
    if (FileExists(fontColFntPath)) sprintf(FontColFnt, "FontCol%s", GameLanguage);
    
    char gameSFXPath[256];
    sprintf(gameSFXPath, ".\\%sGAMESFX%s", ResourcePath, GameLanguage);
    if (DirectoryExists(gameSFXPath))
    {
        sprintf(GameSFX, "GameSFX%s\\%s", GameLanguage, "%s");
        sprintf(GameSFXWav, "GameSFX%s\\%s.wav", GameLanguage, "%s");
    }
    
    char moviePath[256];
    sprintf(moviePath, ".\\%sMovies%s", ResourcePath, GameLanguage);
    if (DirectoryExists(moviePath)) sprintf(MoviesResourcePath, "%sMovies%s\\", ResourcePath, GameLanguage);
    
    Graphlib__LoadFontFile();
}
