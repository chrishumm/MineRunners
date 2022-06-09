#pragma once
#include <iostream>
#include "movableBlock.h"
#include "crushingBlock.h"
#include "crumblingPlatform.h"
#include "input.h"
#include "blocks.h"
#include "hero.h"
#include "monster.h"
#include "shooting_monster.h"
#include "arena.h"
//////////////////////////////
#include "level_objects.h"
#include "door.h"
#include "lever.h"
///////////pickups/////////////
#include "goldcoins.h"
#include "heart.h"
#include "lives.h"
#include "idol.h"
///////////////////////////////
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <sstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "movingPlatform.h"
#include "fileio.h"
#include "hud.h"
#include "particleSystem.h"
#include "end_trigger.h"
#include "keys.h"
//////////////////////////here we include everything we want within our game/////////////////////////////////////
class game
{
public:
	game(ALLEGRO_DISPLAY*, int m_current_level, int m_difficulty); //we want to reuse current display, get current level and difficulty
	game(ALLEGRO_DISPLAY*, int m_current_level, int m_difficulty, int lives);
	~game();
	bool m_has_dave; //does player have dave?
	int playGame(); //starts maingame loop
	bool createGameObjects(); //creates objects from text file
private:
	void deAlloc(); //deallocates all game objects
	bool render_frame; //are we rendering?
	bool render_logic;// or is it logic step?
	hero* newhero; //our player
	arena* level; // our level
	fileio hiscoreInstance; // reading/writing files
	hud* create_player_height; // our hud
	int m_current_level; // current level
	int m_difficulty; // current difficulty (object creation)
	int m_lives;
	vector<creature*> GameObjects; // rendering all creatures
	vector<pickups*> ObjectPickups; // goldcoins and collectables (rendering and collision)
	vector<monster*> pointermonster; // monster collision data
	vector<movingPlatform*> moving_platform; // moving platforms collision data
	vector<movableBlock*> movingBlock;		  // moving block collision data
	vector<blocks*> platform_container; //contains all objects within blocks
	vector<level_objects*> levelobjects;// contains misc objects such as levers
	particleSystem* newparticles; // example particle system
	ALLEGRO_BITMAP* loading;
	bool flip; // flipping?
	bool loop; // used for loop
protected:
};
