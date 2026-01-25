#include "macros/patch.h"
#include "dune2000.h"
#include "extended-templates.h"

// Custom implementation of function DestroyBuilding
DETOUR(0x0049CA10, 0x0049CDD7, _Mod__DestroyBuilding);

void Mod__DestroyBuilding(eSideType side_id, short objIndex, char a3)
{
  int art_height; // edi
  unsigned char side_id_; // bl
  Building *bld; // eax
  Building *bld_; // esi
  int building_type; // ecx
  int posy; // kr00_4
  unsigned int xpos; // ebp
  signed int check_bit; // edx
  signed int tile_counter; // ecx
  int random_x; // esi
  int random_y; // edi
  unsigned short random_xpos; // si
  int random_ypos; // edi
  char random_val; // al
  TImage *base_image; // eax
  unsigned int building_center_xpos; // esi
  unsigned int building_center_ypos; // edi
  unsigned char debris_number; // ST50_1
  char random_val2; // al
  short bullet_index; // ax MAPDST
  int bullet_index_; // esi
  UnitFlags speed_x; // edi
  int speed_y; // edi
  int posz; // edi
  int falling_speed; // edi
  int *bullet_flags_ptr; // esi
  int bullet_flags; // eax
  int building_flags; // eax
  int tiles_occupied_solid; // [esp+8h] [ebp-20h]
  int explosionType; // [esp+Ch] [ebp-1Ch]
  unsigned int counter; // [esp+10h] [ebp-18h]
  Building *bld__; // [esp+14h] [ebp-14h]
  int xpos_; // [esp+18h] [ebp-10h]
  int ypos_top; // [esp+1Ch] [ebp-Ch] MAPDST
  unsigned int num_solid_tiles_times_2; // [esp+24h] [ebp-4h]
  int num_solid_tiles; // [esp+2Ch] [ebp+4h]

  side_id_ = side_id;
  bld = GetBuilding(side_id, objIndex);
  bld_ = bld;
  bld__ = bld;
  if ( bld )
  {
    bld->__IsSelected = 0;
    building_type = bld->Type;
    explosionType = _templates_buildattribs[building_type]._____DeathExplosion;
    if ( explosionType >= 0 )
    {
      num_solid_tiles = 0;
      art_height = _templates_buildattribs[building_type]._____ArtHeight;
      posy = bld->__PosY;
      tiles_occupied_solid = _templates_buildattribs[building_type]._____TilesOccupiedSolid;
      xpos = bld->__PosX / 0x10000;
      xpos_ = bld->__PosX / 0x10000;
      ypos_top = posy / 0x10000 - art_height;
      check_bit = 1;
      tile_counter = 16;
      do
      {
        if ( check_bit & tiles_occupied_solid )
        {
          ++num_solid_tiles;
        }
        check_bit *= 2;
        --tile_counter;
      }
      while ( tile_counter );
      PlaySoundAt(
        _templates_explosionattribs[explosionType].__Sound,
        xpos >> 5,
        (unsigned int)(posy / 0x10000 - art_height) >> 5);
      if ( _templates_buildattribs[bld_->Type]._____ScreenShake )
      {
        _ScreenShakes = 10;
      }
      counter = 0;
      num_solid_tiles_times_2 = 2 * num_solid_tiles;
      if ( 2 * num_solid_tiles )
      {
        while ( 1 )
        {
          do
          {
            random_x = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7048) & 3;
            random_y = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7049) & 3;
          }
          while ( !((1 << (random_x + 4 * random_y)) & tiles_occupied_solid) );
          xpos = (unsigned short)xpos;
          random_xpos = (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7054) & 0x1F)
                      + xpos
                      + 32 * random_x;
          ypos_top = (unsigned short)ypos_top;
          random_ypos = (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7055) & 0x1F)
                      + (unsigned short)ypos_top
                      + 32 * random_y;
          random_val = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7056);
          ModelAddExplosion(
            (eSideType)side_id_,
            explosionType,
            random_xpos,
            random_ypos,
            0,
            EXPFLAGS_20,
            (random_val & 0x1Fu) > 0,
            0,
            0);
          if ( _templates_GroupIDs.DebrisCount )
          {
            base_image = gBuildingElements[_templates_buildattribs[bld__->Type]._____BuildingArt][0];
            if ( !base_image )
            {
              base_image = dataimage[_templates_buildattribs[bld__->Type]._____BuildingArt];
            }
            building_center_xpos = xpos + ((unsigned int)(unsigned char)base_image->c_field_1A__Width2 >> 1);
            building_center_ypos = ypos_top + ((unsigned int)(unsigned char)base_image->c_field_19__Height2 >> 1);
            debris_number = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7068)
                          % (unsigned int)(unsigned char)_templates_GroupIDs.DebrisCount;
            random_val2 = GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7070);
            bullet_index = ModelAddBullet(
                             side_id_,
                             _templates_GroupIDs.Debris[debris_number],
                             (random_val2 & 0xF) + 8,
                             objIndex,
                             building_center_xpos,
                             building_center_ypos,
                             building_center_xpos,
                             building_center_ypos,
                             -1,
                             -1);
            if ( bullet_index != -1 )
            {
              if ( side_id_ >= 8u || (unsigned short)bullet_index > 1000u )
              {
                DebugFatal("Unit.cpp", "Oh dear. Stuff out of range");
              }
              bullet_index_ = (unsigned short)bullet_index;
              Bullet *bullet = (Bullet *)&GetSide(side_id_)->__ObjectArray[bullet_index_];
              speed_x = ((GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7077) & 0x1F) - 16) << 13;
              bullet->__SpeedX = speed_x;
              speed_y = ((GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7078) & 0x1F) - 16) << 13;
              bullet->__SpeedY = speed_y;
              posz = (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7079) & 3) << 16;
              bullet->__PosZHeight = posz;
              falling_speed = (-2 - (GetRandomValue("C:\\MsDev\\Projects\\July2000\\code\\unit.cpp", 7080) & 1)) << 16;
              bullet->__FallingSpeed = falling_speed;
              bullet_flags_ptr = (int *)&bullet->Flags;
              bullet_flags = *bullet_flags_ptr;
              bullet_flags |= BULFLAGS_400;
              *bullet_flags_ptr = bullet_flags;
            }
          }
          if ( ++counter >= num_solid_tiles_times_2 )
          {
            break;
          }
          xpos = xpos_;
        }
        bld_ = bld__;
      }
    }
    bld_->__DeadStateTimeCounter = 30;
    if ( a3 )
    {
      building_flags = bld_->Flags;
      building_flags |= BFLAGS_2000;
      bld_->Flags = building_flags;
    }
    SetBuildingState(bld_, BLD_STATE_17_DEAD);
  }
}
