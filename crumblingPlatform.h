#pragma once
#include "blocks.h"
#include "arena.h"

class crumblingPlatform : public blocks //this class 
{
public:
	crumblingPlatform(int x, int y,string name,string filename,int speed,arena* collision);
	~crumblingPlatform();
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
	int m_countdown;;
	bool canMove(int direction);
	void crumble();
	bool m_has_collided;
protected:
	virtual void push(int direction);
	bool update(creature* hero);
};