#pragma once
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <vector>
#include "entities.h"
#include "arena.h"
using namespace std;

class input  //we instantiate this class within hero class
{
public:
	enum KEYS_PRESSED_LIST //contains all keys we can press in game
{
   KEY_ESCAPE, KEY_W, KEY_S, KEY_A, KEY_D,KEY_F,KEY_SPACE,KEY_C, KEY_V, KEY_B, KEY_N, SIZE_OF_KEYS_PRESSED_ENUM
}; //constructor passes through most things established in game.cpp, also borrows event queues.
	input(int*, int*,ALLEGRO_EVENT* ev,ALLEGRO_EVENT_QUEUE* d,int* width, int* height, arena* collision,int speed, int* frame,bool*,bool*,bool*,bool*,entities* x);	
	input(entities* creature_Object);
	bool keys_pressed[SIZE_OF_KEYS_PRESSED_ENUM];
	bool last_pressed_key[SIZE_OF_KEYS_PRESSED_ENUM];
	bool minimised;
    void IsKeyPressed(ALLEGRO_EVENT* ev, ALLEGRO_EVENT_QUEUE* event_queue, bool freeze);
	bool exitGame();
	int* m_x;
	int* m_y;
	int* m_score;
	void setVelocityY(int);
	void setVelocityX(int);
    bool KeyReleased;
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_EVENT* ev;
	void Draw();
	int* m_frame;
	bool* m_flip_frame1;
	bool* climb;
	bool* m_render;
	bool* m_logic;
	float m_y_velocity;
	bool platformLift(int direction);
	float m_x_velocity;
private:
	bool m_exit_game;
	bool m_on_platform;
	bool platform_below;
	bool isPlatform();
	bool in_same_row;
	void jump(float);
	void up(bool);
	void left(float,bool);
	void right(float,bool);
	void down(bool);
	void gravity();
	arena* mcollision;
	bool m_is_jumping;
	bool m_platform;
	int m_current_jump_height;
	int m_max_jump_height;
	int width;
	int height;
	int m_speed;
	bool sliding;


	entities* m_creature;
protected:
};