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
  int32_t dw_field_4;
  int32_t dw_field_8_worthamount;
  int32_t dw_field_C_morale_strength;
  int32_t dw_field_10_morale_strength;
  int32_t dw_field_14;
  int32_t dw_field_18_counter;
  char Task_1C; //eTaskType
  char c_field_1D;
  char field_1E;
  char field_1F;
  int32_t dw_field_20__flags;
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
  int32_t dw_field_8_Strength;
  int32_t field_C_StartDelay;
  int32_t *field_10_Unknown;
}DefendAreaStruct;

typedef struct CAI_
{
  uint8_t AISide;
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
  int32_t dw_field_C_fConcrete;
  int32_t dw_field_10;
  int32_t SquishRange;
  int32_t dw_field_18_bestsidecountdown;
  char SquishInfantry;
  char __GuardGroupSizeNot0_1D;
  char __GoBeserk_OtherStates;
  char c_field_1F;
  char field_20;
  char field_21;
  char field_22;
  char field_23;
  int32_t __best_base_pos_x;
  int32_t __best_base_pos_y;
  AIGroup __AIGroups_2C[32];
  StructForCAI __InitialBuildingPositions[150];
  char __BuildingCount;
  char FreePercentage;
  char field_F0E;
  char field_F0F;
  int32_t FirstAttackBuildingDelay;
  int32_t TimeBetweenBuildingAttacks;
  int32_t __AttackBuildingDelay;
  int32_t dw_field_F1C;
  int32_t dw_field_F20;
  int32_t __TimingRandomPercentage2;
  int32_t EmergencyBuildRate;
  int32_t dw_field_F2C_x_KickUnitsOutofArea;
  int32_t dw_field_F30_y_KickUnitsOutofArea;
  int32_t dw_field_F34_x_KickUnitsOutofArea;
  int32_t dw_field_F38_y_KickUnitsOutofArea;
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
  int32_t MinBaseStrength;
  int32_t MaxBaseStrength;
  int32_t BaseDefenceRadius;
  char c_field_F54_carryall;
  char c_field_F55;
  char GuardBuildings;
  char base_x_field_F57;
  char base_y_field_F58;
  char field_F59;
  char field_F5A;
  char field_F5B;
  int32_t GuardGroupSize;
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
  int32_t CashStash[9];
  char Morale[19];
  char DontFaceUnitsAwayFromConYard; // Newly added parameter (mod)
  float DefendBuildingPriority[100];
  float AttackBuildingPriority[100];
  float BuildingRepairValue[100];
  int32_t TimeBeforeRebuilding[100];
  float BuildRatio[100];
  int32_t MaxBuildings[100];
  char c_field_19EC;
  char AttackSidePriority[8];
  char AutoBerserkMode; // Newly added parameter (mod)
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
  int32_t BuildingRepairTicks;
  int32_t UnitRepairTicks;
  char RepairUnits;
  char RepairGuardingUnits;
  char field_1D16;
  char field_1D17;
  int32_t SpecialWeaponDelay;
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
  int32_t __BuildDelay_1D28;
  int32_t UnitBuildRate;
  char field_1D30;
  char field_1D31;
  char field_1D32;
  char field_1D33;
  int32_t ProtectTicks;
  int32_t DefendTicks;
  char c_field_1D3C_firstfreegroup;
  char c_field_1D3D_firstfreegroup;
  char field_1D3E;
  char field_1D3F;
  int32_t selected_dw_1D40;
  char AirliftAndAttack;
  char RebuildBuildings;
  char __RepairPadBuildingGroup;
  char __BuildingTobuildX;
  char __BuildingToBuildY;
  char __BuildingToBuildActive;
  char __BuildingToBuildType;
  char field_1D4B;
  int32_t __BuildingToBuildWaitingForFreeSpaceCount;
  char __BuildingTobuildConcreteNeededBitfield;
  char __DefenseAreaCount1;
  char __DefenseAreaCount2;
  char field_1D53;
  DefendAreaStruct __DefendAreas[5];
}CAI_;
