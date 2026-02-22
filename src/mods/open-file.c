#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"

// Add native support for loading files directly from CustomCampaignData folder

// Custom implementation of function OpenFile
DETOUR(0x0042BA50, 0x0042BB32, _Mod__OpenFile);
FILE *Mod__OpenFile(LPCSTR filename, char *mode, char *path)
{
  FILE *file; // eax
  char Buffer[300]; // [esp+Ch] [ebp-98h]
  char *basepath;

  if (path)
    basepath = path;
  else
    basepath = gRES_PATH;

  if (strlen(CampaignFolder) && strlen(ModsFolder))
  {
    if (!strcmp(filename, "bin\\Colours.bin"))
      sprintf(Buffer, "CustomCampaignData\\%s\\Colours\\%s", CampaignFolder, ColoursFile);
    else
      sprintf(Buffer, "CustomCampaignData\\%s\\%s\\%s%s", CampaignFolder, ModsFolder, basepath, filename);
    file = fopen(Buffer, mode);
    if (!file)
      goto NO_CUSTOM_CAMPAIGN_DATA;
    ++_gFilesOpen;
    ++_gFilesOpen2_0;
    if ( _gFilesOpen > _gFilesOpen2 )
    {
      _gFilesOpen2 = _gFilesOpen;
    }
    return file;
  }

NO_CUSTOM_CAMPAIGN_DATA:

  sprintf(Buffer, "%s%s", basepath, filename);

  if ( path
    && !strncmp(path, gMOVIES_RES_PATH, strlen(gMOVIES_RES_PATH))
    && !DoesFileExist((char *)filename)
    && !CheckForCD(0, 1) )
  {
    return 0;
  }
  file = fopen(Buffer, mode);
  if ( file )
  {
    ++_gFilesOpen;
    ++_gFilesOpen2_0;
  }
  if ( _gFilesOpen > _gFilesOpen2 )
  {
    _gFilesOpen2 = _gFilesOpen;
  }
  return file;
}
