#include "movableBlock.h"
#include "arena.h"
#include "hero.h"
movableBlock::movableBlock( int x, int y, string name, string filename,int speed,arena* collision) : blocks(x, y,name, filename,speed,collision )
{
	m_xpos = x;
	m_ypos = y;
	move_left = true;
	move_right = true;
	entityParticles = new particleSystem(10,collision);
	soundEffect = al_load_sample("assets/open.wav"); //sound used when block is moved
}
movableBlock::~movableBlock()
{
}
bool movableBlock::update(creature* m_hero)
{
	return false;
}
int movableBlock::getX()
{
	return m_xpos;
}

int movableBlock::getY()
{
	return m_ypos;
}

void movableBlock::push(int direction) //here, depending on direction found
{									   //we can move the block
		m_y_velocity = m_y_velocity += 0.1; //basic gravity implementation for our block
		if(this->down(m_xpos,m_ypos) == false)
		{
			m_y_velocity = 0.0f;
		}
		else
		{
			for(int i = 0; i < m_y_velocity;i++)
			{
				if(this->down(m_xpos,m_ypos) == false) //calling blocks
				{									   //up/down/left/right
					break;							   //checks to detect collisions
				}
				else
				{
					setY(m_ypos++);						// no collision? move the block
				}
			}
		}
	if(direction == -1)	//invalid directions are returned 
		return;
	if(direction == 0)
		return;
	if(direction == 1)
	{
		if(this->right(m_xpos,m_ypos) == false)
		{
			move_right = false;
		}
		else
		{
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			move_left = true;
			entityParticles->addParticle(m_xpos,m_ypos,10,0,50); //we can apply effects directly to
			setX(m_xpos++);										//a specific direction, allowing more control
		}
	}
	else if(direction == 2)
	{
		if(this->left(m_xpos,m_ypos) == false)
		{
			move_left = false;
		}
		else
		{
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); //even sound ! (plays open.wav)
			move_right = true;
			setX(m_xpos--);
		}
	}
}

bool movableBlock::canMove(int direction) //this important method
{										//detects if we can move the block in certain circumstances
	if(direction == 1)					//and stops players from clipping through block when block cannot
	{									 // move
		if(move_right == false)
			return false;
	}
	else if(direction == 2)
	{
		if(move_left == false)
			return false;
	}
		else
			return true;

	return true;
}