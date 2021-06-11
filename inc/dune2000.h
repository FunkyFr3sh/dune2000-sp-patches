#include <stdbool.h>
#include <stdint.h>
#include "patch.h"

// This header works with sym.asm which defines the Vanilla symbols
// This header will be split up as it becomes larger

typedef void TImage;
typedef int eSideType;

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

// ### AI class structs ###

typedef enum eTaskType
{
  TASK_NO_PURPOSE = 0x0,
  TASK_1 = 0x1,
  TASK_2 = 0x2,
  TASK_PROTECTING = 0x3,
  TASK_4 = 0x4,
  TASK_ATTACK_BUILDING = 0x5,
  TASK_ATTACK_UNIT = 0x6,
  TASK_GUARD_BUILDING = 0x7,
  TASK_8 = 0x8,
  TASK_9 = 0x9,
  TASK_RETURN = 0xA,
  TASK_REPAIR = 0xB,
  TASK_12 = 0xC,
  TASK_13 = 0xD,
  TASK_14 = 0xE,
  TASK_DEFEND_AREA = 0xF,
  TASK_16 = 0x10,
  TASK_17 = 0x11,
  TASK_18 = 0x12,
  TASK_COUNT = 0x13,
}eTaskType;

typedef struct AIGroup
{
  char __side;
  char field_1;
  int16_t w_field_2_index;
  int dw_field_4;
  int dw_field_8_worthamount;
  int dw_field_C_morale_strength;
  int dw_field_10_morale_strength;
  int dw_field_14;
  int dw_field_18_counter;
  char Task_1C; //eTaskType
  char c_field_1D;
  char field_1E;
  char field_1F;
  int dw_field_20__flags;
  char c_field_24_x;
  char c_field_25_y;
  char __NotFree_26;
  char field_27;
  char field_28;
  char field_29;
  char field_2A;
  char field_2B;
}AIGroup;


typedef struct StructForCAI
{
  char xpos_0;
  char ypos_1;
  int16_t index_2;
  int16_t field_4;
  char field_6;
  char field_7;
  char field_8;
  char field_9;
  char field_A;
  char field_B;
  char type_C;
  char __state_D;
  char field_E;
  char field_F;
}StructForCAI;

typedef struct DefendAreaStruct
{
  char __minX;
  char __maxX;
  char __minY;
  char __maxY;
  char c_field_4_Unknown;
  int dw_field_8_Strength;
  int field_C_StartDelay;
  int *field_10_Unknown;
}DefendAreaStruct;

typedef struct CAI_
{
  char AISide;
  char __IsAI;
  char c_field_2_BestTargetSide;
  char EnablePractice;
  char BuyUnits;
  char UpgradeBuildings;
  char c_field_6;
  char DontSell;
  char defense_bool_field_8;
  char c_field_9_cashstate;
  char c_field_A_FindBestUnitToBuild;
  char c_field_B;
  int dw_field_C_fConcrete;
  int dw_field_10;
  int SquishRange;
  int dw_field_18_bestsidecountdown;
  char SquishInfantry;
  char __GuardGroupSizeNot0_1D;
  char __GoBeserk_OtherStates;
  char c_field_1F;
  char field_20;
  char field_21;
  char field_22;
  char field_23;
  int __best_base_pos_x;
  int __best_base_pos_y;
  AIGroup __AIGroups_2C[32];
  StructForCAI __InitialBuildingPositions[150];
  char __BuildingCount;
  char FreePercentage;
  char field_F0E;
  char field_F0F;
  int FirstAttackBuildingDelay;
  int TimeBetweenBuildingAttacks;
  int __AttackBuildingDelay;
  int dw_field_F1C;
  int dw_field_F20;
  int __TimingRandomPercentage2;
  int EmergencyBuildRate;
  int dw_field_F2C_x_KickUnitsOutofArea;
  int dw_field_F30_y_KickUnitsOutofArea;
  int dw_field_F34_x_KickUnitsOutofArea;
  int dw_field_F38_y_KickUnitsOutofArea;
  char c_field_F3C;
  char AttackBuildingStrength;
  char ProtectStrength;
  char DefendStrength;
  char c_field_F40_strength;
  char MaxAttackUnitGroups;
  char MaxAttackBuildingGroups;
  char MaxProtectGroups;
  char MaxDefendGroups;
  char MaxGuardGroups;
  char c_field_F46;
  char c_field_F47;
  int MinBaseStrength;
  int MaxBaseStrength;
  int BaseDefenceRadius;
  char c_field_F54_carryall;
  char c_field_F55;
  char GuardBuildings;
  char base_x_field_F57;
  char base_y_field_F58;
  char field_F59;
  char field_F5A;
  char field_F5B;
  int GuardGroupSize;
  float f_field_F60;
  float f_field_F64;
  float f_field_F68;
  char AttackBuildingRandomPercent;
  char BuildUnitRandomPercentage;
  char TimingRandomPercentage;
  char GroupSizeRandomPercentage;
  float SpeedWeight[19];
  float HPWeight[19];
  float FirePowerWeight[19];
  int CashStash[9];
  char Morale[19];
  char field_108B;
  float DefendBuildingPriority[100];
  float AttackBuildingPriority[100];
  float BuildingRepairValue[100];
  int TimeBeforeRebuilding[100];
  float BuildRatio[100];
  int MaxBuildings[100];
  char c_field_19EC;
  char AttackSidePriority[8];
  char field_19F5;
  char field_19F6;
  char field_19F7;
  float UnitBuildPriority[60];
  float UnitRepairValue[60];
  float __AttackUnitPriority_1BD8[60];
  char ProtectUnit[60];
  char HarvestersPerRefinery;
  char CarryallsPerRefinery;
  char RecogniseThreat;
  char c_field_1D07_group;
  char c_field_1D08;
  char field_1D09;
  char field_1D0A;
  char field_1D0B;
  int BuildingRepairTicks;
  int UnitRepairTicks;
  char RepairUnits;
  char RepairGuardingUnits;
  char field_1D16;
  char field_1D17;
  int SpecialWeaponDelay;
  char MoraleModifier;
  char __Emergency_1D1D;
  char MinMorale;
  char AirliftPercentage;
  char field_1D20_dontusemorale;
  char field_1D21;
  int16_t w_field_1D22;
  char field_1D24;
  char field_1D25;
  char field_1D26;
  char field_1D27;
  int __BuildDelay_1D28;
  int UnitBuildRate;
  char field_1D30;
  char field_1D31;
  char field_1D32;
  char field_1D33;
  int ProtectTicks;
  int DefendTicks;
  char c_field_1D3C_firstfreegroup;
  char c_field_1D3D_firstfreegroup;
  char field_1D3E;
  char field_1D3F;
  int selected_dw_1D40;
  char AirliftAndAttack;
  char RebuildBuildings;
  char __TimingRandomPercentage3_repair;
  char c_field_1D47_x;
  char c_field_1D48_y;
  char c_field_1D49_fConcrete;
  char c_field_1D4A;
  char field_1D4B;
  int dw_field_1D4C;
  char bitfield_c_field_1D50_fConcrete;
  char __DefenseAreaCount1;
  char __DefenseAreaCount2;
  char field_1D53;
  DefendAreaStruct __DefendAreas[5];
}CAI_;

// ### Templates.bin structs ###

typedef struct UnitAtribStruct
{
  char __OwnerSide;
  char __UnitType;
  char __Armour;
  char __TurningSpeed;
  int __Strength;
  int __Speed;
  char __PrimaryWeapon;
  char __SecondaryWeapon;
  char __RateOfFire;
  char __BarrelTurnSpeed;
  char __ViewDistance;
  char __IsInfantry;
  char field_12;
  char field_13;
  int __UnitArt;
  int __BarrelArt;
  int __Cost;
  int __BuildSpeed;
  char __TechReq;
  char __AvailableInStarport;
  char __HasBarrel;
  char __UpgradesNeeded;
  int __PreReq1;
  char __SideNeeded;
  char __Behavior;
  char c_field_2E;
  char __DestroyAnim;
  int __PreReq2;
  char c_field_34;
  char __CanCrush;
  char __HealthbarSize;
  char __ProjectileShootOffset;
  int Flags;
  char __DirectionFrames[32];
  int __ReportingSounds[3][3];
  int __ConfirmationSounds[3][3];
  int __VoicePriority;
  char __FireFlashAnim;
  char __VehicleType;
  char __MPOnly;
  char field_AB[85];
}UnitAtribStruct;

typedef struct BuildingAtrbStruct
{
  int _____HitPoints;
  char _____OwnerSide;
  char Armour;
  char _____BarrelRotationSpeed;
  char _____RateOfFire;
  int _____ScreenShake;
  char _____PrimaryWeapon;
  char _____SecondaryWeapon;
  char _____SightRadius;
  char _____ActLikeTurret;
  char _____TechLevelBuild;
  char _____TechLevelUpgrade[3];
  int _____BuildingArt;
  int _____BarrelArt;
  int _____CostBuild;
  int _____CostUpgrade1;
  int _____CostUpgrade2;
  int _____CostUpgrade3;
  int _____BuildSpeedBuild;
  int _____BuildSpeedUpgrade1;
  int _____BuildSpeedUpgrade2;
  int _____BuildSpeedUpgrade3;
  int __PowerDrain;
  int _____Prereq1BuildingType;
  char __Prereq1OwnerSide;
  char __Prereq1UpgradesNeeded;
  char field_46;
  char field_47;
  int _____Prereq2BuildingType;
  char _____Prereq2OwnerSide;
  char _____Prereq2UpgradesNeeded;
  char _____RequireEnoughPower;
  char _____DeathExplosion;
  int _____TilesOccupiedAll;
  int _____TilesOccupiedSolid;
  int _____Flags;
  char __Behavior;
  char _____SellPriority;
  char ___c_field_5E_artflag;
  char _____HealthBarSize;
  char _____ExitPoint1X;
  char _____ExitPoint1Y;
  char _____ExitPoint2X;
  char _____ExitPoint2Y;
  char _____DirectionFrames[32];
  char field_84;
  char _____AnimationSpeed;
  char _____ArtHeight;
  char _____ArtWidth;
  char GroupType;
  char _____BuildupFramesToShow;
  char _____BuildupArt;
  char _____BuildingAnimation;
  char _____explosionindex2;
  char field_8D[127];
}BuildingAtrbStruct;

typedef struct BullAtrbStruct
{
  int __ProjectileSpeed;
  int __Damage;
  int Flags;
  int __FiringSound;
  char __ProjectileArt;
  char __HitExplosion;
  char __TrailExplosion;
  char c_field_13;
  char AntiAircraft;
  char Warhead;
  char field_16;
  char field_17;
  int __Range;
}BullAtrbStruct;

typedef struct ExploisonAtrbStruct
{
  char __Type;
  char __FiringPattern;
  char field_2;
  char field_3;
  int __Sound;
}ExploisonAtrbStruct;

typedef struct GroupIDsStruct
{
  char __carryall;
  char Barracks;
  char Wor;
  char LightFactory;
  char HeavyFactory;
  char HighTech;
  char AtHighTech;
  char Refinery;
  char Barrel;
  char Scenery;
  char AtPalace;
  char HarkPalace;
  char OrdPalace;
  char Concrete1;
  char Frigate;
  char MCV;
  char Harvester;
  char Carryall;
  char Ornithopter;
  char DeathHand;
  char Saboteur;
  char Fremen;
  char BARREL;
  char CRATE;
  char SARDDEATH;
  char Debris[24];
  char DebrisCount;
  char SpiceExpl;
  char Devastator;
  char DHM;
  char EX_CrateReveal;
  char EX_CASH;
  char EX_CrateNoMap;
  char EX_CrateStealth;
  char EX_PUFF;
  char EX_WHITEN1;
  char EX_SMOKE3;
  char EX_FLASH;
  char EX_DHM;
  char EX_DEATHHAND;
  char EX_SPICEBLOOM;
  char EX_HARV;
  char EX_HARVDUST0;
  char EX_HARVDUST1;
  char EX_HARVDUST2;
  char EX_HARVDUST3;
  char EX_HARVDUST4;
  char EX_HARVDUST5;
  char EX_HARVDUST6;
  char EX_HARVDUST7;
  char EX_WORMSN1;
  char EX_WORMSN2;
  char EX_WORMSN3;
  char EX_WORMSN4;
  char EX_WORMSN5;
  char EX_WORMSN6;
  char EX_THPUFF;
  char EX_REF;
  char EX_DEVSPARK1;
  char EX_DEVSPARK2;
  char EX_DEVSPARK3;
  char EX_DEVDEATH;
  char Concrete2;
}GroupIDsStruct;

// ### VARS.BIN structs ###

typedef struct VariableStruct
{
  int harvestUnloadDelay;
  int harvestBlobValue;
  int harvestLoadSpiceDelay;
  int starportUpdateDelay;
  int starportStockIncreaseDelay;
  int starportStockIncreaseProb;
  int starportCostVariationPercent;
  int starportFrigateDelay;
  int refineryExplosionOffsetX;
  int refineryExplosionOffsetY;
  int HarvesterDriveDistance;
  int RepairDriveDistance;
  int BuildingRepairValue;
  int UnitRepairValue;
  int SinglePlayerDelay;
  char NumberOfFremen;
  char SandWormAppetite;
  char SandWormInitialSleep;
  char SandWormFedSleep;
  char SandWormShotSleep;
  char NumberOfCrates;
  char CratesPerPlayer;
  char DevastatorExplodeDelay;
  int IgnoreDistance;
  int CrateCash;
  char ShowWarnings;
  char DeathHandAccuracy;
}VariableStruct;

// ### MAP structs ###

typedef enum TileFlags
{
  TileFlags_1_OWNER = 0x1,
  TileFlags_2_OWNER = 0x2,
  TileFlags_4_OWNER = 0x4,
  TileFlags_8_OCC_UNIT = 0x8,
  TileFlags_10_OCC_BUILDING = 0x10,
  TileFlags_20_CSPOT_MID = 0x20,
  TileFlags_40_CSPOT_TR = 0x40,
  TileFlags_80_CSPOT_DR = 0x80,
  TileFlags_100_CSPOT_DL = 0x100,
  TileFlags_200_CSPOT_TL = 0x200,
  TileFlags_400_HAS_WALL = 0x400,
  TileFlags_800_HAS_CONCRETE = 0x800,
  TileFlags_1000 = 0x1000,
  TileFlags_2000_DRIVE_ON = 0x2000,
  TileFlags_4000_WALK_ON = 0x4000,
  TileFlags_8000_BUILD_ON = 0x8000,
  TileFlags_10000_SANDY = 0x10000,
  TileFlags_20000 = 0x20000,
  TileFlags_40000 = 0x40000,
  TileFlags_80000 = 0x80000,
  TileFlags_100000_SPICE = 0x100000,
  TileFlags_200000_SPICE = 0x200000,
  TileFlags_400000_SPICE = 0x400000,
  TileFlags_800000 = 0x800000,
  TileFlags_1000000 = 0x1000000,
  TileFlags_2000000 = 0x2000000,
  TileFlags_4000000 = 0x4000000,
  TileFlags_8000000 = 0x8000000,
  TileFlags_10000000 = 0x10000000,
  TileFlags_20000000_TERRAIN_TYPE2 = 0x20000000,
  TileFlags_40000000_TERRAIN_TYPE2 = 0x40000000,
  TileFlags_80000000_TERRAIN_TYPE2 = 0x80000000,
}TileFlags;

typedef struct GameMapTileStruct
{
  __int16 __tile_index;
  __int16 w_field_2;
  TileFlags __tile_bitflags;
  char __shroud_flags;
  unsigned __int8 c_field_9;
}GameMapTileStruct;

typedef struct GameMapStruct
{
  int width;
  int height;
  GameMapTileStruct map[16384];
}GameMapStruct;

typedef struct CrateStruct
{
  uint8_t __x;
  uint8_t __y;
  uint8_t __type;
  uint8_t __is_active;
  uint8_t __image;
  uint8_t __times_to_respawn;
  uint8_t ext_data_field; // Field for extended crate functionality (mod)
  uint8_t field_7;
  int32_t __timing;
}CrateStruct;

// ### MIS file structs ###

typedef struct MiscData
{
  char Tech[8];
  int  Cash[8];
  char Empty[40];
}MiscData;

typedef struct EventData
{
    uint32_t xpos;
    uint32_t ypos;
    int32_t value;      // Flag value, Message unknown value
    uint8_t num_conditions;
    uint8_t event_type;
    uint8_t amount;         // Number of units, reveal Map radius
    uint8_t side_id;        // Also Set Flag flag number
    uint8_t allegiance_target;
    uint8_t allegiance_type;
    uint8_t deploy_action;
    char condition_index[14];
    char condition_not[14];
    char units[21];
    uint32_t message_index;
}EventData;

typedef struct ConditionData
{
  int32_t time_amount;
  int32_t start_delay;
  int32_t value;
  uint32_t xpos;
  uint32_t ypos;
  float casualties_ratio;
  char side_id;
  char condition_type;
  char building_type;
  char unit_type_or_comparison_function;
}ConditionData;

typedef struct GameEvent // 168 byte
{
    int unknown1;
    int unknown2;
    int ticks;
    
    //cellX 30
    //ticks 8
    //cellY 28
    //Order 35
    //unknown1 24
    //type 38
    //sideId 36
}GameEvent;

// ### Constants ###

enum Sides
{
    SD_ATREIDES,
    SD_HARKONNEN,
    SD_ORDOS,
    SD_EMPEROR,
    SD_FREMEN,
    SD_SMUGGLERS,
    SD_MERCENARIES,
    SD_SANDWORM
};

enum Colors
{
    CL_BLUE,
    CL_RED,
    CL_TEAL,
    CL_PURPLE,
    CL_GRAY,
    CL_BROWN,
    CL_GOLD,
    CL_LIGHTBROWN
};

enum GameEndStates
{
    GES_ENDEDNORMALLY,
    GES_ISURRENDERED,
    GES_OPPONENTSURRENDERED,
    GES_OUTOFSYNC,
    GES_CONNECTIONLOST,
    GES_WASHGAME,
    GES_DRAWGAME,
    GES_UNKNOWNENDSTATE
};

enum GameTypes
{
    GT_SINGLEPLAYER,
    GT_SKIRMISH,
    GT_LAN,
    GT_SERIAL,
    GT_MODEM,
    GT_WOL
};

enum GameStates
{
    GS_INTRO,
    GS_BRIEFING,
    GS_MAINLOOP,
    GS_LOAD,
    GS_SAVE,
    GS_QUIT
};

enum EventTypes
{
    ET_REINFORCE,
    ET_STARPORT_DELIVERY,
    ET_ALLEGIANCE,
    ET_LEAVE,
    ET_BESERK,
    ET_PLAYSOUND,
    ET_SETBUILDRATE,
    ET_SETATTACKBUILDINGRATE,
    ET_SETCASH,
    ET_SETTECH,
    ET_WIN,
    ET_LOSE,
    ET_BLOXFILE,
    ET_ATTRIBFILE,
    ET_REVEAL,
    ET_SHOWTIMER,
    ET_HIDETIMER,
    ET_SHOWMESSAGE,
    ET_ADDUNITS,
    ET_SETFLAG,
    ET_UNUSED,
    ET_PLAYMUSIC
};

enum EventConditions
{
    EC_BUILDINGEXISTS,
    EC_UNITEXISTS,
    EC_INTERVAL,
    EC_TIMER,
    EC_CASUALTIES,
    EC_BASEDESTROYED,
    EC_UNITSDESTROYED,
    EC_REVEALED,
    EC_HARVESTED,
    EC_FLAG
};

typedef enum eCrateType
{
    CT_CASH,
    CT_EXPLODE,
    CT_REVEAL,
    CT_NOMAP,
    CT_UNIT,
    CT_STEALTH,
    CT_UNSUPPORTED6,
    CT_SPICE_BLOOM_SPAWNER,
    CT_SPICE_BLOOM_SMALL,
    CT_SPICE_BLOOM_MEDIUM,
    CT_SPICE_BLOOM_LARGE
}eCrateType;

typedef enum eCrateImage
{
    CI_BLUE_CRATE,
    CI_RED_CRATE,
    CI_GREEN_CRATE,
    CI_PURPLE_CRATE,
    CI_INVISIBLE,
    CI_SMALL_BLOOM,
    CI_MEDIUM_BLOOM,
    CI_LARGE_BLOOM
}eCrateImage;

// Side (HouseClass)
#define HC_SIDEID 0x24252
#define HC_CREDITS 0x2425C
#define HC_SILO_CREDITS 0x24254
#define HC_SPICE_HARVESTED 0x2477C
#define HC_BUILDINGS_DESTROYED 0x24C94
#define HC_UNITS_KILLED 0x24C90
#define HC_BUILDINGS_OWNED 0x24784
//not sure about this one
#define HC_BUILDING_PRODUCTION_AVAILABLE 0x2652D
//0x5A00 = 100%
#define HC_BUILDING_PRODUCTION_PROGRESS 0x2651E
#define HC_HARKONNEN_PALACE_EXISTS 0x260AA
#define HC_CURRENTLY_PRODUCED_BUILDING_ID 0x2651C

// Special Weapons
#define SW_SABOTEUR 4
#define SW_AIRSTRIKE 9
#define SW_DEATH_HAND_ROCKET 10
#define SW_FREMEN 12

// ### Variables ###

//Multiplayer settings
extern char NetworkGame;
extern unsigned char NetUnitCount;
extern unsigned char NetTechLevel;
extern unsigned short NetStartingCredits;
extern unsigned char NetAIPlayers;
extern bool NetCrates;
extern unsigned char NetWorms;
extern char NetPlayerName[];
extern unsigned char NetPlayerSide;
extern unsigned char NetPlayerColor;
extern unsigned char NetPlayerHandicap;
extern char NetIPAddress[];
extern char NetModemPhone[];
extern unsigned char NetSerialComPort;
extern char NetMap[];
extern char LoginName[];
extern int NetPlayerCount;
extern unsigned int NetPlayersStartingCredits[];
extern char NetPlayersTechLevel[];
extern char NetPlayersColor[]; //Actually there is more than just the colors, not sure what... struct size 60byte, first byte = color
extern char MissionMap[];
extern char NetPlayerNamesArray[6][60];

//Game settings
extern bool FirstTimePlay;
extern unsigned int ScrollRate;
extern unsigned short GameSpeed;
extern unsigned int GameBitsPerPixel;
extern unsigned int SFXVolume;
extern unsigned int MusicVolume;
extern bool MoviesEnabled;
extern bool SoundsEnabled;
extern bool JumpToWOLLogin;
extern bool VideoBackBuffer;
extern bool DebugMode;
extern bool ForceNoCD;
extern bool FullScreen;
extern bool Force480Height;

//WOL
extern bool AllowFindUser;
extern bool AllowPageUser;
extern bool LanguageFilter;
extern bool ShowAllGames;
extern bool Microphone;
extern unsigned char WOLMaxPlayers;
extern bool WOLTournamentGame;
extern bool WOLPrivateGame;
extern int WOLGameId;
extern int GameEndTickCount;

//Screen display stuff
extern int ScreenWidth;
extern int ScreenHeight;
extern int GameWidth;
extern int GameHeight;
extern int CreditsUIPosX;
extern int PowerBarUIPosX;
extern int PowerBarUIPosY;
extern int SideBarPanelRightUIPosX;
extern int SideBarPanelLeftUIPosX;
extern int SideBarPanelsPosY;
extern int SideBarIconCount;
extern int SideBarIconHeight;
extern int MiniMapUIPosX;
extern int SideBarWidth;
extern int BattleFieldWidth;
extern int BattleFieldHeight;
extern int OptionsBarHeight;
extern int CurrentCursorId;

//Others

extern unsigned char MissionNumber;



extern int                  GameState;

extern short                DifficultyLevel;
extern int                  gBitsPerPixel;
extern int                  MousePositionX;
extern int                  MousePositionY;
extern int                  RandSeed;
extern CAI_                 gAIArray[];
extern char                 ResourcePath[];
extern char                 MoviesResourcePath[];
extern char                 MusicResourcePath[];
extern char                 MissionsResourcePath[];
extern char                 MapsResourcePath[];
extern unsigned int         gGameTicks;
extern char                 _cheatstates[8];
extern char                 _radarcolor_byte_517780_spicecolor;
extern short                _radarcolor_word_517898_spicecolor;

extern short                _ScreenShakes;
extern CrateStruct          gCrates[30];
extern GameMapStruct        gGameMap;
extern int                  gGameMapWidth;
extern int                  gGameMapHeight;
extern int                  _CellNumbersWidthSpan[128];
extern char *               _ptr_circle_1x1grid;
extern int                  _mapvisstate_548010;

extern bool                 gUnitsExist[];
extern int                  gTimerValue;
extern bool                 gWin;
extern bool                 gLose;
extern ConditionData        gConditionArray[];
extern bool                 gBuildingsExist[];
extern MiscData             gMiscData;
extern VariableStruct       _gVariables;


extern GroupIDsStruct       _templates_GroupIDs;
extern void *               _RadarMap1;
extern char                 _templates_UnitTypeCount;

extern UnitAtribStruct      _templates_unitattribs[60];
extern void *               _RadarMap2;
extern ExploisonAtrbStruct  _templates_explosionattribs[64];
extern BullAtrbStruct       _templates_bulletattribs[64];
extern bool                 SpawnLocationUsedBoolArray[];
extern int                  SoundClassObject;
extern int                  CUIManagerObject;

extern int                  GameType;


extern bool                 BitsPerPixelChanged;


extern unsigned char        MySideID;
extern unsigned char        gSideId;
extern unsigned char        gDiplomacy[8][8];
extern int                  OSMajorVersion;
extern int                  OSMinorVersion;

// ### Functions ###

// drawing
void            Load_Image(char *fileName, int unk);
void            Image__BlitClipTImage(int *unk1, int x, int y, int *image, int unk2, int unk3, int unk4);

// Others
bool __stdcall  IsLocalIp(char *Ip);

bool            IsCurrentlyShown(char *menu);
void            WOL__StartGuestINetGame();
void            WOL__StartHostINetGame();
void            WOL__OpenWebsite(char *URL);
void            QueueMessage(const char *message, int type);
void            DebugFatal(char *caption, char *format, ...);
// Graphlib
void            Graphlib__TextOnScreen(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void            Graphlib__DrawRightAlignedText(int *image, char *text, int x, int y, bool bold_unk, int color_unk, int unk2);
void            Graphlib__DrawTextWithBlackShadow(int *image, char *text, int x, int y, int unk, int color_unk);
void            Graphlib__LoadFontFile();
// Image
void            BlitClipTImage1(TImage *lpTITo, int toX, int toY, TImage *lpTIFrom, RECT *rect, bool trans, int a7);
void            ClearTImage(TImage *a1, int color, int unusable);
// Other
int             GetRandomValue(char *, int);
// Map
int             RevealMap();
void            Map__PlayerDefeated(uint8_t sideId);

void            RevealCircle(int x, int y, int size);
char            GetFreeCrateIndex();
void            PlaceCrate(int x, int y, int timing, eCrateType type, eCrateImage image, int respawn_count);
eCrateType      GetCrateFromMap(int x, int y);
void            UpdateSpiceInRegion(RECT *a1);
void            SpiceMound(unsigned __int8 xpos, unsigned __int8 ypos, int range);
void            RecycleCrate(unsigned __int8 index);
int             GetMapVisState();
// Memory
void *          Memory__HeapAllocWrapper(size_t size, char *debugString);
// Mission

void            Mission__CheckEvents();
void            Mission__LoadVarsFile();

// Model

index           ModelAddUnit(unsigned char side, unsigned char type, unsigned char add_at_x, unsigned char add_at_y, unsigned char move_to_x, unsigned char move_to_y, int pixel_offset_x, int pixel_offset_y);

signed __int16  ModelAddExplosion(int side, unsigned __int8 explosionType, unsigned __int16 x, unsigned __int16 y, int a5, int a6, char a7, int a8, int a9);
int             Model__GenerateUnitMoveOrder(int,int,int);
int             Model__GenerateUnitAttackUnitOrder(int,int,int);
void            Model__ResetVars();
void            Model__ExecuteGameEvent(GameEvent event);
// Setup
void            Setup__LoadUIBBFile();

void            SetPixelOnRadar8(unsigned __int8 x, unsigned __int8 y, char color);
void            SetPixelOnRadar16(unsigned __int8 x, unsigned __int8 y, __int16 color);

// CSide
void __thiscall CSide__update_list_of_available_buildings_and_units(CSide *side);
uint8_t __thiscall CSide__MyVersionOfUnit(CSide *this, char unit, bool bool1);
void __thiscall CSide_add_cash_drip(CSide *this, int a2);
void __thiscall CSide__BlowupAll_surrender(CSide *side);
void            CSide__SurrenderAbort(bool unknown);
void __thiscall CSide__reset_enemy(CSide *this, char a2);
// Sound
void            PlaySoundAt(int id, unsigned __int8 xpos, unsigned __int8 ypos);
void            Sound__PlaySample(int id, char state, int time, int priority);
void __thiscall Sound__LoadMusicFile(int this, char *fileName);
void __thiscall Sound__SetMusicVolume(int soundClassObject, int volume);
void            Sound__PlayMusic(char *fileName);
// CUIManager
void            CUIManager__JumpToMenu(char *menu);
void __thiscall CUIManager__LoadMission(int cUIManagerObject, char *map);
void            CUIManager__LoadDune2000Cfg();
void            CUIManager__SaveDune2000Cfg();
void __thiscall CUIManager__StartGame(int cUIManagerObject);

void            CUIManager__CreateWigNet();
void            CUIManager__GetCD(char *arg);
// Data

char *          Data__GetTextString(int stringId, bool showError);
int             Data__GetSoundTableID(const char *key);
// Other
void            DestroyBuilding(int side, int objIndex, char a3);
void            DestroyUnit(eSideType side, __int16 index);
char            DamageTiles(unsigned int xpos, unsigned int ypos, unsigned int a3, unsigned __int8 bulletType, int ai_side, __int16 ai_index, char a7);
void            MakeUnitsStealthInRange(unsigned __int8 x, unsigned __int8 y, eSideType side);
int             side_mapvis_49F4D0(eSideType side);


CSide *         GetSide(int sideId);
bool            FindNearestFreeTile(unsigned char *x, unsigned char *y, unsigned char a3);
void            cinit();

