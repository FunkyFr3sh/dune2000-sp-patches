
enum EventTypes
{
  ET_REINFORCEMENT,
  ET_STARPORT_DELIVERY,
  ET_ALLEGIANCE,
  ET_LEAVE,
  ET_BESERK,
  ET_PLAYSOUND,
  ET_SETBUILDRATE,
  ET_SETATTACKBUILDINGRATE,
  ET_SETCASH,
  ET_SETTECH,
  ET_WIN,
  ET_LOSE,
  ET_SWITCH_MY_SIDE,
  ET_HIDE_MAP,
  ET_REVEAL,
  ET_SHOWTIMER,
  ET_HIDETIMER,
  ET_SHOWMESSAGE,
  ET_UNIT_SPAWN,
  ET_SET_FLAG,
  ET_UN_BLOCK_EVENT,
  ET_PLAY_MUSIC,
  ET_DAMAGE_TILES,
  ET_ADD_UNIT,
  ET_ADD_BUILDING,
  ET_ADD_PROJECTILE,
  ET_ADD_EXPLOSION,
  ET_ADD_CRATE,
  ET_ADD_CONCRETE,
  ET_SPICE_BLOOM,
  ET_SHAKE_SCREEN,
  ET_CENTER_VIEWPORT,
  ET_CHANGE_MAP_BLOCK,
  ET_TRANSFORM_TILES,
  ET_CHANGE_TILE_ATTRIBUTES,
  ET_CHANGE_TILE_DAMAGE
};

enum ConditionTypes
{
  COND_BUILDINGEXISTS,
  COND_UNITEXISTS,
  COND_INTERVAL,
  COND_TIMER,
  COND_CASUALTIES,
  COND_BASEDESTROYED,
  COND_UNITSDESTROYED,
  COND_REVEALED,
  COND_HARVESTED,
  COND_FLAG
};

enum EventFlags
{
  EVENTFLAG_AUTO_BLOCK = 1,
  EVENTFLAG_BLOCKED = 2,
  EVENTFLAG_CONDITIONS_OR = 4
};

typedef struct EventContext
{
  int coord_x[4];
  int coord_y[4];
  int args[6];
  char *data;
} EventContext;

bool ProcessCondition(int condition_index);
void ProcessEvent(int event_index);
void ExecuteEventAction(int event_type, EventContext *e);
