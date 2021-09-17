#include "macros/patch.h"
#include "dune2000.h"
#include "rules.h"

// Custom implementation of function CSide_add_cash_drip
// If rule returnCreditsToSpiceStorage is set, the credits which are returned from cancelled builds, starport orders, sold buildings and cash crate
// are stored into spice storage with priority. If not enough storage capacity, remaining credits are given as non-spice cash.
LJMP(0x0046C7B0, _Mod__CSide_add_cash_drip);

void __thiscall Mod__CSide_add_cash_drip(CSide *this, int amount)
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
