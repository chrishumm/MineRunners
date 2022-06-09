#pragma once;
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

using std::cout;
using std::string;

const int menuItemsO = 6;
const int menuHeightO = 10;
class options				//options effect how the game creates objects, how many to create and player lives
{
public:
	options(int*, ALLEGRO_FONT*,ALLEGRO_DISPLAY*,ALLEGRO_EVENT_QUEUE*); 
	~options();
	void menuCreate();
	bool update();
protected:
private:
	enum difficulty{easy = 1,medium,hard,infinite_lives};
	//enum refers to values for our difficulty and used to assign our pointer a value
	string menuItemName[menuItemsO];
	int menuItemPosX[menuItemsO];
	int menuItemPosY[menuItemsO];
	int menuItemWidth[menuItemsO];

	int* m_difficulty;
	char buffer[500];
};