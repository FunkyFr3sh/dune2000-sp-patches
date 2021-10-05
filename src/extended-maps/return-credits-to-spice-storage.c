#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"

// Function which replaces some of calls to function CSide_add_cash_drip
// If rule returnCreditsToSpiceStorage is set, then credits which are returned from cancelled builds and starport orders
// are stored into spice storage. If not enough storage capacity, remaining credits are given as non-spice cash.
// This does not apply to sold buildings and cash crates - those will always give non-spice cash.
CALL(0x00446BA2, _CSide_return_credits); // DoIconBuild
CALL(0x00455815, _CSide_return_credits); // ModelBuildUnitCancel
CALL(0x004561B3, _CSide_return_credits); // ModelBuildBuildingCancel
CALL(0x00457117, _CSide_return_credits); // ModelStarportUnpick
CALL(0x00457329, _CSide_return_credits); // ModelStarportCancel
CALL(0x004574F3, _CSide_return_credits); // ModelUpgradeCancel
CALL(0x0046C2F1, _CSide_return_credits); // CSide__update_list_of_available_buildings_and_units
CALL(0x0046C38D, _CSide_return_credits); // CSide__update_list_of_available_buildings_and_units
CALL(0x0046EAD4, _CSide_return_credits); // CSide_cash_46EAC0

void __thiscall CSide_return_credits(CSide *this, int amount)
{
  if (rulesExt__returnCreditsToSpiceStorage)
  {
    int free_spice_storage = this->__MaxStorage - (this->SpiceReal + this->SpiceDrip);
    if (amount <= free_spice_storage)
      this->SpiceDrip += amount;
    else
    {
      this->SpiceDrip += free_spice_storage;
      this->CashDrip += (amount - free_spice_storage);
    }
  }
  else
    this->CashDrip += amount; 
}
