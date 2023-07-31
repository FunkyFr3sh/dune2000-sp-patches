typedef enum eValueOperation
{
  VALUEOPERATION_SET,
  VALUEOPERATION_ADD,
  VALUEOPERATION_SUBSTRACT,
  VALUEOPERATION_MULTIPLY,
  VALUEOPERATION_MULPERCENT,
  VALUEOPERATION_DIVIDE,
  VALUEOPERATION_MODULO,
  VALUEOPERATION_CAPMAX,
  VALUEOPERATION_CAPMIN,
  VALUEOPERATION_BITAND,
  VALUEOPERATION_BITOR,
  VALUEOPERATION_BITXOR,
  VALUEOPERATION_BITLSHIFT,
  VALUEOPERATION_BITRSHIFT,
  VALUEOPERATION_SETRANDOM,
  VALUEOPERATION_ADDRANDOM,
  VALUEOPERATION_SUBRANDOM,
  VALUEOPERATION_MULRANDOM,
  VALUEOPERATION_ADDRANDOMPERCENT,
  VALUEOPERATION_SUBRANDOMPERCENT
} eValueOperation;

typedef enum eFlagOperation
{
  FLAGOPERATION_SET,
  FLAGOPERATION_UNSET,
  FLAGOPERATION_TOGGLE
} eFlagOperation;

typedef enum eDataType
{
  DATATYPE_BYTE,
  DATATYPE_WORD,
  DATATYPE_DWORD,
  DATATYPE_FLOAT
} eDataType;

int ValueOperation(int val1, int val2, eValueOperation operation);
float ValueOperationFloat(float val1, float val2, eValueOperation operation);
int FlagOperation(int value, int flag, eFlagOperation operation);
void SetDataValue(char *data_ptr, eDataType data_type, int offset, eValueOperation operation, int value);
int GetDataValue(char *data_ptr, eDataType data_type, int offset);
bool CompareValue(int val, int comp_val, bool comparison);
bool CompareValueFloat(float val, float comp_val, bool comparison);
bool CompareDataValue(char *data_ptr, eDataType data_type, int offset, int comp_val, bool comparison);
bool CompareDistance(int x1, int y1, int x2, int y2, int comp_val, bool comparison);

void RestoreUnitSelection(int side_id, bool restore_selection);
void RestoreBuildingSelection(int side_id, bool restore_selection);

void ShowDataOnScreen(char *header, unsigned char *data_ptr);
