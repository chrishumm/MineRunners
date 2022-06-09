#include "hero.h"
int hero::score = 0;
#include <map>
#include <algorithm>

hero::hero( int x, int y, string name,string filename,int speed,arena * collision, int lives, ALLEGRO_EVENT* ev,ALLEGRO_EVENT_QUEUE* d, bool* renderFrame,bool* render_logic) : creature(x, y,name, filename,speed,collision)
{
	current_arena = collision; //pass through values and set default values
	m_xpos = x;
 	m_ypos = y;
	m_name = name;
	m_lives = lives;
	score = 0;
	m_health = 3;
	m_has_key = false; //keys and dave to false
	m_has_dave = false;
	m_speed = speed;
	control = NULL; //set current control input class to null
	width =	playerFrameWidth; //calculate frame widths and height for the player
	height = playerFrameHeight;
	int* current_frame = &m_current_frame;
	bool* m_flip_frame = &flip_frame;
	//here we create our new input object, allowing our heros movement to be controlled idependantly
	control = new input(&m_xpos, &m_ypos, ev, d, &width, &height, collision, m_speed, current_frame, m_flip_frame, &climb, renderFrame, render_logic,this);
	entityParticles = new particleSystem(5000,collision); //particle system used for death animation
	weaponSystem = new particleSystem(400, collision);
	invinc_frame = false;
	soundEffect = al_load_sample("assets/hurt.wav"); //hurtsound pre-loaded when needed
}

hero::~hero()
{
	delete control; //deallocating input object
	control = NULL;
	delete weaponSystem;
	weaponSystem = NULL;
	delete entityParticles;
	entityParticles = NULL;
}


void hero::deductlives()
{
	m_lives--;
}

int hero::getlives()			//lots of getters/setters
{								//often used by other objects to effect
	return m_lives;				//the player directly
}

void hero::setLives(int lives)
{
	m_lives = 3;
}

int hero::getX()
{
	return m_xpos;
}
int hero::getY()
{
	return m_ypos;
}

void hero::setX(int x)
{
	 m_xpos = x;
}

void hero::setY(int y)
{
	 m_ypos = y;
}

int hero::getWidth()
{
	return playerFrameWidth;
}

int hero::getHeight()
{
	return this->height;
}

int hero::getGold()
{
	return score;
}

int hero::getHealth()
{
	return m_health;
}

void hero::setHealth(int health)
{
	if(invinc_frame == true) //checks to see if player is currently invincible due to being hit recently
		return; //returns if true
	else if(health < m_health) //checks to see player has been hurt
	{
		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); //plays hurt sound

		m_health = health;
		if(m_health <= 0) //if health is 0
			killPlayer(); //kill the player

		invinc_frame = true; //set player invincible, giving them a chance
		return;
	}
	else
	{
		if(health > 3) // else, have we hit max amount of health for the player?
			return; //return
		else
			m_health = health;
	}
}

void hero::addKey()
{
	m_has_key = true; //used to open doors
}

void hero::gotDave()
{
	m_has_dave = true; //used to track if we get hidden level or not
}

void hero::killPlayer()
{
	if(creature_is_alive == false) //first we check if we've already killed the player...
		return;
	for(int i = 0; i < 200;i++) //now we add the awesome particle effects to our pre-defined particle system
		entityParticles->addParticle(m_xpos,m_ypos,10,10,100);

	creature_is_alive = false; //set alive to false, so we don't kill him again..
	m_health -= 1000; //player will instantly lose a life
}

bool hero::isInvinc()
{
	return invinc_frame; //checks to see if player is currently invincible
}

void hero::resetInvinc()
{
	invinc_frame = false;
}

void hero::attack(int type)
{
	switch (type)
	{
		case spear:
		{

		}break;
		case shootNormal:
		{
			weaponSystem->addParticle(getX(), getY(), 10, 10, 10); //if player is in distance, add particle that is a projectile!

		}break;
		case shootFast:
		{
			weaponSystem->addParticle(getX(), getY(), 10, 10, 10);
		}break;
		case bigArc:
		{
			weaponSystem->addParticle(getX(), getY(), 10, 10, 10);
		}break;
		case flameThrower:
		{
			weaponSystem->addParticle(getX(), getY(), 10, 10, 10);
		}break;
	   default:
		{

		}break;
	};
}
