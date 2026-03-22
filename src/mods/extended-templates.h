
// Templates data
extern ExploisonAtrbStruct  _templates_explosionattribs[MAX_EXPLOSION_TYPES];
extern char                 _templates_ExplosionNameList[MAX_EXPLOSION_TYPES][50];
extern int                  _templates_AnimationArtFrames[MAX_EXPLOSION_TYPES];
extern int                  _templates_Explosiondata_AnimationArtFlags[MAX_EXPLOSION_TYPES];
extern TImage *             gExplosionElements[MAX_EXPLOSION_TYPES][40];

// Armour data
extern WarheadStruct        _WarheadData[MAX_WARHEAD_TYPES];
extern char                 _WarheadNames[MAX_WARHEAD_TYPES][50];
extern char                 _ArmourNames[MAX_ARMOUR_TYPES][50];

void ReadTemplates();
