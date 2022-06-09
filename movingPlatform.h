#pragma once
#include "blocks.h"
#include "arena.h"
#include "hero.h"

class movingPlatform : public blocks
{
public:
	enum directional_movement
{
   upd,downd,leftd,rightd
};
	movingPlatform(int x, int y,string name,string filename,int speed,arena* collision, hero*,int);
	~movingPlatform();
	bool m_down,m_up,m_left,m_right;
	void move (hero* newhero);
	virtual int getX();
	virtual int getY();
private:
	arena* collision;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
	hero* m_newhero;
	arena* collision1;
protected:
	bool update(creature* hero);
	int getDirection();
	bool canMove(int change);
};