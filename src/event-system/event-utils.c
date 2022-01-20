#include <stdio.h>
#include "dune2000.h"
#include "utils.h"
#include "event-utils.h"

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
    default: return val1;
  }
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

void SetDataValue(char *data_ptr, eDataSize data_size, int offset, eValueOperation operation, int value)
{
  switch (data_size)
  {
    case DATASIZE_BYTE:
    {
      int8_t *p = (int8_t *)&data_ptr[offset];
      *p = ValueOperation(*p, value, operation);
      break;
    }
    case DATASIZE_WORD:
    {
      int16_t *p = (int16_t *)&data_ptr[offset];
      *p = ValueOperation(*p, value, operation);
      break;
    }
    case DATASIZE_DWORD:
    {
      int32_t *p = (int32_t *)&data_ptr[offset];
      *p = ValueOperation(*p, value, operation);
      break;
    }
  }
}

bool CompareValue(int val, int comp_val, bool comparison)
{
  if (comparison)
    return val >= comp_val;
  else
    return val == comp_val;
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
    QueueMessage(buf[i], -1);
  QueueMessage(header, -1);
  for (int i = 0; i < 5; i++)
    _gMessageData.__ticks[i] = gGameTicks;
}
