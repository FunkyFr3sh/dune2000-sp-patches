// ### Graphics library structs ###

typedef struct TImage
{
  int width;
  int height;
  int right;
  int bottom;
  char *buffer;
  /*TImagePalette*/ void *palette;
  char bitdepth;
  char c_field_19__Height2;
  char c_field_1A__Width2;
} TImage;

typedef struct FontHeader
{
  char c_field_0;
  char c_field_1_minspacing;
  char c_field_2;
  char c_field_3;
  char bitcount;
  TImage *__CharImages[256];
} FontHeader;
