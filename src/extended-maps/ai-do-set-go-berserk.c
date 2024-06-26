#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

// 1. If a Practice AI went berserk or started without buildings/units and later gets a MCV, it will not go berserk and build base normally
// 2. Add new AI property AutoBerserkMode

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
    // New logic start
    // Manage AutoBerserkMode AI property
    // 0 = Enabled only for practice AI
    // 1 = Disabled
    // 2 = Enabled also for non-practice AI
    if ( (ai->EnablePractice && ai->AutoBerserkMode == 0) || (ai->AutoBerserkMode == 2) )
    // New logic end
    {
      if ( !ai->__GoBeserk_OtherStates )
      {

        // New logic start
        // If side started with no buildings or units, do not set berserk mode at all
        if (side->__UnitsBuilt == 0 && side->__BuildingsBuilt == 0)
        {
          return;
        }
        // New logic end
        
        if ( !side->__BuildingsExistPerGroup[(unsigned __int8)_templates_GroupIDs.Refinery] )
        {
          no_refineries = 1;
        }
        if ( !side->__HarvesterCount )
        {
          no_harvesters = 1;
        }
        if ( side->__PrimaryConyard == -1 && !side->__UnitsExistPerType[(unsigned __int8)_templates_GroupIDs.MCV] )
        {
          no_conyard = 1;
        }
        if ( side->__PrimaryStarport != -1
          || side->__PrimaryLightFactory != -1
          || side->__PrimaryHeavyFactory != -1
          || side->__PrimaryBarracks != -1
          || side->__PrimaryHarkPalace != -1
          || side->__PrimaryOrdPalace != -1
          || (no_production_building = 1, side->__PrimaryAtrPalace != -1) )
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
        if (side->__UnitsExistPerType[(unsigned __int8)_templates_GroupIDs.MCV])
        {
          ai->__GoBeserk_OtherStates = 0;
        }
        // New logic end
      }
    }
  }
}
