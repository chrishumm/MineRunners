#include "heart.h"
#include "arena.h"

heart::heart( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : pickups(x, y,name, filename,speed,collision )
{
	soundEffect = al_load_sample("assets/healthPickup.wav"); //loads a soundeffect we inherited from entities
}															//plays on pickup

heart::~heart()
{
}


void heart::resetPos(int* x, int* y)
{
	m_xpos = *x;
	m_ypos = *y;
}

void heart::applyEffect(hero* appliedHero)
{
	al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); //players sound effect
	appliedHero->setHealth(appliedHero->getHealth()+ 1);
}