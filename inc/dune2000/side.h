// ### Side class structs ###

typedef enum UnitFlags
{
  UFLAGS_1 = 0x1,
  UFLAGS_2 = 0x2,
  UFLAGS_4 = 0x4,
  UFLAGS_8 = 0x8,
  UFLAGS_10_STEALTH = 0x10,
  UFLAGS_20 = 0x20,
  UFLAGS_40_FLYING = 0x40,
  UFLAGS_80 = 0x80,
  UFLAGS_100 = 0x100,
  UFLAGS_200 = 0x200,
  UFLAGS_400 = 0x400,
  UFLAGS_800 = 0x800,
  UFLAGS_1000_DEVIATED = 0x1000,
  UFLAGS_2000 = 0x2000,
  UFLAGS_4000 = 0x4000,
  UFLAGS_8000_FIXED_BARREL = 0x8000,
  UFLAGS_10000 = 0x10000,
  UFLAGS_20000_DEATHHAND = 0x20000,
  UFLAGS_40000 = 0x40000,
  UFLAGS_80000 = 0x80000,
  UFLAGS_100000 = 0x100000,
  UFLAGS_200000 = 0x200000,
  UFLAGS_400000 = 0x400000,
  UFLAGS_800000_SELFHEALING = 0x800000,
  UFLAGS_1000000 = 0x1000000,
  UFLAGS_2000000 = 0x2000000,
  UFLAGS_4000000 = 0x4000000,
  UFLAGS_8000000_INVULNERABLE = 0x8000000,
  UFLAGS_BLOCKTOMARKED = 0x10000000,
  UFLAGS_20000000 = 0x20000000,
  UFLAGS_40000000 = 0x40000000,
  UFLAGS_80000000 = 0x80000000,
}UnitFlags;

typedef enum UnitBehaviorType
{
  UnitBehaviorType_0 = 0,
  UnitBehavior_HARVESTER = 1,
  UnitBehavior_CARRYALL = 2,
  UnitBehavior_ENGINEER = 3,
  UnitBehavior_SABOTEUR = 4,
  UnitBehavior_SANDWORM = 5,
  UnitBehavior_MCV = 6,
  UnitBehavior_DEVASTATOR = 7,
  UnitBehavior_FRIGATE = 8,
  UnitBehavior_ORNITHOPTER = 9,
  UnitBehavior_DEATH_HAND = 10,
  UnitBehavior_SARDAUKAR = 11,
  UnitBehavior_FREMEN = 12,
  UnitBehavior_FR_NON_STEALTH = 13,
  UnitBehavior_THUMPER = 14,
  UnitBehavior_BIRD = 15,
  UnitBehavior_DEVIATOR = 16,
  UnitBehavior_11 = 17,
}UnitBehaviorType;

typedef enum eUnitState
{
  UNIT_STATE_DUMMY = 0x0,
  UNIT_STATE_1 = 0x1,
  UNIT_STATE_2_IDLE = 0x2,
  UNIT_STATE_3_GUARDING = 0x3,
  UNIT_STATE_4_ATTACKING_UNIT = 0x4,
  UNIT_STATE_5_ATTACKING_BUILDING = 0x5,
  UNIT_STATE_6_ATTACKING_TILE = 0x6,
  UNIT_STATE_7_MOVING = 0x7,
  UNIT_STATE_8_LEAVING_BUILDING = 0x8,
  UNIT_STATE_9_MOVING_AWAY = 0x9,
  UNIT_STATE_10 = 0xA,
  UNIT_STATE_11_MOVING_TO_HARVEST = 0xB,
  UNIT_STATE_12_HARVESTING = 0xC,
  UNIT_STATE_13_MOVING_TO_REFINERY = 0xD,
  UNIT_STATE_14_ENTERING_REFINERY = 0xE,
  UNIT_STATE_15_UNLOADING_SPICE = 0xF,
  UNIT_STATE_16_LEAVING_REFINERY = 0x10,
  UNIT_STATE_17_DEAD = 0x11,
  UNIT_STATE_18_AWAITING_AIRLIFT = 0x12,
  UNIT_STATE_19_AIRLIFTED = 0x13,
  UNIT_STATE_20_INVALID = 0x14,
  UNIT_STATE_21_FLYING_DOWN = 0x15,
  UNIT_STATE_22_FLYING_UP = 0x16,
  UNIT_STATE_23_INFILTRATING = 0x17,
  UNIT_STATE_24_MOVING_TO_REPAIR_PAD = 0x18,
  UNIT_STATE_25_REPAIRING = 0x19,
  UNIT_STATE_26_LEAVING_REPAIR_PAD = 0x1A,
  UNIT_STATE_27_WANDERING = 0x1B,
  UNIT_STATE_28_EATING = 0x1C,
  UNIT_STATE_29_SLEEPING = 0x1D,
  UNIT_STATE_30_DEPLOYING = 0x1E,
  UNIT_STATE_31_SELFDESTRUCT = 0x1F,
  UNIT_STATE_32_THUMPERING = 0x20,
  UNIT_STATE_33 = 0x21,
}eUnitState;

typedef __int16 index;

typedef struct Unit
{
  int32_t Speed;
  int32_t Health;
  UnitFlags Flags;
  index EnemyIndex;
  uint8_t Tag; // Additional field for extended functionality (mod)
  uint8_t PrevWasSelected; // Additional field for extended functionality (mod)
  int32_t State;
  uint32_t OldState;
  uint8_t Type;
  uint8_t __IsSelected;
  char __UnitTurnDelayCounter;
  char __TurretTurnDelayCounter;
  char __ReloadDelayCounter;
  char __FlickerFrames;
  char __CurrentAnimFrame;
  char field_1F;
  int32_t __CurrentAnimType;
  char __Lying;
  char __CurrentAnimDelayCounter_SandwormSleepTimeCounter;
  char EnemySide;
  char __MakeSmokeTimeCounter;
  char __GroupID;
  char __SpecialPurpose;
  int8_t LastAttacked;
  char LastFired;
  char LastMoved;
  char OldSide;
  char __DeviatedTimeCounter;
  char __DeadStateTimeCounter;
  char __AttackerSide;
  char __StealthUnCloakDelayCounter;
  int16_t __AttackerIndex;
  uint32_t __PosX;
  uint32_t __PosY;
  int32_t __PosZHeight;
  uint32_t __posx2;
  uint32_t __posy2;
  uint8_t TargetX;
  uint8_t TargetY;
  uint8_t BlockFromX;
  uint8_t BlockFromY;
  uint8_t BlockToX;
  uint8_t BlockToY;
  char __pos_stepsmax;
  uint8_t pos_steps;
  char __RememberPosX;
  char __RememberPosY;
  char __CellspotTo;
  char __CellspotFrom;
  char __Facing;
  char __FacingTurret;
  char __FacingTurretTarget;
  char field_57;
  int16_t RefineryIndex;
  int16_t RepairPadIndex;
  char __RouteX[12];
  char __RouteY[12];
  char __RouteSteps;
  char RouteMode;
  int16_t __RouteTotalSteps;
  int16_t __RouteRetryDelayCounter;
  char RB_c_field_7A;
  char field_7B;
  struct Unit *Next;
  struct Unit *Prev;
  int32_t __dw_field_84_ptr;
  struct Unit **__dw_field_88_ptr;
  int16_t MyIndex;
  int16_t field_8E;
  int32_t ObjectType;
}Unit;

typedef enum BuildingFlags
{
  BFLAGS_1_REPAIRING = 0x1,
  BFLAGS_2_BUILDUP = 0x2,
  BFLAGS_4_SELLING = 0x4,
  BFLAGS_8_SOLD = 0x8,
  BFLAGS_10_ANIM_PERMANENT = 0x10,
  BFLAGS_20_ANIM_ONCE = 0x20,
  BFLAGS_40_HAS_ANIMATION = 0x40,
  BFLAGS_80_PRIMARY = 0x80,
  BFLAGS_100 = 0x100,
  BFLAGS_200_SELECTABLE_REPAIRABLE = 0x200,
  BFLAGS_400_CAN_BE_CAPTURED = 0x400,
  BFLAGS_800 = 0x800,
  BFLAGS_1000_WAS_ATTACKED = 0x1000,
  BFLAGS_2000 = 0x2000,
  BFLAGS_4000 = 0x4000,
  BFLAGS_8000_DECAY = 0x8000,
  BFLAGS_10000_DONT_REMOVE_STUFF = 0x10000,
  BFLAGS_20000_WAS_SOLD = 0x20000,
  BFLAGS_40000_NEED_RELEASE_INFANTRY = 0x40000,
  BFLAGS_80000_INFANTRY_RELEASED = 0x80000,
  BFLAGS_100000_INVISIBLE = 0x100000,
  BFLAGS_200000_HAS_SKIRT = 0x200000,
  BFLAGS_400000_NO_CONCRETE = 0x400000,
  BFLAGS_800000_ANIM_ALPHA = 0x800000,
  BFLAGS_1000000_INFILTRATED = 0x1000000,
  BFLAGS_2000000_WAS_CAPTURED = 0x2000000,
  BFLAGS_4000000_SOLD_BY_AI = 0x4000000,
  BFLAGS_8000000 = 0x8000000,
  BFLAGS_10000000 = 0x10000000,
  BFLAGS_20000000 = 0x20000000,
  BFLAGS_40000000 = 0x40000000,
  BFLAGS_80000000 = 0x80000000,
}BuildingFlags;

typedef enum BuildingBehaviorType
{
  BuildingBehavior_DEFAULT = 0x0,
  BuildingBehavior_OUTPOST = 0x1,
  BuildingBehavior_CONSTRUCTION = 0x2,
  BuildingBehavior_WINDTRAP = 0x3,
  BuildingBehavior_REFINERY = 0x4,
  BuildingBehavior_SILO = 0x5,
  BuildingBehavior_REPAIR = 0x6,
  BuildingBehavior_LIGHT_FACTORY = 0x7,
  BuildingBehavior_HEAVY_FACTORY = 0x8,
  BuildingBehavior_HITEECH = 0x9,
  BuildingBehavior_AT_HITEECH = 0xA,
  BuildingBehavior_BARRACKS = 0xB,
  BuildingBehavior_WOR = 0xC,
  BuildingBehavior_STARPORT = 0xD,
  BuildingBehavior_WALL = 0xE,
  BuildingBehavior_CONCRETE = 0xF,
  BuildingBehavior_TURRET = 0x10,
  BuildingBehavior_HK_PALACE = 0x11,
  BuildingBehavior_OR_PALACE = 0x12,
  BuildingBehavior_A_PALACE = 0x13,
  BuildingBehaviorType_14 = 0x14,
}BuildingBehaviorType;

typedef struct Building
{
  int32_t __PosX;
  uint32_t __PosY;
  char flags_field_8;
  char field_9;
  char field_A;
  char field_B;
  char field_C;
  char field_D;
  uint8_t Tag; // Additional field for extended functionality (mod)
  uint8_t PrevWasSelected; // Additional field for extended functionality (mod)
  char field_10;
  char field_11;
  char field_12;
  char field_13;
  char __AttackTileX;
  char __AttackTileY;
  char __MinXPos;
  char __MinYPos;
  char __MaxXPos;
  char __MaxYPos;
  char field_1A;
  char field_1B;
  char field_1C;
  char field_1D;
  char field_1E;
  char field_1F;
  char __WallFrame;
  char __Facing;
  char field_22;
  char field_23;
  uint32_t Health;
  int32_t Flags;
  int16_t EnemyIndex;
  char field_2E;
  char field_2F;
  int32_t __State;
  uint8_t Type;
  char __IsSelected;
  char __TurretTurnDelayCounter;
  char __ReloadDelayCounter_refinery;
  uint8_t __FlickerFrames;
  char __DirectionFrame;
  char c_field_3A_unused;
  char __DeadStateTimeCounter;
  char EnemySide;
  char __MakeSmokeTimeCounter;
  uint8_t __BuildupAnimCounter;
  uint8_t __BuildingAnimCounter;
  char __MySide;
  char __LastAttacked;
  int16_t __MyIndex;
  int32_t __addedonticks1;
  int32_t __addedonticks2;
  int32_t __SmokeAnimActiveBitfield;
  char __AttackerSide;
  char field_51;
  int16_t __AttackerIndex;
  char field_54;
  char field_55;
  char field_56;
  char field_57;
  char field_58;
  char field_59;
  char field_5A;
  char field_5B;
  char field_5C;
  char field_5D;
  char field_5E;
  char field_5F;
  char field_60;
  char field_61;
  char field_62;
  char field_63;
  char field_64;
  char field_65;
  char field_66;
  char field_67;
  char field_68;
  char field_69;
  char field_6A;
  char field_6B;
  char field_6C;
  char field_6D;
  char field_6E;
  char field_6F;
  char field_70;
  char field_71;
  char field_72;
  char field_73;
  char field_74;
  char field_75;
  char field_76;
  char field_77;
  char field_78;
  char field_79;
  char field_7A;
  char field_7B;
  struct Building *Next;
  struct Building *Prev;
  int32_t dw_field_84_ptr;
  int32_t dw_field_88_ptr;
  int16_t MyIndex;
  int16_t w_field_8E;
  int32_t ObjectType;
}Building;

typedef struct Bullet
{
  int32_t field_0__Range;
  uint32_t Flags;
  int32_t dw_field_8;
  int32_t dw_field_C;
  int32_t field_10__ProjectileSpeed;
  int16_t w_field_14_firer;
  int16_t __homing_i;
  uint8_t Type;
  char __homing_s;
  char c_field_1A;
  char c_field_1B;
  int16_t w_field_1C;
  int16_t w_field_1E;
  uint32_t dw_field_20;
  uint32_t dw_field_24_x;
  uint32_t dw_field_28_y;
  uint32_t dw_field_2C_z;
  char field_30;
  char field_31;
  char field_32;
  char field_33;
  int32_t dw_field_34;
  char field_38;
  char field_39;
  char field_3A;
  char field_3B;
  char field_3C;
  char field_3D;
  char field_3E;
  char field_3F;
  char field_40;
  char field_41;
  char field_42;
  char field_43;
  uint8_t unsigned44_art;
  char c_field_45;
  char c_field_46;
  char field_47;
  char field_48;
  char field_49;
  char field_4A;
  char field_4B;
  char field_4C;
  char field_4D;
  char field_4E;
  char field_4F;
  char field_50;
  char field_51;
  char field_52;
  char field_53;
  char field_54;
  char field_55;
  char field_56;
  char field_57;
  char field_58;
  char field_59;
  char field_5A;
  char field_5B;
  char field_5C;
  char field_5D;
  char field_5E;
  char field_5F;
  char field_60;
  char field_61;
  char field_62;
  char field_63;
  char field_64;
  char field_65;
  char field_66;
  char field_67;
  char field_68;
  char field_69;
  char field_6A;
  char field_6B;
  char field_6C;
  char field_6D;
  char field_6E;
  char field_6F;
  char field_70;
  char field_71;
  char field_72;
  char field_73;
  char field_74;
  char field_75;
  char field_76;
  char field_77;
  char field_78;
  char field_79;
  char field_7A;
  char field_7B;
  struct Bullet *Next;
  int32_t dw_field_80_ptr;
  int32_t dw_field_84_ptr;
  int32_t dw_field_88_ptr;
  int16_t MyIndex;
  int16_t w_field_8E;
  int32_t ObjectType;
}Bullet;

typedef struct Explosion
{
  uint8_t Type;
  uint8_t __explosionAni;
  char c_field_2;
  char field_3;
  int32_t Flags;
  int16_t w_field_8;
  char field_A;
  char field_B;
  uint32_t __PosX;
  uint32_t __PosY;
  uint32_t __PosZHeight;
  uint8_t gap_18[24];
  uint32_t dw_field_30;
  uint32_t dw_field_34;
  uint8_t gap_38[68];
  struct Explosion *Next;
  int32_t dw_field_80_ptr;
  int32_t dw_field_84_ptr;
  int32_t dw_field_88_ptr;
  int16_t MyIndex;
  int16_t w_field_8E;
  int32_t ObjectType;
}Explosion;

typedef struct StructForCSide2
{
  int32_t __kills_per_side[8];
}StructForCSide2;

typedef struct DeliveryStruct
{
  char __xpos;
  char __ypos;
  char c_field_2;
  char __is_active;
  int32_t __delivery_time;
  char __units[41];
  char field_31;
  char field_32;
  char field_33;
  char field_34;
  char __delivery_type;
  char __deploy_action;
  char field_37;
}DeliveryStruct;

typedef struct BuildQueueStruct
{
  int16_t __type;
  int16_t w_field_2_buildprogress;
  int16_t w_field_4_buildprogress;
  char field_6;
  char field_7;
  float f_field_8_credits;
  int32_t dw_field_C_credits;
  char __on_hold;
  char c_field_11_cancel;
  char field_12;
  char field_13;
}BuildQueueStruct;

typedef struct PickupQueueStruct
{
  char __from_x;
  char __from_y;
  char __to_x;
  char __to_y;
  char c_field_4;
  char __state;
  int16_t __unit_index;
  int16_t __carryall_index;
  char field_A;
  char field_B;
  int32_t dw_field_C_counter;
  int32_t __unit_state;
}PickupQueueStruct;

typedef struct CSide
{
  Unit *__ObjectArrayPtr;
  Unit *__FirstFreeObjectPtr;
  Unit *__FirstUnitPtr;
  Unit *__LastUnitPtr;
  Building *__FirstBuildingPtr;
  Building *__LastBuildingPtr;
  Bullet *__FirstBulletPtr;
  Bullet *__LastBulletPtr;
  Explosion *__FirstExplosionPtr;
  Explosion *__LastExplosionPtr;
  int32_t dw_field_28_ptr;
  int32_t dw_field_2C_ptr;
  Unit __ObjectArray[1000];
  char __ParticipatesInGame;
  char c_field_24251_unused;
  char __SideId;
  char fHouseID;
  int32_t SpiceReal;
  int32_t __MaxStorage;
  int32_t CashReal;
  int32_t CashDrip;
  int32_t SpiceDrip;
  int32_t __StarportUnitTypePicked[60];
  int32_t __StarportUnitTypeStock[60];
  int32_t __StarportUnitTypeCost[60];
  uint32_t __StarportUpdateDelayCounter;
  uint32_t __StarportStockIncreaseDelayCounter;
  uint32_t __StarportOrderCost;
  uint8_t __BuildingGroupUpgradeCount[100];
  uint32_t __PowerOutput;
  uint32_t __PowerDrained;
  uint32_t __PowerPercent1;
  int32_t __PowerPercent2;
  char __HarvesterCount;
  char field_245B9;
  char field_245BA;
  char field_245BB;
  int32_t dw_field_245BC_unused;
  char __CarryallCount;
  char __BuildingsExistPerGroup[100];
  char field_24625;
  char field_24626;
  char field_24627;
  int32_t __UnitsExistPerType[60];
  char __BuildingTypeCanBeBuilt[100];
  int32_t __SpiceHarvested;
  int32_t __UnitsBuilt;
  int32_t __BuildingsBuilt;
  int32_t __UnitsBuiltPerType[60];
  int32_t __BuildingsBuiltPerType[100];
  int32_t __UnitsLost;
  int32_t __BuildingsLost;
  int32_t __UnitsLostPerType[60];
  int32_t ar_field_24B00_unused[100];
  uint32_t __UnitsKilled;
  uint32_t __BuildingsKilled;
  StructForCSide2 __UnitsKilledPerTypeAndSide[60];
  StructForCSide2 __BuildingsKilledPerTypeAndSide[100];
  int16_t __PrimaryConyard;
  int16_t __PrimaryLightFactory;
  int16_t __PrimaryHeavyFactory;
  int16_t __PrimaryHitech;
  int16_t __PrimaryAtHitech;
  int16_t __PrimaryBarracks;
  int16_t __PrimaryWor;
  int16_t __PrimaryStarport;
  int16_t __PrimaryAtrPalace;
  int16_t __PrimaryHarkPalace;
  int16_t __PrimaryOrdPalace;
  char field_260AE;
  char field_260AF;
  Unit *__LastAddedUnit;
  uint32_t __BuildingIconCount;
  int32_t __BuildingIconCountPrev;
  int32_t __BuildingIcons[60];
  uint32_t __UnitIconCount;
  int32_t __UnitIconCountPrev;
  int32_t __UnitIcons[60];
  uint8_t __StarportPurchaseOrderGenerated;
  uint8_t __StarportDeliveryInProgress;
  char field_262A6;
  char field_262A7;
  int32_t __StarportIcons[8];
  int32_t __UpgradeIcons[8];
  char __BasePosMinY;
  char __BasePosMaxY;
  char __BasePosMinX;
  char __BasePosMaxX;
  DeliveryStruct __Deliveries[10];
  BuildQueueStruct __BuildingBuildQueue;
  BuildQueueStruct __UnitBuildQueue[10];
  BuildQueueStruct __BuildingUpgradeQueue;
  PickupQueueStruct __PickupQueue[32];
  char __PickupQueueCount;
  char __CarryallCount2;
  char field_2688E;
  char field_2688F;
  char ar_field_26890_unused[256];
}CSide;

typedef CSide *Side;
