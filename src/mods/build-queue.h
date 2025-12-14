// Support functions
bool IsUnitOnHold(int side_id, int unit_type);
bool IsUnitBuilt(int side_id, int unit_type);
int GetBuildQueueNumber(int unit_type);
int GetUnitsQueuedCount(int side_id, int unit_type);
// Action functions
void InitBuildQueues(void);
void AddToBuildQueue(int side_id, int unit_type, bool bulk_increment, bool priority);
void RemoveFromBuildQueue(int side_id, int unit_type, bool bulk_increment);
void ProcessBuildQueues(void);
