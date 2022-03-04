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
