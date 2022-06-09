#include "hud.h"
#include <sstream> //used to translate ints to strings,

ALLEGRO_FONT* big = NULL; //fonts used for drawing
ALLEGRO_FONT* smaller = NULL; //

const int m_image_width = 83; //how big each heart icon is
const int m_image_height= 72;
hud::hud(hero* player, arena* current_level, ALLEGRO_TIMER* timex)
{
	current_time = 999; //time for current level to be beaten
	invinc_finish = 1; //invincibility frames counter
	countdown_current_timer = 0; //level countdown timer
	previous_time = current_time; //tracking countdown time
	m_time_x = timex; //c++ time variable
	m_health_image = al_load_bitmap("assets/health.bmp"); //bitmap to display hearts
	al_convert_mask_to_alpha(m_health_image, al_map_rgb(255, 255, 255)); //transparancy
	m_hero = player; // hero methods
	m_level = current_level; //level methods
	onStart(); //initial creation
}

hud::~hud()
{
}

void hud::update()
{

	int test =  (al_get_timer_count(m_time_x) / 60); //update every second
	if(test != previous_time) //is the retrieve time, different from previous?
	{
		previous_time = test; //set previous to retrieved
		if(current_time == 1 ) //is current time 1?
		{
			current_time--; //deicrement
			//m_hero->killPlayer(); //kill the player
		}
		else if (current_time > 0)
		{
			current_time--; //deicrement current_time on our display
		}
	}
	else
	{
		previous_time = test;
	}

	if(m_hero->isInvinc() == true) //has our player been hit recently?
	{
		if(invinc_finish == 0) //has the player just been hit?
			invinc_finish = current_time - 3; //player has 3 seconds of invincibility
		else if(current_time == invinc_finish) //has it finished?
		{
			invinc_finish = 0; //reset back to default
			m_hero->resetInvinc(); //allow player to be hit again
		}
	}

	// Hud //
			ALLEGRO_COLOR color = al_map_rgba(60 , 60, 60, 1); // colour for background of hud

			al_draw_filled_rectangle(30,20,800,100,color); //draw rectangle attop
			al_draw_textf(big , al_map_rgb(165, 16, 16), 30, 20, 0, "Health:"); //displays our heath font
			//depending on amount of health player has, display appropriate number of images.
			al_draw_bitmap_region(m_health_image, 0, 0, m_image_width*m_hero->getHealth(), m_image_height, 220, 20, 0);

			al_draw_textf(smaller , al_map_rgb(233, 237, 16), 460, 20, 0, "Gold: %d",m_hero->getGold()); //gold
			al_draw_textf(smaller , al_map_rgb(233, 237, 16), 460, 50, 0, "Timer: %d",current_time); //updated timer

			if(m_hero->getHealth() < 1) //if players health reached 0
				al_draw_textf(big , al_map_rgb(165, 16, 16), 400, 500, 0, "YOU DIED"); //display on screen
}

void hud::onStart()
{
		big = al_load_ttf_font("assets/DejaVuSans.ttf", 50 ,0); //init fonts
		smaller = al_load_ttf_font("assets/DejaVuSans.ttf", 35 ,0);
    	al_init_font_addon();
    	al_init_ttf_addon();

}

bool hud::countdownOnKill() //called when player is killed
{//this method allows player to recieve 3 seconds before respawning into the action!
	if(countdown_current_timer == 0) //has player just been killed?
		countdown_current_timer = current_time - 3; //3 seconds cooloff time
	else if(countdown_current_timer == current_time) //time ran out?
	{
		countdown_current_timer = 0; //set back to 0
		return true; //return true to tell game loop to exit
	}
	else
		return false;

}


