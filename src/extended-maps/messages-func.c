#include <windows.h>
#include <stdio.h>
#include <limits.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "utils.h"
#include "rules.h"
#include "messages-func.h"

// New data structure for message data

MessageData gMessageData[MAX_MESSAGES];
unsigned int message_counter;

// Custom implementation of function InitMessageData

CALL(0x004489A8, _Mod__InitMessageData);

void Mod__InitMessageData()
{
  memset(gMessageData, 0, sizeof(gMessageData));
  message_counter = 1;
}

void InitFontColors()
{
  SetFontColorSolid(2, 0xffff00, 0); // 0x404000); // Yellow
  SetFontColorSolid(3, 0x00ffff, 0); // 0x004040); // Cyan
  SetFontColorSolid(4, 0xff00ff, 0); // 0x400040); // Magenta
  SetFontColorSolid(5, 0x00ff00, 0); // 0x004000); // Green
  SetFontColorSolid(6, 0x2020ff, 0); // 0x000040); // Blue
  SetFontColorSolid(7, 0x808080, 0); // 0x101010); // Grey
  //SetFontColorGradient(2, 0xffff00, 0x808000, -1);
  //SetFontColorGradient(3, 0x808000, 0xffff00, -1);
  //SetFontColorGradient(4, 0xffff00, 0xff0000, -1);
  //SetFontColorGradient(5, 0xffff00, 0x00ffff, -1);
  //SetFontColorGradient(6, 0xffffff, 0xff8080, -1);
  //SetFontColorGradient(7, 0x0000ff, 0x808080, -1);
  // Side colors
  if ( gBitsPerPixel == 16 )
  {
    for (int i = 0; i < 8; i++)
    {
      uint16_t *fontpal16 = (uint16_t *)GetFontPaletteHandle(8 + i);
      //fontpal16[1] = _ColoursBinData[i * 16];
      for (int j = 2; j < 16; j++)
        fontpal16[j] = _ColoursBinData[i * 16 + 8];
    }
  }
}

void SetFontColorSolid(int index, int color, int shadow_color)
{
  if ( gBitsPerPixel == 16 )
  {
    uint16_t *fontpal16 = (uint16_t *)GetFontPaletteHandle(index);
    fontpal16[1] = GetColor16bit(_colormask1, shadow_color);
    uint16_t color16 = GetColor16bit(_colormask1, color);
    for (int i = 2; i < 16; i++)
      fontpal16[i] = color16;
  }
}

void SetFontColorGradient(int index, int color1, int color2, int stage, int num_stages)
{
  if ( gBitsPerPixel == 16 )
  {
    uint16_t *fontpal16 = (uint16_t *)GetFontPaletteHandle(index);
    int R1 = (color1 >> 16) & 255;
    int G1 = (color1 >> 8) & 255;
    int B1 = (color1 >> 0) & 255;
    int R2 = (color2 >> 16) & 255;
    int G2 = (color2 >> 8) & 255;
    int B2 = (color2 >> 0) & 255;
    int Rdiff = R2 - R1;
    int Gdiff = G2 - G1;
    int Bdiff = B2 - B1;
    if (stage != -1)
    {
      int R = R1 + (Rdiff * stage) / num_stages;
      int G = G1 + (Gdiff * stage) / num_stages;
      int B = B1 + (Bdiff * stage) / num_stages;
      uint16_t color16 = GetColor16bit(_colormask1, (R << 16) | (G << 8) | B);
      for (int i = 0; i < 14; i++)
        fontpal16[i + 2] = color16;
    }
    else
    {
      for (int i = 0; i < 14; i++)
      {
        int R = R1 + (Rdiff * i) / 14;
        int G = G1 + (Gdiff * i) / 14;
        int B = B1 + (Bdiff * i) / 14;
        uint16_t color16 = GetColor16bit(_colormask1, (R << 16) | (G << 8) | B);
        fontpal16[i + 2] = color16;
      }
    }
  }
}

// Custom implementation of function QueueMessage
LJMP(0x00425720, _Mod__QueueMessage);

void Mod__QueueMessage(const char *message, int type)
{
  (void)type;
  QueueMessageExt(message, 0, 0, 0, 0, 0, 0, 0);
}

void QueueMessageExt(const char *message, int duration, int ref_id, int screen_pos_type, int offset_x, int offset_y, int color, bool type_on)
{
  // Find slot for message
  int slot = -1;

  // Step 1: try to find existing message with same ref_id
  if (ref_id != 0)
  {
    for (int i = 0; i < MAX_MESSAGES; i++)
      if (gMessageData[i].ref_id == ref_id)
      {
        slot = i;
        break;
      }
  }
  // Step 2: try to find a free message slot, or chat message with nearest expire time
  if (slot == -1)
  {
    unsigned int nearest_expire_ticks = UINT_MAX;
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
      if (gMessageData[i].expire_ticks <= gGameTicks)
      {
        slot = i;
        break;
      }
      if (gMessageData[i].is_chat && gMessageData[i].expire_ticks < nearest_expire_ticks)
      {
        nearest_expire_ticks = gMessageData[i].expire_ticks;
        slot = i;
      }
    }
  }
  // Step 3: try to find non-chat message with nearest expire time
  if (slot == -1)
  {
    unsigned int nearest_expire_ticks = UINT_MAX;
    for (int i = 0; i < MAX_MESSAGES; i++)
      if (!gMessageData[i].is_chat && gMessageData[i].expire_ticks < nearest_expire_ticks)
      {
        nearest_expire_ticks = gMessageData[i].expire_ticks;
        slot = i;
      }
  }
  // Should not happen
  if (slot == -1)
    DebugFatal("messages-func.c", "Could not find slot for a message.");

  // Compute message screen position
  if (screen_pos_type == 0)
  {
    gMessageData[slot].is_chat = true;
    gMessageData[slot].screen_pos_x = 8;
    gMessageData[slot].screen_pos_y = OptionsBarHeight + 4;
  }
  else
  {
    int xpos = 0;
    int ypos = 0;
    int horizontal_screen_pos = (screen_pos_type - 1) % 5;
    int vertical_screen_pos = (screen_pos_type - 1) / 5;
    switch (horizontal_screen_pos)
    {
    case 0: xpos = offset_x; break;
    case 1: xpos = offset_x - GetStringPixelWidth(message, 1); break;
    case 2: xpos = (_ViewportWidth / 2) - (GetStringPixelWidth(message, 1) / 2) + offset_x; break;
    case 3: xpos = _ViewportWidth - offset_x; break;
    case 4: xpos = _ViewportWidth - offset_x - GetStringPixelWidth(message, 1); break;
    }
    switch (vertical_screen_pos)
    {
    case 0: ypos = OptionsBarHeight + offset_y; break;
    case 1: ypos = OptionsBarHeight + (_ViewportHeight / 2) - 6 - offset_y; break;
    case 2: ypos = OptionsBarHeight + (_ViewportHeight / 2) - 6 + offset_y; break;
    case 3: ypos = OptionsBarHeight + _ViewportHeight - 17 - offset_y; break;
    }
    gMessageData[slot].is_chat = false;
    gMessageData[slot].screen_pos_x = xpos;
    gMessageData[slot].screen_pos_y = ypos;
  }

  // Handle duration
  unsigned int expire_ticks = gGameTicks + duration;
  if (duration == 0)
  {
    if (ref_id == 0)
      expire_ticks = gGameTicks + 400;
    else
      expire_ticks = UINT_MAX;
  }

  // Set up message
  gMessageData[slot].order_added = message_counter++;
  gMessageData[slot].expire_ticks = expire_ticks;
  gMessageData[slot].ref_id = ref_id;
  gMessageData[slot].color = color;
  gMessageData[slot].typeon_mode = type_on;
  gMessageData[slot].typeon_pos = 0;
  memset(gMessageData[slot].text, 0, 128);
  strncpy(gMessageData[slot].text, message, 127);
}

// Custom implementation of function BlitMessages
CALL(0x004490AD, _Mod__BlitMessages);

void Mod__BlitMessages(TImage *img)
{
  // Draw timer
  if ( _gTimerValue != -1 )
  {
    char Buffer[20];
    sprintf(Buffer, "%02d:%02d", _gTimerValue / 1500, _gTimerValue / 25 % 60);
    Graphlib__DrawTextWithBlackShadow(img, Buffer, HighResPatchEnabled?420+HighResAddedWidth:420, OptionsBarHeight + 4, 1, 0);
  }

  // Draw messages
  unsigned int processed_bitfield = 0;

  // First pass - draw non-chat messages
  for (int i = 0; i < MAX_MESSAGES; i++)
  {
    // Message is expired - skip
    if (gGameTicks >= gMessageData[i].expire_ticks)
    {
      processed_bitfield |= (1 << i);
      continue;
    }
    // Messages not in chat can be drawn now
    if (!gMessageData[i].is_chat)
    {
      DrawMessage(img, &gMessageData[i], 0);
      processed_bitfield |= (1 << i);
    }
  }
  // Second pass - draw chat messages
  int messages_drawn = 0;
  int chat_ypos = 0;
  if ( IsCurrentlyShown("PAGE_INGAME") || IsCurrentlyShown("GAMEMESSAGE") )
  {
    chat_ypos += 20;
  }
  while (messages_drawn < rulesExt__maxChatMessages && processed_bitfield != 0xffffffff)
  {
    int slot_to_draw = -1;
    unsigned int max_order_added = 0;

    for (int i = 0; i < MAX_MESSAGES; i++)
    {
      // Already processed - skip
      if (processed_bitfield & (1 << i))
        continue;
      // Draw message which was added latest
      if (gMessageData[i].order_added > max_order_added)
      {
        max_order_added = gMessageData[i].order_added;
        slot_to_draw = i;
      }
    }
    // Should not happen
    if (slot_to_draw == -1)
      DebugFatal("messages-func.c", "Error drawing chat messages: slot_to_draw == -1");

    DrawMessage(img, &gMessageData[slot_to_draw], chat_ypos);
    messages_drawn++;
    chat_ypos += 20;
    processed_bitfield |= (1 << slot_to_draw);
  }
}

void DrawMessage(TImage *img, MessageData *msg, int chat_ypos)
{
  if (msg->typeon_mode)
  {
    msg->typeon_pos = HLIMIT(msg->typeon_pos + 1, 127);
  }
  BlitStringPalExt(img, msg->text, msg->screen_pos_x, msg->screen_pos_y + chat_ypos, 1, msg->color, msg->typeon_mode?msg->typeon_pos:127);
}

void BlitStringPalExt(TImage *img, char *string, int x, int y, unsigned char font, unsigned char fontpalidx, unsigned char maxlength)
{
  int chars_drawn = 0;
  for (int i = 0; string[i]; i++)
  {
    if (chars_drawn == maxlength)
      break;

    // Color change
    if (string[i] == '^')
    {
      char cl = string[i+1];
      if (cl >= '0' && cl <= '9')
        fontpalidx = cl - '0';
      else if (cl >= 'A' && cl <= 'F')
        fontpalidx = cl - 'A' + 10;
      else if (cl >= 'a' && cl <= 'f')
        fontpalidx = cl - 'a' + 10;
      else
        DebugFatal("messages-func.c", "Invalid color character: %c", cl);
      i++;
      continue;
    }

    // Draw character
    unsigned char cha = _FontBinData[(unsigned char)string[i]];
    if ( cha == ' ' )
    {
      x += _FontData[font].c_field_1_minspacing;
    }
    else
    {
      TImage *char_img = _FontData[font].__CharImages[cha - _FontData[font].c_field_2];
      BlitFontChar_0(img, x - char_img->right, y - char_img->bottom, char_img, (_WORD *)_FontPals[fontpalidx]);
      x += char_img->width + _FontData[font].c_field_3;
    }
    chars_drawn++;
  }
}
