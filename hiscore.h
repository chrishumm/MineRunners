#pragma once
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "fileio.h"

using std::cout;
using std::string;

const int menuItemsH = 5;
const int menuHeightH = 10;
class hiscore				//this class allows us to output the hiscores for our game
{
public:
	hiscore(ALLEGRO_FONT*,ALLEGRO_DISPLAY*,ALLEGRO_EVENT_QUEUE*);
	~hiscore();
	void menuCreate(); //fileio is used here
	bool update();
	fileio retrieveHiscore; //used to retrieveHiscore method and output hiscores
protected:
private:
	string menuItemName[menuItemsH];
	int menuItemPosX[menuItemsH];
	int menuItemPosY[menuItemsH];
	int menuItemWidth[menuItemsH];

	char buffer[500];
};