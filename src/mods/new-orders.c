#include "dune2000.h"

// Implement factory rally points
void GenerateBuildingSetRallyPointOrder(unsigned char side_id, int operation, int x, int y)
{
  Building *building;
  Orderdata order;

  order.__ObjectsServed1 = gGameTicks;
  order.OrderType = eOrderType_42_BUILDINGSETRALLYPOINT;
  order.SideId = side_id;
  for ( building = GetSide(side_id)->__FirstBuildingPtr; building; building = building->Next )
  {
    if ( building->__IsSelected && building->__State != BLD_STATE_17_DEAD )
    {
      order.ObjectArray[0] = building->MyIndex;
      order.BlockToX = x;
      order.BlockToY = y;
      order.field_25 = operation;
      order.NumObjects = 1;
    }
  }
  if ( order.NumObjects )
  {
    Orderdata_add(&order);
  }
}
