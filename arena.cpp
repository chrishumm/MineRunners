#include <fstream>
#include <cmath>
#include "arena.h"
const int tilesize = 12;

arena::arena(string filename,string filename2,string filename3,string filename4,string filename5,string filename6, string levelname,int width, int height)
{
	if(levelname == "assets/level2.txt") //here we check if it's the second level
		is_dark = true; //if true, we cover the level in a dark filter to simulate darkness!
	else
		is_dark = false;

	m_viewport_x = 0; //default settings on start
	m_gravity = 0.1;
	m_screen_width = width;
	m_screen_height = height;
	m_viewport_y = 0;
	m_max_length = 0;
	image = al_load_bitmap(filename.c_str()); //loading passed through bitmaps for blocks
	image2 = al_load_bitmap(filename2.c_str());
	image3 = al_load_bitmap(filename3.c_str());
	image4 = al_load_bitmap(filename4.c_str());
	image5 = al_load_bitmap(filename5.c_str());
	image6 = al_load_bitmap(filename6.c_str());
	background1 = al_load_bitmap("assets/test_background.bmp"); //loading background images
	background2 = al_load_bitmap("assets/cloudsfront.bmp");
	background3 = al_load_bitmap("assets/cloudsclosest.bmp");
	background4 = al_load_bitmap("assets/sand.bmp");
	black_screen = al_load_bitmap ("assets/black_screen.bmp");
	al_convert_mask_to_alpha(black_screen,al_map_rgb( 255, 255, 255 ) ); //converting all images for
	al_convert_mask_to_alpha(image5,al_map_rgb( 255, 255, 255 ) ); //transparency
	al_convert_mask_to_alpha(image6,al_map_rgb( 255, 255, 255 ) );
	al_convert_mask_to_alpha(background2,al_map_rgb( 0, 0, 0 ) );
	al_convert_mask_to_alpha(background3,al_map_rgb( 0, 0, 0 ) );
	al_convert_mask_to_alpha(background4,al_map_rgb( 255, 255, 255 ) );
	al_convert_mask_to_alpha(background1,al_map_rgb( 0, 0, 0 ) );
	string temp;
	ifstream file(levelname.c_str()); //open our level file
	m_speed_scale = 0.25; //setting default speed for parralex
 while(std::getline(file, temp)) //store each line into our level vector
 {
	 level.push_back(temp); //loading our level
 }
 max_viewport_height = level[0].capacity();
 max_viewport_width = level[0].size()*tilesize;
 map_height = level.size();
}

void arena::render()
{
        float a = .1;
		ALLEGRO_COLOR transparent_color = al_map_rgba_f(a, a, a, a);  //used for transparent bitmaps

	int m_parallax_bitmap_width = al_get_bitmap_width(background1); //create our widths and height from our bitamps
	int m_parallax_bitmap_height = al_get_bitmap_height(background1);

	m_speed_scale = 0.125; //how fast the bitmap should scroll
	for(int w = ( (int) (-m_viewport_y* m_speed_scale) % m_parallax_bitmap_height ) - m_parallax_bitmap_height;w< m_screen_height; w += m_parallax_bitmap_height)
	{
		for(int q = ( (int) (-m_viewport_x* m_speed_scale) % m_parallax_bitmap_width ) - m_parallax_bitmap_width;q < m_screen_width;q += m_parallax_bitmap_width)
		{
					al_draw_bitmap(background1,q,w,0); //draw tiled depending on width and height, against the background
		}
	}
	 //now we draw our individual tiles, depending on the character in the vector
	for(int y = m_viewport_y/al_get_bitmap_width(image);y < m_viewport_y/al_get_bitmap_width(image)+(m_screen_height/al_get_bitmap_width(image));y++)
	{
		for(int x = m_viewport_x/al_get_bitmap_width(image); x < m_viewport_x/al_get_bitmap_width(image)+(m_screen_width/al_get_bitmap_width(image));x++)
		{
			if(level[y][x] == '.') //blank space? do nothing
			{}
			else if(level[y][x] == 'W')
			{
				{
					al_draw_bitmap(image,(x*al_get_bitmap_width(image))-m_viewport_x,(y*al_get_bitmap_width(image))-m_viewport_y,0);
				}
			}
			else if(level[y][x] == 'P')
			{
				al_draw_bitmap(image2,(x*al_get_bitmap_width(image))-m_viewport_x,(y*al_get_bitmap_width(image))-m_viewport_y,0);
			}
			else if(level[y][x] == 'B')
			{
				al_draw_bitmap(image3,(x*al_get_bitmap_width(image))-m_viewport_x,(y*al_get_bitmap_width(image))-m_viewport_y,0);
			}
			else if(level[y][x] == 'I')
			{
				al_draw_bitmap(image4,(x*al_get_bitmap_width(image))-m_viewport_x,(y*al_get_bitmap_width(image))-m_viewport_y,0);
			}
			else if(level[y][x] == '/')
			{
				al_draw_bitmap(image5,(x*al_get_bitmap_width(image))-m_viewport_x,(y*al_get_bitmap_width(image))-m_viewport_y,0);
			}
			else if(level[y][x] == '\\')
			{
				al_draw_bitmap(image6,(x*al_get_bitmap_width(image))-m_viewport_x,(y*al_get_bitmap_width(image))-m_viewport_y,0);
			}
			else{}
		}
	}
	//redeclare more parralex information for our next background item
	 m_parallax_bitmap_width = al_get_bitmap_width(background4);
	 m_parallax_bitmap_height = al_get_bitmap_height(background4);

	m_speed_scale =0.43; //set a different speed
	for(int w = ( (int) (-m_viewport_y* m_speed_scale) % m_parallax_bitmap_height ) - m_parallax_bitmap_height;w< m_screen_height;w += m_parallax_bitmap_height)
	{
		for(int q = ( (int) (-m_viewport_x* m_speed_scale) % m_parallax_bitmap_width ) - m_parallax_bitmap_width;q < m_screen_width;q += m_parallax_bitmap_width)
		{// draw ONLY AT A SPECIFIC SECTION of the screen (bottom), tint it and use parralex effet
					al_draw_tinted_scaled_bitmap(background4,transparent_color,0,0,m_parallax_bitmap_width,m_parallax_bitmap_height,q,m_screen_height-m_parallax_bitmap_height,m_parallax_bitmap_width,m_parallax_bitmap_height,0);

		}
	}

	if(is_dark == true) //second level, draw tinted to simulate darkness
		al_draw_tinted_bitmap(black_screen, al_map_rgba_f(.9, .9, .9, .9),0,0,0);
 }

int arena::getHeight()
{
	int i = level.size();
	return level.size();
}

int arena::getWidth(int x)
{
	return level[x].length();
}
int arena::getScreenHeight()
{
    return m_screen_height;
}

int arena::getScreenWidth()
{
    return m_screen_width;
}

bool arena::isWall(int creature_x, int creature_y)
{
		int x= creature_x  / al_get_bitmap_width(image); //convert to check within vector
		int y = creature_y / al_get_bitmap_height(image);

		 if(level[y][x] == '.') // We now check EVERY possible arena object in game
		{					   //
			return false;
		}
		else if(level[y][x] == 'W') //walls
		{
			return true;
		}
		else if(level[y][x] == '/') //right slopes
		{
			return true;
		}
		else if(level[y][x] == '\\') //left slopes
		{
			return true;
		}
		else if(level[y][x] == 'B') //bouncy platforms
		{
			return true;
		}
		else if(level[y][x] == 'I') //ice
		{
			return true;
		}
		return false;
}

bool arena::isIce(int creature_x, int creature_y) //individual check for ice
{
	int x= creature_x  / al_get_bitmap_width(image);
	int y = creature_y / al_get_bitmap_height(image);

	if(level[y][x] == 'I')
	{
		return true;
	}
	return false;
}

bool arena::isInSameRow(int y, int by) //check to see if platform is in the same row
{
	 y= y / al_get_bitmap_width(image);
	 int y_row = (y+1) / al_get_bitmap_width(image);

	if( y == y_row)
	{
		return true;
	}
	 return false;
}
bool arena::isPlatform(int creature_x, int creature_y) //check to see if we've on a platform
{
	int x= creature_x / al_get_bitmap_width(image);
	int y = creature_y / al_get_bitmap_height(image);

	if(level[y][x] == 'P')
	{
		return true;
	}
	return false;
}

bool arena::isBouncy(int creature_x, int creature_y) //check for bouncy platforms
{
	int x= creature_x / al_get_bitmap_width(image);
	int y = creature_y / al_get_bitmap_height(image);
	if(level[y][x] == 'B')
	{
		return true;
	}
	return false;
}

bool arena::isRightSlope(int creature_x, int creature_y) //checking for slopes
{
	int x= creature_x / al_get_bitmap_width(image);
	int y = creature_y / al_get_bitmap_height(image);
	if(level[y][x] == '/')
	{
		return true;
	}
	return false;
}

bool arena::isLeftSlope(int creature_x, int creature_y)//checking for slopes
{
	int x= creature_x / al_get_bitmap_width(image);
	int y = creature_y / al_get_bitmap_height(image);
	if(level[y][x] == '\\')
	{
		return true;
	}
	return false;
}

int arena::getViewPortX() //returns current viewport x
{
	return m_viewport_x;
}

int arena::getViewPortY() //returns current viewport y
{
	return m_viewport_y;
}

void arena::setViewPortX(int x) //sets viewport x
{
	if(x < 0) //off the screen? don't allow it
		return;
	else if(x < max_viewport_width) //less than max? allow it
	{
		 m_viewport_x = x;
	}
}

void arena::setViewPortY(int y) //same as above, just with y
{
	if(y < 0)
		return;
	if(y < max_viewport_height)
	{
		m_viewport_y = y;
	}
}

bool arena::endOfLevel() //we can check to see if player is at the end of the level
{
	for(int y = m_viewport_y/al_get_bitmap_width(image);y < m_viewport_y/al_get_bitmap_width(image)+(m_screen_height/al_get_bitmap_width(image));y++)
	{
		for(int x = m_viewport_x/al_get_bitmap_width(image); x < m_viewport_x/al_get_bitmap_width(image)+(m_screen_width/al_get_bitmap_width(image));x++)
		{
			if(m_viewport_x/al_get_bitmap_width(image)+(m_screen_width/al_get_bitmap_width(image)) > level[y].size()-1)
			{
				return true;
			}
			else{}
		}
	}
	return false;
}

bool arena::isCentreX(int x) //currently centre on x axis?
{
	int currentviewport = getViewPortX();
	int maxsize = currentviewport+(m_screen_width*0.5); //screensize
	if(x > maxsize-60  && x < maxsize+60) //space between player is in centre
		return true;
	else if(currentviewport == 0 && x+49 < maxsize+49) //taking viewport into account
		return true;
	else
		return false; //none above? player is not centre
}

bool arena::isCentreYBot(int y) //we check if the player is centre whilst going down
{
	int currentviewport = getViewPortY();
	int maxsize = currentviewport+(m_screen_height*0.5); //screensize
	if(y > maxsize-49)
		return true;
}

bool arena::isCentreYTop(int y)  //we check if the player is centre whilst going up
{
	int currentviewport = getViewPortY();
	int maxsize = currentviewport+(m_screen_height*0.5); //screensize
	if(y < maxsize-49)
		return true;
}

void arena::zoom(int zoomFactor)
{
	m_zoom_factor = zoomFactor;
}

int arena::getzoom()
{
	return m_zoom_factor;
}

void arena::setGravity(float gravity) //gravity set via this method
{
	m_gravity = gravity;
}

float arena::getGravity() //returns stored gravity
{
	return m_gravity;
}
float arena::getPixelX()
{
    return m_viewport_x;
}

float arena::getPixelY()
{
    return m_viewport_y;

}

int arena::setGameObjects(int* x_pos, int* y_pos, const char item) //used for object creation
{
	int numFound = 0; //number of objects found in level is stored here, used for array
	for(int y = 0;y < getHeight();y++)
	{
		for(int x = 0; x < getWidth(y);x++)
		{
			if(level[y][x] == item)			//loop through until we find the item we're looking for
			{
				x_pos[numFound] = x*tilesize; //set their positions
				y_pos[numFound] = y*tilesize;
				level[y][x] = '.'; //replace area with blank space (object is now created, avoids duplicates)
				numFound++;
			}
		}
	}
	return numFound; //returns number of items found
}
