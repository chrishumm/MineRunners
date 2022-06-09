#include "lever.h"
#include "arena.h"
#include "hero.h"
lever::lever( int x, int y, string name, string filename,int speed,arena* collision,level_objects* connecting_door) : level_objects(x, y,name, filename,speed,collision )
{
	active = false; //set our lever to inactive for checks
	door = connecting_door; //pass through door the lever opens, via object pointer
	m_xpos = x;
	m_ypos = y;
	m_name = "lever";
	entityParticles = new particleSystem(10,collision); //here we can override entities particle system with our own.
}
lever::~lever()
{
}

bool lever::update(creature* m_hero) //check to see if lever has been activated
{
	if(active == true)
	{
		door->canMove(1);
	}
	return false;
}
int lever::getX()	//used to retrieve coordinates within world.
{
	return m_xpos;
}

int lever::getY()
{
	return m_ypos;
}


bool lever::collidesWith(creature* monster) //override level_objects collidesWith method
{
	if(active == true)	//if we've already activated the lever, exit out of check
		return false;

	int y1 = monster->getY();	//storing variables for collision checks
	int x1 = monster->getX();
	int x2 = this->getX();
	int y2 = this->getY();

	if ((x2 >= x1 + playerFrameWidth - 1) || // is b1 on the right side of b2?
		(y2 >= y1 + playerFrameHeight - 1) || // is b1 under b2?
		(x1 >= x2 + entityFrameWidth - 1) || // is b2 on the right side of b1?
        (y1 >= y2 + entityFrameHeight - 1))   // is b2 under b1?
    {
        return false;
    }
	else
	{
		ALLEGRO_SAMPLE* soundEffect = al_load_sample("assets/open.wav"); //play our music sample
		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); // set playmode to once
		active = true; //if player is colliding, make the lever active.
	}
	return false;
}