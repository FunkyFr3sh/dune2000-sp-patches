
typedef enum eValueOperation
{
  VALUEOPERATION_SET,
  VALUEOPERATION_ADD,
  VALUEOPERATION_SUBSTRACT,
  VALUEOPERATION_MULPERCENT,
  VALUEOPERATION_MODULO,
  VALUEOPERATION_CAPMAX,
  VALUEOPERATION_CAPMIN,
  VALUEOPERATION_BITAND,
  VALUEOPERATION_BITOR,
  VALUEOPERATION_BITXOR,
  VALUEOPERATION_BITLSHIFT,
  VALUEOPERATION_BITRSHIFT
} eValueOperation;

typedef enum eFlagOperation
{
  FLAGOPERATION_SET,
  FLAGOPERATION_UNSET,
  FLAGOPERATION_TOGGLE
} eFlagOperation;

typedef enum eDataSize
{
  DATASIZE_BYTE,
  DATASIZE_WORD,
  DATASIZE_DWORD
} eDataSize;

int ValueOperation(int val1, int val2, eValueOperation operation);
int FlagOperation(int value, int flag, eFlagOperation operation);
void SetDataValue(char *data_ptr, eDataSize data_size, int offset, eValueOperation operation, int value);
bool CompareValue(int val, int comp_val, bool comparison);

void RestoreUnitSelection(int side_id, bool restore_selection);
void RestoreBuildingSelection(int side_id, bool restore_selection);

void ShowDataOnScreen(char *header, unsigned char *data_ptr);
