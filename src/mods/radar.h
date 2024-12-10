
// Type definitions

typedef struct RadarColorRule
{
  long long attr;
  long long not_attr;
  uint32_t color;
  uint8_t color_8bit;
  uint16_t color_16bit;
} RadarColorRule;

typedef struct RadarMarker
{
  uint8_t x;
  uint8_t y;
  uint8_t thickness;
  uint8_t color_8bit;
  uint16_t color_16bit;
  int counter;
  int max_game_ticks;
} RadarMarker;

// Variables

#define MAX_RADAR_MARKERS 32

extern RadarMarker gRadarMarkers[MAX_RADAR_MARKERS];

// Functions

void InitDefaultRadarColorRules();
void LoadRadarColorRulesFromFile(char *filename);
void Mod__SetupRadarMap();
void SetPixelOnRadar(unsigned char x, unsigned char y);
