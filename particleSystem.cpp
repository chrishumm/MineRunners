#include "particleSystem.h"
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include "hero.h"
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

const int tilesize = 12;

particleSystem::particleSystem(int max_particles, arena* level) // normal, used for objects
{
	is_projectile = false; //is not a projectile
	m_max_y_velocity = 20.0f; //max speed it can fly at
	m_level = level; //passing through for level colisiions
	m_max_particles = max_particles; //passing through max amoutn of particles for the system
	m_particle = new basic_particle[max_particles]; //create a new dynamic structure containing max amount
	for(int i = 0;i < m_max_particles;i++) //populate structure with null values
	{
		m_particle[i].lifetime = 0; // how long its alive
		m_particle[i].x = 0; //position in world
		m_particle[i].y = 0; //
		m_particle[i].x_velocity = 0; //speed going on x axis
		m_particle[i].y_velocity = 0;// speed going y axis
		m_particle[i].active = true; //is particle active?
	}
	m_num_particles = 0; //current number is 0
}
particleSystem::particleSystem(int max_particles, arena* level, hero* player) //bullets
{
    al_install_audio(); //used for shooting sound
    al_init_acodec_addon();
	al_reserve_samples(3); // how many can play at once

	is_projectile = true; //we are a projectile!
	m_hero = player; //passing for hero collision detection
	m_max_y_velocity = 20.0f; //max speed
	m_level = level; //collision with world
	m_max_particles = max_particles; //max amount we need
	m_particle = new basic_particle[max_particles];
	for(int i = 0;i < m_max_particles;i++) //see previous constructor
	{
		m_particle[i].lifetime = 0;
		m_particle[i].x = 0;
		m_particle[i].y = 0;
		m_particle[i].x_velocity = 0;
		m_particle[i].y_velocity = 0;
		m_particle[i].active = true;
	}
	m_num_particles = 0;

}

particleSystem::~particleSystem()
{
	delete [] m_particle; //deallocating particle structure
	m_particle = NULL;
}

void particleSystem::addParticle(float x, float y, float x_vel, float y_vel, int lifetime) //passing through information for
{																						  //single particle
	if(m_num_particles == m_max_particles) //is the system at max?
		return;	//exit

	if(is_projectile == true) //is the particle a projectile
	{
		m_particle[m_num_particles].lifetime = 60.0f; //lifetime is for that one particle
		m_particle[m_num_particles].x = x; // set position in world
		m_particle[m_num_particles].y = y + 15; // We shoot from centre
		m_particle[m_num_particles].x_velocity = x_vel;



		ALLEGRO_SAMPLE* soundEffect = al_load_sample("assets/soundEffect.wav"); //play our music sample
		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); // set playmode to once
	}
	else //else, we are a normal particle
	{
		m_particle[m_num_particles].lifetime = lifetime; //pass through lifetime to our particle
		m_particle[m_num_particles].x = x; //set position
		m_particle[m_num_particles].y = y;
		int xPos = rand() % 2; // these are used to randomly generate which way the particle
		int yPos = rand() % 2; // will appear around the object
		if(xPos == 1)
		{
			m_particle[m_num_particles].x_velocity = 5.0f; //move right 5 pixels
		}
		else
		{
			m_particle[m_num_particles].x_velocity = -5.0f; //move left 5 pixels
		}
		m_particle[m_num_particles].y_velocity = rand() % 20 + 10; //move upwards at a random force
	}

		m_num_particles++; //we've added a particle, so we need to add one to the list, so increment
}
void particleSystem::addParticle(float x, float y, float x_vel, float v_vel, int lifetime, const int type)
{
	if(m_num_particles == m_max_particles) //is the system at max?
		return;	//exit

	switch (type)
	{
		case bulletParticle:
		
			m_particle[m_num_particles].lifetime = 60.0f; //lifetime is for that one particle
			m_particle[m_num_particles].x = x; // set position in world
			m_particle[m_num_particles].y = y + 15; // We shoot from centre


			m_particle[m_num_particles].x_velocity = -5.0f; //set particle going backwards (to the left)
			if (m_hero->getX() >  m_particle[m_num_particles].x) //is the player on the right?
				m_particle[m_num_particles].x_velocity = m_particle[m_num_particles].x_velocity * -1; //change particle to aim right

			ALLEGRO_SAMPLE* soundEffect = al_load_sample("assets/soundEffect.wav"); //play our music sample
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); // set playmode to once
		
			break;
	}

	if (is_projectile == true) //is the particle a projectile
	{

	}
	else //else, we are a normal particle
	{
		m_particle[m_num_particles].lifetime = lifetime; //pass through lifetime to our particle
		m_particle[m_num_particles].x = x; //set position
		m_particle[m_num_particles].y = y;
		int xPos = rand() % 2; // these are used to randomly generate which way the particle
		int yPos = rand() % 2; // will appear around the object
		if (xPos == 1)
		{
			m_particle[m_num_particles].x_velocity = 5.0f; //move right 5 pixels
		}
		else
		{
			m_particle[m_num_particles].x_velocity = -5.0f; //move left 5 pixels
		}
		m_particle[m_num_particles].y_velocity = rand() % 20 + 10; //move upwards at a random force
	}

	m_num_particles++; //we've added a particle, so we need to add one to the list, so increment




}
void particleSystem::update()
{
	for(int i = 0; i < m_num_particles;i++) //now we loop through ALL particles within our structure
	{
		if(m_particle[i].active == true) //is hte particle active?
		{
			m_particle[i].lifetime--; //deicrement lifetime
			if(m_particle[i].lifetime <= 0) //has the time ran out for the particle?
			{
				m_particle[i] = m_particle[m_num_particles-1]; //move another to fill its place
				m_num_particles--; // deicrement our maxmium amount
			}
			else //our particle is still alive!
			{
				if(is_projectile == true) //is it a bullet?
				{		//below deals with player collision, invoking players method to detect collisions
					if(m_hero->collidesWithBullet(10,10,m_particle[i].x - m_level->getViewPortX(),m_particle[i].y - m_level->getViewPortY(),m_particle[i].x_velocity,m_particle[i].y_velocity) == true)
						m_hero->setHealth(m_hero->getHealth() - 1); //its hit, we set our health down by one!

				}

				m_particle[i].x += m_particle[i].x_velocity; //increase our x position according to our velocity


				if(up(m_particle[i].x,m_particle[i].y,i) == false && m_particle[i].y_velocity > 0.0f) //call arena collision to detect upwards collision
					m_particle[i].y_velocity = -1.0f; //if hit, reverse velocity so particle falls
				else if(down(m_particle[i].x,m_particle[i].y,i) == false && m_particle[i].y_velocity < 1.0f) //have we hit the floor?
				{
					m_particle[i].y_velocity = 15.0f; //increase y velocity, simulating bounce effect
				}
				if(right(m_particle[i].x,m_particle[i].y,i) == false) //hit a right wall?
				{
					m_particle[i].x_velocity = m_particle[i].x_velocity * -1; //bounce back, same velocity in opposite direction
				}
				else if(left(m_particle[i].x,m_particle[i].y,i) == false) //hit a left wall?
				{
					m_particle[i].x_velocity = m_particle[i].x_velocity * -1; //bounce back, same velocity in opposite direction
				}
				else
				{
					if(is_projectile != true) //is it not a projectile?
					{
						if(gravity(i) == true) //set gravity for the particle effect.
						{
							m_particle[i].y -= m_particle[i].y_velocity; //constantly pull down on y position with the  y velocity
						}
						else
						{
							m_particle[i].y += (m_particle[i].y_velocity * -1);
						}
					}
				}
			}
		}
	}
}

void particleSystem::draw() //drawing on the screen
{
	if(is_projectile == true) //if the particle is a bullet
	{
		for(int i = 0; i < m_num_particles; i++) //cycle through number of current particles in structure
		{ //draw circle primitive at correct position according to viewport. Give it a random colour!
			al_draw_filled_circle(m_particle[i].x - m_level->getViewPortX(), m_particle[i].y- m_level->getViewPortY(),2,al_map_rgb(219,rand() % 150 + 160,15));
		}
	}
	else // else draw normal particle
	{
		for(int i = 0; i < m_num_particles; i++)
		{
			al_draw_filled_circle(m_particle[i].x - m_level->getViewPortX(), m_particle[i].y- m_level->getViewPortY(),2,al_map_rgb(219,rand() % 150 + 160,15));
		}
	}
}

bool particleSystem::gravity(int i) //pass through current particle we want gravity to effect
{
	float potential_velocity = (m_particle[i].y_velocity - m_level->getGravity()); //calculate potential velocity, by substracting gravity value from velocity
	m_particle[i].y_velocity = m_particle[i].y_velocity - m_level->getGravity();

	if(m_particle[i].y_velocity > m_max_y_velocity) //have we gone above max velocity? (terminal velocity)
	{
		m_particle[i].y_velocity = m_max_y_velocity; //set a terminal velocity
	}
	if(potential_velocity >= 0.0) //does the particle have enough velocity to move up/down?
	{
		return true; //return true
	}
	else
	{
		return false; //return false
	}
}

bool particleSystem::up(int x, int y,int i) //check for upwards collisions on the particle
{
	if(m_particle[i].y_velocity < 0) //ensure velocity isn't minus for wall collisions
	{
		if(m_level->isWall(x,y-((m_particle[i].y_velocity+1)* -1)) == true) //if so, calculate real collision values
		{
			return false;
		}
	}
	else //else it's normal
	{
		if(m_level->isWall(x,y-((m_particle[i].y_velocity+1))) == true) //calculate normal
		{
			return false;
		}
	}
	return true;
}

bool particleSystem::down(int x, int y,int i) //is the particle now going down?
{
	if(y > 1000 || x > 1100)
	{
		m_particle[i].lifetime = 0.0f;
		return false;
	}
	float down_velocity = 0.0f;		//check to calculate proper velocity
	if(m_particle[i].y_velocity  < 0.0f) //if value is less than 0
	{
		down_velocity = m_particle[i].y_velocity * -1; //make it into a positive number for collision check
	}
	else
	{
		return true;
	}
	if(m_level->isWall(x,y+down_velocity+1) == true) //check arenas isWall collision
	{
		return false;
	}
	return true;
}

bool particleSystem::left(int x, int y, int i) //is the particle travelling left?
{
	if(m_level->isWall(x-1,y) == true || m_level->isWall((x-1),(y+1)) == true) //check for left collisions in arena
	{
		return false;
	}
	return true;
}

bool particleSystem::right(int x, int y, int i)//is the particle travelling right?
{
	float down_velocity = 0.0f;							//ensure particle does not have negative down velocity
	if(m_particle[i].y_velocity < 0.0f)					//if so, create a positive for collision checks.
		 down_velocity = m_particle[i].y_velocity * -1;
	else
		 down_velocity = m_particle[i].y_velocity;
	if(m_level->isWall(x+1,y) == true || m_level->isWall(x+1,y-down_velocity) == true) //check for right collisions
	{
		return false;
	}
	return true;
}
