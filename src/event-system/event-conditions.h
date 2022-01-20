typedef enum eCreditsCheck
{
  CREDITSCHECK_TOTAL,
  CREDITSCHECK_SPICE,
  CREDITSCHECK_CASH,
  CREDITSCHECK_MAX_STORAGE
}eCreditsCheck;

typedef enum ePowerCheck
{
  POWERCHECK_PERCENT,
  POWERCHECK_TOTAL_OUTPUT,
  POWERCHECK_TOTAL_DRAIN,
  POWERCHECK_EXTRA_OUTPUT,
  POWERCHECK_EXTRA_DRAIN
}ePowerCheck;

bool Cond_BuildingExists(int side_id, int building_type);
bool Cond_UnitExists(int side_id, int unit_type);
bool Cond_Interval(bool inactive, int start_delay, int next_delay, int run_count, ConditionData *condition);
bool Cond_Timer(bool inactive, int comp_func, int time_amount, int time_shift, ConditionData *condition);
bool Cond_Casualties(int side_id, int threshold, float proportion);
bool Cond_Revealed(int xpos, int ypos, int run_count, ConditionData *condition);
bool Cond_Credits(int side_id, eCreditsCheck check_for, bool specific_side, int credits);
bool Cond_RandomChance(int range, int min_value, int max_value, int fixed_result, ConditionData *condition);
bool Cond_RandomInterval(int status, int run_count, int start_delay, int min_delay, int max_delay, ConditionData *condition);
bool Cond_CheckUnits(ConditionData *condition);
bool Cond_CheckBuildings(ConditionData *condition);
bool Cond_CheckCrates(ConditionData *condition);
bool Cond_CheckTiles(ConditionData *condition);
bool Cond_SpiceInArea(int xpos, int ypos, int width, int height, int amount);
bool Cond_DamageInArea(int xpos, int ypos, int width, int height, bool specific_terrain, int terrain_type, int damage);
bool Cond_Power(int side_id, ePowerCheck check_for, bool equal, int value);
bool Cond_StarportStock(int side_id, int unit_type, bool total, bool equal, int value);
bool Cond_StarportCost(int side_id, int unit_type, bool equal, int value);
bool Cond_StarportPick(int side_id, int unit_type, bool total, bool equal, int value);
bool Cond_BuildingIcon(int side_id, bool status, bool any_building, int building_group);
bool Cond_UnitIcon(int side_id, bool status, bool any_unit, int queue, int unit_group);
bool Cond_UpgradeIcon(int side_id, bool any_building, int building_group);
bool Cond_UnitsBuilt(int side_id, int unit_type, bool total, bool equal, int value);
bool Cond_BuildingsBuilt(int side_id, int building_type, bool total, bool equal, int value);
bool Cond_UnitsLost(int side_id, int unit_type, bool total, bool equal, int value);
bool Cond_BuildingsLost(int side_id, bool equal, int value);
bool Cond_UnitsKilled(int side_id, int unit_type, int enemy, bool total, bool equal, int value);
bool Cond_BuildingsKilled(int side_id, int building_type, int enemy, bool total, bool equal, int value);
