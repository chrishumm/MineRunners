#pragma once
#include "creature.h"
#include "arena.h"
#include "hero.h"
class monster : public creature
{
public:
	monster(int x, int y,string name,string filename,int speed,arena* collision, hero*);
	~monster();
	virtual void chase (hero* newhero); //can be overriden by shooting monster
	virtual int getX();
	virtual int getY();
	void resetPos(int* x, int* y, monster* colliding);
	void patrol();
	void gravity();
private:
	arena* m_level;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
	hero* m_newhero;
	arena* collision1;
	int speed;
	bool flip_frame;
protected:
	bool going_right;
};