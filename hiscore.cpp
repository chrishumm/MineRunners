#include "hiscore.h"				// A lot of the menu drawing is commented in menu.cpp and follows exactly
#include <ctime>					// the same principles.
const int WIDTH = 900;				//
const int HEIGHT = 700;

ALLEGRO_FONT* menuFont2;
ALLEGRO_DISPLAY* displayMenu2;
ALLEGRO_EVENT_QUEUE* eventQueue2;
ALLEGRO_EVENT allegroEvent2;

hiscore::hiscore(ALLEGRO_FONT* fnt,ALLEGRO_DISPLAY* dsp,ALLEGRO_EVENT_QUEUE* eq)
{
	menuFont2 = fnt;
	displayMenu2 = dsp;
	eventQueue2 = eq;
	al_flush_event_queue(eventQueue2);
	menuCreate();
	al_rest(0.2);
	update();
}

hiscore::~hiscore()
{

}

void hiscore::menuCreate()
{
	al_clear_to_color(al_map_rgb(0,0,0));
	menuItemName[0] = "Hiscores";	//our header title
														//we call retrieveHiscore method, with level number
	menuItemName[1] = (sprintf_s(buffer,"Level 1: %d",retrieveHiscore.retrieveHiscores(1)));
	menuItemName[1] =  buffer;
													//sprintf allows us to output the integer in %d
	menuItemName[2] = (sprintf_s(buffer,"Level 2: %d",retrieveHiscore.retrieveHiscores(2)));
	menuItemName[2] =  buffer;
													//this allows us to output our highscores onto screen.
	menuItemName[3] = (sprintf_s(buffer,"Level 3: %d",retrieveHiscore.retrieveHiscores(3)));
	menuItemName[3] =  buffer;

	menuItemName[4] = "Exit";
	for(int i = 0; i < menuItemsH; i++)
	{
		if(i == 0)
		{
			menuItemPosX[i] =	WIDTH*0.5;
			menuItemPosY[i] =	(HEIGHT*0.5) - 20;
		}
		else
		{
			menuItemPosX[i] = menuItemPosX[i-1];
			menuItemPosY[i] = menuItemPosY[i-1] + 20;
		}
		menuItemWidth[i] = menuItemName[i].length() * 20;
		al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeightH,al_map_rgb(0,255,0));
		al_draw_textf(menuFont2 , al_map_rgb(45, 245, 10), menuItemPosX[i], menuItemPosY[i], 0,menuItemName[i].c_str());
	}
}

bool hiscore::update() //once creatured, we loop update
{
	ALLEGRO_MOUSE_STATE getMousePos;
	getMousePos.x = 0.0f;
	getMousePos.y = 0.0f;
for(;;)
{
	
	al_get_mouse_state(&getMousePos);

	bool got_event = al_get_next_event(eventQueue2,&allegroEvent2);
	if(allegroEvent2.mouse.timestamp > 1.0f)
	{
		allegroEvent2.mouse.timestamp = 0.0f; //old events are cleared
	}
	else if(allegroEvent2.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		 switch(allegroEvent2.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			exit(1); //exit
			break;
		 }
	}
   else if (allegroEvent2.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
   {
	  for(int i = 0; i < menuItemsH;i++)
	  {
			if (getMousePos.x >= menuItemPosX[i] && getMousePos.x < menuItemPosX[i]+menuItemWidth[i] &&
				getMousePos.y >= menuItemPosY[i] && getMousePos.y < menuItemPosY[i] + menuHeightH)
			{
				if(i == 4) //only once possible choice, if player clicks exit, we return back to menu cpp
				{
					al_clear_to_color(al_map_rgb(0,0,0)); //clears current screen.
					al_rest(0.2);
					return true;
				}
			}
	  }
   }
		for(int i = 0; i < menuItemsH;i++) //draws our text and boxes.
		{
			if (getMousePos.x >= menuItemPosX[i] && getMousePos.x < menuItemPosX[i]+menuItemWidth[i] &&
				getMousePos.y >= menuItemPosY[i] && getMousePos.y < menuItemPosY[i] + menuHeightH)
			{
				al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeightH,al_map_rgb(53,56,144));
			}
			else
			{
				al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeightH,al_map_rgb(255,0,0));
			}

			al_draw_textf(menuFont2 , al_map_rgb(45, 245, 10), menuItemPosX[i], menuItemPosY[i], 0, menuItemName[i].c_str());
			al_flip_display();
		}
}
}
