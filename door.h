#pragma once
#include "level_objects.h"
#include "arena.h"

class door : public level_objects
{
public:
	door(int x, int y,string name,string filename,int speed,arena* collision);
	~door();
	virtual int getX();
	virtual int getY();
	void display();
private:
	arena* collision;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
	arena* collision1;
	int speed;
	bool flip_frame;
	int moveUpValue;
	bool canMove(int direction);
	void moveUp();
protected:
	virtual void push(int direction);
	bool update(creature* hero);
};