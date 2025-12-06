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

int GetQueueNumber(int unit_type)
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
      ((IsUnitBuilt(side_id, unit_type) || gSideExtraData[side_id].build_queues[GetQueueNumber(unit_type)].pending_ordered_unit_type == unit_type)?1:0);
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
      q->back = -1;
      q->pending_ordered_unit_type = -1;
      q->last_built_unit_type = -1;
    }
    for (int i = 0; i < MAX_UNIT_TYPES; i++)
      sideext->build_queue_unit_type_count[i] = 0;
  }
}

void AddToBuildQueue(int side_id, int unit_type, bool bulk_increment, bool priority)
{
  // Get the queue
  int queue_num = GetQueueNumber(unit_type);
  if (queue_num == -1)
    return;
  BuildQueue *q = &gSideExtraData[side_id].build_queues[queue_num];
  // Add entries into queue
  int amount = bulk_increment?rulesExt__buildQueuesBulkIncrement:1;
  int search_pos = 0;
  for (int i = 0; i < amount; i++)
  {
    // Check if queue is full
    int queued_count = GetUnitsQueuedCount(side_id, unit_type);
    if (q->entry_count + 1 == rulesExt__buildQueuesMaxPerFactory || queued_count == rulesExt__buildQueuesMaxPerUnitType)
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
      if (q->back == -1)
        q->back = search_pos;
    }
    else
    {
      // Add to back
      if (q->front == -1)
        q->front = search_pos; // Add first entry into empty queue
      else
        q->entries[(int)q->back].next = search_pos; // Add entry to back of queue
      q->back = search_pos;
    }
    q->entries[search_pos].unit_type = unit_type;
    gSideExtraData[side_id].build_queue_unit_type_count[unit_type]++;
    q->entry_count++;
  }
}

void RemoveFromBuildQueue(int side_id, int unit_type, bool bulk_increment)
{
  // Get the queue
  int queue_num = GetQueueNumber(unit_type);
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
      q->entry_count--;
      if (q->entry_count == 0)
        q->back = -1;
      amount--;
    }
    else
    {
      prev_ptr = &q->entries[pos].next;
    }
    pos = next;
  }
  // Fix pointer to last entry
  pos = q->front;
  while (pos != -1)
  {
    q->back = pos;
    pos = q->entries[pos].next;
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
    // Process all build queues
    for (int i = 0; i < NUM_BUILD_QUEUES; i++)
    {
      BuildQueue *q = &gSideExtraData[side_id].build_queues[i];
      // Remove all unit types that cannot be built from queue
      bool anything_removed = false;
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
          if (q->entry_count == 0)
            q->back = -1;
          anything_removed = true;
        }
        else
        {
          prev_ptr = &q->entries[pos].next;
        }
        pos = next;
      }
      // Fix pointer to last entry
      if (anything_removed)
      {
        pos = q->front;
        while (pos != -1)
        {
          q->back = pos;
          pos = q->entries[pos].next;
        }
      }
      // Wait for pending ordered unit to be started built
      if (q->pending_ordered_unit_type != -1 && !IsUnitBuilt(side_id, q->pending_ordered_unit_type))
        continue;
      q->pending_ordered_unit_type = -1;
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
          int queued_count = gSideExtraData[side_id].build_queue_unit_type_count[unit_type];
          int total_count = q->entry_count;
          gSideExtraData[side_id].build_queue_unit_type_count[unit_type]--;
          q->entry_count--;
          if (q->entry_count == 0)
            q->back = -1;
          // If infinity is enabled and maximum units of this type were queued, requeue it
          if (rulesExt__buildQueuesInfinityEnabled)
          {
            if (queued_count + ((q->last_built_unit_type == unit_type)?1:0) == rulesExt__buildQueuesMaxPerUnitType)
            {
              AddToBuildQueue(side_id, unit_type, false, false);
            }
            if ((q->last_built_unit_type != -1 && q->last_built_unit_type != unit_type && gSideExtraData[side_id].build_queue_unit_type_count[(int)q->last_built_unit_type] + 1 == rulesExt__buildQueuesMaxPerUnitType) || (total_count + 1 == rulesExt__buildQueuesMaxPerFactory))
            {
              AddToBuildQueue(side_id, q->last_built_unit_type, false, false);
            }
          }
          q->last_built_unit_type = unit_type;
        }
      }
      else
      {
        if (!IsUnitBuilt(side_id, q->last_built_unit_type))
          q->last_built_unit_type = -1;
      }
    }
  }
}

// Extension wrapper for function BlitUIIcons
CALL(0x004491B0, _Ext__BlitUIIcons); // GameLoop

void Ext__BlitUIIcons(TImage *img, int strip_bld_scroll_pos, int strip_bld_scroll_pos_prev, int strip_unit_scroll_pos, int strip_unit_scroll_pos_prev, bool full_redraw)
{
  if (rulesExt__buildQueuesEnabled)
    ProcessBuildQueues();

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
          if (rulesExt__buildQueuesInfinityEnabled && (queued_count == rulesExt__buildQueuesMaxPerUnitType || gSideExtraData[gSideId].build_queues[GetQueueNumber(unit_type)].entry_count + 1 == rulesExt__buildQueuesMaxPerFactory))
            sprintf(s, "+");
          else
            sprintf(s, "%d", queued_count);
          BlitFontChars(gBackBuf, s, SideBarPanelRightUIPosX + 6, SideBarPanelsPosY + i * SideBarIconHeight + 6, 0, 0x0, -1);
          BlitFontChars(gBackBuf, s, SideBarPanelRightUIPosX + 5, SideBarPanelsPosY + i * SideBarIconHeight + 5, 0, 0xFFFFFFFF, -1);
        }
      }
    }
  }
}
