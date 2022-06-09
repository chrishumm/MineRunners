#include "crushingBlock.h"
#include "arena.h"
#include "hero.h"
crushingBlock::crushingBlock( int x, int y, string name, string filename,int speed,arena* collision) : blocks(x, y,name, filename,speed,collision )
{
	m_xpos = x;
	m_ypos = y;
	move_up = false;
	move_down = true;
	moved_by = 0;
	entityParticles = new particleSystem(10,collision);
}

crushingBlock::~crushingBlock()
{
}

void crushingBlock::display()
{
	
}
int crushingBlock::getX()
{
	return m_xpos;
}

int crushingBlock::getY()
{
	return m_ypos;
}

void crushingBlock::push(int pushing_limit) //push here is used to push the block downwards
{
	if(pushing_limit == 0)
		return;
		if(move_down == true)	//here we check whether or not to move our block up or down
		{
			setY(m_ypos++);
			moved_by++;
			if(moved_by >= pushing_limit)	//once we hit the limit, we reverse
			{
				moved_by = 0;
				this->move_down = false;
				this->move_up = true;
			}
		}
		else if(move_up == true) //now moving up is true, we do the same
		{						 //increment the position back up

			setY(m_ypos--);
			moved_by++;
			if(moved_by >= pushing_limit)
			{
				moved_by = 0;
				this->move_down = true;
				this->move_up = false;
			}
		}
}

bool crushingBlock::update(creature* hero)
{
	push(50); //passing through pixel limit on how far to move block
	return false;
}

bool crushingBlock::canMove(int direction)
{
	return true;
}