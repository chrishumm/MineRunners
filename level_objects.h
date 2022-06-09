#pragma once;
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "input.h"
#include "entities.h"
#include "creature.h"

using std::cout;
using std::string;

class level_objects : public entities //this parent class details with misc level objects
{										// such as doors, levers and end-trigger
public:
	level_objects(int x, int y, string name, string filename, int speed, arena* level);
	~level_objects();
	void setX(int x);
	void setY(int y);
	virtual int getX(void);
	virtual int getY(void);
	void cleanup();
	int getWidth();
	int getHeight();
	virtual bool collidesWith(creature* monster); //overriden in lever.cpp
	virtual int collidesWithMovingBlock(creature* monster, int speed, int yspeed);
	vector<ALLEGRO_BITMAP*> image;
	vector<ALLEGRO_BITMAP*> climbing;
	vector<ALLEGRO_BITMAP*> collision_memory;
	bool pixelCheck(creature*);
	int getFrame();
	virtual void push(int direction);
	virtual bool canMove(int direction);
	virtual bool update(creature*);
	virtual int getDirection();
	virtual void setActive();
	bool isActive();
	string getName();

protected:
	arena* collision;
	float m_y_velocity;
	int m_xpos;
	int m_ypos;
	string m_name;
	int width;
	int height;
	int m_speed;
	bool active;
	int playerFrameWidth;
	int playerFrameHeight;
	bool m_on_platform;
private:
};
