#pragma once
#include <iostream>
#include <vector>
#include "arena.h"

class hero; //here we semi-declare hero class, so we can used and pass through a hero pointer and access that heros methods

using namespace std; //using standard namespace so we don't type std::

struct basic_particle //basic structure for each particle within the game, we can create a dynamic array of these to contain x.
{
	float x, y;
	float x_velocity, y_velocity;
	int lifetime;
	bool active;
};

enum type {

	normalParticle,
	bulletParticle,
	bouncyParticle,
	grenadeLauncherParticle,
	flameThrowerParticle
};

class particleSystem //used to create particle effects (usually circular primitives) in our game
{
public:
	particleSystem(int max_particles,arena* level);
	particleSystem(int max_particles,arena* level, hero*);

	~particleSystem();
	void addParticle(float x, float y, float x_vel, float v_vel, int lifetime);
	void addParticle(float x, float y, float x_vel, float v_vel, int lifetime, const int type);//adding particles
	void update(void); //updating our logic particles
	void draw(void); //drawing our particles
	bool gravity(int i); //gravity on our particles
private:
	hero* m_hero;	//hero method checks
	bool is_projectile;
	int m_max_particles; 
	float m_max_y_velocity;
	int m_num_particles; 
	int particles; 
	basic_particle* m_particle; //structure
	arena* m_level;
	bool up(int,int, int);	//collisions
	bool down(int,int,int);
	bool left(int,int,int);
	bool right(int,int,int);
};