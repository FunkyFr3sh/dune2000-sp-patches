#include <windows.h>
#include "dune2000.h"
#include "macros/patch.h"

// Fix crash when AI tries to place different building type from what is actually built
// That can happen when human player overrides building initiated by AI
// Used solution is to cancel the built building and start building correct building again

// Replace call to CAI_PlaceBuiltBuilding with custom function with additional logic
CALL(0x0041ECEE, _CAI_PlaceBuiltBuildingExt); // w_CAI::RebuildBuildings

void __thiscall CAI_PlaceBuiltBuildingExt(CAI_ *this)
{
  CSide *side = GetSide(this->AISide);
  if (
      this->__BuildingTobuildConcreteNeededBitfield && // Concrete is supposed to be built
      side->__BuildingBuildQueue.__type != side->__BuildingIcons[0] // The building being built is not concrete
      )
  {
    GenerateBuildBuildingCancelOrder(this->AISide, side->__BuildingBuildQueue.__type);
    GenerateBuildBuildingPickOrder(this->AISide, side->__BuildingIcons[0]);
    return;
  }
  if (
      !this->__BuildingTobuildConcreteNeededBitfield && // A building is supposed to be built
      side->__BuildingBuildQueue.__type != this->__BuildingToBuildType // The building being built is not what AI plans to build
      )
  {
    GenerateBuildBuildingCancelOrder(this->AISide, side->__BuildingBuildQueue.__type);
    GenerateBuildBuildingPickOrder(this->AISide, this->__BuildingToBuildType);
    return;
  }
  CAI_PlaceBuiltBuilding(this);
}
