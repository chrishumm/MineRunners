#pragma once
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

class blocks : public entities
{
public:
	blocks(int x, int y, string name, string filename, int speed, arena* level);
	~blocks();
	void addFrame(string filename, bool movement, bool ascending);
	bool left(int, int);
	bool right(int, int);
	bool up(int, int);
	bool down(int, int);
	//void down(int*, int*);
	void setX(int x);
	void setY(int y);
	virtual int getX(void);
	virtual int getY(void);
	virtual void gravity();
	void cleanup();
	int getWidth();
	int getHeight();
	virtual bool collidesWith(creature* monster);
	virtual bool collidesWithBot(creature* monster);
	virtual bool collidesWithMovingPlatform(creature* monster, int speed);
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
	bool climb;
	bool m_on_platform;
private:
};