#pragma once
#include "level_objects.h"		
#include "arena.h"

class lever : public level_objects//lever used to open doors
{
public:
	lever(int x, int y,string name,string filename,int speed,arena* collision,level_objects* door);
	~lever();
	int getX();
	int getY();
private:
	arena* collision;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
	arena* collision1;
	level_objects* door; //connecting door stored here
	bool collidesWith(creature* monster);
protected:
	bool update(creature* hero);
};