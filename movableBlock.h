#pragma once
#include "blocks.h"
#include "arena.h"

class movableBlock : public blocks
{
public:
	movableBlock(int x, int y,string name,string filename,int speed,arena* collision);
	~movableBlock();
	virtual int getX();
	virtual int getY();
private:
	arena* collision;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
	arena* collision1;
	bool move_left, move_right;
	bool canMove(int direction);
protected:
	virtual void push(int direction);
	bool update(creature* hero);
};