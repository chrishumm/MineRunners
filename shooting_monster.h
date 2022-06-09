#pragma once
#include "monster.h"
#include "arena.h"
#include "hero.h"

class shooting_monster : public monster //inherited from monster, uses same code apart from chase
{
public:
	shooting_monster(int x, int y,string name,string filename,int speed,arena* collision, hero*);
	~shooting_monster();
	void chase (hero* newhero);
private:
	arena* m_level;
	float m_timer;
	string m_name;	//
	hero* m_newhero;
	arena* collision1;
	int speed;
	bool flip_frame;
protected:

};