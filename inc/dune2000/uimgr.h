// ### UI Manager structs ###

typedef void CInterface;

typedef struct CUIManager
{
  CInterface *__InterfaceStack[20];
  _BYTE byte50;
  char __CurrentInterface;
  char field_52;
  char field_53;
  int data_dword54_id;
  char str_field_58[60];
  char str_field_94[124];
  int dw_field_110_index;
  int fBackImage1;
  int fBackImage2;
  char c_field_11C;
  char __movie_field_11D;
  char field_11E;
  char field_11F;
  int interfacecount_dw_field_120_fade;
  RECT __Rect;
  char field_134;
  char field_135;
  char c_field_136;
  char field_137;
  int dw_field_138;
  int __CreatedOnTick;
  char field_140;
  char field_141;
  char field_142;
  char field_143;
  char field_144;
  char field_145;
  char field_146;
  char field_147;
  char field_148;
  char field_149;
  char field_14A;
  char field_14B;
} CUIManager;
