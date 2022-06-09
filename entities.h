#pragma once;
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include "arena.h"
#include "particleSystem.h"
using std::cout;
using std::string;

class entities
{
public:
	entities(int x, int y, string name, string filename, int speed, arena* level);
	~entities();
	void draw(int x, int y, bool flip,bool hero);
	void addFrame(string filename, bool movement, bool ascending);
	virtual void setX(int);
	virtual void setY(int);
	virtual int getX();
	virtual int getY();
	bool isOnScreen();
	virtual void attack(int mode);
protected:
	
	vector<ALLEGRO_BITMAP*> image;
	vector<ALLEGRO_BITMAP*> climbing;
	vector<ALLEGRO_BITMAP*> collision_memory;
	ALLEGRO_SAMPLE* soundEffect;
	arena* m_level;
	float m_y_velocity;
	int m_xpos;
	int m_ypos;
	string m_name;
	int width;
	int height;
	int m_speed;
	int m_current_frame;
	bool flip_frame;
	int playerFrameWidth;
	int playerFrameHeight;
	bool climb;
	bool m_on_platform;
	int m_health;
	bool creature_is_alive;
	int entityFrameWidth;
	int entityFrameHeight;
	particleSystem* entityParticles;
private:
};
