#include "movingPlatform.h"
#include "arena.h"

movingPlatform::movingPlatform( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero,int upordown) : blocks(x, y,name, filename,speed,collision )
{
	if(upordown == upd) //here we set preset moving platform variables depending on the direction we want the platform go
	{
		m_up = true;
		m_down = false;
	}
	else if(upordown == downd)	//first two deal with vertical movement
	{
		m_down = true;
		m_up = false;
	}
	else if(upordown == leftd)	//this and bottom else if deal with horizontal
	{
		m_left = true;
		m_right = false;
	}
	else if(upordown == rightd) //depending on these presets, will effect later movement of the block
	{
		m_left = true;
		m_right = false;
	}
	else{}
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	m_newhero = newhero;
}

movingPlatform::~movingPlatform()
{
}

bool movingPlatform::update(creature* current_hero) //called every frame
{
	move((hero*)current_hero); //we move the block and current hero.
							   //we cast hero* onto our creature to access hero methods.
	return false;
}


void movingPlatform::move(hero* newhero) //now we check for movement
{

width = al_get_bitmap_width(image[m_current_frame]);
height = al_get_bitmap_height(image[m_current_frame]);

if(m_down == true || m_up == true) //first we check if up or true is true
{								   //if false, we know it's horizontal movement
	if(m_down == true)
	{
		if(this->down(m_xpos,m_ypos) == false)	//we now deal with moving the platform
		{										//by borrowing blocks movement methods
			m_down = false;						//we check collision.
			m_up = true;						// Importantly, when we detect a collision, we flip
		}										//what is true. So block moves in opposite direction
		else
		{
			setY(m_ypos++);
		}
	}
	if(m_up == true)
	{
		if(this->up(m_xpos,m_ypos) == false)
		{
			m_down = true;
			m_up = false;
		}
		else
		{
			setY(m_ypos--);
		}
	}
}
else if(m_left== true || m_right == true) //this uses same logic as above
{										 //but applies to horizontal movement
	if(m_right == true)
		{
			if(this->right(m_xpos,m_ypos) == false)
			{
				m_left = true;
				m_right = false;
			}
			else
			{
				setX(m_xpos++);
			}
		}
	if(m_left == true)
		{
			if(this->left(m_xpos,m_ypos) == false)
			{
				m_right = true;
				m_left = false;
			}
			else
			{
				setX(m_xpos--);
			}
		}
		}
}

int movingPlatform::getX()
{
	return m_xpos;
}

int movingPlatform::getY()
{
	return m_ypos;
}

int movingPlatform::getDirection() //here we can get what direction the platform is currently moving
{
	if(m_up == true)
		return 0;
	else if(m_down == true)
		return 1;
	else if(m_left == true)
		return 2;
	else if(m_right == true)
		return 3;

	return -1;
}

bool movingPlatform::canMove(int change) //here we instantly change if player is about to get squashed
{										//by the platform and stopping the platform from clipping the player
	m_up = false;						//into the wall
	m_down = true;

	return true;
}
