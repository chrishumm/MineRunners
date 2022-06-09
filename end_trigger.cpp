#include "end_trigger.h"
#include "arena.h"
#include "hero.h"
end_trigger::end_trigger( int x, int y, string name, string filename,int speed,arena* collision) : level_objects(x, y,name, filename,speed,collision )
{
	m_xpos = x;
	m_ypos = y;
	speed = speed;
}
end_trigger::~end_trigger()
{
}
int end_trigger::getX()	//used for coordinates within the world
{
	return m_xpos;
}

int end_trigger::getY()
{
	return m_ypos;
}