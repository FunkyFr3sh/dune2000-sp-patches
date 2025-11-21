#include "macros/patch.h"
#include "dune2000.h"

// Custom implementation of function DoCash
DETOUR(0x0044ABE0, 0x0044AC94, _Mod__DoCash);

void Mod__DoCash()
{
  unsigned int side_id; // edi
  CSide *side; // esi
  Building *bld; // ecx
  unsigned int spice_real; // edx
  unsigned int max_storage; // ecx
  int spice_drip; // eax
  unsigned char building_type; // [esp+10h] [ebp-4h]

  side_id = 0;
  do
  {
    side = GetSide(side_id);
    CSide__UpdateCashDrip(side);
    bld = side->__FirstBuildingPtr;
    for ( side->__MaxStorage = 0; bld; bld = bld->Next )
    {
      // New logic start
      // Customizable refinery/silo storage capacity
      building_type = bld->Type;
      BuildingAtrbStruct *building_template = &_templates_buildattribs[building_type];
      if ( building_template->__Behavior == BuildingBehavior_REFINERY )
      {
        side->__MaxStorage += (building_template->StorageCapacity?building_template->StorageCapacity:2000);
      }
      if ( building_template->__Behavior == BuildingBehavior_SILO )
      {
        side->__MaxStorage += (building_template->StorageCapacity?building_template->StorageCapacity:1500);
      }
      // New logic end
    }
    spice_real = side->SpiceReal;
    max_storage = side->__MaxStorage;
    if ( spice_real > max_storage )
    {
      spice_drip = side->SpiceDrip;
      if ( spice_drip < 0 )
      {
        side->SpiceDrip = spice_real + spice_drip - max_storage;
      }
      else
      {
        side->SpiceDrip = 0;
      }
      side->SpiceReal = max_storage;
    }
    ++side_id;
  }
  while ( side_id < 8 );
}
