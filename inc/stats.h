typedef struct UnitTracker //do not change the order/size!
{
    int LightInfantry; //id 0
    int Trooper; //id 1 ...
    int Engineer;
    int ThumperInfantry;
    int Sardaukar;
    int Trike;
    int Raider;
    int Quad;
    int Harvester;
    int CombatTankAtreides;
    int CombatTankHarkonnen;
    int CombatTankOrdos;
    int MCV;
    int MissileTank;
    int Deviator;
    int SiegeTank;
    int SonicTank;
    int Devastator;
    int Carryall;
    int Carryall2;
    int Ornithopter;
    int StealthFremen;
    int Fremen;
    int Saboteur;
    int DeathHandRocket;
    int Sandworm;
    int ChoamFrigate;
    int Grenadier;
    int StealthRaider;
    int SardaukarMP;
}UnitTracker;

typedef struct BuildingTracker //do not change the order/size!
{
    int ConstructionYardAtreides; //id 0
    int ConstructionYardHarkonnen; //id 1 ...
    int ConstructionYardOrdos;
    int ConstructionYardUnknown1; //black
    int Unknown1; //Building type is concrete - display.cpp
    int Unknown2; //Building type is concrete - display.cpp
    int Unknown3; //Building type is concrete - display.cpp
    int Unknown4; //Building type is concrete - display.cpp
    int Unknown5; //Building type is concrete - display.cpp
    int ConstructionYardUnknown2; //white
    int WindTrapAtreides;
    int WindTrapHarkonnen;
    int WindTrapOrdos; 
    int BarracksAtreides;
    int BarracksHarkonnen;
    int BarracksOrdos;
    int Sietch;
    int WallAtreides;
    int WallHarkonnen;
    int WallOrdos;
    int RefineryAtreides;
    int RefineryHarkonnen;
    int RefineryOrdos;
    int GunTurretAtreides;
    int GunTurretHarkonnen;
    int GunTurretOrdos;
    int OutpostAtreides;
    int OutpostHarkonnen;
    int OutpostOrdos;
    int RocketTurretAtreides;
    int RocketTurretHarkonnen;
    int RocketTurretOrdos;
    int HighTechFactoryAtreides;
    int HighTechFactoryHarkonnen;
    int HighTechFactoryOrdos;
    int LightFactoryAtreides;
    int LightFactoryHarkonnen;
    int LightFactoryOrdos;
    int SiloAtreides;
    int SiloHarkonnen;
    int SiloOrdos; 
    int HeavyFactoryAtreides;
    int HeavyFactoryHarkonnen;
    int HeavyFactoryOrdos;
    int HeavyFactoryUnknown1; //gold
    int HeavyFactoryUnknown2; //black
    int StarportAtreides;
    int StarportHarkonnen;
    int StarportOrdos;
    int StarportUnknown; //gold
    int RepairPadAtreides;
    int RepairPadHarkonnen;
    int RepairPadOrdos;
    int IXResearchCenterAtreides;
    int IXResearchCenterHarkonnen;
    int IXResearchCenterOrdos;
    int AtreidesPalace;
    int HarkonnenPalace;
    int OrdosPalace; 
    int EmperorsPalace; //60
    int ModifiedOutpost1;
    int ModifiedOutpost2;
}BuildingTracker;

int GetInfantryOwned(int house);
int GetHeavyVehiclesOwned(int house);
int GetLightVehiclesOwned(int house);
int GetAirUnitsOwned(int house);

extern UnitTracker PlayersUnitsOwned[8];
extern BuildingTracker PlayersBuildingsOwned[8];
