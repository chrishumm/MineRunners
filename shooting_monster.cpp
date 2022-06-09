#include "shooting_monster.h"
#include "arena.h"
#include <cmath>

shooting_monster::shooting_monster( int x, int y, string name, string filename,int speed,arena* collision, hero* newhero) : monster(x, y,name, filename,speed,collision,newhero)
{
	m_name = name;
	m_newhero = newhero;
	speed = 100;
	m_level = collision;
	entityParticles = new particleSystem(2,collision,newhero);
	going_right = true;
	m_timer  = 0.0f;
	m_health = 10;
}

shooting_monster::~shooting_monster()
{
	delete entityParticles;
	entityParticles = NULL;
}
void shooting_monster::chase(hero* target) //called to move to hero
{
	double point_diff_x = target->getX()-getX(); //here we check if the player is near to the monster
	double point_diff_y = target->getY()-getY();
	double distance = sqrt(point_diff_x*point_diff_x + point_diff_y * point_diff_y); //we calculate the distance
	monster::chase(target);
	if(distance > 300.0f)
	{
		if(m_timer < 20.0f)
		{
			m_timer += 0.3;
		}
		else
		{
		m_y_velocity = 5.0f;
		m_timer = 0.0f;
		}
		gravity();
		patrol();
		return;
	}
	float projectile_x = 10.0f;
	float projectile_y = 0.0f;
	int projectile_lifetime = 10.0f;
	if (target->getX() < this->getX())
		projectile_x *= -1;

	entityParticles->addParticle(getX(),getY(), projectile_x, projectile_y, projectile_lifetime); //if player is in distance, add particle that is a projectile!
}
