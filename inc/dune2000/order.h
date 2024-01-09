// ### Order structs ###

typedef void OrderEntry;

typedef struct OrderStruct
{
  int32_t dw_field_0;
  int32_t dw_field_4;
  int32_t dw_field_8;
  OrderEntry *data;
  int32_t dw_field_10;
  int32_t dw_field_14;
  int32_t dw_field_18;
  int32_t dw_field_1C;
  int32_t dw_field_20;
  int32_t dw_field_24;
  int32_t dw_field_28;
  int32_t dw_field_2C;
  int32_t dw_field_30;
  char c_field_34;
  char c_field_35;
  char c_field_36;
  char c_field_37;
  char c_field_38;
  char c_field_39;
  char c_field_3A;
  char field_3B_paddingmaybe;
} OrderStruct;

typedef enum eOrderType
{
  eOrderType_0 = 0,
  eOrderType_1 = 1,
  eOrderType_2_CISML = 2,
  eOrderType_3_SCI = 3,
  eOrderType_4 = 4,
  eOrderType_5_CCSM = 5,
  eOrderType_6_SCM = 6,
  eOrderType_7 = 7,
  eOrderType_8 = 8,
  eOrderType_9 = 9,
  eOrderType_A = 10,
  eOrderType_B = 11,
  eOrderType_C = 12,
  eOrderType_D = 13,
  eOrderType_E = 14,
  eOrderType_F = 15,
  eOrderType_10 = 16,
  eOrderType_11 = 17,
  eOrderType_12 = 18,
  eOrderType_13_UNITMOVE = 19,
  eOrderType_14_UNITATTACKUNIT = 20,
  eOrderType_15_UNITATTACKBUILDING = 21,
  eOrderType_16_UNITATTACKTILE = 22,
  eOrderType_17_DOCKWITHREFINERY = 23,
  eOrderType_18_GUARD = 24,
  eOrderType_19_SCATTER = 25,
  eOrderType_1A_RETREAT = 26,
  eOrderType_1B_ALLY = 27,
  eOrderType_1C_STOPUNIT = 28,
  eOrderType_1D_STOPBUILDING = 29,
  eOrderType_1E_BUILDINGATTACKUNIT = 30,
  eOrderType_1F_BUILDINGATTACKBUILDING = 31,
  eOrderType_20_SETPRIMARY = 32,
  eOrderType_21_UNITREPAIR = 33,
  eOrderType_22_BUILDINGREPAIR = 34,
  eOrderType_23_BUILDINGSELL = 35,
  eOrderType_24_BUILDINGPICK = 36,
  eOrderType_25_BUILDINGCANCEL = 37,
  eOrderType_26_BUILDINGPLACE = 38,
  eOrderType_27_UNITPICK = 39,
  eOrderType_28_UNITCANCEL = 40,
  eOrderType_29_STARPORTPICK = 41,
  eOrderType_2A_STARPORTUNPICK = 42,
  eOrderType_2B_STARPORTPURCHASE = 43,
  eOrderType_2C_STARPORTCANCEL = 44,
  eOrderType_2D_UPGRADEPICK = 45,
  eOrderType_2E_UPGRADECANCEL = 46,
  eOrderType_2F_DEPLOY = 47,
  eOrderType_30_SPECIAL = 48,
  eOrderType_31_SURRENDER = 49,
  eOrderType_32_AITAKEOVER = 50,
  eOrderType_33_OFFERDRAW = 51,
  eOrderType_34_OFFEREDDRAW = 52,
  eOrderType_35 = 53,
  eOrderType_36 = 54,
  eOrderType_37 = 55,
  eOrderType_38 = 56,
  eOrderType_39 = 57,
  eOrderType_3A = 58,
  eOrderType_3B = 59,
  eOrderType_3C = 60,
  eOrderType_3D = 61,
  eOrderType_3E = 62,
  eOrderType_3F = 63,
  eOrderType_40 = 64,
  eOrderType_41 = 65,
} eOrderType;

typedef struct Orderdata
{
  int __ObjectsServed1;
  __int16 w_field_4;
  __int16 w_field_6;
  int GameTicks;
  _DWORD OrderNumber;
  int __ObjectsServed2;
  int __PlaceBuildingX;
  __int16 NumObjects;
  __int16 __PlaceBuildingY;
  __int16 BlockToX;
  __int16 BlockToY;
  __int16 TargetObjectIndex;
  char TargetSideId;
  _BYTE OrderType;
  unsigned __int8 SideId;
  char field_25;
  __int16 ObjectArray[64];
} Orderdata;
