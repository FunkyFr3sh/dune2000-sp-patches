typedef struct ObjectFilterStruct
{
  uint8_t pos_min_x;
  uint8_t pos_min_y;
  uint8_t pos_max_x;
  uint8_t pos_max_y;
  uint8_t pos_flags;
  uint8_t criteria_var_flags;
  uint16_t criteria_and_or;
  uint8_t criteria_type[8];
  uint8_t criteria_value[8];
} ObjectFilterStruct;

enum eObjectFilterPosFlags
{
  OBJFILTERPOSFLAG_DOCHECK = 1,
  OBJFILTERPOSFLAG_NEGATE = 2,
  OBJFILTERPOSFLAG_MINX_VAR = 4,
  OBJFILTERPOSFLAG_MINY_VAR = 8,
  OBJFILTERPOSFLAG_MAXX_VAR = 16,
  OBJFILTERPOSFLAG_MAXY_VAR = 32
};

typedef enum eUnitFilterCriteriaType
{
  UNITCRITERIATYPE_NONE,
  UNITCRITERIATYPE_TYPE,
  UNITCRITERIATYPE_GROUP,
  UNITCRITERIATYPE_BEHAVIOR,
  UNITCRITERIATYPE_CATEGORY,
  UNITCRITERIATYPE_TAG,
  UNITCRITERIATYPE_TYPE_OWNER,
  UNITCRITERIATYPE_ARMOR,
  UNITCRITERIATYPE_SPEED_TYPE,
  UNITCRITERIATYPE_PRI_WEAPON,
  UNITCRITERIATYPE_SEC_WEAPON,
  UNITCRITERIATYPE_SIGHT,
  UNITCRITERIATYPE_RANGE,
  UNITCRITERIATYPE_SPEED,
  UNITCRITERIATYPE_RATE_OF_FIRE,
  UNITCRITERIATYPE_HP100_MAX,
  UNITCRITERIATYPE_HP100_CUR,
  UNITCRITERIATYPE_HEALTH_PERCENT,
  UNITCRITERIATYPE_FLAG,
  UNITCRITERIATYPE_STATE,
  UNITCRITERIATYPE_GROUPNO,
  UNITCRITERIATYPE_TILE_ATTRIB,
  UNITCRITERIATYPE_TILE_TERRAIN,
  UNITCRITERIATYPE_TILE_BLD_TYPE,
  UNITCRITERIATYPE_TILE_BLD_GROUP,
  UNITCRITERIATYPE_TILE_BLD_TAG,
  UNITCRITERIATYPE_TILE_BLD_FLAG,
  UNITCRITERIATYPE_TILE_BLD_TILENO,
  UNITCRITERIATYPE_NEXT_XPOS,
  UNITCRITERIATYPE_NEXT_YPOS,
  UNITCRITERIATYPE_NEXT_ATTRIB,
  UNITCRITERIATYPE_NEXT_TERRAIN,
  UNITCRITERIATYPE_TARG_XPOS,
  UNITCRITERIATYPE_TARG_YPOS,
  UNITCRITERIATYPE_TARG_ATTRIB,
  UNITCRITERIATYPE_TARG_TERRAIN,
  UNITCRITERIATYPE_TARG_DIST,
  UNITCRITERIATYPE_ENEMY_SIDE,
  UNITCRITERIATYPE_ENEMY_TYPE,
  UNITCRITERIATYPE_ENEMY_GROUP,
  UNITCRITERIATYPE_ENEMY_CATEGORY,
  UNITCRITERIATYPE_ENEMY_TAG,
  UNITCRITERIATYPE_ENEMY_ARMOR,
  UNITCRITERIATYPE_ENEMY_DIST,
  UNITCRITERIATYPE_ENEMY_INDEX,
  UNITCRITERIATYPE_ATTACKER_SIDE,
  UNITCRITERIATYPE_ATTACKER_TYPE,
  UNITCRITERIATYPE_ATTACKER_GROUP,
  UNITCRITERIATYPE_ATTACKER_CATEGORY,
  UNITCRITERIATYPE_ATTACKER_TAG,
  UNITCRITERIATYPE_ATTACKER_WEAPON,
  UNITCRITERIATYPE_ATTACKER_DIST,
  UNITCRITERIATYPE_ATTACKER_INDEX,
  UNITCRITERIATYPE_FACING,
  UNITCRITERIATYPE_FACING_TURRET,
  UNITCRITERIATYPE_ANIM_FRAME,
  UNITCRITERIATYPE_SPEC_PURPOSE,
  UNITCRITERIATYPE_LAST_ATTACKED,
  UNITCRITERIATYPE_LAST_FIRED,
  UNITCRITERIATYPE_LAST_MOVED,
  UNITCRITERIATYPE_OLD_SIDE,
  UNITCRITERIATYPE_DEVIATED_TIME,
  UNITCRITERIATYPE_DEAD_STATE_TIME,
  UNITCRITERIATYPE_RELOAD_DELAY
} eUnitFilterCriteriaType;

typedef enum eUnitCategory
{
  UNITCATEGORY_INFANTRY,
  UNITCATEGORY_VEHICLE,
  UNITCATEGORY_LIGHT_VEH,
  UNITCATEGORY_HEAVY_VEH,
  UNITCATEGORY_SPECIAL,
  UNITCATEGORY_HAS_PRI_WEAPON,
  UNITCATEGORY_HAS_SEC_WEAPON,
  UNITCATEGORY_HAS_BARREL,
  UNITCATEGORY_CAN_BE_UPGRADED,
  UNITCATEGORY_MY_SIDE,
  UNITCATEGORY_ALLY_SIDE,
  UNITCATEGORY_ENEMY_SIDE,
  UNITCATEGORY_NEUTRAL_SIDE,
  UNITCATEGORY_IS_SELECTED,
  UNITCATEGORY_UNDER_CURSOR,
  UNITCATEGORY_CENTER_OF_TILE,
  UNITCATEGORY_REVEALED_TILE_FULL,
  UNITCATEGORY_REVEALED_TILE_PART,
  UNITCATEGORY_LYING,
  UNITCATEGORY_ENEMY_VALID,
  UNITCATEGORY_ENEMY_UNIT,
  UNITCATEGORY_ENEMY_BUILDING,
  UNITCATEGORY_ENEMY_MY_SIDE,
  UNITCATEGORY_ENEMY_ALLY_SIDE,
  UNITCATEGORY_ENEMY_ENEMY_SIDE,
  UNITCATEGORY_ENEMY_NEUTRAL_SIDE,
  UNITCATEGORY_ATTACKER_VALID,
  UNITCATEGORY_ATTACKER_UNIT,
  UNITCATEGORY_ATTACKER_BUILDING,
  UNITCATEGORY_ATTACKER_MY_SIDE,
  UNITCATEGORY_ATTACKER_ALLY_SIDE,
  UNITCATEGORY_ATTACKER_ENEMY_SIDE,
  UNITCATEGORY_ATTACKER_NEUTRAL_SIDE,
  UNITCATEGORY_OLD_SIDE_MY_SIDE,
  UNITCATEGORY_OLD_SIDE_ALLY,
  UNITCATEGORY_OLD_SIDE_ENEMY,
  UNITCATEGORY_OLD_SIDE_NEUTRAL,
  UNITCATEGORY_ROUTE_MODE_0,
  UNITCATEGORY_ROUTE_MODE_1,
  UNITCATEGORY_ROUTE_MODE_2,
  UNITCATEGORY_ROUTE_MODE_3
} eUnitCategory;

typedef enum eBuildingFilterCriteriaType
{
  BUILDINGCRITERIATYPE_NONE,
  BUILDINGCRITERIATYPE_TYPE,
  BUILDINGCRITERIATYPE_GROUP,
  BUILDINGCRITERIATYPE_BEHAVIOR,
  BUILDINGCRITERIATYPE_CATEGORY,
  BUILDINGCRITERIATYPE_TAG,
  BUILDINGCRITERIATYPE_TYPE_OWNER,
  BUILDINGCRITERIATYPE_ARMOR,
  BUILDINGCRITERIATYPE_PRI_WEAPON,
  BUILDINGCRITERIATYPE_SEC_WEAPON,
  BUILDINGCRITERIATYPE_SIGHT,
  BUILDINGCRITERIATYPE_POWER_CONS,
  BUILDINGCRITERIATYPE_POWER_PROD,
  BUILDINGCRITERIATYPE_RANGE,
  BUILDINGCRITERIATYPE_RATE_OF_FIRE,
  BUILDINGCRITERIATYPE_HP100_MAX,
  BUILDINGCRITERIATYPE_HP100_CUR,
  BUILDINGCRITERIATYPE_HEALTH_PERCENT,
  BUILDINGCRITERIATYPE_FLAG,
  BUILDINGCRITERIATYPE_STATE,
  BUILDINGCRITERIATYPE_UPGRADES,
  BUILDINGCRITERIATYPE_TILES_TOTAL,
  BUILDINGCRITERIATYPE_TILES_ON_CONC,
  BUILDINGCRITERIATYPE_TILES_ON_CONC_PERCENT,
  BUILDINGCRITERIATYPE_RESERVED1,
  BUILDINGCRITERIATYPE_RESERVED2,
  BUILDINGCRITERIATYPE_RESERVED3,
  BUILDINGCRITERIATYPE_RESERVED4,
  BUILDINGCRITERIATYPE_ENEMY_SIDE,
  BUILDINGCRITERIATYPE_ENEMY_TYPE,
  BUILDINGCRITERIATYPE_ENEMY_GROUP,
  BUILDINGCRITERIATYPE_ENEMY_CATEGORY,
  BUILDINGCRITERIATYPE_ENEMY_TAG,
  BUILDINGCRITERIATYPE_ENEMY_ARMOR,
  BUILDINGCRITERIATYPE_ENEMY_DIST,
  BUILDINGCRITERIATYPE_ENEMY_INDEX,
  BUILDINGCRITERIATYPE_ATTACKER_SIDE,
  BUILDINGCRITERIATYPE_ATTACKER_TYPE,
  BUILDINGCRITERIATYPE_ATTACKER_GROUP,
  BUILDINGCRITERIATYPE_ATTACKER_CATEGORY,
  BUILDINGCRITERIATYPE_ATTACKER_TAG,
  BUILDINGCRITERIATYPE_ATTACKER_WEAPON,
  BUILDINGCRITERIATYPE_ATTACKER_DIST,
  BUILDINGCRITERIATYPE_ATTACKER_INDEX,
  BUILDINGCRITERIATYPE_FACING,
  BUILDINGCRITERIATYPE_DIR_FRAME,
  BUILDINGCRITERIATYPE_BUILDUP_FRAME,
  BUILDINGCRITERIATYPE_ANIM_FRAME,
  BUILDINGCRITERIATYPE_SPEC_PURPOSE,
  BUILDINGCRITERIATYPE_LAST_ATTACKED,
  BUILDINGCRITERIATYPE_RESERVED5,
  BUILDINGCRITERIATYPE_DEAD_TIME,
  BUILDINGCRITERIATYPE_RELOAD_TIME,
  BUILDINGCRITERIATYPE_TURN_TIME,
  BUILDINGCRITERIATYPE_CUST_BYTE_1,
  BUILDINGCRITERIATYPE_CUST_BYTE_2,
  BUILDINGCRITERIATYPE_CUST_BYTE_3,
  BUILDINGCRITERIATYPE_CUST_BYTE_4,
  BUILDINGCRITERIATYPE_CUST_WORD_1,
  BUILDINGCRITERIATYPE_CUST_WORD_2,
  BUILDINGCRITERIATYPE_CUST_DWORD_1,
  BUILDINGCRITERIATYPE_CUST_DWORD_2,
  BUILDINGCRITERIATYPE_CUST_DWORD_3,
  BUILDINGCRITERIATYPE_CUST_DWORD_4
} eBuildingFilterCriteriaType;

typedef enum eBuildingCategory
{
  BUILDINGCATEGORY_POWER_CONS,
  BUILDINGCATEGORY_POWER_PROD,
  BUILDINGCATEGORY_REQ_ENOUGH_POW,
  BUILDINGCATEGORY_SHAKE_SCREEN,
  BUILDINGCATEGORY_UNUSED0,
  BUILDINGCATEGORY_HAS_PRI_WEAPON,
  BUILDINGCATEGORY_HAS_SEC_WEAPON,
  BUILDINGCATEGORY_HAS_BARREL,
  BUILDINGCATEGORY_CAN_BE_UPGRADED,
  BUILDINGCATEGORY_MY_SIDE,
  BUILDINGCATEGORY_ALLY_SIDE,
  BUILDINGCATEGORY_ENEMY_SIDE,
  BUILDINGCATEGORY_NEUTRAL_SIDE,
  BUILDINGCATEGORY_IS_SELECTED,
  BUILDINGCATEGORY_UNDER_CURSOR,
  BUILDINGCATEGORY_UNUSED1,
  BUILDINGCATEGORY_UNUSED2,
  BUILDINGCATEGORY_UNUSED3,
  BUILDINGCATEGORY_UNUSED4,
  BUILDINGCATEGORY_ENEMY_VALID,
  BUILDINGCATEGORY_ENEMY_UNIT,
  BUILDINGCATEGORY_ENEMY_BUILDING,
  BUILDINGCATEGORY_ENEMY_MY_SIDE,
  BUILDINGCATEGORY_ENEMY_ALLY_SIDE,
  BUILDINGCATEGORY_ENEMY_ENEMY_SIDE,
  BUILDINGCATEGORY_ENEMY_NEUTRAL_SIDE,
  BUILDINGCATEGORY_ATTACKER_VALID,
  BUILDINGCATEGORY_ATTACKER_UNIT,
  BUILDINGCATEGORY_ATTACKER_BUILDING,
  BUILDINGCATEGORY_ATTACKER_MY_SIDE,
  BUILDINGCATEGORY_ATTACKER_ALLY_SIDE,
  BUILDINGCATEGORY_ATTACKER_ENEMY_SIDE,
  BUILDINGCATEGORY_ATTACKER_NEUTRAL_SIDE
} eBuildingCategory;

typedef enum eBulletFilterCriteriaType
{
  BULLETCRITERIATYPE_NONE,
  BULLETCRITERIATYPE_TYPE,
  BULLETCRITERIATYPE_CATEGORY,
  BULLETCRITERIATYPE_TAG,
  BULLETCRITERIATYPE_DAMAGE,
  BULLETCRITERIATYPE_RANGE,
  BULLETCRITERIATYPE_SPEED,
  BULLETCRITERIATYPE_WARHEAD,
  BULLETCRITERIATYPE_HIT_EXPLOSION,
  BULLETCRITERIATYPE_TRAIL_EXPLOSION,
  BULLETCRITERIATYPE_FLAG,
  BULLETCRITERIATYPE_MOVE_STEPS,
  BULLETCRITERIATYPE_SPEED_X,
  BULLETCRITERIATYPE_SPEED_Y,
  BULLETCRITERIATYPE_FIRER_INDEX,
  BULLETCRITERIATYPE_HOMING_INDEX,
  BULLETCRITERIATYPE_HOMING_SIDE,
  BULLETCRITERIATYPE_TARGET_X,
  BULLETCRITERIATYPE_TARGET_Y,
  BULLETCRITERIATYPE_POS_Z,
  BULLETCRITERIATYPE_ANIM_FRAME,
  BULLETCRITERIATYPE_CUST_BYTE_1,
  BULLETCRITERIATYPE_CUST_BYTE_2,
  BULLETCRITERIATYPE_CUST_BYTE_3,
  BULLETCRITERIATYPE_CUST_BYTE_4,
  BULLETCRITERIATYPE_CUST_WORD_1,
  BULLETCRITERIATYPE_CUST_WORD_2,
  BULLETCRITERIATYPE_CUST_DWORD_1,
  BULLETCRITERIATYPE_CUST_DWORD_2,
  BULLETCRITERIATYPE_CUST_DWORD_3,
  BULLETCRITERIATYPE_CUST_DWORD_4
} eBulletFilterCriteriaType;

typedef enum eBulletCategory
{
  BULLETCATEGORY_ANTI_AIRCRAFT
} eBulletCategory;

typedef enum eExplosionFilterCriteriaType
{
  EXPLOSIONCRITERIATYPE_NONE,
  EXPLOSIONCRITERIATYPE_TYPE,
  EXPLOSIONCRITERIATYPE_TAG,
  EXPLOSIONCRITERIATYPE_FLAG,
  EXPLOSIONCRITERIATYPE_ANIM_FRAME,
  EXPLOSIONCRITERIATYPE_ANIM_DELAY,
  EXPLOSIONCRITERIATYPE_POS_Z,
  EXPLOSIONCRITERIATYPE_CUST_BYTE_1,
  EXPLOSIONCRITERIATYPE_CUST_BYTE_2,
  EXPLOSIONCRITERIATYPE_CUST_BYTE_3,
  EXPLOSIONCRITERIATYPE_CUST_BYTE_4,
  EXPLOSIONCRITERIATYPE_CUST_WORD_1,
  EXPLOSIONCRITERIATYPE_CUST_WORD_2,
  EXPLOSIONCRITERIATYPE_CUST_DWORD_1,
  EXPLOSIONCRITERIATYPE_CUST_DWORD_2,
  EXPLOSIONCRITERIATYPE_CUST_DWORD_3,
  EXPLOSIONCRITERIATYPE_CUST_DWORD_4
} eExplosionFilterCriteriaType;

typedef enum eCrateFilterCriteriaType
{
  CRATECRITERIATYPE_NONE,
  CRATECRITERIATYPE_TYPE,
  CRATECRITERIATYPE_IMAGE,
  CRATECRITERIATYPE_CATEGORY,
  CRATECRITERIATYPE_EXT_DATA,
  CRATECRITERIATYPE_EXT_DATA_BIT,
  CRATECRITERIATYPE_RESPAWN_TIMES,
  CRATECRITERIATYPE_TIME_TICKS,
  CRATECRITERIATYPE_TIME_SECS,
  CRATECRITERIATYPE_CUST_BYTE_1,
  CRATECRITERIATYPE_TILECHECK
} eCrateFilterCriteriaType;

typedef enum eCrateCategory
{
  CRATECATEGORY_UNDER_CURSOR,
  CRATECATEGORY_SPICE_BLOOM,
  CRATECATEGORY_SHOOTABLE,
  CRATECATEGORY_IGNORED_BY_AI
} eCrateCategory;

typedef enum eTileFilterCriteriaType
{
  TILECRITERIATYPE_NONE,
  TILECRITERIATYPE_IDX,
  TILECRITERIATYPE_COL,
  TILECRITERIATYPE_ROW,
  TILECRITERIATYPE_CATEGORY,
  TILECRITERIATYPE_DAMAGE,
  TILECRITERIATYPE_SHROUD,
  TILECRITERIATYPE_TILECHECK
} eTileFilterCriteriaType;

typedef enum eTileCategory
{
  TILECATEGORY_UNDER_CURSOR,
  TILECATEGORY_FULLY_HIDDEN,
  TILECATEGORY_HIDDEN,
  TILECATEGORY_PART_REVEALED,
  TILECATEGORY_REVEALED,
  TILECATEGORY_OVERRIDEN,
  TILECATEGORY_HAS_INFANTRY,
  TILECATEGORY_HAS_ANY_UNIT,
  TILECATEGORY_HAS_STRUCTURE
} eTileCategory;

typedef enum eSideFilterCriteriaType
{
  SIDECRITERIATYPE_NONE,
  SIDECRITERIATYPE_INDEX,
  SIDECRITERIATYPE_HOUSE_ID,
  SIDECRITERIATYPE_TECH,
  SIDECRITERIATYPE_CATEGORY,
  SIDECRITERIATYPE_ALLY_TO,
  SIDECRITERIATYPE_ENEMY_TO,
  SIDECRITERIATYPE_NEUTRAL_TO,
  SIDECRITERIATYPE_CREDITS,
  SIDECRITERIATYPE_SPICE,
  SIDECRITERIATYPE_CASH,
  SIDECRITERIATYPE_STORAGE,
  SIDECRITERIATYPE_POWER_PERCENT,
  SIDECRITERIATYPE_POWER_OUTPUT,
  SIDECRITERIATYPE_POWER_DRAIN,
  SIDECRITERIATYPE_1_UPGRADE,
  SIDECRITERIATYPE_2_UPGRADES,
  SIDECRITERIATYPE_3_UPGRADES,
  SIDECRITERIATYPE_HARVESTED,
  SIDECRITERIATYPE_UNITS_BUILT,
  SIDECRITERIATYPE_BUILDINGS_BUILT,
  SIDECRITERIATYPE_UNITS_LOST,
  SIDECRITERIATYPE_BUILDINGS_LOST,
  SIDECRITERIATYPE_UNITS_KILLED,
  SIDECRITERIATYPE_BUILDINGS_KILLED
} eSideFilterCriteriaType;

typedef enum eSideCategory
{
  SIDECATEGORY_MY_SIDE,
  SIDECATEGORY_ALLY_TO_MY_SIDE,
  SIDECATEGORY_ENEMY_TO_MY_SIDE,
  SIDECATEGORY_NEUTRAL_TO_MY_SIDE,
  SIDECATEGORY_AI_ENABLED,
  SIDECATEGORY_PARTICIPATES_GAME,
  SIDECATEGORY_UNITS_EXIST,
  SIDECATEGORY_BUILDINGS_EXIST
} eSideCategory;

typedef enum eUnitTypeFilterCriteriaType
{
  UNITTYPECRITERIATYPE_NONE,
  UNITTYPECRITERIATYPE_TYPE,
  UNITTYPECRITERIATYPE_GROUP,
  UNITTYPECRITERIATYPE_BEHAVIOR,
  UNITTYPECRITERIATYPE_CATEGORY,
  UNITTYPECRITERIATYPE_OWNER,
  UNITTYPECRITERIATYPE_TECH,
  UNITTYPECRITERIATYPE_PREREQ1,
  UNITTYPECRITERIATYPE_PREREQ2,
  UNITTYPECRITERIATYPE_ARMOR,
  UNITTYPECRITERIATYPE_SPEED_TYPE,
  UNITTYPECRITERIATYPE_PRI_WEAPON,
  UNITTYPECRITERIATYPE_SEC_WEAPON,
  UNITTYPECRITERIATYPE_SIGHT,
  UNITTYPECRITERIATYPE_RANGE,
  UNITTYPECRITERIATYPE_SPEED,
  UNITTYPECRITERIATYPE_RATE_OF_FIRE,
  UNITTYPECRITERIATYPE_HP100_MAX,
  UNITTYPECRITERIATYPE_FLAG,
} eUnitTypeFilterCriteriaType;

typedef enum eUnitTypeCategory
{
  UNITTYPECATEGORY_INFANTRY,
  UNITTYPECATEGORY_VEHICLE,
  UNITTYPECATEGORY_LIGHT_VEH,
  UNITTYPECATEGORY_HEAVY_VEH,
  UNITTYPECATEGORY_SPECIAL,
  UNITTYPECATEGORY_HAS_PRI_WEAPON,
  UNITTYPECATEGORY_HAS_SEC_WEAPON,
  UNITTYPECATEGORY_HAS_BARREL,
  UNITTYPECATEGORY_CAN_BE_UPGRADED,
} eUnitTypeCategory;

typedef enum eBuildingTypeFilterCriteriaType
{
  BUILDINGTYPECRITERIATYPE_NONE,
  BUILDINGTYPECRITERIATYPE_TYPE,
  BUILDINGTYPECRITERIATYPE_GROUP,
  BUILDINGTYPECRITERIATYPE_BEHAVIOR,
  BUILDINGTYPECRITERIATYPE_CATEGORY,
  BUILDINGTYPECRITERIATYPE_OWNER,
  BUILDINGTYPECRITERIATYPE_TECH,
  BUILDINGTYPECRITERIATYPE_PREREQ1,
  BUILDINGTYPECRITERIATYPE_PREREQ2,
  BUILDINGTYPECRITERIATYPE_ARMOR,
  BUILDINGTYPECRITERIATYPE_PRI_WEAPON,
  BUILDINGTYPECRITERIATYPE_SEC_WEAPON,
  BUILDINGTYPECRITERIATYPE_SIGHT,
  BUILDINGTYPECRITERIATYPE_POWER_CONS,
  BUILDINGTYPECRITERIATYPE_POWER_PROD,
  BUILDINGTYPECRITERIATYPE_RANGE,
  BUILDINGTYPECRITERIATYPE_RATE_OF_FIRE,
  BUILDINGTYPECRITERIATYPE_HP100_MAX,
  BUILDINGTYPECRITERIATYPE_FLAG,
} eBuildingTypeFilterCriteriaType;

typedef enum eBuildingTypeCategory
{
  BUILDINGTYPECATEGORY_POWER_CONS,
  BUILDINGTYPECATEGORY_POWER_PROD,
  BUILDINGTYPECATEGORY_REQ_ENOUGH_POW,
  BUILDINGTYPECATEGORY_SHAKE_SCREEN,
  BUILDINGTYPECATEGORY_HAS_PRI_WEAPON,
  BUILDINGTYPECATEGORY_HAS_SEC_WEAPON,
  BUILDINGTYPECATEGORY_HAS_BARREL,
} eBuildingTypeCategory;

typedef enum eTileCheckType
{
  TILECHECK_XPOS,
  TILECHECK_YPOS,
  TILECHECK_ATTRIB,
  TILECHECK_TERRAIN,
  TILECHECK_SPICE,
  TILECHECK_CONCOWN,
  TILECHECK_OWNER1,
  TILECHECK_OWNER2,
  TILECHECK_UNIT_TYPE,
  TILECHECK_UNIT_GROUP,
  TILECHECK_UNIT_TAG,
  TILECHECK_UNIT_FLAG,
  TILECHECK_UNIT_STATE,
  TILECHECK_BUILDING_TYPE,
  TILECHECK_BUILDING_GROUP,
  TILECHECK_BUILDING_TAG,
  TILECHECK_BUILDING_FLAG,
  TILECHECK_BUILDING_STATE,
  TILECHECK_BUILDING_TILENO
} eTileCheckType;

typedef enum eObjectCheckType
{
  OBJECTCHECK_TYPE,
  OBJECTCHECK_GROUP,
  OBJECTCHECK_CATEGORY,
  OBJECTCHECK_TAG,
  OBJECTCHECK_ARMOR,
  OBJECTCHECK_WEAPON,
  OBJECTCHECK_DIST
} eObjectCheckType;

bool CheckIfUnitMatchesFilter(ObjectFilterStruct *filter, Unit *unit, eSideType side_id);
bool CheckIfBuildingMatchesFilter(ObjectFilterStruct *filter, Building *building, eSideType side_id);
bool CheckIfBulletMatchesFilter(ObjectFilterStruct *filter, Bullet *bullet);
bool CheckIfExplosionMatchesFilter(ObjectFilterStruct *filter, Explosion *explosion);
bool CheckIfCrateMatchesFilter(ObjectFilterStruct *filter, CrateStruct *crate);
bool CheckIfTileMatchesFilter(ObjectFilterStruct *filter, GameMapTileStruct *tile, int pos_x, int pos_y);
bool CheckIfSideMatchesFilter(ObjectFilterStruct *filter, int side_id);
bool CheckIfUnitTypeMatchesFilter(ObjectFilterStruct *filter, int unit_type);
bool CheckIfBuildingTypeMatchesFilter(ObjectFilterStruct *filter, int building_type);

void GetBoundsForPosFilter(ObjectFilterStruct *filter, int *min_x, int *min_y, int *max_x, int *max_y);
