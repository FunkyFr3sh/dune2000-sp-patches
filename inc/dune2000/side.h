// ### Side class structs ###

typedef enum UnitFlags
{
  UFLAGS_1 = 0x1,
  UFLAGS_2 = 0x2,
  UFLAGS_4 = 0x4,
  UFLAGS_8 = 0x8,
  UFLAGS_10_STEALTH = 0x10,
  UFLAGS_20 = 0x20,
  UFLAGS_40 = 0x40,
  UFLAGS_80 = 0x80,
  UFLAGS_100 = 0x100,
  UFLAGS_200 = 0x200,
  UFLAGS_400 = 0x400,
  UFLAGS_800 = 0x800,
  UFLAGS_1000 = 0x1000,
  UFLAGS_2000 = 0x2000,
  UFLAGS_4000 = 0x4000,
  UFLAGS_8000_FIXED_BARREL = 0x8000,
  UnitFlags_10000 = 0x10000,
  UnitFlags_20000 = 0x20000,
  UnitFlags_40000 = 0x40000,
  UnitFlags_80000 = 0x80000,
  UnitFlags_100000 = 0x100000,
  UnitFlags_200000 = 0x200000,
  UnitFlags_400000 = 0x400000,
  UFLAGS_800000_SELFHEALING = 0x800000,
  UnitFlags_1000000 = 0x1000000,
  UnitFlags_2000000 = 0x2000000,
  UnitFlags_4000000 = 0x4000000,
  UnitFlags_8000000 = 0x8000000,
  UFLAGS_BLOCKTOMARKED = 0x10000000,
  UnitFlags_20000000 = 0x20000000,
  UnitFlags_40000000 = 0x40000000,
  UnitFlags_80000000 = 0x80000000,
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
  UNIT_STATE_26 = 0x1A,
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
  int16_t w_field_E;
  int32_t State;
  uint32_t OldState;
  uint8_t Type;
  uint8_t __ClearUnits_SelectedGroup_c_field_19_state;
  char TurningSpeed;
  char TurretTurnSpeed;
  char ReloadRate;
  char c_field_1D;
  char CAF_animframec_field_1E_x;
  char field_1F;
  int32_t CAT_rotationc_field_20;
  char L_c_field_24;
  char CAD___harvestUnloadDelay_SandWormInitialSleep;
  char EnemySide;
  char S_c_field_27;
  char GroupID_28;
  char __SpecialVal_S;
  int8_t LastAttacked;
  char LastFired;
  char LastMoved;
  char OldSide;
  char DT_field_2E_deliveryTime;
  char ED_c_field_2F_50Inf_10Unit;
  char AS_side_field_30_ASide;
  char S_c_field_31;
  int16_t AI_w_field_32_AIndex;
  uint32_t __posx;
  uint32_t __posy;
  int32_t dw_field_3C;
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
  char c_field_50_BlockToX_TargetX;
  char c_field_51_BlockToY_TargetY;
  char c_field_52_cellspot;
  char c_field_53_cellspot;
  char c_field_54_facingcurrent;
  char c_field_55_facingcurrent;
  char c_field_56_facingcurrent;
  char field_57;
  int16_t RefineryIndex;
  int16_t RepairPadIndex;
  char __RouteX[12];
  char __RouteY[12];
  char c_field_74_route;
  char RouteMode;
  int16_t RTS_w_field_76;
  int16_t RRD_w_field_78;
  char RB_c_field_7A;
  char field_7B;
  struct Unit *Next;
  int32_t __dw_field_80_ptr;
  int32_t __dw_field_84_ptr;
  struct Unit **__dw_field_88_ptr;
  int16_t MyIndex;
  int16_t field_8E;
  int32_t ObjectType;
}Unit;

typedef enum BuildingFlags
{
  BFLAGS_1_REPAIRING = 0x1,
  BuildingFlags_2 = 0x2,
  BuildingFlags_4 = 0x4,
  BuildingFlags_8 = 0x8,
  BFLAGS_10_ANIM_PERMANENT = 0x10,
  BuildingFlags_20 = 0x20,
  BFLAGS_40_HAS_ANIMATION = 0x40,
  BuildingFlags_80 = 0x80,
  BuildingFlags_100 = 0x100,
  BFLAGS_200_SELECTABLE_REPAIRABLE = 0x200,
  BFLAGS_400_CAN_BE_CAPTURED = 0x400,
  BuildingFlags_800 = 0x800,
  BuildingFlags_1000 = 0x1000,
  BuildingFlags_2000 = 0x2000,
  BuildingFlags_4000 = 0x4000,
  BuildingFlags_8000 = 0x8000,
  BuildingFlags_10000 = 0x10000,
  BuildingFlags_20000 = 0x20000,
  BuildingFlags_40000 = 0x40000,
  BuildingFlags_80000 = 0x80000,
  BuildingFlags_100000 = 0x100000,
  BFLAGS_200000_HAS_SKIRT = 0x200000,
  BFLAGS_400000_NO_CONCRETE = 0x400000,
  BFLAGS_800000_ANIM_ALPHA = 0x800000,
  BuildingFlags_1000000 = 0x1000000,
  BuildingFlags_2000000 = 0x2000000,
  BuildingFlags_4000000 = 0x4000000,
  BuildingFlags_8000000 = 0x8000000,
  BuildingFlags_10000000 = 0x10000000,
  BuildingFlags_20000000 = 0x20000000,
  BuildingFlags_40000000 = 0x40000000,
  BuildingFlags_80000000 = 0x80000000,
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
  int32_t dw_field_0_x;
  uint32_t dw_field_4_y;
  char flags_field_8;
  char field_9;
  char field_A;
  char field_B;
  char field_C;
  char field_D;
  char field_E;
  char field_F;
  char field_10;
  char field_11;
  char field_12;
  char field_13;
  char c_field_14_x;
  char c_field_15_y;
  char c_field_16_exitx1;
  char c_field_17_exity1;
  char c_field_18_x;
  char c_field_19_x;
  char field_1A;
  char field_1B;
  char field_1C;
  char field_1D;
  char field_1E;
  char field_1F;
  char c_field_20_wall;
  char c_field_21_facing;
  char field_22;
  char field_23;
  uint32_t Health;
  int32_t Flags;
  int16_t EnemyIndex;
  char field_2E;
  char field_2F;
  int32_t __State;
  uint8_t Type;
  char c_field_35_bool;
  char _____BarrelRotationSpeed;
  char _____RateOfFire_refinery;
  uint8_t c_field_38;
  char c_field_39;
  char c_field_3A;
  char c_field_3B;
  char EnemySide;
  char c_field_3D_counter_randvalue;
  uint8_t unsigned3E_counter;
  uint8_t unsigned3F;
  char c_field_40_side;
  char c_field_41_somelimit;
  int16_t __FacilityIndex;
  int32_t __addedonticks1;
  int32_t __addedonticks2;
  int32_t dw_field_4C_randflag;
  char c_field_50_side;
  char field_51;
  int16_t w_field_52_index;
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
  int32_t field_80;
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
  uint32_t dw_field_24;
  uint32_t dw_field_28_x;
  uint32_t dw_field_2C_y;
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
  uint32_t dw_field_C;
  uint32_t dw_field_10;
  uint32_t dw_field_14;
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
  int32_t field_0;
  char field_4;
  char field_5;
  char field_6;
  char field_7;
  char field_8;
  char field_9;
  char field_A;
  char field_B;
  char field_C;
  char field_D;
  char field_E;
  char field_F;
  char field_10;
  char field_11;
  char field_12;
  char field_13;
  char field_14;
  char field_15;
  char field_16;
  char field_17;
  char field_18;
  char field_19;
  char field_1A;
  char field_1B;
  char field_1C;
  char field_1D;
  char field_1E;
  char field_1F;
}StructForCSide2;

typedef struct DeliveryStruct
{
  char __xpos;
  char __ypos;
  char c_field_2;
  char __is_active;
  int32_t __delivery_time;
  char __units[20];
  char field_1C;
  char field_1D;
  char field_1E;
  char field_1F;
  char field_20;
  char field_21;
  char field_22;
  char field_23;
  char field_24;
  char field_25;
  char field_26;
  char field_27;
  char field_28;
  char field_29;
  char field_2A;
  char field_2B;
  char field_2C;
  char field_2D;
  char field_2E;
  char field_2F;
  char field_30;
  char field_31;
  char field_32;
  char field_33;
  char field_34;
  char __delivery_type;
  char __deploy_action;
  char field_37;
}DeliveryStruct;

typedef struct StructForCSide
{
  int16_t w_field_0;
  int16_t w_field_2;
  int16_t w_field_4;
  char field_6;
  char field_7;
  float f_field_8;
  int32_t dw_field_C;
  char c_field_10;
  char c_field_11;
  char field_12;
  char field_13;
}StructForCSide;

typedef struct CSide
{
  int32_t dw_field_0_ptr_Objects_Ptr;
  Unit *object_dw_field_4_ptr;
  Unit *_Units_8;
  int32_t dw_field_C_ptr;
  Building *_Buildings_10;
  int32_t dw_field_14_ptr;
  Bullet *bullets_field_18_ptr;
  int32_t dw_field_1C_anims_ptr;
  Explosion *expanims_dw_field_20_anims_ptr;
  int32_t dw_field_24_ptr;
  int32_t dw_field_28_ptr;
  int32_t dw_field_2C_ptr;
  Unit __Objects_30[1000];
  char c_field_24250_canbuild;
  char c_field_24251;
  char __ai_id;
  char fHouseID;
  int32_t SpiceReal;
  int32_t __MaxStorage;
  int32_t CashReal;
  int32_t CashDrip;
  int32_t SpiceDrip;
  int32_t unit_ar_field_24268_delivery[60];
  int32_t unit_ar_field_24358_delivery[60];
  int32_t starport_unit_ar_field_24448_cash[60];
  uint32_t dw_field_24538_starport_counter;
  uint32_t dw_field_2453C__starport_counter;
  uint32_t dw_field_24540_cash;
  uint8_t __building_group_upgrade_count[100];
  uint32_t __poweramount;
  uint32_t __drainedpower;
  uint32_t __PowerPercent1;
  int32_t __PowerPercent2;
  char __HarvesterCount_245B8;
  char field_245B9;
  char field_245BA;
  char field_245BB;
  int32_t dw_field_245BC;
  char __CarryallCount;
  char building_a_field_245C1_buildingcount[100];
  char field_24625;
  char field_24626;
  char field_24627;
  int32_t unit_count_ar_field_24628[60];
  char building_ar_field_24718[100];
  int32_t __harvestBlobValuedw_field_2477C;
  int32_t __units_built;
  int32_t __buildings_built;
  int32_t __unit_builtcount_24788[60];
  int32_t building_ar_field_24878[100];
  int32_t __units_lost;
  int32_t __buildings_lost;
  int32_t unit_ar_field_24A10_losses[60];
  int32_t ar_field_24B00[100];
  uint32_t __units_killed;
  uint32_t __buildings_killed;
  StructForCSide2 ar_field_24C98[60];
  StructForCSide2 ar_field_25418[100];
  int16_t __primary_conyard;
  int16_t __primary_light_factory;
  int16_t __primary_heavy_factory;
  int16_t __primary_hitech;
  int16_t __primary_at_hitech;
  int16_t __primary_barracks;
  int16_t __primary_wor;
  int16_t __primary_starport;
  int16_t __primary_at2_palace;
  int16_t __primary_palace;
  int16_t __primary_at1_palace;
  char field_260AE;
  char field_260AF;
  int32_t __LastAddedUnit;
  uint32_t __building_icon_count;
  int32_t __building_icon_count2;
  int32_t __building_icons[60];
  uint32_t __unit_icon_count_261AC;
  int32_t __unit_icon_count2;
  int32_t __unit_icons_261B4[60];
  uint8_t delivery_c_field_262A4;
  uint8_t starport_delivery_c_field_262A5;
  char field_262A6;
  char field_262A7;
  int32_t ar_field_262A8_starporttypes[8];
  int32_t UpgradeIconData_ar_field_262C8[8];
  char c_field_262E8__minY;
  char c_field_262E9__maxY;
  char c_field_262EA__minX;
  char c_field_262EB__maxX;
  DeliveryStruct delivery_field_262EC[10];
  StructForCSide s_field_2651C;
  StructForCSide ar_10_26530[10];
  int16_t w_field_265F8;
  int16_t w_field_265FA;
  int16_t w_field_265FC;
  char field_265FE;
  char field_265FF;
  float f_field_26600;
  int32_t dw_field_26604;
  char c_field_26608;
  char field_26609;
  char field_2660A;
  char field_2660B;
  StructForCSide ar_field_2660C[32];
  char c_field_2688C_max32__CarryallCount2;
  char __CarryallCount2;
  char field_2688E;
  char field_2688F;
  char ar_field_26890[256];
}CSide;

typedef CSide *Side;
