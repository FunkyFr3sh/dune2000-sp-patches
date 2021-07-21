#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

// If a Practice AI went berserk or started without buildings/units and later gets a MCV, it will not go berserk and build base normally

// Custom implementation of function CAI__DoSetGoBerserk
CALL(0x0041B73F, _Mod__CAI__DoSetGoBerserk);

void __thiscall Mod__CAI__DoSetGoBerserk(CAI_ *this)
{
  CAI_ *ai; // esi
  char no_conyard; // bl
  CSide *side; // eax
  char no_production_building; // cl
  unsigned int actual_cash; // edx
  char no_money; // al
  char no_refineries; // [esp+Ch] [ebp-4h]
  char no_harvesters; // [esp+Dh] [ebp-3h]

  ai = this;
  no_conyard = 0;
  no_refineries = 0;
  no_harvesters = 0;
  if ( (gGameTicks & 7) == this->AISide )
  {
    side = GetSide((eSideType)this->AISide);
    if ( ai->EnablePractice )
    {
      if ( !ai->__GoBeserk_OtherStates )
      {

        // New logic start
        // If side started with no buildings or units, do not set berserk mode at all
        if (side->__units_built == 0 && side->__buildings_built == 0)
        {
          return;
        }
        // New logic end
        
        if ( !side->building_a_field_245C1_buildingcount[(unsigned __int8)_templates_GroupIDs.Refinery] )
        {
          no_refineries = 1;
        }
        if ( !side->__HarvesterCount_245B8 )
        {
          no_harvesters = 1;
        }
        if ( side->__primary_conyard == -1 && !side->unit_count_ar_field_24628[(unsigned __int8)_templates_GroupIDs.MCV] )
        {
          no_conyard = 1;
        }
        if ( side->__primary_starport != -1
          || side->__primary_light_factory != -1
          || side->__primary_heavy_factory != -1
          || side->__primary_barracks != -1
          || side->__primary_palace != -1
          || side->__primary_at1_palace != -1
          || (no_production_building = 1, side->__primary_at2_palace != -1) )
        {
          no_production_building = 0;
        }
        actual_cash = side->CashReal + side->SpiceReal;
        no_money = 1;
        if ( actual_cash >= 400 )
        {
          no_money = 0;
        }
        if ( no_refineries )
        {
          if ( no_conyard )
          {
            if ( no_money )
            {
              ai->__GoBeserk_OtherStates = 1;
            }
          }
        }
        if ( no_production_building )
        {
          if ( no_conyard )
          {
            ai->__GoBeserk_OtherStates = 1;
          }
        }
        if ( no_money )
        {
          if ( no_harvesters || no_refineries )
          {
            ai->__GoBeserk_OtherStates = 1;
          }
        }
      }
      else
      {
        // New logic start
        // If side receives a MCV, berserk mode will turn off
        if (side->unit_count_ar_field_24628[(unsigned __int8)_templates_GroupIDs.MCV])
        {
          ai->__GoBeserk_OtherStates = 0;
        }
        // New logic end
      }
    }
  }
}
