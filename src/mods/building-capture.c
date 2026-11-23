#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function CaptureBuilding
DETOUR(0x0046D6D0, 0x0046D99B, _Mod__CaptureBuilding);

char Mod__CaptureBuilding(eSideType source_side_id, eSideType target_side_id, unsigned short source_building_index)
{
  CSide *source_side; // ebx MAPDST
  Building *source_building; // ebp
  int target_building_index; // eax
  Building *target_building; // ebx
  char is_selected; // al
  BuildingBehaviorType behavior; // al
  int credits; // esi
  int building_flags; // eax
  Unit *unit; // esi
  int *units_lost_per_type_ptr; // eax
  Unit *new_unit; // eax
  CSide *target_side; // [esp+10h] [ebp-10h]
  short target_building_index_; // [esp+18h] [ebp-8h]

  source_side = GetSide(source_side_id);
  target_side = GetSide(target_side_id);
  if ( source_building_index >= 1000u )
  {
    DebugFatal("SIDE.CPP", "Index out of range (3) ");
  }
  source_building = (Building *)&source_side->__ObjectArray[source_building_index];
  if ( source_side->__ObjectArray[source_building_index].__GroupID & 0xC )
  {
    return 0;
  }
  RemoveBuildingStuff(
    37 * source_building_index,
    source_side->__ObjectArray[source_building_index].Speed / 0x10000 / 32,
    (source_side->__ObjectArray[source_building_index].Health / 0x10000
   - _templates_buildattribs[LOBYTE(source_side->__ObjectArray[source_building_index].__PosX)]._____ArtHeight)
  / 32,
    source_side_id);
  target_building_index = (unsigned short)ModelAddBuilding(
                                              target_side_id,
                                              source_side->__ObjectArray[source_building_index].__PosX,
                                              source_side->__ObjectArray[source_building_index].Speed / 0x10000 / 32,
                                              (source_side->__ObjectArray[source_building_index].Health / 0x10000
                                             - _templates_buildattribs[LOBYTE(source_side->__ObjectArray[source_building_index].__PosX)]._____ArtHeight)
                                            / 32,
                                              0,
                                              1,
                                              1);
  target_building_index_ = target_building_index;
  if ( target_building_index == 0xFFFF )
  {
    return 0;
  }
  target_building = (Building *)&target_side->__ObjectArray[target_building_index];
  memcpy(target_building, source_building, 0x7Cu);
  target_building->__MyIndex = target_building_index;
  is_selected = ((Building *)&target_side->__ObjectArray[target_building_index])->__IsSelected;
  target_building->__MySide = target_side_id;
  target_building->__AttackerSide = -1;
  target_building->__AttackerIndex = -1;
  // New logic start
  // Reset rally points
  target_building->RallyPointEnabled = 0;
  target_building->RallyPointX = 0;
  target_building->RallyPointY = 0;
  // New logic end
  if ( is_selected == 1 )
  {
    DeselectAllForAllSides();
    target_building->__IsSelected = 1;
  }
  DoCash();
  behavior = _templates_buildattribs[source_building->Type].__Behavior;
  if ( behavior == BuildingBehavior_REFINERY || behavior == BuildingBehavior_SILO )
  {
    // New logic start
    // Capturing refinery/silo will give you proper amount of credits according to actual building capacity
    int capacity = _templates_buildattribs[source_building->Type].StorageCapacity;
    if (capacity == 0)
      capacity = behavior == BuildingBehavior_REFINERY?2000:1500;
    credits = (unsigned int)(capacity * source_side->SpiceReal) / source_side->__MaxStorage;
    // New logic end
    CSide__AddSpice(target_side, credits);
    CSide__SpendMoneyIfSufficient(source_side, credits, 0);
  }
  source_building->__DeadStateTimeCounter = 1;
  SetBuildingState(source_building, BLD_STATE_17_DEAD);
  source_building->Flags |= BFLAGS_100000_INVISIBLE|BFLAGS_10000_DONT_REMOVE_STUFF;
  building_flags = target_building->Flags;
  if ( (building_flags & BFLAGS_80_PRIMARY) != 0 )
  {
    building_flags = building_flags & ~(BFLAGS_80_PRIMARY|BFLAGS_10_ANIM_PERMANENT);
    target_building->Flags = building_flags;
  }
  target_building->Flags = (target_building->Flags & ~(BFLAGS_1000000_INFILTRATED|BFLAGS_1_REPAIRING)) | BFLAGS_2000000_WAS_CAPTURED;
  for ( unit = source_side->__FirstUnitPtr; unit; unit = unit->Next )
  {
    if ( _templates_unitattribs[unit->Type].__Behavior == UnitBehavior_HARVESTER
      && unit->State == UNIT_STATE_15_UNLOADING_SPICE
      && (unsigned short)unit->RefineryIndex == source_building_index )
    {
      source_side = GetSide(source_side_id);
      ++source_side->__UnitsLost;
      units_lost_per_type_ptr = &GetSide(source_side_id)->__UnitsLostPerType[unit->Type];
      ++*units_lost_per_type_ptr;
      new_unit = ChangeUnitOwner(source_side_id, target_side_id, unit->MyIndex, 1);
      if ( new_unit )
      {
        new_unit->RefineryIndex = target_building_index_;
      }
    }
  }
  SetBuildingState(target_building, BLD_STATE_2_IDLE);
  return 1;
}
