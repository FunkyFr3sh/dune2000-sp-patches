
bool Cond_BuildingExists(int side_id, int building_type);
bool Cond_UnitExists(int side_id, int unit_type);
bool Cond_Interval(bool inactive, int start_delay, int next_delay, int run_count, int base_time);
bool Cond_Timer(bool inactive, int comp_func, int time_amount, int time_shift, int base_time);
bool Cond_Casualties(int side_id, int threshold, float proportion);
bool Cond_Revealed(int xpos, int ypos, int run_count, ConditionData *condition);
bool Cond_Harvested(int side_id, bool specific_side, int credits);
