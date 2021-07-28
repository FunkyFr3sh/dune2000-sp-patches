// ### MAP structs ###

typedef enum TileFlags
{
  TileFlags_1_OWNER = 0x1,
  TileFlags_2_OWNER = 0x2,
  TileFlags_4_OWNER = 0x4,
  TileFlags_8_OCC_UNIT = 0x8,
  TileFlags_10_OCC_BUILDING = 0x10,
  TileFlags_20_CSPOT_MID = 0x20,
  TileFlags_40_CSPOT_TR = 0x40,
  TileFlags_80_CSPOT_DR = 0x80,
  TileFlags_100_CSPOT_DL = 0x100,
  TileFlags_200_CSPOT_TL = 0x200,
  TileFlags_400_HAS_WALL = 0x400,
  TileFlags_800_HAS_CONCRETE = 0x800,
  TileFlags_1000 = 0x1000,
  TileFlags_2000_DRIVE_ON = 0x2000,
  TileFlags_4000_WALK_ON = 0x4000,
  TileFlags_8000_BUILD_ON = 0x8000,
  TileFlags_10000_SANDY = 0x10000,
  TileFlags_20000 = 0x20000,
  TileFlags_40000 = 0x40000,
  TileFlags_80000 = 0x80000,
  TileFlags_100000_SPICE = 0x100000,
  TileFlags_200000_SPICE = 0x200000,
  TileFlags_400000_SPICE = 0x400000,
  TileFlags_800000 = 0x800000,
  TileFlags_1000000 = 0x1000000,
  TileFlags_2000000 = 0x2000000,
  TileFlags_4000000 = 0x4000000,
  TileFlags_8000000 = 0x8000000,
  TileFlags_10000000 = 0x10000000,
  TileFlags_20000000_TERRAIN_TYPE2 = 0x20000000,
  TileFlags_40000000_TERRAIN_TYPE2 = 0x40000000,
  TileFlags_80000000_TERRAIN_TYPE2 = 0x80000000,
}TileFlags;

typedef struct GameMapTileStruct
{
  __int16 __tile_index;
  __int16 w_field_2;
  TileFlags __tile_bitflags;
  char __shroud_flags;
  unsigned __int8 c_field_9;
}GameMapTileStruct;

typedef struct GameMapStruct
{
  int width;
  int height;
  GameMapTileStruct map[16384];
}GameMapStruct;