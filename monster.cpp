#include "monster.h"
#include "arena.h"
#include <cmath>
static float down_velocity = 0.0f; //gravity force
monster::monster( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : creature(x, y,name, filename,speed,collision )
{
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	m_newhero = newhero;
	m_level = collision;
	entityParticles = new particleSystem(1,collision,newhero);
	going_right = true;
}

monster::~monster()
{
	down_velocity = 0.0f;
}

void monster::chase(hero* newhero)
{
	double point_diff_x =  newhero->getX()-this->m_xpos; //checking to see if we're in distance for the player to chase
	double point_diff_y =  newhero->getY()-this->m_ypos;
	double distance = sqrt(point_diff_x*point_diff_x + point_diff_y * point_diff_y);

	if(distance > 300.0f)
	{
		patrol();
		gravity();
		return;
	}

	width = al_get_bitmap_width(image[m_current_frame]);
	height = al_get_bitmap_height(image[m_current_frame]);

	gravity(); //this monster is effected by gravity
	if(this->m_xpos + 30.0f < newhero->getX()) //movement values, to move the monster at certain speeds
		{									   //towards the hero
			for(int i = 0; i < m_speed;i++)
				{
			if(this->right(m_xpos,m_ypos) == false)
			{
			}
			else
			{
				setX(m_xpos++);
				draw(m_xpos, m_ypos, false, false);
			}
				}
		}
	if(this->m_xpos-  30.0f > newhero->getX())
		{
			for(int i = 0; i < m_speed;i++)
				{
			if(this->left(m_xpos,m_ypos) == false)
			{
		
			}
			else{
				setX(m_xpos--);
				draw(m_xpos, m_ypos, false, false);
			}
				}
		}
}

int monster::getX()
{
	return m_xpos;
}

int monster::getY()
{
	return m_ypos;
}

void monster::resetPos(int* x, int* y, monster* colliding) //resetting position if two monsters clip
{
	if(going_right == true)
	{
		if(going_right == true)
		{
			m_xpos = *x - 30.0f;
			going_right = false;
		}
		if(colliding->going_right == false)
		{
			colliding->m_xpos = colliding->m_xpos + 30.0f;
			colliding->going_right = true;
		}
	}
	else
	{
		if(going_right == false)
		{
			m_xpos = *x + 30.0f;
			going_right = true;
		}
		if(colliding->going_right == true)
		{
			colliding->m_xpos = colliding->m_xpos - 30.0f;
			colliding->going_right = false;
		}
		
	}
}
void monster::gravity()
{
	float potential_velocity = (m_y_velocity - m_level->getGravity()); //get gravity value from arena
	m_y_velocity = m_y_velocity - m_level->getGravity();
	if(potential_velocity > 0.0f)
	{
 		if(up(m_xpos,m_ypos) == true) //if up potential_velocity is true
		{
			setY(m_ypos--); //move monster up
		}
	}
	else
	{
 		if(down(m_xpos,m_ypos) == true) //else drag monster down
		{
			down_velocity = down_velocity += 0.1;
			for(int i = 0; i < down_velocity;i++)
			{
				if(this->down(m_xpos,m_ypos) == false)
				{
					m_y_velocity = 0.0f;
					down_velocity = 0.0f;
					break;
				}
				else
				{
					setY(m_ypos++);
				}
			}
		}
	}
}
void monster::patrol()
{
	if(going_right == true) //movement values, to move the monster at certain speeds
		{									   //towards the hero
			for(int i = 0; i < m_speed;i++)
				{
					if(this->right(m_xpos,m_ypos) == false)
					{
						going_right = false;
					}
					else
					{
						setX(m_xpos++);
					}
				}
		}
	else if(going_right == false)
		{
			for(int i = 0; i < m_speed;i++)
				{
					if(this->left(m_xpos,m_ypos) == false)
					{
						going_right = true;
					}
					else{setX(m_xpos--);}
				}
		}
}