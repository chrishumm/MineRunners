#pragma once;
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "entities.h"
#include "creature.h"
#include "hero.h"

using std::cout;
using std::string;

class pickups : public entities //parent class to pickup objects. 
{
public:
	pickups(int x, int y, string name,string filename, int speed, arena* level);
	~pickups();
	void setX(int x);
	void setY(int y);
	virtual int getX(void);
	virtual int getY(void);
	virtual void applyEffect(hero* appliedHero);
	virtual void update();
	int getWidth();
	int getHeight();
	virtual bool collidesWith(creature* monster);
	vector<ALLEGRO_BITMAP*> image;
	vector<ALLEGRO_BITMAP*> climbing;
	vector<ALLEGRO_BITMAP*> collision_memory;
	bool pixelCheck(creature*);
	int getFrame();
protected:
	arena* collision;
	int m_xpos;
	int m_ypos;
	string m_name;
	int width;
	int height;
	int m_speed;
	int playerFrameWidth;
	int playerFrameHeight; 
private:
};