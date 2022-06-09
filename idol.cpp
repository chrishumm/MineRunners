#include "idol.h"
#include "arena.h"

idol::idol( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : pickups(x, y,name, filename,speed,collision )
{
}

idol::~idol()
{
}


void idol::resetPos(int* x, int* y)
{
	m_xpos = *x;
	m_ypos = *y;
}

void idol::applyEffect(hero* appliedHero)
{
	appliedHero->gotDave(); //sets players dave variable to true
}