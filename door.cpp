#include "door.h"
#include "arena.h"
#include "hero.h"
door::door( int x, int y, string name, string filename,int speed,arena* collision) : level_objects(x, y,name, filename,speed,collision )
{
	moveUpValue = 0;
	m_xpos = x;
	m_ypos = y;
	speed = speed;
	al_convert_mask_to_alpha(image[0], al_map_rgb(255, 255, 255));
	entityParticles = new particleSystem(10,collision);

}
door::~door()
{
}

bool door::update(creature* m_hero)
{
	if(active == true) //has door been opened by a key or lever?
		moveUp(); //call moveup
	return false;
}


const int moveUpMax = 100; //max amount of pixels door can move up
void door::moveUp()
{
	if(moveUpValue == moveUpMax) //if we've reached max, do nothing
	{}
	else 
	{
		m_ypos--; //move up one pixel
		moveUpValue++; //move our counter up
	}
}

void door::display()
{

}
int door::getX()
{
	return m_xpos;
}

int door::getY()
{
	return m_ypos;
}

void door::push(int direction)
{

}

bool door::canMove(int direction) //here we check if we can move the door upwards, once this method id called, we call move up
{
	moveUp();
	return true;
}