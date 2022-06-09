#include "options.h"				// A lot of the menu drawing is commented in menu.cpp and follows exactly
#include <ctime>					// the same principles.
const int WIDTH = 900;				//
const int HEIGHT = 700;
ALLEGRO_FONT* menuFont;
ALLEGRO_DISPLAY* displayMenu;
ALLEGRO_EVENT_QUEUE* eventQueue;
ALLEGRO_EVENT allegroEvent;

options::options(int* difficulty, ALLEGRO_FONT* fnt,ALLEGRO_DISPLAY* dsp,ALLEGRO_EVENT_QUEUE* eq)
{
	m_difficulty = difficulty;
	menuFont = fnt;
	displayMenu = dsp;
	eventQueue = eq;
	al_flush_event_queue(eventQueue);
	menuCreate(); //creating our menu
	al_rest(0.2);
	update(); //now looping the update
}

options::~options()
{

}

void options::menuCreate()
{
	al_clear_to_color(al_map_rgb(0,0,0));
	menuItemName[0] = (sprintf_s(buffer, "Current Difficulty: %d - Easy", *m_difficulty));
	menuItemName[0] =  buffer;
	menuItemName[1] = "Easy";
	menuItemName[2] = "Medium";
	menuItemName[3] = "Hard";
	menuItemName[4] = "Infinite Lives";
	menuItemName[5] = "Exit";
	for(int i = 0; i < menuItemsO; i++)
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
		al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeightO,al_map_rgb(0,255,0));
		al_draw_textf(menuFont , al_map_rgb(45, 245, 10), menuItemPosX[i], menuItemPosY[i], 0,menuItemName[i].c_str());
	}
}

bool options::update()
{
	ALLEGRO_MOUSE_STATE getMousePos; //grab mouse current coordinates
	getMousePos.x = 0.0f;
	getMousePos.y = 0.0f;
for(;;)
{
	
	al_get_mouse_state(&getMousePos);

	bool got_event = al_get_next_event(eventQueue,&allegroEvent);
	if(allegroEvent.mouse.timestamp > 1.0f) //ensure we don't record old mouse events
	{
		allegroEvent.mouse.timestamp = 0.0f;
	}
	else if(allegroEvent.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		 switch(allegroEvent.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			exit(1); //exits out game
			break;
		 }
	}
   else if (allegroEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
   {
	  for(int i = 0; i < menuItemsO;i++)
	  {
			if (getMousePos.x >= menuItemPosX[i] && getMousePos.x < menuItemPosX[i]+menuItemWidth[i] &&
				getMousePos.y >= menuItemPosY[i] && getMousePos.y < menuItemPosY[i] + menuHeightO)
			{
				if(i == easy) //i refers to menu box we've clicked on
				{
					*m_difficulty = easy; //pointer points to int in menu.cpp, we assign a value from our enum
					sprintf_s(buffer, "Current Difficulty: %d - Easy", *m_difficulty); //writes output on menu
					menuItemName[0] =  buffer;
					al_draw_textf(menuFont , al_map_rgb(45, 245, 10), menuItemPosX[0], menuItemPosY[0], 0,menuItemName[0].c_str());
				}	//draws the text with our difficulty
			    if(i == medium)
				{
					*m_difficulty = medium;
					sprintf_s(buffer, "Current Difficulty: %d - Medium", *m_difficulty);
					menuItemName[0] =  buffer;
					al_draw_textf(menuFont , al_map_rgb(45, 245, 10), menuItemPosX[0], menuItemPosY[0], 0,menuItemName[0].c_str());
				}
				if(i == hard)
				{
					*m_difficulty = hard;
					sprintf_s(buffer, "Current Difficulty: %d - Hard", *m_difficulty);
					menuItemName[0] =  buffer;
					al_draw_textf(menuFont , al_map_rgb(45, 245, 10), menuItemPosX[0], menuItemPosY[0], 0,menuItemName[0].c_str());
				}
				if(i == infinite_lives)
				{
					*m_difficulty = infinite_lives;
					sprintf_s(buffer, "Current Difficulty: %d - Infinite Lives (You cheat!)", *m_difficulty);
					menuItemName[0] =  buffer;
					al_draw_textf(menuFont , al_map_rgb(45, 245, 10), menuItemPosX[0], menuItemPosY[0], 0,menuItemName[0].c_str());
				}
				if(i == 5)
				{
					al_clear_to_color(al_map_rgb(0,0,0));
					return true; //if this returns true, it tells our program to go back to menu.cpp
				}
			}
	  }
   }
		for(int i = 0; i < menuItemsO;i++) //constantly updates our screen with the menu items
		{
			if (getMousePos.x >= menuItemPosX[i] && getMousePos.x < menuItemPosX[i]+menuItemWidth[i] &&
				getMousePos.y >= menuItemPosY[i] && getMousePos.y < menuItemPosY[i] + menuHeightO)
			{
				al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeightO,al_map_rgb(53,56,144));
			}
			else
			{
				al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeightO,al_map_rgb(255,0,0));
			}

			al_draw_textf(menuFont , al_map_rgb(45, 245, 10), menuItemPosX[i], menuItemPosY[i], 0, menuItemName[i].c_str());
			al_flip_display();
		}
}
}
