#pragma once
#include "pickups.h"
#include "arena.h"

class goldcoins : public pickups //gold coins are recorded in player and part of the score
{
public:
	goldcoins(int x, int y,string name,string filename,int speed,arena* collision, hero*);
	~goldcoins();
	void applyEffect(hero* appliedHero);
	void update();
private:
	arena* m_level;
	string m_name;	//
	hero* m_newhero;
	arena* collision1;
	int speed;
	bool flip_frame;
	int	current_frame_count;
protected:

};