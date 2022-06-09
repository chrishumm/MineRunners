#include "lives.h"
#include "arena.h"

lives::lives( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : pickups(x, y,name, filename,speed,collision )
{
}

lives::~lives()
{
}


void lives::resetPos(int* x, int* y)
{
	m_xpos = *x;
	m_ypos = *y;
}

void lives::applyEffect(hero* appliedHero)
{
	appliedHero->setLives(appliedHero->getlives() + 1); //increment lives
}