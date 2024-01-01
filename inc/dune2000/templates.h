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
  // Additional fields for extended functionality (mod)
  char UnitUpgradeAllowed;
  unsigned char UnitUpgradeTargetType;
  char MovementRestriction;
  char field_AD[82];
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
  uint8_t _____ArtHeight;
  uint8_t _____ArtWidth;
  char GroupType;
  char _____BuildupFramesToShow;
  char _____BuildupArt;
  char _____BuildingAnimation;
  char _____explosionindex2;
  // Additional fields for extended functionality (mod)
  char BuildRestriction;
  char BuildMaxDistance;
  char field_8D[125];
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

typedef struct WarheadStruct
{
  char Verses[12];
  int Radius;
  char InfDeath;
}WarheadStruct;

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
  char Debris[8]; // Array stripped from 24 to 8 members (mod)
  char CrateAnimations[16]; // Free bytes reused for more customized crate pick-up animations (mod)
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
