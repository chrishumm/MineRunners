#include "entities.h"
#include <sstream>

stringstream cc;

extern ALLEGRO_FONT* gameover;
const unsigned int tilesize = 12;

entities::entities(int x, int y, string name, string filename,int speed, arena* level)
{
	soundEffect = NULL; //setting all our sounds to null

	al_install_audio(); // starting our audio for all objects
    al_init_acodec_addon();//
	al_reserve_samples(2); // how many can play at once
	climb = false; // player is not climbing
	m_level = level; //pass through level
	creature_is_alive = true; //is our creature alive? yes
	entityFrameWidth = 0;	// all are defaults for all game objects
	entityFrameHeight = 0;

	playerFrameWidth = 43;
	m_y_velocity = 0.0f;
	playerFrameHeight= 43;
	m_xpos = x;
	m_ypos = y;
	m_name = name;
	ALLEGRO_BITMAP* temp_bitmap; //loading bitmaps

	temp_bitmap = al_load_bitmap(filename.c_str());	// loading all bitmaps from passed through file
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP); //setting memory bitamps for pixel perfect checks
	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255)); //making our bitmaps transparent
	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 0, 255));
	collision_memory.push_back(temp_bitmap); //pushing into our vectors
	width= al_get_bitmap_width(temp_bitmap);
	height= al_get_bitmap_height(temp_bitmap);

	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 255, 255));
	al_convert_mask_to_alpha(temp_bitmap, al_map_rgb(255, 0, 255));
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP); //converting back to normal
	m_current_frame = 0; //setting default frames for animation
	m_speed = speed;
	image.push_back(temp_bitmap); //pushing our video bitmap in
	flip_frame = false; //bitmap flip indicator
	entityParticles = new particleSystem(1,level); //particle system (overwritten by child classes)
}

entities::~entities() //destructor
{
	delete entityParticles;
	entityParticles= NULL;
	for(int i = 0; i < image.size();i++) //clearing stored bitmaps
		al_destroy_bitmap(image[i]);

	//for (int i = 0; i < collision_memory.size(); i++) //clearing memory bitmaps
	//	al_destroy_bitmap(collision_memory[i]);

	for (int i = 0; i < climbing.size(); i++) //clearing stored bitmaps
		al_destroy_bitmap(climbing[i]);
}

void entities::draw(int x, int y, bool flip, bool hero) //draw method, used by ALL child classes in game
{
	if(creature_is_alive == false) //used for particle animations
	{
			entityParticles->update(); //update all current particle classes in child classes
			entityParticles->draw(); // draw them
			return; //exit
	}

	x = x - m_level->getViewPortX();
	y = y - m_level->getViewPortY();

	if(hero == true) //drawing for hero only
	{
		int mMaxHealth = 3;
		if ((playerFrameWidth / 3*m_health) >= (playerFrameWidth / 3 * 2))
			al_draw_filled_rectangle(x , y - 10, x + (playerFrameWidth / 3* m_health), y - 5, al_map_rgb(0, 255, 0));
		else if (((playerFrameWidth / mMaxHealth*m_health) >= (playerFrameWidth / 3)) && ((playerFrameWidth / mMaxHealth*m_health) < (playerFrameWidth / 3 * 2)))
			al_draw_filled_rectangle(x, y - 10, x + (playerFrameWidth / mMaxHealth*m_health), y - 5, al_map_rgb(255, 255, 0));
		else if ((playerFrameWidth / mMaxHealth*m_health) < (playerFrameWidth / 3))
			al_draw_filled_rectangle(x, y - 10, x + (playerFrameWidth / mMaxHealth*m_health) , y - 5, al_map_rgb(255, 0, 0));


		if(flip_frame == true)
		{
			al_draw_bitmap_region(image[0], m_current_frame*playerFrameWidth, 0, playerFrameWidth, playerFrameHeight, x, y, ALLEGRO_FLIP_HORIZONTAL);
		}
		else if(flip_frame == false)
		{
			al_draw_bitmap_region(image[0], m_current_frame*playerFrameWidth, 0, playerFrameWidth, playerFrameHeight, x, y, 0);
		}
		if(climb == true)
		{
			if(m_current_frame < 5)
			{
				m_current_frame = 5;
				al_draw_bitmap_region(image[0], m_current_frame*playerFrameWidth, 0, playerFrameWidth, playerFrameHeight, x, y, 0);
			}
			else if(m_current_frame > 6)
			{
				m_current_frame = 5;
				al_draw_bitmap_region(image[0], m_current_frame*playerFrameWidth, 0, playerFrameWidth, playerFrameHeight, x, y, 0);
			}
		}
		else{}

		 const float r = 2.0f; // Orange torch light
		 const float g = 1.0f;
		 const float b = 0.0f;
		 const float a = 0.004f;

		 const int lighting_intensity= 25;
		 int circle_size = 125;
		 for(int i = 0; i < lighting_intensity; i++)
		 {
			 al_draw_filled_circle(x+(playerFrameWidth * 0.5), y+(playerFrameHeight * 0.5), circle_size, al_map_rgba_f(r*a , g*a , b*a , a));
			 if(i < 2)
			 {
				 circle_size -= rand() % 7 + 3;
			 }
			 else
				 circle_size -= rand() % 4 + 1;
		 }
	}
	if(hero == false)
	{
		if(flip == true)
		{
			al_draw_bitmap(image[m_current_frame],x,y,ALLEGRO_FLIP_VERTICAL);
		}
		else if(flip == false)
		{
			if(entityFrameWidth == 0)
				al_draw_bitmap(image[0], x, y, 0);
			else
				al_draw_bitmap_region(image[0], m_current_frame*entityFrameWidth, 0, entityFrameWidth, entityFrameHeight, x, y, 0);

		}
	}
	entityParticles->update();
	entityParticles->draw();
}

void entities::addFrame(string filename,bool movement, bool ascending)
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

void entities::setX(int)
{
}

void entities::setY(int)
{
}

int entities::getX()
{
	return 0;
}

int entities::getY()
{
	return 0;
}

bool entities::isOnScreen()
{
    float start_x = m_level->getPixelX();
    float start_y = m_level->getPixelY();

    float far_x = start_x + m_level->getScreenHeight() + 200;
    float far_y = start_y + m_level->getScreenHeight() + 200;

    if(m_xpos < start_x || m_xpos > far_x || m_ypos < start_y || m_ypos > far_y)
        return false;
    else
        return true;

}

void entities::attack(int mode)
{
}
