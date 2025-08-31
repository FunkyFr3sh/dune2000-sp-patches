
// Tileset data structs

typedef struct TilesetHeader
{
  int version_major;
  int version_minor;
  int reserved1;
  int reserved2;
  char custom_minimap_colors_allowed;
  char default_paint_group;
  char reserved3;
  char reserved4;
  char rule_do_not_draw_rock_craters;
  char rule_do_not_draw_sand_craters;
  char reserved_rules[14];
  char tileset_fancy_name[32];
  char author_name[32];
} TilesetHeader;

typedef struct RadarColorRule
{
  char name[32];
  long long attr;
  long long not_attr;
  uint32_t color;
  uint8_t color_8bit;
  uint16_t color_16bit;
} RadarColorRule;

// Tileset data

extern int            num_tileset_tiles;
extern uint32_t       _TileBitflags[4000];
extern uint32_t       extra_tileflags[4000];
extern int            _TileTooltips[4000];
extern int            radar_color_rules_used;
extern RadarColorRule radar_color_rules[32];
