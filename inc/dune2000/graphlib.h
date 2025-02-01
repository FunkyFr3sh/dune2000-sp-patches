// ### Graphics library structs ###

typedef struct TImagePalette
{
  _DWORD refcount;
  short *data;
} TImagePalette;

typedef struct TImage
{
  int width;
  int height;
  int right;
  int bottom;
  char *buffer;
  TImagePalette *palette;
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

typedef struct DisplayListStruct
{
  int xpos;
  int ypos;
  char __object_type;
  char field_9;
  char field_A;
  char field_B;
  Building *__object_ptr;
  TImage *__base_image;
  TImage *__barrel_image;
  TImage *__anim_image;
  struct DisplayListStruct *next;
  char __side_id;
  char __is_selected;
  char __healthbarsize;
  char field_23;
  int __health;
  int __max_health;
  int __sort_ypos;
  int __flags;
} DisplayListStruct;
