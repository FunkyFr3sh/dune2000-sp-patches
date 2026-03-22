#include "macros/patch.h"
#include "dune2000.h"
#include "extended-templates.h"

// Extend number of explosion types to 128
// Disable chosen buildings or units according to spawn.ini settings

// Templates data
ExploisonAtrbStruct  _templates_explosionattribs[MAX_EXPLOSION_TYPES]; // Extended array
char                 _templates_ExplosionNameList[MAX_EXPLOSION_TYPES][50]; // Extended array
int                  _templates_AnimationArtFrames[MAX_EXPLOSION_TYPES]; // Extended array
int                  _templates_Explosiondata_AnimationArtFlags[MAX_EXPLOSION_TYPES]; // Extended array
TImage *             gExplosionElements[MAX_EXPLOSION_TYPES][40]; // Extended array

// Armour data
WarheadStruct        _WarheadData[MAX_WARHEAD_TYPES];
char                 _WarheadNames[MAX_WARHEAD_TYPES][50];
char                 _ArmourNames[MAX_ARMOUR_TYPES][50];

// Multiplayer settings to disable chosen buildings or units
bool DisableEngineer = false;
bool DisableTurrets = false;
bool NoCarryall = false;

// Change address of arrays
SETDWORD(0x0049C005, __templates_explosionattribs + 4); // UpdateBullet
SETDWORD(0x0049C5DF, __templates_explosionattribs + 4); // UpdateBullet
SETDWORD(0x0049C968, __templates_explosionattribs + 4); // UpdateBullet
SETDWORD(0x0042824D, __templates_AnimationArtFrames); // BlitCursorPuffAnimations
SETDWORD(0x0044847A, __templates_AnimationArtFrames); // InitCursorPuffQueue
SETDWORD(0x00456D5F, __templates_AnimationArtFrames); // AddCursorPuffAnimationToQueue
SETDWORD(0x00460397, __templates_AnimationArtFrames); // AddTImagesToResource
SETDWORD(0x00460A49, __templates_AnimationArtFrames); // SetDataResourceImages
SETDWORD(0x0046A78D, __templates_AnimationArtFrames); // ReleaseGraphics
SETDWORD(0x00428285, _gExplosionElements); // BlitCursorPuffAnimations
SETDWORD(0x004283FC, _gExplosionElements); // BlitExplosions
SETDWORD(0x00460392, _gExplosionElements); // AddTImagesToResource
SETDWORD(0x00460A44, _gExplosionElements); // SetDataResourceImages
SETDWORD(0x0046A788, _gExplosionElements); // ReleaseGraphics

void ReadTemplates()
{
  FILE *file = _OpenFile("bin\\Templates.bin", "rb", 0);
  if ( file )
  {
    _ReadFile(_templates_unitattribs, 1u, 0x3C00u, file);
    _ReadFile(_templates_buildattribs, 1u, 26800u, file);
    _ReadFile(_templates_bulletattribs, 1u, 0x700u, file);
    _ReadFile(_templates_explosionattribs, 1u, MAX_ORIG_EXPLOSION_TYPES * sizeof(ExploisonAtrbStruct), file);
    _ReadFile(_templates_UnitArtAnimationFrames, 1u, 360u, file);
    _ReadFile(_templates_UnitArtDirectionFrames, 1u, 360u, file);
    _ReadFile(&gNumUnitElements, 1u, 4u, file);
    _ReadFile(&gUnitTypeNum, 1u, 1u, file);
    _ReadFile(&gNumExplosionElements, 1u, 4u, file);
    _ReadFile(_templates_AnimationArtFrames, 1u, MAX_ORIG_EXPLOSION_TYPES * sizeof(int), file);
    _ReadFile(&gExplosionTypeNum, 1u, 1u, file);
    _ReadFile(&gNumBulletElements, 1u, 4u, file);
    _ReadFile(_templates_ProjectileArtDirections, 1u, 0x100u, file);
    _ReadFile(&gBulletTypeNum, 1u, 1u, file);
    _ReadFile(&gNumBuildingElements, 1u, 4u, file);
    _ReadFile(_templates_BuildingArtDirections, 1u, 480u, file);
    _ReadFile(&gBuildingTypeNum, 1u, 1u, file);
    _ReadFile(_templates_BuildingNameList, 1u, 0xAFC8u, file);
    _ReadFile(_templates_BulletNameList, 1u, 0xC80u, file);
    _ReadFile(_templates_ExplosionNameList, 1u, MAX_ORIG_EXPLOSION_TYPES * 50, file);
    _ReadFile(_templates_UnitNameList, 1u, 0x6978u, file);
    _ReadFile(_templates_UnitGroupNameList, 1u, 0xBB8u, file);
    _ReadFile(_templates_BuildingGroupNameList, 1u, 0x1388u, file);
    _ReadFile(&_templates_GroupIDs, 1u, 86u, file);
    _ReadFile(_templates_Explosiondata_AnimationArtFlags, 1u, MAX_ORIG_EXPLOSION_TYPES * sizeof(int), file);
    _ReadFile(_templates_BuildingAnimationFrames, 1u, 100u, file);
    _ReadFile(_templates_BuildupArtFrames, 1u, 100u, file);
    _ReadFile(&_templates_BuildingGroupCount, 1u, 1u, file);
    _ReadFile(&_templates_UnitGroupCount, 1u, 1u, file);
    // New logic start
    // Load extra data for explosion types up to 128
    _ReadFile(&_templates_explosionattribs[MAX_ORIG_EXPLOSION_TYPES], 1, sizeof(ExploisonAtrbStruct) * (MAX_EXPLOSION_TYPES - MAX_ORIG_EXPLOSION_TYPES), file);
    _ReadFile(&_templates_ExplosionNameList[MAX_ORIG_EXPLOSION_TYPES], 1, 50 * (MAX_EXPLOSION_TYPES - MAX_ORIG_EXPLOSION_TYPES), file);
    _ReadFile(&_templates_AnimationArtFrames[MAX_ORIG_EXPLOSION_TYPES], 1, sizeof(int) * (MAX_EXPLOSION_TYPES - MAX_ORIG_EXPLOSION_TYPES), file);
    _ReadFile(&_templates_Explosiondata_AnimationArtFlags[MAX_ORIG_EXPLOSION_TYPES], 1, sizeof(int) * (MAX_EXPLOSION_TYPES - MAX_ORIG_EXPLOSION_TYPES), file);
    // New logic end
    CloseFile(file);
    // New logic start
    // Disable chosen buildings or units according to spawn.ini settings
    for (int i = 0; i < gUnitTypeNum; i++)
    {
      if (DisableEngineer && _templates_unitattribs[i].__Behavior == UnitBehavior_ENGINEER)
        _templates_unitattribs[i].__TechReq = 255;
      if (NoCarryall && _templates_unitattribs[i].__Behavior == UnitBehavior_CARRYALL)
        _templates_unitattribs[i].__TechReq = 255;
    }
    for (int i = 0; i < gBuildingTypeNum; i++)
    {
      if (DisableTurrets && _templates_buildattribs[i].__Behavior == BuildingBehavior_TURRET)
        _templates_buildattribs[i]._____TechLevelBuild = 255;
    }
    // New logic end
  }
  else
  {
    ReportFileError("bin\\Templates.bin", 0);
  }
}

// Custom implementation of function ReadArmour
DETOUR(0x0046B0F0, 0x0046B68B, _Mod__ReadArmour);

void Mod__ReadArmour()
{
  FILE *file; // eax MAPDST
  char Buffer[80]; // [esp+20h] [ebp-3D8h]

  _ArmourCount = 0;
  _WarheadCount = 0;
  if ( _ReadBinaries )
  {
    sprintf(Buffer, "bin\\%s.bin", "armour");
    file = _OpenFile(Buffer, "rb", 0);
    if ( !file )
    {
      ReportFileError(Buffer, 0);
      return;
    }
    /*_ReadFile(_WarheadData, 1u, 0x258u, file);
    _ReadFile(_WarheadNames, 1u, 1500u, file);
    _ReadFile(_ArmourNames, 1u, 600u, file);
    _ReadFile(&_WarheadCount, 1u, 1u, file);
    _ReadFile(&_ArmourCount, 1u, 1u, file);*/
    for (int i = 0; i < MAX_ORIG_WARHEAD_TYPES; i++)
    {
      _ReadFile(&_WarheadData[i].Verses[0], 1, sizeof(_WarheadData[i].Verses[0]) * MAX_ORIG_ARMOUR_TYPES, file);
      _ReadFile(&_WarheadData[i].Radius, 1, sizeof(_WarheadData[i].Radius), file);
      _ReadFile(&_WarheadData[i].InfDeath, 1, 4, file);
    }
    _ReadFile(_WarheadNames[0], 1, sizeof(_WarheadNames[0]) * MAX_ORIG_WARHEAD_TYPES, file);
    _ReadFile(_ArmourNames[0], 1, sizeof(_ArmourNames[0]) * MAX_ORIG_ARMOUR_TYPES, file);
    _ReadFile(&_WarheadCount, 1, sizeof(_WarheadCount), file);
    _ReadFile(&_ArmourCount, 1, sizeof(_ArmourCount), file);
    // Extended data
    for (int i = 0; i < MAX_ORIG_WARHEAD_TYPES; i++)
      _ReadFile(&_WarheadData[i].Verses[MAX_ORIG_ARMOUR_TYPES], 1, sizeof(_WarheadData[i].Verses[MAX_ORIG_ARMOUR_TYPES]) * (MAX_ARMOUR_TYPES - MAX_ORIG_ARMOUR_TYPES), file);
    _ReadFile(&_WarheadData[MAX_ORIG_WARHEAD_TYPES], 1, sizeof(_WarheadData[MAX_ORIG_WARHEAD_TYPES]) * (MAX_WARHEAD_TYPES - MAX_ORIG_WARHEAD_TYPES), file);
    _ReadFile(_WarheadNames[MAX_ORIG_WARHEAD_TYPES], 1, sizeof(_WarheadNames[MAX_ORIG_WARHEAD_TYPES]) * (MAX_WARHEAD_TYPES - MAX_ORIG_WARHEAD_TYPES), file);
    _ReadFile(_ArmourNames[MAX_ORIG_ARMOUR_TYPES], 1, sizeof(_ArmourNames[MAX_ORIG_ARMOUR_TYPES]) * (MAX_ARMOUR_TYPES - MAX_ORIG_ARMOUR_TYPES), file);
    CloseFile(file);
  }
}

// Custom implementation of function LoadData
DETOUR(0x00466140, 0x00469785, _Mod__LoadData);

void Mod__LoadData()
{
  FILE *file; // esi MAPDST
  char (*dense_spice_tile_mapping_ptr)[8]; // edi
  unsigned int x; // esi
  unsigned int i; // esi
  unsigned int v7; // ebx
  short *v8; // ebp
  unsigned int v9; // ecx
  unsigned int v10; // edi
  signed int v11; // edx
  char v12; // al

  if ( gBitsPerPixel == 8 )
  {
    _isnot8bit_byte_515BB0 = 0;
  }
  _templates_GroupIDs.DebrisCount = 0;
  if ( _ReadBinaries )
  {
    file = _OpenFile("bin\\circles.bin", "rb", 0);
    if ( !file )
    {
      ReportFileError("bin\\circles.bin", 0);
    }
    _ReadFile(_circle_1x1grid, 1u, 1u, file);
    _ReadFile(_circle_3x3grid, 1u, 9u, file);
    _ReadFile(_circle_5x5grid, 1u, 25u, file);
    _ReadFile(_circle_7x7grid, 1u, 49u, file);
    _ReadFile(_circle_9x9grid, 1u, 81u, file);
    _ReadFile(_circle_11x11grid, 1u, 121u, file);
    _ReadFile(_circle_13x13grid, 1u, 169u, file);
    _ReadFile(_circle_15x15grid, 1u, 225u, file);
    CloseFile(file);
  }
  dense_spice_tile_mapping_ptr = _DenseSpiceTileMapping;
  do
  {
    x = 0;
    do
    {
      (*dense_spice_tile_mapping_ptr)[x++] = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\Setup.cpp", 3922) & 3;
    }
    while ( x < 8 );
    ++dense_spice_tile_mapping_ptr;
  }
  while ( dense_spice_tile_mapping_ptr < (char (*)[8])0x00790550 );
  i = 0;
  do
  {
    _crater_draw_offsets_x[i] = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\Setup.cpp", 3929) % 12u - 12;
    ++i;
    *(_DWORD *)(i * 4 + 5254868) = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\Setup.cpp", 3930) % 12u - 12;
  }
  while ( i < 16 );
  v7 = 0;
  v8 = _blitarray_6D8DA8;
  do
  {
    v9 = 0;
    v10 = v7;
    v11 = v7;
    do
    {
      v12 = v10;
      if ( v10 >= 31 )
      {
        v12 = 31;
      }
      *((_BYTE *)v8 + v9) = v12;
      _blitarray_790110[v9++ + (char *)v8 - (char *)_blitarray_6D8DA8] = v11 <= 0 ? 0 : v11;
      ++v10;
      --v11;
    }
    while ( v9 < 32 );
    v8 += 16;
    ++v7;
  }
  while ( v8 < (short *)_tangentValues );
  if ( _ReadBinaries )
  {
    ReadTemplates();
    InitLensData(16);
    ReadDataTypes();
    ReadSmokeData();
    ReadArmour();
    ReadSpeed();
    if ( gBitsPerPixel == 8 )
    {
      LoadDataResourceFile("data.r8");
    }
    else
    {
      LoadDataResourceFile("data.r16");
    }
  }
}
