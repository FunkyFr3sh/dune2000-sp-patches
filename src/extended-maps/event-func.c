#include <windows.h>
#include <stdio.h>
#include "macros/patch.h"
#include "dune2000.h"
#include "patch.h"
#include "ini.h"
#include "utils.h"

CALL(0x00453F3C, _Mod__HandleEvent);

void Mod__HandleEvent(EventData *event)
{
  switch ( event->event_type )
  {
    case ET_REINFORCE:
    case ET_STARPORT_DELIVERY:
    {
      CSide *side = GetSide(event->side_id);
      if (event->event_type == ET_STARPORT_DELIVERY && side->__primary_starport == -1)
        break;
      int found_free_slot = 0;
      for (int i = 0; i <= 10; i++)
      {
        if (i == 10)
        {
          DebugFatal("MISSION.CPP", "Too many deliveries");
        }
        if (!side->delivery_field_262EC[i].__is_active)
        {
          found_free_slot = i;
          break;
        }
      }
      side->delivery_field_262EC[found_free_slot].__is_active = 1;
      side->delivery_field_262EC[found_free_slot].c_field_2 = 0;
      side->delivery_field_262EC[found_free_slot].__xpos = event->xpos;
      side->delivery_field_262EC[found_free_slot].__ypos = event->ypos;
      side->delivery_field_262EC[found_free_slot].__delivery_time = gGameTicks;
      side->delivery_field_262EC[found_free_slot].__deploy_action = event->deploy_action;
      side->delivery_field_262EC[found_free_slot].__delivery_type = (event->event_type == ET_STARPORT_DELIVERY)?2:1;
      for (int i = 0; i < event->amount; i++)
      {
        side->delivery_field_262EC[found_free_slot].__units[i] = event->units[i];
      }
      side->delivery_field_262EC[found_free_slot].__units[event->amount] = -1;
      break;
    }
    case ET_ALLEGIANCE:
    {
      gDiplomacy[event->side_id][event->allegiance_target] = event->allegiance_type;
      if ( event->allegiance_type == 0 || event->allegiance_type == 2 )
      {
        CSide__reset_enemy(GetSide(event->side_id), event->allegiance_target);
      }
      break;
    }
    case ET_LEAVE:
      CSide__BlowupAll_surrender(GetSide(event->side_id));
      break;      
    case ET_BESERK:
      gAIArray[event->side_id].__GoBeserk_OtherStates = 1;
      break;
    case ET_PLAYSOUND:
      Sound__PlaySample(event->value, 0, 0, 0);
      break;
    case ET_SETBUILDRATE:
      gAIArray[event->side_id].UnitBuildRate = event->value;
      break;
    case ET_SETATTACKBUILDINGRATE:
      gAIArray[event->side_id].TimeBetweenBuildingAttacks = event->value;
      break;
    case ET_SETCASH:
    {
      CSide *side = GetSide(event->side_id);
      int actual_cash = side->SpiceReal + side->CashReal;
      int target_drip = event->value;
      if (actual_cash + side->CashDrip + target_drip < 0)
           target_drip += (actual_cash + side->CashDrip + target_drip) * -1; 
      side->CashDrip += target_drip;
      break;
    }
    case ET_SETTECH:
    {
      gMiscData.Tech[event->side_id] = event->value;
      CSide__update_list_of_available_buildings_and_units(GetSide(event->side_id));
      break;
    }
    case ET_WIN:
      if ( !gLose )
      {
        gWin = 1;
      }
      break;
    case ET_LOSE:
      if ( !gWin )
      {
        gLose = 1;
      }
      break;
    //case ET_BLOXFILE:
    //case ET_ATTRIBFILE:
    case ET_REVEAL:
      if (event->amount == 0)
      {
        RevealMap();
        BlitClipTImage1(_RadarMap1, 0, 0, _RadarMap2, 0, 0, 0);
      }
      else
        RevealCircle(event->xpos, event->ypos, event->amount);
      break;
    case ET_SHOWTIMER:
      gTimerValue = event->value;
      break;
    case ET_HIDETIMER:
      gTimerValue = -1;
      break;
    case ET_SHOWMESSAGE:
    { 
      // Play message sound
      int sample_id = Data__GetSoundTableID("S_CHATMSG");
      Sound__PlaySample(sample_id, 0, 0, 0);
      // Attempt to get custom text from mission ini file
      char mapIniPath[256];
      char id[12];
      char custom_text[512];
      sprintf(mapIniPath, ".\\%s%s", GameType == GT_SINGLEPLAYER ? MissionsResourcePath : MapsResourcePath, PathChangeExtension(MissionMap, ".ini"));
      sprintf(id, "%d", event->message_index);
      IniGetString("Text", id, "", custom_text, 512, mapIniPath);
      if (strlen(custom_text) > 0) 
          QueueMessage(custom_text, -1);
      else 
      // Get text from string table
        QueueMessage(Data__GetTextString(event->message_index, 1), -1);
      break;
    }
    case ET_ADDUNITS:
    {
      for (int i = 0; i < event->amount; i++)
      {
        unsigned char x = event->xpos;
        unsigned char y = event->ypos;
        FindNearestFreeTile(&x, &y, 12u);
        ModelAddUnit(event->side_id, event->units[i], x, y, x, y, 0, 0);
      }
      break;
    }
    case ET_SETFLAG:
      gConditionArray[event->side_id].value = event->value;
      break;
    case ET_UNUSED:
      return;
    case ET_PLAYMUSIC:
    {
      //abusing the units array to pass the file name
      if (strcmp(event->units, "*") == 0) PlayRandomMusic();
      else if (strlen(event->units) > 0) Sound__PlayMusic(event->units);
      break;
    }
    default:
      DebugFatal("Mission.cpp", "Unknown event type %d", event->event_type);
  }
}
