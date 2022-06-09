#include "level_objects.h"
#include <sstream>

extern ALLEGRO_FONT* gameover;
const unsigned int tilesize = 12;

level_objects::level_objects(int x, int y, string name, string filename,int speed, arena* level) : entities(x,y,name,filename,speed, level)
{
	collision = level;
	playerFrameWidth = 43;
	m_y_velocity = 0.0f;
	playerFrameHeight= 43;
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	ALLEGRO_BITMAP* temp_bitmap;

	temp_bitmap = al_load_bitmap(filename.c_str());
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255));
	collision_memory.push_back(temp_bitmap);
	int widthx= al_get_bitmap_width(temp_bitmap);
	int heightx= al_get_bitmap_height(temp_bitmap);

	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255));
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

	width = al_get_bitmap_height(temp_bitmap);
	height = al_get_bitmap_width(temp_bitmap);
	m_current_frame = 0;
	m_speed = speed;
	image.push_back(temp_bitmap);
	flip_frame = false;
}

level_objects::~level_objects()
{
	for(int i = 0; i < image.size();i++)
	al_destroy_bitmap(image[i]);

	for(int i = 0; i < image.size();i++)
	collision_memory.pop_back();
}


void level_objects::setX(int x)
{
	m_xpos = x;
}

void level_objects::setY(int y)
{
	m_ypos = y;
}

int level_objects::getX(void)
{
	int i = collision->getViewPortX() * tilesize;
	return m_xpos+i;
}

int level_objects::getY(void)
{
	int i = collision->getViewPortY() * tilesize;
	return m_ypos+i;
}

bool level_objects::pixelCheck(creature* monster) //not used, but could be used if later expanded upon a
{												  //new level object that required more precision,
	al_lock_bitmap(collision_memory[0], al_get_bitmap_format(collision_memory[0]), ALLEGRO_LOCK_READONLY );
	al_lock_bitmap(monster->collision_memory[0], al_get_bitmap_format(monster->collision_memory[0]), ALLEGRO_LOCK_READONLY );
	int c_height = al_get_bitmap_height(collision_memory[0]);
	int c_width = al_get_bitmap_height(collision_memory[0]);
	int m_height = playerFrameHeight;
	int m_width = playerFrameWidth;

	int mY = monster->getY();
	int mX =  monster->getX();

	int bY = this->getY();
	int bX = this->getX();

	int top = max(bY, mY);
    int bottom = min(bY + c_height, mY + playerFrameHeight);
    int left = max(bX, mX);
    int right = min(bX + c_width, mX + playerFrameWidth);

    for(int i = top; i < bottom; i++)
        {
            for(int j = left; j < right; j++)
            {
				int player_frame = monster->getFrame();
				int player_posx =  (j - mX+(playerFrameWidth * player_frame));
                ALLEGRO_COLOR color = al_get_pixel(collision_memory[0], j - bX, i - bY);
                ALLEGRO_COLOR color2 = al_get_pixel(monster->collision_memory[0], player_posx, i - mY);

                if(color.a != 0 && color2.a != 0)
                {
					al_unlock_bitmap(collision_memory[0]);
					al_unlock_bitmap(monster->collision_memory[0]);
                    return true;
                }
            }
        }
	al_unlock_bitmap(collision_memory[0]);
	al_unlock_bitmap(monster->collision_memory[0]);
 	return false;
}
bool level_objects::collidesWith(creature* monster) //overriden in lever.cpp
{
	return true;
}

int level_objects::collidesWithMovingBlock(creature* monster, int xspeed,int yspeed) //used specifically for door
{																					//see block.cpp for detailed explaination
	if(xspeed == 0)
	{
		xspeed = 1;
	}
	else if(xspeed < 0)
	{
		xspeed *= -1;
	}
	if(yspeed == 0)
	{
		yspeed = 1;
	}
	else if(yspeed < 0)
	{
		yspeed *= -1;
	}
	int y1 = monster->getY();
	int x1 = monster->getX();
	int x2 = this->getX();
	int y2 = this->getY();

	int player_bottom = monster->getY() + playerFrameHeight;
	int tiles_bottom = y2 + height;
	int player_right = monster->getX() + playerFrameWidth;
	int tiles_right = x2 + width;

	int b_collision = tiles_bottom - monster->getY();
	int t_collision = player_bottom - y2;
	int l_collision = player_right - x2;
	int r_collision = tiles_right - monster->getX();

	if ((x2 >= x1 + playerFrameWidth - xspeed) || // is b1 on the right side of b2?
		(y2 >= y1 + playerFrameHeight - yspeed) || // is b1 under b2?
		(x1 >= x2 + this->width - xspeed) || // is b2 on the right side of b1?
        (y1 >= y2 + this->height - yspeed))   // is b2 under b1?
    {
        return 0;
    }
if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision )
{
	return -1;
}
if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision )
{
	return 1;
}
else if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision )
{
	return 2;
}
else return 0;
}

int level_objects::getFrame()
{
	return m_current_frame;
}

int level_objects::getHeight()
{
	if(entityFrameHeight == 0)
		return (al_get_bitmap_height(image[0]));
	else
		return entityFrameHeight;
}

int level_objects::getWidth()
{
	if(entityFrameHeight == 0)
		return (al_get_bitmap_width(image[0]));
	else
		return entityFrameHeight;
}

void level_objects::push(int d)
{
}

bool level_objects::canMove(int d)
{return true;}

bool level_objects::update(creature* hero)
{
	return true;
}

int level_objects::getDirection()
{
	return 0;
}

string level_objects::getName()
{
	return m_name;
}

void level_objects::setActive()
{
	active = true;
}

bool level_objects::isActive()
{
    return active;
}
