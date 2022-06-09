#include "blocks.h"
#include <sstream>
extern ALLEGRO_FONT* gameover; //use preexisting font
const unsigned int tilesize = 12; //tilesize for arena

blocks::blocks(int x, int y, string name, string filename,int speed, arena* level) : entities(x,y,name,filename,speed, level)
{	//blocks class deals with objects with a blocky nature - movable platforms, crushing blocks, moving platforms
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

	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255));
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

	width = al_get_bitmap_height(temp_bitmap);
	height = al_get_bitmap_width(temp_bitmap);
	m_current_frame = 0;
	m_speed = speed;
	image.push_back(temp_bitmap);
	flip_frame = false;
}

blocks::~blocks()
{
	for(int i = 0; i < image.size();i++) //deallocates images in vector
	al_destroy_bitmap(image[i]);

	for(int i = 0; i < image.size();i++)
	collision_memory.pop_back();
}
bool blocks::up(int x, int y) //see creture.cpp for explaination.
{
	if(collision->isWall(x,y-1) == true || collision->isWall(x+(width-(tilesize*2))-1,y-1) == true || collision->isWall(x+width-1,y-1) == true|| collision->isWall(x+(width-tilesize)-1,y-1) == true|| collision->isWall(x+(width-(width - tilesize))-1,y-1) == true)
	{
		return false;
	}
	return true;
}

bool blocks::down(int x, int y)
{
	if(collision->isWall(x,y+height+1) == true || collision->isWall(x+width-1,y+height+1) == true || collision->isWall(x+(width-(tilesize*2))-1,y+height+1) == true || collision->isWall(x+(width-tilesize)-1,y+height+1) == true || collision->isWall(x+(width-(width - tilesize))-1,y+height+1) == true)
	{
		return false;
	}
	return true;
}

bool blocks::left(int x, int y)
{
	if(collision->isWall(x-1,y) == true || collision->isWall(x-1,y+height-1) == true|| collision->isWall(x-1,y+(height-(tilesize*2))-1) == true|| collision->isWall(x-1,y+height-(height-tilesize)-1) == true|| collision->isWall((x-1),y+height-tilesize) == true || collision->isWall(x-1,y+height) == true)
	{
		return false;
	}
	return true;
}

bool blocks::right(int x, int y)
{
	if(collision->isWall(x+width+1,y) == true || collision->isWall(x+width+1,y) == true || collision->isWall(x+width+1,y+(height-(tilesize*2))+1) == true || collision->isWall(x+width+1,y+(height-tilesize)+1) == true || collision->isWall(x+width+1,y+height-(height-tilesize)+1) == true)
	{
		return false;
	}
	return true;
}

void blocks::setX(int x)
{
	m_xpos = x;
}

void blocks::setY(int y)
{
	m_ypos = y;
}

int blocks::getX(void)
{
	int i = collision->getViewPortX() * tilesize;
	return m_xpos+i;
}

int blocks::getY(void)
{
	int i = collision->getViewPortY() * tilesize;
	return m_ypos+i;
}

bool blocks::pixelCheck(creature* monster) //see creature.cpp for perfect pixel check explanation
{
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
bool blocks::collidesWith(creature* monster) //deals with normal collision
{
	int y1 = monster->getY();
	int x1 = monster->getX();
	int x2 = this->getX();
	int y2 = this->getY();

	int player_bottom = monster->getY() + playerFrameHeight; //calculates colliding areas
	int blocks_bottom = y2 + height;
	int player_right = monster->getX() + playerFrameWidth;
	int blocks_right = x2 + width;

	int bottom_collision = blocks_bottom - monster->getY(); //calculates colliding areas
	int top_collision = player_bottom - y2;
	int left_collision = player_right - x2;
	int right_collision = blocks_right - monster->getX();

	if ((x2 >= monster->getX() + playerFrameWidth - 1) || // is b1 on the right side of b2?
		(y2 >= monster->getY() + playerFrameHeight - 1) || // is b1 under b2?
		(monster->getX() >= x2 + entityFrameWidth - 1) || // is b2 on the right side of b1?
        (monster->getY() >= y2 + entityFrameHeight - 1))   // is b2 under b1?
    {
        return false;
    }
if (top_collision < bottom_collision && top_collision < left_collision && top_collision < right_collision ) //if any area
{																											//is colliding
	return true;																							//return true
}
if (left_collision < right_collision && left_collision < top_collision && left_collision < bottom_collision )
{
	return true;
}
else if (right_collision < left_collision && right_collision < top_collision && right_collision < bottom_collision )
{
	return true;
}
else return false;
}

bool blocks::collidesWithBot(creature* monster) //checks for collision on bottom of the object
{

	int height = al_get_bitmap_height(image[m_current_frame]);
	int width = al_get_bitmap_width(image[m_current_frame]);

	int player_y = monster->getY();	//gets preset coordinates
	int player_x = monster->getX();

	int crusher_y = getY();
	int crusher_x = getX();

	int c_height = al_get_bitmap_height(monster->collision_memory[m_current_frame]);
	int c_width = al_get_bitmap_width(monster->collision_memory[m_current_frame]);


	if ((crusher_x >= player_x + playerFrameHeight - 1) || // is b1 on the right side of b2?
		(crusher_y >= player_y + playerFrameHeight - 1) || // is b1 under b2?
		(player_x >= crusher_x + width - 1) || // is b2 on the right side of b1?
        (player_y >= crusher_y + height - 1))   // is b2 under b1?
    {
        // no collision
        return false;
    }

int player_bottom = player_y + playerFrameHeight; //calculate boxes.
int blocks_bottom = crusher_y + height;
int player_right = player_x + playerFrameWidth;
int blocks_right = crusher_x + width;

int bottom_collision = blocks_bottom - player_y;
int top_collision = player_bottom - crusher_y;
int left_collision = player_right - crusher_x;
int right_collision = blocks_right - player_x;
//specifically check for only the bottom collision
if(bottom_collision < top_collision && bottom_collision < left_collision && bottom_collision < right_collision)
{
		if(pixelCheck(monster) == false) //ensure player is truly colliding with pixel perfect check
		{
			return false;
		}
	return true;
}

return false;
}
/*


*/
bool blocks::collidesWithMovingPlatform(creature* monster,int speed)
{
	if(speed == 0)
	{
		speed = 1;
	}
	else if(speed < 0) //set speed to positive integer for collision checks
	{
		speed *= -1;
	}

	int y1 = monster->getY(); //get coordinates
	int x1 = monster->getX();
	int y2 = monster->getY();

	if(m_name != "Crumbling_Platform")
	{
		int height = al_get_bitmap_height(image[m_current_frame]);
		int width = al_get_bitmap_width(image[m_current_frame]);
	}


	int player_y = monster->getY();
	int player_x = monster->getX();

	int platform_y = getY();
	int platform_x = getX();


	if ((platform_x >= player_x + playerFrameHeight - 1) || // is b1 on the right side of b2?
		(platform_y >= player_y + playerFrameHeight - 1) || // is b1 under b2?
		(player_x >= platform_x + width - 1) || // is b2 on the right side of b1?
        (player_y >= platform_y + height - 1))   // is b2 under b1?
    {
        // no collision
        return false;
    }
	 //calculate if collided
	int player_bottom = monster->getY() + playerFrameHeight;
	int blocks_bottom = y2 - height;
	int player_right = monster->getX() + playerFrameWidth;
	int blocks_right = this->m_xpos + width;

	int bottom_collision = blocks_bottom - monster->getY();
	int top_collision = player_bottom - y2;
	int left_collision = player_right - this->m_xpos;
	int right_collision = blocks_right - monster->getX();

 	if(this->m_ypos >= monster->getY() + (height - 30)) //checks if player is on top
	{
		m_on_platform = true; //if true, set to true
		return true;
	}
	else
	{
		m_on_platform = false; //else player is not colliding
		return false;
	}
}

int blocks::collidesWithMovingBlock(creature* monster, int xspeed,int yspeed)
{
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
	int blocks_bottom = y2 + height;
	int player_right = monster->getX() + playerFrameWidth;
	int blocks_right = x2 + width;

	int bottom_collision = blocks_bottom - monster->getY();
	int top_collision = player_bottom - y2;
	int left_collision = player_right - x2;
	int right_collision = blocks_right - monster->getX();

	if ((x2 >= monster->getX() + playerFrameWidth - xspeed) || // is b1 on the right side of b2?
		(y2 >= monster->getY() + playerFrameHeight - yspeed) || // is b1 under b2?
		(monster->getX() >= x2 + this->width - xspeed) || // is b2 on the right side of b1?
        (monster->getY() >= y2 + this->height - yspeed))   // is b2 under b1?
    {
        return 0;
    }

if (top_collision < bottom_collision && top_collision < left_collision && top_collision < right_collision )
{                           
	return -1;
}
if (left_collision < right_collision && left_collision < top_collision && left_collision < bottom_collision )
{
	return 1;
}
else if (right_collision < left_collision && right_collision < top_collision && right_collision < bottom_collision )
{
	return 2;
}
else return 0;
}

void blocks::addFrame(string filename,bool movement, bool ascending)
{
	ALLEGRO_BITMAP* temp_bitmap;
	temp_bitmap = al_load_bitmap(filename.c_str());
	al_convert_mask_to_alpha( temp_bitmap, al_map_rgb( 0, 0, 0 ) );
	if(movement == true)
	{
		image.push_back(temp_bitmap);
	}
	if(ascending == true)
	{
		climbing.push_back(temp_bitmap);
	}
}

int blocks::getFrame()
{
	return m_current_frame;
}

void blocks::gravity()
{
}

int blocks::getHeight()
{
	if(entityFrameHeight == 0) //if normal, returns bitmap height
		return (al_get_bitmap_height(image[0]));
	else//if pre-defined, call entityFrameHeight specifically for spritesheet
		return entityFrameHeight;
}

int blocks::getWidth()
{
	if(entityFrameHeight == 0)
		return (al_get_bitmap_width(image[0]));
	else
		return entityFrameHeight;
}

void blocks::push(int d)
{

}

bool blocks::canMove(int d) //a lot of these are overriden in child classes
{return true;}

bool blocks::update(creature* hero)
{
	return true;
}

int blocks::getDirection()
{
	return 0;
}

string blocks::getName()
{
	return m_name;
}

void blocks::setActive() //overrided
{
	active = true;
}