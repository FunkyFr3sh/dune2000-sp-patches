
#define MAX_EXPLOSION_TYPES 128
#define MAX_ORIG_EXPLOSION_TYPES 64

// Templates data
extern ExploisonAtrbStruct  _templates_explosionattribs[MAX_EXPLOSION_TYPES];
extern char                 _templates_ExplosionNameList[MAX_EXPLOSION_TYPES][50];
extern int                  _templates_AnimationArtFrames[MAX_EXPLOSION_TYPES];
extern int                  _templates_Explosiondata_AnimationArtFlags[MAX_EXPLOSION_TYPES];
extern TImage *             gExplosionElements[MAX_EXPLOSION_TYPES][40];

void ReadTemplates();
