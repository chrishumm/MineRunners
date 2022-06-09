#include "goldcoins.h"
#include "arena.h"

goldcoins::goldcoins( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : pickups(x, y,name, filename,speed,collision )
{
	m_current_frame = 0;
	entityFrameWidth = 83; //we set the size of our coins here
	entityFrameHeight = 72;
	current_frame_count = 0;
}

goldcoins::~goldcoins()
{
}
const int frame_limit = 10; 
void goldcoins::update() //in update, we can update our frames for our goldcoins
{
	if(current_frame_count == frame_limit)	//updating too fast would make the 
	{										//sprite look weird. So we make a 
		m_current_frame++;					//limit before updating
		if(m_current_frame > 5)
			m_current_frame = 0;

		current_frame_count = 0;			//this effect gives our coins a floating effect
	}
	else
		current_frame_count++;
}

void goldcoins::applyEffect(hero* appliedHero)
{
	appliedHero->score++; //increment players score
}