
// Type definitions

typedef struct MessageData
{
  unsigned int order_added;
  unsigned int expire_ticks;
  int screen_pos_x;
  int screen_pos_y;
  unsigned char ref_id;
  unsigned char color;
  bool is_chat;
  bool typeon_mode;
  unsigned char typeon_pos;
  char text[128];
} MessageData;

// Variables

#define MAX_MESSAGES 32

extern MessageData gMessageData[MAX_MESSAGES];

// Functions

void InitFontColors();
void SetFontColorSolid(int index, int color, int shadow_color);
void SetFontColorGradient(int index, int color1, int color2, int stage, int num_stages);
void QueueMessageExt(const char *message, int duration, int ref_id, int screen_pos_type, int offset_x, int offset_y, int color, bool type_on);
void DrawMessage(TImage *img, MessageData *msg, int chat_ypos);
void BlitStringPalExt(TImage *img, char *string, int x, int y, unsigned char font, unsigned char fontpalidx, unsigned char maxlength);
