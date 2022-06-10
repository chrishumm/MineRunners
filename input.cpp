#include "input.h"
#include <vector>
const int tilesize = 12; //arena tilesize
static int framecap = 0;
static int max_frames = 35; //max amount of frames before update

bool update_logic = true; //check to see if we're updating logic
int consecutive_logic_updates = 0;
int max_consecutive_logic_updates = 10; //max amount of logic updates we want
static bool platform_stop = false;
input::input(int* x, int* y, ALLEGRO_EVENT* ev,ALLEGRO_EVENT_QUEUE* d, int* widthx, int* heighty, arena* collision,int speed,int* frame,bool* m_flip_frame,bool* m_climb,bool* renderFrame, bool* render_logic, entities* creature1)
{
	m_creature = creature1;
	sliding = false;		//setting default values for all movement
	m_on_platform = false;
	m_y_velocity = 0.0f;
	m_x_velocity = 0.2f;
	m_render = renderFrame;
	m_logic = render_logic;
	climb = m_climb;
	m_frame = frame;
	minimised = false;
	m_flip_frame1 = m_flip_frame;
	mcollision = collision;
	width = *widthx;		//reassigning height/width from points
	height = *heighty;
	m_x = x;
	m_y = y;
	for(int i = 0; i < SIZE_OF_KEYS_PRESSED_ENUM; i++) //setting all our controls to voice
        keys_pressed[i] = false;

	m_speed = 0.1;
	m_is_jumping = false;
	m_max_jump_height = 70;
	m_exit_game = false;
}

input::input(entities * creature_Object)
{
	m_creature = creature_Object;
}

bool input::exitGame()
{
	if (m_exit_game == false)
		return false;
	else
		return true;
}

void input::IsKeyPressed(ALLEGRO_EVENT* ev, ALLEGRO_EVENT_QUEUE* d, bool freeze) //check for key presses
{
bool got_event = al_get_next_event(d,ev); //grab our events from allegro
if(got_event == false) return;
	if(ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE || ev->type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
	{
		*m_logic = false;
		*m_render = false;
		minimised = true;
		return;
	}
	else if(ev->type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
	{
		*m_logic = true;
		minimised = false;
	}
	else if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		m_creature->attack(1);
	}
    if(ev->type == ALLEGRO_EVENT_KEY_DOWN && freeze == false) //key pressed and player hasn't died?
    {
        switch(ev->keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			m_exit_game = true;
			//exit(1); //exits out of the game
			break;
		case ALLEGRO_KEY_W:
			keys_pressed[KEY_W] = true; //sets an array of bool to true
			break;						//this will correspond to movement later on
		case ALLEGRO_KEY_S:
			keys_pressed[KEY_S] = true;
			break;
		case ALLEGRO_KEY_A:
			keys_pressed[KEY_A] = true;
			break;
		case ALLEGRO_KEY_D:
			keys_pressed[KEY_D] = true;
			break;
		case ALLEGRO_KEY_F:
			keys_pressed[KEY_F] = true;
			break;
		case ALLEGRO_KEY_SPACE:
			keys_pressed[KEY_SPACE] = true;
			if(m_is_jumping == false) //ensures player is not already jumping
			{
				m_is_jumping = true; //set to true
				jump(15.0); //call jump method.
			}
			else{}
			keys_pressed[KEY_SPACE] = false;
			break;
		}
    }
	else if(ev->type == ALLEGRO_EVENT_KEY_UP && freeze == false) //key is let go
    {
        switch(ev->keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			exit(1); //if esc, exit.
			break;
		case ALLEGRO_KEY_SPACE:				//we now set our keys back to false
			keys_pressed[KEY_SPACE] = false;//telling our later movement methods to stop moving
			break;
		case ALLEGRO_KEY_W:
			keys_pressed[KEY_W] = false;
			break;
		case ALLEGRO_KEY_F:
			keys_pressed[KEY_F] = false;
			break;
		case ALLEGRO_KEY_S:
			keys_pressed[KEY_S] = false;
			break;
		case ALLEGRO_KEY_A:
			keys_pressed[KEY_A] = false;
			last_pressed_key[KEY_A] = true;
			last_pressed_key[KEY_D] = false;
			break;
		case ALLEGRO_KEY_D:
			keys_pressed[KEY_D] = false;
			last_pressed_key[KEY_D] = true;
			last_pressed_key[KEY_A] = false;
			break;
		}
    }
	if(m_x_velocity > 0 && keys_pressed[KEY_D] == false && keys_pressed[KEY_A] == false && m_on_platform == false) //ffs lol
	{
		m_x_velocity = m_x_velocity - 0.1;
		if(keys_pressed[KEY_A] == false && last_pressed_key[KEY_A] == true && keys_pressed[KEY_D] == false)
            {
                m_x_velocity = m_x_velocity - 0.009;
                left(m_x_velocity,false);
			}

		else if(keys_pressed[KEY_D] == false && last_pressed_key[KEY_D] == true && keys_pressed[KEY_A] == false)
            {
                m_x_velocity = m_x_velocity - 0.009;
                right(m_x_velocity,false);
			}
		else{}
		if(m_x_velocity < 0)
            m_x_velocity = 0;
	}
if(got_event && ev->type == ALLEGRO_EVENT_TIMER) //timer deals with updates per frame
{
	if(minimised == true)
	{
		al_rest(0.02);
		return;
	}

	if(++consecutive_logic_updates < max_consecutive_logic_updates) //logic updates not max?
	{
		*m_logic = true; //allow logic to be completed
	}

	if(al_is_event_queue_empty(d)) //all events empty?
	{
		*m_render = true; //allow rendering to take place
		consecutive_logic_updates=0;
	}
}
	 if(update_logic == true)
	 {
		 Draw();
	 }
}
void input::Draw()
{
    if(m_x_velocity >= 7.0f)
        m_x_velocity = 7.0f;

		if(keys_pressed[KEY_A] == true)
		{
            if(m_x_velocity < 5.0f)
                m_x_velocity += 0.1f;

			left(m_x_velocity,false);
		}
		if(keys_pressed[KEY_D] == true)
		{
            if(m_x_velocity < 5.0f)
                m_x_velocity += 0.1f;

			right(m_x_velocity,false);
		}

		if(m_is_jumping =true)
            gravity();

}

void input::jump(float y_veloc) //desired y velocity is passed through
{
    m_y_velocity = y_veloc; //update velocity
}

void input::gravity()
{ //here we calculate our gravity for our player
	float potential_velocity = (m_y_velocity - mcollision->getGravity());
	if(potential_velocity > 0.0f) //if our velocity is above 0
	{
		if(m_is_jumping == true)
			*(climb) = true; //player jumping animation is true

 		up(false);// call up
	}
	else if(mcollision->getGravity() == 0.0f) //if we have no gravity
	{
		down(true); //still go down at a steady pace
	}
	else
	{
		down(false); //go down with normal gravity
	}
}

void input::up(bool platformUp) //called going up.
{								//bool checks if it's a platform
	if (platformUp == true)
	{	m_y_velocity = 0.0;
		mcollision->setGravity(0.0);
		mcollision->setViewPortY(mcollision->getViewPortY() - 2);
	}
	if (mcollision->isWall(m_creature->getX(), m_creature->getY() - 3) == true || mcollision->isWall((m_creature->getX() + width - 3), m_creature->getY()) == true)
	{
		m_y_velocity = 0; //if we collide with a wall, reset velocity
		platform_stop = true; //tell platform to stop
		return;
	}
                for(int i = 0; i < m_y_velocity; i++)
                {
                            if(platformUp == false)
                            {
                                m_y_velocity = m_y_velocity - mcollision->getGravity();
								m_creature->setY(m_creature->getY() - 0.5); //we effect heros object via pointer
                            }
                            bool centre = mcollision->isCentreYTop(m_creature->getY());
                            if (centre  == true && mcollision->getViewPortY() > 5)
                            { //here we check if hero is currently centred
                                mcollision->setViewPortY(mcollision->getViewPortY() - 1);
                            } //if not, call arena and change viewport (changes rendering)
                        

                }

		 //here we now adjust the y velocity with gravity
		 if(platformUp == true)
		 {
			if(framecap == max_frames)
			{ //now we update the frames, only updating after max_frames
				framecap = 0;	//has been met
				if(platformUp == false)
				{
					(*m_frame)++; //we now update our frames
				}
			}
			else //else frame is not to be updated
			{	 //player is falling, so fall animation is used
				(*climb) = true;
				framecap++;
			}
        }

}

void input::down(bool onPlatform) //used when player is moving down
{
	bool collided = false;
	m_y_velocity = m_y_velocity - mcollision->getGravity(); //update gravity
	float m_y_falling = 0; //m_y_falling used to convert velocity
	if(m_y_velocity < 0) //into a positive, for collision checks
        m_y_falling = m_y_velocity*-1;

if(onPlatform == true)
	m_y_falling = 0.0f; //stops playing from falling through platforms.
else
{
	m_y_falling += 2;
}
	for(int q = 0; q < height;q++) //we cycle through height and width for collisions
	{
			for(int i = 0; i < width; i++)
			{
					if(mcollision->isWall((m_creature->getX()),((m_creature->getY()) + height)) == true || mcollision->isWall(((m_creature->getX())+q),((m_creature->getY())+i) + m_y_falling) == true || mcollision->isPlatform((m_creature->getX() +q),((m_creature->getY())+height) + m_y_falling) == true)
					{
						for(int x = 0; x < m_y_falling + 10;x++)
						{
							if(mcollision->isBouncy((m_creature->getX()),((m_creature->getY()) + height)) == true || mcollision->isBouncy(((m_creature->getX())+q),((m_creature->getY())+i) + x) == true)
							{
								jump(10.5f); //if bouncy, call player to jump
								sliding = false;
								return;
							}
							if((mcollision->isWall((m_creature->getX()),((m_creature->getY()) + height)) == true || mcollision->isWall(((m_creature->getY())+q),((m_creature->getY())+i) + x) == true || mcollision->isPlatform((m_creature->getY() +q),(m_creature->getY() +height) + x) == true) &&  mcollision->isIce((m_creature->getX() +q),(m_creature->getY() +height) + x) == false)
							{
								m_y_falling = 0; //if we're hitting the floor or a platform
								m_y_velocity = 0;
								sliding = false; //stop sliding and break out of checks.
								break;
							}
							else if( mcollision->isIce((m_creature->getX() +q),(m_creature->getY() +height) + x) == true)
							{
								m_y_falling =  x; //we fall by certain amount until collided.
								for (int c = 0; c < m_y_falling - 5; c++)
									m_creature->setY(m_creature->getY() + 0.5);
								m_y_falling = 0;
								sliding = true; //we now set sliding to try
								break;
							}
						}
								m_y_velocity = 0;
								m_y_falling = 0;//all velocity is stopped
								if(m_is_jumping == true)
									(*m_frame) = 1;

								m_is_jumping = false;
								collided = true; //we are now colliding
								*(climb) = false; // we are now not falling

								return; // return
					}

			}
	}
	//

	for(int i = 0; i < m_y_falling-1;i++)
		{
		m_creature->setY(m_creature->getY() + 1); //we now move down by the amount we can fall
				bool centre = mcollision->isCentreYBot(m_creature->getY());
				if(centre == true)
				{
					mcollision->setViewPortY(mcollision->getViewPortY() + 1); //we set the viewport to match
				}
                if(m_creature->getY() - mcollision->getScreenHeight() * 0.5 < 0)
                {
                    float current_pos = m_creature->getY() - mcollision->getScreenHeight() * 0.5;
                    //mcollision->setViewPortY(0);
                }																// correct rendering location
		}
			if(framecap == max_frames) //update frame animation
			{
				framecap = 0;
				if(onPlatform == false)
				{
					(*m_frame)++;
				}
				else
				{
				}
				if(*m_frame >3)
				(*m_frame) = 0;
			}
			else
			{
			//	(*m_flip_frame1) = false;
				framecap++;
			}
}

void input::right(float yveloc,bool platform) //right collision
{
			bool break_loop = false;
			bool centre = mcollision->isCentreX(m_creature->getX());

if (platform == false)	
for(int x = 0; x < m_x_velocity;x++) //we now check every pixel in front of us up to the current speed
{									//we're travelling at. This allows us to check for walls
	for(int q = 0; q < height;q++)
	{
			for(int i = 0; i < width; i++)
			{
				if(mcollision->isWall((m_creature->getX() + width + 1) + x,(m_creature->getY())) == true || mcollision->isWall((m_creature->getX() +width + 1) + (x),(m_creature->getY() +q)) == true)
				{
					if(mcollision->isRightSlope((m_creature->getX()) ,(m_creature->getY())) == true || mcollision->isRightSlope((m_creature->getX() +i) + (width +1),(m_creature->getY() +q)) == true)
					{	//we check right slopes
						bool centre = mcollision->isCentreYTop(m_creature->getY());
						if (centre  == true && mcollision->getViewPortY() > 1) //if slope is found
						{													 //update viewport
							mcollision->setViewPortY(mcollision->getViewPortY() - 2);
						}
						for (int i = 0; i < m_x_velocity; i++) //now we move up the slope
							m_creature->setX(m_creature->getX() + 1);
						for(int i = 0; i < m_x_velocity; i++)
							m_creature->setY(m_creature->getY() - 2);
						break_loop = true; //now we break out from our checks
						break;
					}
					else
					{
						m_x_velocity = 0.0f;
						return;
					}
				}

			}
			if(break_loop == true)
				break;
	}
			if(break_loop == true)
				break;

			m_creature->setX(m_creature->getX() + 1); //now we update, allowing us to move one up, as the check has allows us
}			  //to go uright, as we're not colliding
else 
{
	m_creature->setX(m_creature->getX() + 2);
	if ((centre == true))	//if we're not centre
	{						// add to the view port
		mcollision->setViewPortX(mcollision->getViewPortX() + 2);
	}
}

			for(int i = 0; i < m_x_velocity;i++)
			{	//depending on how much we've moved up, we check if we're centre
				centre = mcollision->isCentreX(m_creature->getX());
				if((centre == true))	//if we're not centre
				{						// add to the view port
					mcollision->setViewPortX(mcollision->getViewPortX() + 1);
				}

			}
			if(framecap == max_frames) //update moving animation
			{
				framecap = 0;
				if(platform == false)
				{
					(*m_frame)++;
				}
				if(*m_frame >3)
				{
					(*m_frame) = 0;
				}
				if(*climb == true) //check if we're moving right
				{					//and jumping
					if(*m_frame > 1)
					(*m_frame) = 0;
				}
			}
			else
			{
				if(keys_pressed[KEY_A] == true && keys_pressed[KEY_D] == true)
				{
					(*climb) = true;
				}
				else
				{
					(*climb) = true;
				}
				(*m_flip_frame1) = false;
				framecap++; //adjusting frame count
			}
}


void input::left(float yveloc, bool platform) //identical to right
{									//we now factor in going left
	bool centre = mcollision->isCentreX(m_creature->getX());

bool break_loop = false;
if(platform == false)
{
    for(int x = 0; x < m_x_velocity;x++)
    {
        for(int q = 0; q < height;q++)
        {
                for(int i = 0; i < width; i++)
                {
                    if(mcollision->isWall((m_creature->getX()),(m_creature->getY())) == true || mcollision->isWall((m_creature->getX() +q)-(x+1),(m_creature->getY() +i)) == true)
                    {
                        if(mcollision->isLeftSlope((m_creature->getX()),(m_creature->getY())) == true || mcollision->isLeftSlope((m_creature->getX() +q)-(x+1),(m_creature->getY() +i)) == true)
                        {
                            bool centre = mcollision->isCentreYTop(m_creature->getY());
                            if (centre  == true && mcollision->getViewPortY() > 1)
                            {
                                mcollision->setViewPortY(mcollision->getViewPortY() - 2);
                            }
                            for(int i = 0; i < m_x_velocity; i++)
                                m_creature->setX(m_creature->getX() - 1);
                            for(int i = 0; i < m_x_velocity; i++)
                                m_creature->setY(m_creature->getY() - 1);
                            break_loop = true;
                            break;
                        }
                        m_x_velocity = 0.0f;
                        return;
                    }
                }
                if(break_loop == true)
                    break;
        }
                if(break_loop == true)
                    break;
                m_creature->setX(m_creature->getX() -1);
    }
}
else {
	m_creature->setX(m_creature->getX() - 2);
	if ((centre == true))	//if we're not centre
	{						// add to the view port
		mcollision->setViewPortX(mcollision->getViewPortX() - 2);
	}
}
			for(int i = 0; i < m_x_velocity;i++)
			{
				 centre = mcollision->isCentreX(m_creature->getX());
				if(centre == false)
				{
						mcollision->setViewPortX(mcollision->getViewPortX() - 1);
				}
			}
			if(framecap == max_frames)
			{
				framecap = 0;
				if(platform == false)
				{
					(*m_frame)++;
				}
				if(*m_frame >3)
				{
				(*m_frame) = 0;
				}
				if(*climb == true)
				{
					if(*m_frame > 1)
					(*m_frame) = 0;
				}
			}
			else
			{
				if(keys_pressed[KEY_C] == true && keys_pressed[KEY_A] == true)
				{
					(*climb) = true;
				}
				else
				{
				}
				(*m_flip_frame1) = true;
				framecap++;
			}
		}

bool input::platformLift(int direction) //called from game.cpp
{//this method allows us to tell our player we're on a platform and move
	m_is_jumping = false; //we reset all player states
	m_y_velocity = 0.0f; //velocity is 0, we don't want to fall
	switch(direction) //depending on direction of movement
	{
		case 0 :
			{
				up(true);//call player to go up
				if(platform_stop == true) //if player hits
					{					  //tell our platform to stop
						platform_stop = false;
						return true;
					}

				break;
			}
		case 1 : //platform is going down
			{
				bool centre = mcollision->isCentreYBot(m_creature->getY()); //check if player is centre
				if(centre == true) //update viewport
					mcollision->setViewPortY(mcollision->getViewPortY() + 1);

				m_on_platform = true;
				m_creature->setY(m_creature->getY() + 1); //move player
			}
				break;
		case 2 :
				left(1.0f,true); //move player left
				break;
		case 3 :
				right(1.0f,true); //move player right
				break;
		case 4 :
				jump(10.5f); //cause player to bounce up
				up(true);
				break;
		default:
				break;
	}
	return false;
}

void input::setVelocityY(int velocity) //called from the game.cpp to change
{										// our input
	m_y_velocity = 0.0f;
}

void input::setVelocityX(int velocity)
{
	m_x_velocity = 0.0f;
}
