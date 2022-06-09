#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
using namespace std;
class arena
{
public:
	arena(string filename,string filename2,string filename3,string filename4,string filename5,string filename6,string levelname,int m_screen_width,int m_screen_height);
	bool isWall(int x, int y);
	void render();
	int setGameObjects(int*, int*,const char item);
	int getHeight();
	int getScreenHeight();
	int getScreenWidth();

	int getWidth(int x);
	bool isPlatform(int creature_x, int creature_y);
	bool isInSameRow(int y, int by);
	void setViewPortX(int);
	int getViewPortX();
	void setViewPortY(int);
	int getViewPortY();
	bool isIce(int,int);
	bool isCentreX(int);
	bool isCentreYTop(int);
	bool isCentreYBot(int);
	bool endOfLevel();
	void zoom(int);
	int getzoom();
	void setGravity(float);
	bool isBouncy(int, int);
	bool isRightSlope(int, int);
	bool isLeftSlope(int, int);
	float getGravity();

	float getPixelX();
	float getPixelY();
	int maxsize;
private:
	enum AboveBelow
{
   Above,Below,Neither
};
	int max_viewport_height;
	int max_viewport_width;
	int m_viewport_x;
	int m_screen_width;
	int m_screen_height;
	int m_viewport_y;
	int m_max_length;
	int blocksize;
	bool is_dark;
	float m_speed_scale;
	float m_zoom_factor;
	float m_gravity;
	int map_height;
	int playerHeight, playerWidth;
	ALLEGRO_BITMAP* image;
	ALLEGRO_BITMAP* image2;
	ALLEGRO_BITMAP* image3;
	ALLEGRO_BITMAP* image4;
	ALLEGRO_BITMAP* image5;
	ALLEGRO_BITMAP* image6;
	ALLEGRO_BITMAP* background1;
	ALLEGRO_BITMAP* background2;
	ALLEGRO_BITMAP* background3;
	ALLEGRO_BITMAP* background4;
	ALLEGRO_BITMAP* black_screen;
	vector<string> level;
protected:
};
