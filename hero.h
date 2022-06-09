#pragma once

#include "creature.h"
#include "arena.h"

class hero : public creature //our hero is a creature, therefore we inherit from creature
{							//mostly getters/setters
public:
	hero(int x, int y,string name,string filename,int speed,arena* collision, int lives, ALLEGRO_EVENT* ev,ALLEGRO_EVENT_QUEUE* d, bool* render_frame,bool* render_logic);
	~hero();
	void draw(int x, int y, bool flip);
	void deductlives(); 
	int getlives();
	void setX(int x);
	void setY(int y);
	virtual int getX();
	virtual int getY();
	int getWidth();
	int getHeight();
	int getGold();
	void setHealth(int);
	int getHealth();
	void addKey();
	void setLives(int);
	void gotDave(); //checks to see if we have the hidden idol
	void killPlayer();
	bool isInvinc();
	void resetInvinc();
	input* control; //this is our input object. All controls for the hero is handled within a seperate object.
	static int score;
	bool m_has_dave;
	bool m_has_key;
	void attack(int type);
private:
	bool invinc_frame;
	arena* collision;
	arena* current_arena;
	int width;
	int current_frame;
	int height;
	int m_lives;
	enum weapons{spear,shootNormal,shootFast,bigArc,flameThrower};

protected:
};