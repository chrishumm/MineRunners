#pragma once
#include "blocks.h"
#include "arena.h"

class crushingBlock : public blocks //this class deals with crushing block 
{									//by detecting only bottom collision with player
public:
	crushingBlock(int x, int y,string name,string filename,int speed,arena* collision);
	~crushingBlock();
	int getX();
	int getY();
	void display();
private:
	arena* collision;
	int m_xpos;		//
	int m_ypos;		//
	string m_name;	//
	arena* collision1;
	int speed;
	int moved_by;

	bool move_up, move_down;
	bool canMove(int direction);

protected:
	void push(int direction);
	bool update(creature* hero);
};