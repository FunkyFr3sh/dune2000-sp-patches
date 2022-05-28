#include <stdio.h>
#include "dune2000.h"
#include "utils.h"
#include "event-utils.h"
#include "../extended-maps/messages-func.h"

int ValueOperation(int val1, int val2, eValueOperation operation)
{
  switch (operation)
  {
    case VALUEOPERATION_SET:        return val2; break;
    case VALUEOPERATION_ADD:        return val1 + val2; break;
    case VALUEOPERATION_SUBSTRACT:  return val1 - val2; break;
    case VALUEOPERATION_MULPERCENT: return (val1 * val2) / 100; break;
    case VALUEOPERATION_MODULO:     if (!val2) DebugFatal("event-utils.c", "Division by zero!"); return val1 % val2; break;
    case VALUEOPERATION_CAPMAX:     return HLIMIT(val1, val2); break;
    case VALUEOPERATION_CAPMIN:     return LLIMIT(val1, val2); break;
    case VALUEOPERATION_BITAND:     return val1 & val2; break;
    case VALUEOPERATION_BITOR:      return val1 | val2; break;
    case VALUEOPERATION_BITXOR:     return val1 ^ val2; break;
    case VALUEOPERATION_BITLSHIFT:  return val1 << val2; break;
    case VALUEOPERATION_BITRSHIFT:  return val1 >> val2; break;
    case VALUEOPERATION_SETRANDOM:  return rand()%val2; break;
    case VALUEOPERATION_ADDRANDOM:  return val1 + rand()%val2; break;
    case VALUEOPERATION_SUBRANDOM:  return val1 - rand()%val2; break;
    case VALUEOPERATION_MULRANDOM:  return val1 * (rand()%val2); break;
    case VALUEOPERATION_ADDRANDOMPERCENT:  return val1 + rand()%((val1 * val2) / 100); break;
    case VALUEOPERATION_SUBRANDOMPERCENT:  return val1 - rand()%((val1 * val2) / 100); break;
    default: return val1;
  }
}

float ValueOperationFloat(float val1, float val2, eValueOperation operation)
{
  switch (operation)
  {
    case VALUEOPERATION_SET:        return val2; break;
    case VALUEOPERATION_ADD:        return val1 + val2; break;
    case VALUEOPERATION_SUBSTRACT:  return val1 - val2; break;
    case VALUEOPERATION_MULPERCENT: return (val1 * val2) / 100; break;
    case VALUEOPERATION_CAPMAX:     return HLIMIT(val1, val2); break;
    case VALUEOPERATION_CAPMIN:     return LLIMIT(val1, val2); break;
    default: DebugFatal("event-utils.c", "Invalid operation %d for floating-point value.", operation);
  }
  return 0;
}

int FlagOperation(int value, int flag, eFlagOperation operation)
{
  switch (operation)
  {
    case FLAGOPERATION_SET:     return value | (1 << flag); break;
    case FLAGOPERATION_UNSET:   return value & ~(1 << flag); break;
    case FLAGOPERATION_TOGGLE:  return value ^ (1 << flag); break;
    default: return value;
  }
}

void SetDataValue(char *data_ptr, eDataType data_type, int offset, eValueOperation operation, int value)
{
  switch (data_type)
  {
    case DATATYPE_BYTE:
    {
      int8_t *p = (int8_t *)&data_ptr[offset];
      *p = ValueOperation(*p, value, operation);
      break;
    }
    case DATATYPE_WORD:
    {
      int16_t *p = (int16_t *)&data_ptr[offset];
      *p = ValueOperation(*p, value, operation);
      break;
    }
    case DATATYPE_DWORD:
    {
      int32_t *p = (int32_t *)&data_ptr[offset];
      *p = ValueOperation(*p, value, operation);
      break;
    }
    case DATATYPE_FLOAT:
    {
      float *p = (float *)&data_ptr[offset];
      float *fvalptr = (float *)&value;
      *p = ValueOperationFloat(*p, *fvalptr, operation);
      break;
    }
  }
}

int GetDataValue(char *data_ptr, eDataType data_type, int offset)
{
  switch (data_type)
  {
    case DATATYPE_BYTE:
    {
      int8_t *p = (int8_t *)&data_ptr[offset];
      return *p;
    }
    case DATATYPE_WORD:
    {
      int16_t *p = (int16_t *)&data_ptr[offset];
      return *p;
    }
    case DATATYPE_DWORD:
    case DATATYPE_FLOAT:
    {
      int32_t *p = (int32_t *)&data_ptr[offset];
      return *p;
    }
  }
  return 0;
}

bool CompareValue(int val, int comp_val, bool comparison)
{
  if (comparison)
    return val >= comp_val;
  else
    return val == comp_val;
}

bool CompareValueFloat(float val, float comp_val, bool comparison)
{
  if (comparison)
    return val >= comp_val;
  else
    return val == comp_val;
}

bool CompareDataValue(char *data_ptr, eDataType data_type, int offset, int comp_val, bool comparison)
{
  int value = GetDataValue(data_ptr, data_type, offset);
  if (data_type == DATATYPE_FLOAT)
  {
    float *fvalptr = (float *)&value;
    float *fcompvalptr = (float *)&comp_val;
    return CompareValueFloat(*fvalptr, *fcompvalptr, comparison);
  }
  return CompareValue(value, comp_val, comparison);
}

bool CompareDistance(int x1, int y1, int x2, int y2, int comp_val, bool comparison)
{
  int xdist = abs(x1 - x2);
  int ydist = abs(y1 - y2);
  int val = xdist * xdist + ydist * ydist;
  return CompareValue(val, comp_val * comp_val, comparison);
}

void RestoreUnitSelection(int side_id, bool restore_selection)
{
  if (!restore_selection)
    return;
  CSide *side = GetSide(side_id);
  for (Unit *unit = side->__FirstUnitPtr; unit; unit = unit->Next)
  {
    unit->__IsSelected = unit->PrevWasSelected;
    unit->PrevWasSelected = 0;
  }
}

void RestoreBuildingSelection(int side_id, bool restore_selection)
{
  if (!restore_selection)
    return;
  CSide *side = GetSide(side_id);
  for (Building *building = side->__FirstBuildingPtr; building; building = building->Next)
  {
    building->__IsSelected = building->PrevWasSelected;
    building->PrevWasSelected = 0;
  }
}

void ShowDataOnScreen(char *header, unsigned char *data_ptr)
{
  char buf[4][128];
  memset(buf, 0, sizeof(buf));
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      sprintf(&buf[j][2 * i + (i / 4) + (i / 16)], "%02X", data_ptr[i + j * 32]);
      if (i >= 4)
        buf[j][9 * (i / 4) + (i / 16) - 1] = ' ';
      if (i >= 16)
        buf[j][37 * (i / 16) - 2] = ' ';
    }
  }
  for (int i = 3; i >= 0; i--)
    QueueMessageExt(buf[i], 1, 251 + i, 0, 0, 0, 0, 0);
  QueueMessageExt(header, 1, 250, 0, 0, 0, 0, 0);
}
