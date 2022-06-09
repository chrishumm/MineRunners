#pragma once;
#include <iostream>
#include <string>
#include <allegro5/allegro.h> //including allegro libaries for drawing
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "hero.h"	//including these header files for creation
#include "arena.h"
#include "fileio.h"
#include "options.h"
#include "hiscore.h"

using std::cout; //only using these two, for now.
using std::string;

const int menuItems = 6; //fixed amount of items we want to create on our menu
const int menuHeight = 10;//height for each menu item drawn

class menu //from this menu, we can create instances of games, loaded games, hiscores and generally navigate our game
{
public:
	menu(); //our first constructor. populates default values for first start
	bool update();
protected:
private:
	bool allegro_font_init(); //creating important allegro presets to draw on screen
	bool allegro_init();	//creating our font from our image file on start.
	void exitGame();		//called when player clickls exist game
	void startGame();		//called when player clicks start
	void menuCreate();		//called when menu is created, to popular our menu
	void launchOptions();	//called when options is clicked
	ALLEGRO_BITMAP* m_background;	//background for menu
	ALLEGRO_BITMAP* m_font_from_file;// font we've grabbed from bitmap

	unsigned int m_lives; //players lives
	int m_current_level; //current level player is playing
	unsigned int m_max_level; //max amount of levels player CAN player
	int m_difficulty; //current difficulty player is on (effects object generation)

	string menuItemName[menuItems]; //array of strings containing menu names
	int menuItemPosX[menuItems]; //positions on screen for player to click
	int menuItemPosY[menuItems]; //
	int menuItemWidth[menuItems]; //width information depending on length of menu item name string

	static int m_idol_count; //keep count of players current idols in this game (only delcared once)
};