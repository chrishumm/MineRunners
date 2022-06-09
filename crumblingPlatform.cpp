#include "crumblingPlatform.h"
#include "arena.h"
#include "hero.h"
crumblingPlatform::crumblingPlatform( int x, int y, string name, string filename,int speed,arena* collision) : blocks(x, y,name, filename,speed,collision )
{
	m_countdown = 0;
	m_xpos = x;
	m_ypos = y;;
	entityFrameWidth = 76;
	entityFrameHeight = 22;
	active = false;
	height = 76;
	width = 22;
	//m_name = "crumble";
	speed = speed;
	entityParticles = new particleSystem(10,collision);
	m_has_collided = true;
}

crumblingPlatform::~crumblingPlatform()
{
}

void crumblingPlatform::display()
{

}
int crumblingPlatform::getX()
{
	return m_xpos;
}

int crumblingPlatform::getY()
{
	return m_ypos;
}

void crumblingPlatform::push(int pushing_limit)
{
	
}

const int max_time = 50;
bool crumblingPlatform::update(creature* hero) //update is called every frame
{
	if(active == true) //if the crumbling platform has been collided wiyth
	{
		m_has_collided = true; //set to true
		crumble(); //call crumble method
		m_countdown++; //countdown
	}
	if(m_countdown >= max_time) //when this reaches true, we return true, telling game.cpp to
	{							//delete this object from the vector
		return true;
	}
	
		return false;
}

const int max_frames = 20;
static int current_frames = 0;
void crumblingPlatform::crumble() //this is called every frame when collided is true
{
	if(current_frames == max_frames) //we do a simple countdown here to tell
	{								 //whether or not to finish crumbling
		current_frames = 0;			// but this also deals with updating different
		m_current_frame++;			//frames for rendering, giving a different
		if(m_current_frame == 2)	//effect
			m_current_frame = 0;
	}
	else
		current_frames++;
}
bool crumblingPlatform::canMove(int direction)
{
	return true;
}