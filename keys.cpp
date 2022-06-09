#include "keys.h"
#include "arena.h"

keys::keys( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : pickups(x, y,name, filename,speed,collision )
{
}

keys::~keys()
{
}


void keys::resetPos(int* x, int* y)
{
	m_xpos = *x;
	m_ypos = *y;
}

void keys::applyEffect(hero* appliedHero)
{
	appliedHero->addKey(); //calls add key, making players m_has_key true
}