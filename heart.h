#pragma once
#include "pickups.h"
#include "arena.h"

class heart : public pickups //heart pickups replenish our heroes health
{
public:
	heart(int x, int y,string name,string filename,int speed,arena* collision, hero*);
	~heart();

	void applyEffect(hero* appliedHero);
	void resetPos(int* x, int* y);
private:
	arena* m_level;
	string m_name;	//
	hero* m_newhero;
	arena* collision1;
	int speed;
	bool flip_frame;
protected:

};