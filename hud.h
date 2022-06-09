#pragma once;
#include <iostream>
#include <string>
#include <allegro5/allegro.h> //used to draw hud on screen
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "hero.h" //hero methods
#include "arena.h" 
#include "fileio.h"

using std::cout; //only using thes two
using std::string;

class hud //hud will be drawn on screen during game loop, displaying important information
{
public:
	hud(hero* player,arena* current_level, ALLEGRO_TIMER*); //constructor passing timer and player information.
	~hud();
	void update(); //constantly called in game loop
	void onStart(); //used on start creation
	bool countdownOnKill(); //used when player is killed, 3 second cool off period
protected:
private:
	hero* m_hero; //grabbing health/information
	arena* m_level;
	ALLEGRO_TIMER* m_time_x; //used to display every second
	fileio* m_get_previous_hiscore;
	int m_previous_score;
	int current_time; //current time display
	int previous_time;//previous time displayed
	int invinc_finish; //invicibility frames
	int countdown_current_timer; //death cooloff timer
	ALLEGRO_BITMAP* m_health_image;
};