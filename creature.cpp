#include "creature.h"
#include <sstream>
extern ALLEGRO_FONT* gameover; //we use extern, grabbing our font from already defined font somewhere else
const unsigned int tilesize = 12; //size of our tiles in game

creature::creature(int x, int y, string name, string filename,int speed, arena* level) : entities(x,y,name,filename,speed, level)
{
	collision = level; //passing through defaults for 
	playerFrameWidth = 43; //seperate defined widths, using spritesheet
	m_y_velocity = 0.0f;
	playerFrameHeight= 43;
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	ALLEGRO_BITMAP* temp_bitmap;

	temp_bitmap = al_load_bitmap(filename.c_str()); //loading our bitmaps for pixel perfect check
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255));
	collision_memory.push_back(temp_bitmap);

	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255)); //loading bitmaps for rendering
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

	width = al_get_bitmap_height(temp_bitmap); //getting default widths for creature
	height = al_get_bitmap_width(temp_bitmap);
	m_current_frame = 0;
	m_speed = speed;
	image.push_back(temp_bitmap); //pushes all bitmaps into single vector
	flip_frame = false;
}

creature::~creature()
{
	for(int i = 0; i < image.size();i++) //deallocating all bitamps
	al_destroy_bitmap(image[i]);

	for(int i = 0; i < image.size();i++) //deallocating all memory bitmaps
	collision_memory.pop_back();
}
bool creature::up(int x, int y) //used for monster checks above
{
	if(collision->isWall(x, y - m_speed) == true  || collision->isWall(x + width - 1, y - m_speed) == true)
	{
		return false;
	}
	return true;
}

bool creature::down(int x, int y) //used for monster checks below
{
	if(collision->isWall(x,y+height+1) == true || collision->isWall(x+width-1,y+height+1) == true || collision->isWall(x+(width-(tilesize*2))-1,y+height+1) == true || collision->isWall(x+(width-tilesize)-1,y+height+1) == true || collision->isWall(x+(width-(width - tilesize))-1,y+height+1) == true)
	{
		return false;
	}
	return true;
}

bool creature::left(int x, int y) //used for monster checks left
{
	if(collision->isWall(x-1,y) == true || collision->isWall(x-1,y+height-1) == true|| collision->isWall(x-1,y+(height-(tilesize*2))-1) == true|| collision->isWall(x-1,y+height-(height-tilesize)-1) == true|| collision->isWall(x-1,y+height-tilesize)-1 == true || collision->isWall(x-1,y+height) == true)
	{
		return false;
	}
	return true;
}

bool creature::right(int x, int y) //used for monster checks right
{
	if(collision->isWall(x+width+1,y) == true || collision->isWall(x+width+1,y) == true || collision->isWall(x+width+1,y+(height-(tilesize*2))+1) == true || collision->isWall(x+width+1,y+(height-tilesize)+1) == true || collision->isWall(x+width+1,y+height-(height-tilesize)+1) == true)
	{
		return false;
	}
	return true;
}

void creature::setX(int x) //setter for x 
{
	m_xpos = x;
}

void creature::setY(int y) //setter for y
{
	m_ypos = y;
}

int creature::getX(void) //getting for x 
{
	int i = collision->getViewPortX() * tilesize; //offsets viewports
	return m_xpos+i;
}

int creature::getY(void) //getting for x 
{
	int i = collision->getViewPortY() * tilesize; //offsets viewports
	return m_ypos+i;
}

bool creature::pixelCheck(creature* monster) //pixel perfect check
{
	al_lock_bitmap(collision_memory[0], al_get_bitmap_format(collision_memory[0]), ALLEGRO_LOCK_READONLY ); //load our bitamps for check
	al_lock_bitmap(monster->collision_memory[0], al_get_bitmap_format(monster->collision_memory[0]), ALLEGRO_LOCK_READONLY );

	int c_height = al_get_bitmap_height(collision_memory[0]); //get the height for bitmap collision
	int c_width = al_get_bitmap_width(collision_memory[0]);
	int m_height = playerFrameHeight; //store player heights
	int m_width = playerFrameWidth;

	int mY = monster->getY();
	int mX =  monster->getX();

	int bY = this->getY();
	int bX = this->getX();

	int top = max(bY, mY);								// calculate overlapping segments
    int bottom = min(bY + c_height, mY + playerFrameHeight);
    int left = max(bX, mX); 
    int right = min(bX + c_width, mX + playerFrameWidth);

    for(int i = top; i < bottom; i++) //loop top left pixels to bottom right
        {
            for(int j = left; j < right; j++)
            {
				int player_frame = monster->getFrame(); //get current frame for the player
				int player_posx =  (j - mX+(playerFrameWidth * player_frame)); //calculate current position
                ALLEGRO_COLOR color = al_get_pixel(collision_memory[0], j - bX, i - bY); //get current pixel for both
                ALLEGRO_COLOR color2 = al_get_pixel(monster->collision_memory[0], player_posx, i - mY);
                 
                if(color.a != 0 && color2.a != 0) //are alphas solid?
                {
					al_unlock_bitmap(collision_memory[0]); //unlock
					al_unlock_bitmap(monster->collision_memory[0]);
                    return true; //we have a collision, return true
                }
            }
        }
	al_unlock_bitmap(collision_memory[0]); //no collision, unlock and return false
	al_unlock_bitmap(monster->collision_memory[0]);
 	return false;
}
bool creature::collidesWith(creature* monster) //general collision checks
{
	if(this == monster) //ensure creature is not colliding against itself
		return false;

	al_get_bitmap_height(image[m_current_frame]); //get height from current frame
	int y1 = monster->getY();
	int x1 = monster->getX();
	
	int c_height = al_get_bitmap_height(monster->collision_memory[m_current_frame]);
	int c_width = playerFrameWidth;

	if ((this->m_xpos >= monster->getX() + c_width - 1) || // is b1 on the right side of b2?
		(this->m_ypos >= monster->getY() + c_height - 1) || // is b1 under b2?
		(monster->getX() >= this->m_xpos + this->width - 1) || // is b2 on the right side of b1?
        (monster->getY() >= this->m_ypos + this->height - 1))   // is b2 under b1?
    {
        // no collision
        return false;
    }
	if(m_on_platform == false)
	{
		//we check to see if player is playing normally, if so, don't check against platform
	}
	else if(pixelCheck(monster) == false) //ensure player is colliding with platform
		return false;
	else
		return true;
}

bool creature::collidesWithBot(creature* monster) //checks for collision on the bottom of creatures only
{
	if(this == monster)
		return false;

	al_get_bitmap_height(image[m_current_frame]);
	int y1 = monster->getY();
	int x1 = monster->getX();
	int c_height = al_get_bitmap_height(monster->collision_memory[m_current_frame]);
	int c_width = al_get_bitmap_width(monster->collision_memory[m_current_frame]);

	if ((this->m_xpos >= monster->getX() + c_width - 1) || // is b1 on the right side of b2?
		(this->m_ypos >= monster->getY() + c_height - 1) || // is b1 under b2?
		(monster->getX() >= this->m_xpos + this->width - 1) || // is b2 on the right side of b1?
        (monster->getY() >= this->m_ypos + this->height - 1))   // is b2 under b1?
    {
        // no collision
        return false;
    }
int player_bottom = monster->getY() + playerFrameHeight + playerFrameHeight;	//calculating current positions
int monster_bottom = this->getY() + height;										//for player and monster
int player_right = monster->getX() + playerFrameWidth;
int monster_right = this->getX() + width;

int bottom_collision = monster_bottom - monster->getY() - playerFrameHeight;
int top_collision = player_bottom - this->getY();
int left_collision = player_right - this->getX();
int right_collision = monster_right - monster->getX();
//this checks to see the direction of the collision, only checking for bottom collisions.
if(bottom_collision < top_collision && bottom_collision < left_collision && bottom_collision < right_collision)
{	
	return true;
}
}

 //this deals with collision with bullets and creatures
bool creature::collidesWithBullet(int b_width, int b_height, int b_x, int b_y, int xvel, int yvel)
{
	int hero_x = getX() - collision->getViewPortX();
	int hero_y = getY() - collision->getViewPortY();

	if (hero_x >= b_x + b_width - 1)	//checks to see if we're within bounds of bullet
		return false;
	if(hero_y >= b_y + b_height - 1)
		return false;
	if(b_x >= hero_x + playerFrameWidth - 1)
		return false;
    if(b_y >= hero_y + playerFrameHeight - 1)
		return false;

	return true; //if we are, return true
	
}

void creature::addFrame(string filename,bool movement, bool ascending) //adds frames to objects without spritesheets
{
	ALLEGRO_BITMAP* temp_bitmap;
	temp_bitmap = al_load_bitmap(filename.c_str());
	al_convert_mask_to_alpha( temp_bitmap, al_map_rgb( 0, 0, 0 ) );
	image.push_back(temp_bitmap);
}

int creature::getFrame()
{
	return m_current_frame;
}

void creature::gravity()
{
}

int creature::getHeight()
{
	return (al_get_bitmap_width(image[m_current_frame]));
}

int creature::getWidth() //grabs width
{
	return (al_get_bitmap_width(image[m_current_frame]));
}

bool creature::getName(creature* isHero) //returns name
{
	if(isHero->m_name  == "Chris") //only return true if name is the players
	{
		return true;	//signifies to draw method within game 
	}
	else
	{
		return false;
	}
}