#include "pickups.h"		// pickups class deals with all pickups within the game
#include <sstream>

extern ALLEGRO_FONT* gameover;
const unsigned int tilesize = 12;

pickups::pickups(int x, int y, string name, string filename,int speed, arena* level) : entities(x,y,name,filename,speed, level)
{
	collision = level;
	playerFrameWidth = 43;
	m_y_velocity = 0.0f;
	playerFrameHeight= 43;
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	ALLEGRO_BITMAP* temp_bitmap;

	temp_bitmap = al_load_bitmap(filename.c_str());							//once again, we push in our bitmaps
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
/*
pickups::pickups(int x, int y, string name,ALLEGRO_BITMAP* bitmap_copy,int speed, arena* level) : entities(x, y, name, filename, speed, level)
{
	collision = level;
	playerFrameWidth = 43;
	m_y_velocity = 0.0f;
	playerFrameHeight = 43;
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	ALLEGRO_BITMAP* temp_bitmap;
	temp_bitmap = bitmap_copy;
	collision_memory.push_back(temp_bitmap);
	int widthx = al_get_bitmap_width(temp_bitmap);
	int heightx = al_get_bitmap_height(temp_bitmap);

	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255));
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

	width = al_get_bitmap_height(temp_bitmap);
	height = al_get_bitmap_width(temp_bitmap);
	m_current_frame = 0;
	m_speed = speed;
	image.push_back(temp_bitmap);
	flip_frame = false;
}
*/
pickups::~pickups()
{
	for(int i = 0; i < image.size();i++) //deallocating
		al_destroy_bitmap(image[i]);

	for(int i = 0; i < image.size();i++)
		collision_memory.pop_back();
}

void pickups::setX(int x)	//getters/setters
{
	m_xpos = x;
}

void pickups::setY(int y)
{
	m_ypos = y;
}

int pickups::getX(void)
{
	return m_xpos;
}

int pickups::getY(void)
{
	return m_ypos;
}

bool pickups::pixelCheck(creature* monster) //perfect pixel collision between player and pickups
{
	if(monster->getName(monster) == false)
		return false;

	al_lock_bitmap(collision_memory[0], al_get_bitmap_format(collision_memory[0]), ALLEGRO_LOCK_READONLY );
	al_lock_bitmap(monster->collision_memory[0], al_get_bitmap_format(monster->collision_memory[0]), ALLEGRO_LOCK_READONLY );

	int c_height = al_get_bitmap_height(collision_memory[0]);
	int c_width = al_get_bitmap_height(collision_memory[0]);
	int m_height = playerFrameHeight;
	int m_width = playerFrameWidth;

	int mY = monster->getY();
	int mX =  monster->getX();

	int top = max(this->m_ypos, mY); 
    int bottom = min(this->m_ypos + c_height, mY + playerFrameHeight);
    int left = max(this->m_xpos, mX); 
    int right = min(this->m_xpos + c_width, mX + playerFrameWidth);

    for(int i = top; i < bottom; i++)
        {
            for(int j = left; j < right; j++)
            {
				int player_frame = monster->getFrame();
				int player_posx =  (j - mX+(playerFrameWidth * player_frame));
                ALLEGRO_COLOR color = al_get_pixel(collision_memory[0], j - this->m_xpos, i - this->m_ypos);
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
bool pickups::collidesWith(creature* monster)
{
	al_get_bitmap_height(image[0]);
	int y1 = monster->getY();
	int x1 = monster->getX();

	if ((this->m_xpos >= x1 + playerFrameWidth - 1) || // is b1 on the right side of b2?
		(this->m_ypos >= y1 + playerFrameHeight - 1) || // is b1 under b2?
		(x1 >= this->m_xpos + this->width - 1) || // is b2 on the right side of b1?
        (y1 >= this->m_ypos + this->height - 1))   // is b2 under b1?
    {
        // no collision
        return false;
    }

	if(pixelCheck(monster) == false)
			return false;
	else
		return true;
}

int pickups::getFrame()
{
	return m_current_frame;
}

int pickups::getHeight()
{
	return (al_get_bitmap_width(image[0]));
}

int pickups::getWidth()
{
	return (al_get_bitmap_width(image[0]));
}

void pickups::applyEffect(hero* appliedHero)
{
}

void pickups::update()
{
}