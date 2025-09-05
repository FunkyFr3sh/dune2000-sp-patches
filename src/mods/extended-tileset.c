#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "extended-tileset.h"
#include "radar.h"

// Tileset data
int             num_tileset_tiles;
TImage *        _images_tiles[4000]; // Extended array
TilesetHeader   tileset_header;
uint32_t        _TileBitflags[4000]; // Extended array
uint32_t        extra_tileflags[4000];
int             _TileTooltips[4000]; // Extended array
uint32_t        tile_restrictions[4000];
int             radar_color_rules_used;
RadarColorRule  radar_color_rules[32];

// Change address of arrays
SETDWORD(0x00424E84, __images_tiles); // DebugPrintOnScreen
SETDWORD(0x0045D8C1, __images_tiles); // UpdateParticles
SETDWORD(0x0046305D, __images_tiles); // LoadTileset
SETDWORD(0x004630D3, __images_tiles + 3200); // LoadTileset

// Custom implementation of function LoadMapData
DETOUR(0x0044C6D0, 0x0044CD09, _Mod__LoadMapData);

void Mod__LoadMapData(char *map_name, char initial_load)
{
  FILE *map_file; // ebp
  int cell_offset; // ecx
  int *cell_number_width_span_ptr; // eax MAPDST
  short width_; // ax
  int x; // esi
  int cell_index; // eax
  FILE *tileatr_file; // eax MAPDST
  char *maps_folder; // [esp-4h] [ebp-3B0h]
  int width; // [esp+14h] [ebp-398h] MAPDST
  int height; // [esp+18h] [ebp-394h]
  int Buffer; // [esp+1Ch] [ebp-390h]
  int y; // [esp+20h] [ebp-38Ch]
  char file_name[452]; // [esp+24h] [ebp-388h]

  memset(_TileTooltips, 0xFFu, sizeof(_TileTooltips));
  if ( !gRestartGame )
  {
    LoadTileset(_BloxFileName);
  }
  if ( gGameType )
  {
    maps_folder = gMAPS_RES_PATH;
  }
  else
  {
    maps_folder = gMISSIONS_RES_PATH;
  }
  map_file = _OpenFile(map_name, "rb", maps_folder);
  if ( !map_file )
  {
    DebugFatal("LoadMapData", "Cannot open file");
  }
  _ReadFile(&width, 2u, 1u, map_file);
  gGameMap.width = (unsigned short)width;
  _ReadFile(&height, 2u, 1u, map_file);
  width = gGameMap.width;
  gGameMap.height = (unsigned short)height;
  // New logic start
  // Map too small crash fix, adjust BattleField size
  SetBattleFieldSize();
  // New logic end
  cell_offset = 0;
  cell_number_width_span_ptr = _CellNumbersWidthSpan;
  do
  {
    *cell_number_width_span_ptr = cell_offset;
    ++cell_number_width_span_ptr;
    cell_offset += width;
  }
  while ( (unsigned int)cell_number_width_span_ptr < 0x547FF0 );
  width_ = width;
  if ( (unsigned short)width > 128u )
  {
    DebugFatal("LoadMapData", "Error in file %s", map_name);
    width_ = width;
  }
  if ( (unsigned short)height > 128u )
  {
    DebugFatal("LoadMapData", "Error in file %s", map_name);
    width_ = width;
  }
  if ( initial_load )
  {
    y = 0;
    if ( (_WORD)height )
    {
      cell_number_width_span_ptr = _CellNumbersWidthSpan;
      do
      {
        x = 0;
        if ( width_ )
        {
          do
          {
            _ReadFile(&Buffer, 2u, 1u, map_file);
            gGameMap.map[*cell_number_width_span_ptr + x].__tile_index = Buffer;
            // New logic start
            // Store back-up tile index into GameMapTileStruct.back_up_tile_index during loading of map
            gGameMap.map[*cell_number_width_span_ptr + x].back_up_tile_index = Buffer;
            // New logic end
            _ReadFile(&Buffer, 2u, 1u, map_file);
            cell_index = *cell_number_width_span_ptr + x++;
            gGameMap.map[cell_index].__tile_bitflags = (unsigned short)Buffer;
            gGameMap.map[cell_index].__shroud = 17;
            gGameMap.map[cell_index].__damage = 0;
            width_ = width;
          }
          while ( x < (unsigned short)width );
        }
        ++cell_number_width_span_ptr;
        ++y;
      }
      while ( y < (unsigned short)height );
    }
  }
  CloseFile(map_file);
  _mapvisstate_548010 = GetMapVisState();
  _ptr_circles[0] = _circle_1x1grid;
  _ptr_circles[1] = _circle_3x3grid;
  _ptr_circles[2] = _circle_5x5grid;
  _ptr_circles[3] = _circle_7x7grid;
  _ptr_circles[4] = _circle_9x9grid;
  _ptr_circles[5] = _circle_11x11grid;
  _ptr_circles[6] = _circle_13x13grid;
  _ptr_circles[7] = _circle_15x15grid;
  // New logic - Load tileset data from TLS file
  if (!gRestartGame)
  {
    bool tls_loaded = false;
    char path[256];
    sprintf(path, "Tilesets\\%s.TLS", _BloxFileName);
    FILE *tls_file = _OpenFile(path, "rb", NULL);
    if (tls_file)
    {
      _ReadFile(&tileset_header,          sizeof(tileset_header),         1, tls_file);
      _ReadFile(_TileBitflags,            sizeof(_TileBitflags),          1, tls_file);
      _ReadFile(extra_tileflags,          sizeof(extra_tileflags),        1, tls_file);
      _ReadFile(_TileTooltips,            sizeof(_TileTooltips),          1, tls_file);
      _ReadFile(tile_restrictions,        sizeof(tile_restrictions),      1, tls_file);
      fseek(tls_file, 8*32, SEEK_CUR);
      _ReadFile(&radar_color_rules_used,  sizeof(radar_color_rules_used), 1, tls_file);
      _ReadFile(radar_color_rules,        sizeof(radar_color_rules),      1, tls_file);
      CloseFile(tls_file);
      tls_loaded = true;
      if (tileset_header.custom_minimap_colors_allowed)
      {
        for (int i = 0; i < radar_color_rules_used; i++)
        {
          uint32_t color = radar_color_rules[i].color;
          if (gBitsPerPixel == 16)
            radar_color_rules[i].color_16bit = GetColor16bit(_colormask1, color);
          if (gBitsPerPixel == 8)
            radar_color_rules[i].color_8bit = GetColor8bit(color >> 18 & 63, color >> 10 & 63, color >> 2 & 63, _PalettePtr, 0, 0, 1);
        }
      }
      else
      {
        InitDefaultRadarColorRules();
      }
    }
    else
    {
      InitDefaultRadarColorRules();
    }
    // Fall back to loading TILEATRx.BIN file
    if (!tls_loaded)
    {
      sprintf(file_name, "bin\\%s.bin", _AttribFileName);
      tileatr_file = _OpenFile(file_name, "rb", 0);
      if ( !tileatr_file )
      {
        ReportFileError(file_name, 0);
        return;
      }
      _ReadFile(_TileBitflags, 1u, 3200u, tileatr_file);
      _ReadFile(_TileTooltips, 1u, 3200u, tileatr_file);
      CloseFile(tileatr_file);
    }
  }
}

// Custom implementation of function LoadTilesetResourceFile
DETOUR(0x00462C80, 0x00462D0B, _Mod__LoadTilesetResourceFile);

void Mod__LoadTilesetResourceFile(char *filename)
{
  FILE *file; // esi
  int fileno; // eax
  int filesize; // edi
  TImage **image_ptr; // esi
  ResourceInfo resource_info;

  file = _OpenFile(filename, "rb", 0);
  if ( !file )
  {
    ReportFileError(filename, 0);
  }
  fileno = __fileno(file);
  filesize = _filelength(fileno);
  _tileset_resource_buffer = Alloc(filesize, 0);
  _ReadFile(_tileset_resource_buffer, 1u, filesize, file);
  CloseFile(file);
  resource_info.address = (int)_tileset_resource_buffer;
  image_ptr = _images_tiles;
  num_tileset_tiles = 0;
  do
  {
    *image_ptr = SetTImageToResource(&resource_info);
    ++image_ptr;
    ++num_tileset_tiles;
  }
  while ( resource_info.address < (int)_tileset_resource_buffer + filesize );
}

// Custom implementation of function BlitTiles
DETOUR(0x00423D20, 0x00423FA8, _Mod__BlitTiles);

void Mod__BlitTiles(TImage *img)
{
  int v1; // esi MAPDST
  int tile_start_y; // edi
  int tile_start_x; // eax
  int cell_index; // edx
  int tile_x_plus_y; // edi
  int draw_y; // ebx
  GameMapTileStruct *tile; // ebp
  int draw_x; // esi
  int viewport_width; // eax
  unsigned char tile_damage; // al
  TileFlags tile_flags; // eax
  int v14; // eax
  int v15; // [esp+10h] [ebp-28h]
  unsigned char crater_degree; // [esp+14h] [ebp-24h]
  unsigned char crater_style; // [esp+18h] [ebp-20h]
  int i; // [esp+20h] [ebp-18h]
  int v20; // [esp+24h] [ebp-14h]
  RECT rect; // [esp+28h] [ebp-10h]

  rect.left = 0;
  v1 = ((_ViewportXPos >> 31) ^ (abs((_BYTE)_ViewportXPos) & 0x1F)) - (_ViewportXPos >> 31);
  rect.right = _ViewportWidth;
  rect.top = _OptionsBarHeight;
  rect.bottom = _ViewportHeight + _OptionsBarHeight;
  v20 = ((_ViewportYPos >> 31) ^ (abs((_BYTE)_ViewportYPos) & 0x1F)) - (_ViewportYPos >> 31);
  tile_start_y = (_ViewportYPos - v20) / 32;
  tile_start_x = (_ViewportXPos - v1) / 32;
  cell_index = tile_start_x + _CellNumbersWidthSpan[tile_start_y];
  tile_x_plus_y = tile_start_x + tile_start_y;
  draw_y = -v20;
  tile = &gGameMap.map[cell_index];
  if ( -v20 < _ViewportHeight - v20 + 32 )
  {
    draw_x = -v1;
    for ( i = draw_x; ; draw_x = i )
    {
      viewport_width = _ViewportWidth;
      if ( draw_x < _ViewportWidth - v1 + 32 )
      {
        v15 = tile_x_plus_y + 16;
        do
        {
          if ( (tile->__shroud & 0xF) != 1 )
          {
            if (tile->__tile_index < num_tileset_tiles)
              BlitTImageCopy(
                img,
                &rect,
                draw_x,
                draw_y + _OptionsBarHeight,
                _images_tiles[(unsigned short)tile->__tile_index]);
            else
              DebugFatal("BlitTiles", "Tile has index %d, but tileset has %d tiles", tile->__tile_index, num_tileset_tiles);
            tile_damage = tile->__damage;
            if ( tile_damage > 16u )
            {
              crater_degree = tile_damage >> 4;
              crater_style = 16 * (tile_x_plus_y % 2);
              tile_flags = tile->__tile_bitflags;
              if ( !(tile_flags & TileFlags_8000_BUILD_ON) )
              {
                if ( tile_flags & TileFlags_10000_SANDY )
                {
                  if (!tileset_header.rule_do_not_draw_sand_craters)
                    BlitClipTImage2(
                      img,
                      &rect,
                      draw_x
                    + _crater_draw_offsets_x[((tile_x_plus_y >> 31) ^ (abs((_BYTE)tile_x_plus_y) & 0xF))
                                           - (tile_x_plus_y >> 31)],
                      _OptionsBarHeight
                    + draw_y
                    + _crater_draw_offsets_y[((v15 >> 31) ^ (abs((_BYTE)v15) & 0xF)) - (v15 >> 31)],
                      _images_crater[crater_style + crater_degree][1],
                      1,
                      1);
                }
              }
              else
              {
                if (!tileset_header.rule_do_not_draw_rock_craters)
                  BlitClipTImage2(
                    img,
                    &rect,
                    draw_x
                  + _crater_draw_offsets_x[((tile_x_plus_y >> 31) ^ (abs((_BYTE)tile_x_plus_y) & 0xF))
                                         - (tile_x_plus_y >> 31)],
                    _OptionsBarHeight
                  + draw_y
                  + _crater_draw_offsets_y[((v15 >> 31) ^ (abs((_BYTE)v15) & 0xF)) - (v15 >> 31)],
                    _images_crater[crater_style + crater_degree][0],
                    1,
                    1);
              }
            }
          }
          viewport_width = _ViewportWidth;
          ++tile_x_plus_y;
          ++v15;
          draw_x += 32;
          ++tile;
        }
        while ( draw_x < _ViewportWidth - v1 + 32 );
      }
      draw_y += 32;
      v14 = viewport_width / 32;
      tile_x_plus_y -= v14;
      tile += _CellNumbersWidthSpan[1] - v14 - 1;
      if ( draw_y >= _ViewportHeight - v20 + 32 )
      {
        break;
      }
    }
  }
}
