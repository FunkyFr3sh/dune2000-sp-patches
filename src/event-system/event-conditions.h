
bool Cond_BuildingExists(int side_id, int building_type);
bool Cond_UnitExists(int side_id, int unit_type);
bool Cond_Interval(int start_delay, int time_amount, int run_count, ConditionData *condition);
bool Cond_Timer(int comp_func, int time_amount, int time_shift);
bool Cond_Casualties(int side_id, int threshold, float proportion);
bool Cond_Revealed(int xpos, int ypos, int run_count, ConditionData *condition);
bool Cond_Harvested(int side_id, bool specific_side, int credits);
