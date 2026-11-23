extern const int restrictions_check[32];

bool CanTargetBeAttacked(int source_xpos, int source_ypos, int target_xpos, int target_ypos, int primary_weapon_type, int secondary_weapon_type, int air_target);
char Ext__NewBestBullet(char primary_weapon, char secondary_weapon, int priority, int source_xpos, int source_ypos, int target_xpos, int target_ypos, char enemy_side_id, short enemy_index, char enemy_object_type);
