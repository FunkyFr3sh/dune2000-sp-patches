#include "dune2000.h"

bool PlaceStaticCrate(int special_value, int x, int y)
{
    switch (special_value)
    {
        case 6:
        {
            PlaceCrate(x, y, INT32_MAX, CT_CASH, CI_BLUE_CRATE, 0);
            break;
        }
        case 7:
        {
            PlaceCrate(x, y, INT32_MAX, CT_EXPLODE, CI_BLUE_CRATE, 0);
            break;
        }
        case 9:
        {
            PlaceCrate(x, y, INT32_MAX, CT_REVEAL, CI_RED_CRATE, 0);
            break;
        }
        case 10:
        {
            PlaceCrate(x, y, INT32_MAX, CT_NOMAP, CI_RED_CRATE, 0);
            break;
        }
        case 12:
        {
            PlaceCrate(x, y, INT32_MAX, CT_UNIT, CI_GREEN_CRATE, 0);
            break;
        }
        case 13:
        {
            PlaceCrate(x, y, INT32_MAX, CT_STEALTH, CI_PURPLE_CRATE, 0);
            break;
        }
        case 15:
        {
            PlaceCrate(x, y, INT32_MAX, CT_UNSUPPORTED6, CI_PURPLE_CRATE, 0);
            break;
        }
        case 16:
        {
            // Invisible mine
            PlaceCrate(x, y, INT32_MAX, CT_EXPLODE, CI_INVISIBLE, 0);
            break;
        }        
        case 46:
        {
            // Static one-time spice bloom
            PlaceCrate(x, y, INT32_MAX, CT_SPICEBLOOM3, CI_LARGE_BLOOM, 0);
            break;
        }
        default:
        {
            return false;
        }
    }
    return true;
}
