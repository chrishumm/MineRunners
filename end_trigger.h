#pragma once
#include "level_objects.h"
#include "arena.h"

class end_trigger : public level_objects	//this class is used to create our object that our player needs to reach!
{											//it'll cause the game to exit out of the game loop, returning and loading next level
public:
	end_trigger(int x, int y,string name,string filename,int speed,arena* collision);
	~end_trigger();
	int getX(); //used for coordinates
	int getY();
private:
	arena* collision;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
protected:
};