typedef struct RadarColorRule
{
  long long attr;
  long long not_attr;
  uint32_t color;
  uint8_t color_8bit;
  uint16_t color_16bit;
} RadarColorRule;

void InitDefaultRadarColorRules();
void LoadRadarColorRulesFromFile(char *filename);
void Mod__SetupRadarMap();
void SetPixelOnRadar(unsigned char x, unsigned char y);
