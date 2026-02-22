#include <dune2000.h>
#include "macros/patch.h"
#include "rules.h"

bool IsUnitOnHold(int side_id, int unit_type)
{
  CSide *side = GetSide(side_id);
  for (int i = 0; i < 10; i++)
    if (side->__UnitBuildQueue[i].__type == unit_type)
      return side->__UnitBuildQueue[i].__on_hold;
  return false;
}

bool IsUnitBuilt(int side_id, int unit_type)
{
  CSide *side = GetSide(side_id);
  for (int i = 0; i < 10; i++)
    if (side->__UnitBuildQueue[i].__type == unit_type)
      return true;
  return false;
}

int GetBuildQueueNumber(int unit_type)
{
  int prereq = _templates_unitattribs[unit_type].__PreReq1;
  if (prereq == _templates_GroupIDs.Barracks)
    return 0;
  else if (prereq == _templates_GroupIDs.Wor)
    return 1;
  else if (prereq == _templates_GroupIDs.LightFactory)
    return 2;
  else if (prereq == _templates_GroupIDs.HeavyFactory)
    return 3;
  else if (prereq == _templates_GroupIDs.HighTech || prereq == _templates_GroupIDs.AtHighTech)
    return 4;
  return -1;
}

int GetUnitsQueuedCount(int side_id, int unit_type)
{
  return gSideExtraData[side_id].build_queue_unit_type_count[unit_type] +
      ((IsUnitBuilt(side_id, unit_type) || gSideExtraData[side_id].build_queues[GetBuildQueueNumber(unit_type)].pending_ordered_unit_type == unit_type)?1:0);
}

int GetFactoryQueuedCount(int side_id, int queue_num)
{
  int in_queue = gSideExtraData[side_id].build_queues[queue_num].entry_count;
  int built = 0;
  if (gSideExtraData[side_id].build_queues[queue_num].pending_ordered_unit_type != -1)
    built = 1;
  else
  {
    CSide *side = GetSide(side_id);
    for (int i = 0; i < 10; i++)
      if (GetBuildQueueNumber(side->__UnitBuildQueue[i].__type) == queue_num)
      {
        built = 1;
        break;
      }
  }
  return in_queue + built;
}

void InitBuildQueues(void)
{
  if (rulesExt__buildQueuesMaxPerFactory > MAX_BUILD_QUEUE_ENTRIES || rulesExt__buildQueuesMaxPerUnitType > MAX_BUILD_QUEUE_ENTRIES)
    DebugFatal("build-queue.c", "Maximum build queue capacity is %d", MAX_BUILD_QUEUE_ENTRIES);
  for (int side_id = 0; side_id < MAX_SIDES; side_id++)
  {
    SideExtraData *sideext = &gSideExtraData[side_id];
    for (int i = 0; i < NUM_BUILD_QUEUES; i++)
    {
      BuildQueue *q = &sideext->build_queues[i];
      for (int j = 0; j < MAX_BUILD_QUEUE_ENTRIES; j++)
      {
        q->entries[j].unit_type = -1;
        q->entries[j].next = -1;
      }
      q->entry_count = 0;
      q->front = -1;
      q->pending_ordered_unit_type = -1;
      q->last_built_unit_type = -1;
    }
    for (int i = 0; i < MAX_UNIT_TYPES; i++)
    {
      sideext->build_queue_unit_type_count[i] = 0;
      sideext->build_queue_unit_type_infinity[i] = false;
    }
  }
}

void AddToBuildQueue(int side_id, int unit_type, bool bulk_increment, bool priority)
{
  // Get the queue
  int queue_num = GetBuildQueueNumber(unit_type);
  if (queue_num == -1)
    return;
  BuildQueue *q = &gSideExtraData[side_id].build_queues[queue_num];
  // Add entries into queue
  int amount = bulk_increment?rulesExt__buildQueuesBulkIncrement:1;
  int search_pos = 0;
  for (int i = 0; i < amount; i++)
  {
    // Check if queue is full
    int units_queued_count = GetUnitsQueuedCount(side_id, unit_type);
    int factory_queued_count = GetFactoryQueuedCount(side_id, queue_num);
    if (factory_queued_count == rulesExt__buildQueuesMaxPerFactory || units_queued_count == rulesExt__buildQueuesMaxPerUnitType)
      break;
    // Find free entry
    while (q->entries[search_pos].unit_type != -1)
      search_pos++;
    // Add to queue
    if (priority)
    {
      // Add to front
      q->entries[search_pos].next = q->front;
      q->front = search_pos;
    }
    else
    {
      // Add to back
      char *next_ptr = &q->front;
      while (*next_ptr != -1)
        next_ptr = &q->entries[(int)*next_ptr].next;
      *next_ptr = search_pos;
    }
    q->entries[search_pos].unit_type = unit_type;
    gSideExtraData[side_id].build_queue_unit_type_count[unit_type]++;
    q->entry_count++;
  }
}

void RemoveFromBuildQueue(int side_id, int unit_type, bool bulk_increment)
{
  // Get the queue
  int queue_num = GetBuildQueueNumber(unit_type);
  if (queue_num == -1)
    return;
  BuildQueue *q = &gSideExtraData[side_id].build_queues[queue_num];
  // Remove entries from queue
  int amount = bulk_increment?rulesExt__buildQueuesBulkIncrement:1;
  char *prev_ptr = &q->front;
  int pos = q->front;
  while (pos != -1 && amount > 0)
  {
    int next = q->entries[pos].next;
    if (q->entries[pos].unit_type == unit_type)
    {
      *prev_ptr = q->entries[pos].next;
      q->entries[pos].unit_type = -1;
      q->entries[pos].next = -1;
      gSideExtraData[side_id].build_queue_unit_type_count[unit_type]--;
      if (gSideExtraData[side_id].build_queue_unit_type_count[unit_type] == 0 && !IsUnitBuilt(side_id, unit_type))
        gSideExtraData[side_id].build_queue_unit_type_infinity[unit_type] = false;
      q->entry_count--;
      amount--;
    }
    else
    {
      prev_ptr = &q->entries[pos].next;
    }
    pos = next;
  }
}

void ProcessBuildQueues(void)
{
  for (int side_id = 0; side_id < MAX_SIDES; side_id++)
  {
    // Check what unit types side can build
    bool can_build_unit_type[MAX_UNIT_TYPES] = {false};
    CSide *side = GetSide(side_id);
    for (unsigned int i = 0; i < side->__UnitIconCount; i++)
      if (side->__UnitIcons[i] != -1)
        can_build_unit_type[side->__UnitIcons[i]] = true;
    // Cancel infinity on unit types that cannot be built
    for (int i = 0; i < MAX_UNIT_TYPES; i++)
      if (!can_build_unit_type[i])
        gSideExtraData[side_id].build_queue_unit_type_infinity[i] = false;
    // Process all build queues
    for (int i = 0; i < NUM_BUILD_QUEUES; i++)
    {
      BuildQueue *q = &gSideExtraData[side_id].build_queues[i];
      // Clear last built unit type if that one cannot be built
      if (q->last_built_unit_type != -1 && !can_build_unit_type[(int)q->last_built_unit_type])
        q->last_built_unit_type = -1;
      if (q->pending_ordered_unit_type != -1 && !can_build_unit_type[(int)q->pending_ordered_unit_type])
        q->pending_ordered_unit_type = -1;
      // Remove all unit types that cannot be built from queue
      char *prev_ptr = &q->front;
      int pos = q->front;
      while (pos != -1)
      {
        int unit_type = q->entries[pos].unit_type;
        int next = q->entries[pos].next;
        if (!can_build_unit_type[unit_type])
        {
          // Remove entry from queue
          *prev_ptr = q->entries[pos].next;
          q->entries[pos].unit_type = -1;
          q->entries[pos].next = -1;
          gSideExtraData[side_id].build_queue_unit_type_count[unit_type]--;
          q->entry_count--;
        }
        else
        {
          prev_ptr = &q->entries[pos].next;
        }
        pos = next;
      }
      // Wait for pending ordered unit to be started built
      if (q->pending_ordered_unit_type != -1 && !IsUnitBuilt(side_id, q->pending_ordered_unit_type))
        continue;
      q->pending_ordered_unit_type = -1;
      // If infinity is enabled, requeue last built unit type
      if (q->last_built_unit_type != -1 && gSideExtraData[side_id].build_queue_unit_type_infinity[(int)q->last_built_unit_type] && CanUnitBeBuilt(side_id, q->last_built_unit_type, 1))
      {
        AddToBuildQueue(side_id, q->last_built_unit_type, false, false);
        q->last_built_unit_type = -1;
      }
      // Handle unit in front of queue
      if (q->front != -1)
      {
        int unit_type = q->entries[(int)q->front].unit_type;
        if (CanUnitBeBuilt(side_id, unit_type, 1))
        {
          // Generate build unit order
          GenerateBuildUnitPickOrder(side_id, unit_type);
          //ModelBuildUnitPick(side_id, unit_type);
          q->pending_ordered_unit_type = unit_type;
          // Remove entry from queue
          int front = q->front;
          q->front = q->entries[front].next;
          q->entries[front].unit_type = -1;
          q->entries[front].next = -1;
          gSideExtraData[side_id].build_queue_unit_type_count[unit_type]--;
          q->entry_count--;
          q->last_built_unit_type = unit_type;
        }
      }
    }
  }
}

// Extension wrapper for function BlitUIIcons
CALL(0x004491B0, _Ext__BlitUIIcons); // GameLoop

void Ext__BlitUIIcons(TImage *img, int strip_bld_scroll_pos, int strip_bld_scroll_pos_prev, int strip_unit_scroll_pos, int strip_unit_scroll_pos_prev, bool full_redraw)
{
  BlitUIIcons(img, strip_bld_scroll_pos, strip_bld_scroll_pos_prev, strip_unit_scroll_pos, strip_unit_scroll_pos_prev, full_redraw);

  // Draw number of queued units in unit icons
  if (rulesExt__buildQueuesEnabled)
  {
    CSide *side = GetSide(gSideId);
    for (int i = 0; i < SideBarIconCount; i++)
    {
      int unit_type = side->__UnitIcons[strip_unit_scroll_pos + i];
      if (unit_type != -1)
      {
        int queued_count = GetUnitsQueuedCount(gSideId, unit_type);
        if (queued_count > 0)
        {
          char s[8];
          sprintf(s, gSideExtraData[gSideId].build_queue_unit_type_infinity[unit_type]?"%d+":"%d", queued_count);
          BlitFontChars(gBackBuf, s, SideBarPanelRightUIPosX + 6, SideBarPanelsPosY + i * SideBarIconHeight + 6, 0, 0x0, -1);
          BlitFontChars(gBackBuf, s, SideBarPanelRightUIPosX + 5, SideBarPanelsPosY + i * SideBarIconHeight + 5, 0, 0xFFFFFFFF, -1);
        }
      }
    }
  }
}
