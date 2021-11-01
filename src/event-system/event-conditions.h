
bool Cond_BuildingExists(int side_id, int building_type);
bool Cond_UnitExists(int side_id, int unit_type);
bool Cond_Interval(bool inactive, int start_delay, int next_delay, int run_count, ConditionData *condition);
bool Cond_Timer(bool inactive, int comp_func, int time_amount, int time_shift, ConditionData *condition);
bool Cond_Casualties(int side_id, int threshold, float proportion);
bool Cond_Revealed(int xpos, int ypos, int run_count, ConditionData *condition);
bool Cond_Harvested(int side_id, bool specific_side, int credits);
bool Cond_RandomChance(int range, int min_value, int max_value, int fixed_result, ConditionData *condition);
bool Cond_RandomInterval(int status, int run_count, int start_delay, int min_delay, int max_delay, ConditionData *condition);
