#include "game.h" //welcome to the main game. This class is created from menu
using namespace std; // and acts as a "scene". Everything within is generated from the
					//level text file that is provided!
////////////////ALLEGRO////////////////////////////////////////////
ALLEGRO_TIMER *timer = NULL; // creates our timer
ALLEGRO_EVENT_QUEUE* event_queue = NULL; // event queues to get events from player
ALLEGRO_EVENT_QUEUE* event_queue_two = NULL;

ALLEGRO_FONT* gameover = NULL; // fonts used for drawing
ALLEGRO_FONT* font1;        //
ALLEGRO_BITMAP *bmp;        //

ALLEGRO_DISPLAY* display = NULL; // current display window
ALLEGRO_MOUSE_STATE state; // mouse input
ALLEGRO_EVENT ev; // current event from queue
//////////////CONST VALUES (Change later)//////////////////////////
const int WIDTH = 900; // width for display
const int HEIGHT = 700; // height for display
const float FPS = 60; // max frames
////////////////////////////////////////////////////////////////////
stringstream ss;

int initialise_allegro(); // function prototype

game::game(ALLEGRO_DISPLAY* newdisplay,int level_no, int difficulty) // default constructor
{
	m_lives = 3;
	display = newdisplay; //changes over our display from menu
	render_frame = false; // defaults
	render_logic = false; //
	m_current_level = level_no; //carries over from menu
	m_difficulty = difficulty; //
}

game::game(ALLEGRO_DISPLAY* newdisplay,int level_no, int difficulty, int player_lives) // default constructor
{
	m_has_dave = false;
	m_lives = player_lives;
	display = newdisplay; //changes over our display from menu
	render_frame = false; // defaults
	render_logic = false; //
	m_current_level = level_no; //carries over from menu
	m_difficulty = difficulty; //
	loading = al_load_bitmap("assets/loading.bmp");
	al_draw_bitmap(loading,0.0,0.0,0);
	al_flip_display();
}

game::~game()
{
	delete level;
	level = NULL;

}

bool game::createGameObjects()
{
	int PosX[30]; //max objects of one type is 30
	int PosY[30]; //only 30 positions wil be stored
	ss << m_current_level; // converts to string
	bool init_ok = initialise_allegro(); //did allegro start?
	//Create a level, pass through wall textures and textfile to load.
	level = new arena("assets/wall1.bmp","assets/wall3.bmp","assets/wall4.bmp","assets/wall5.bmp","assets/wall_right_slope.bmp","assets/wall_left_slope.bmp","assets/level" + ss.str() +".txt",al_get_display_width(display),al_get_display_height(display));

	int numofObjects = level->setGameObjects(PosX,PosY,'S'); //Find player start and store it
	// create our player start position, spritesheet and passsthrough event queue/events.
	newhero = new hero(PosX[0],PosY[0],"Chris","assets/player_sprite.bmp",6,level,m_lives,&ev,event_queue,&render_frame,&render_logic);
	ss.str(""); // clear our stringstream

	numofObjects = level->setGameObjects(PosX,PosY,'E'); //find our exits and create the object, pushing into parent class vector.
	levelobjects.push_back(new end_trigger(PosX[0],PosY[0],"end","assets/exit.bmp",0,level));

	//how many hearts did we find?
	numofObjects = level->setGameObjects(PosX,PosY,'H');
	if(m_difficulty == 3) //If difficulty is hard
		numofObjects = 0; // set found objects to 0

	if(m_difficulty == 2) // if medium
	{
		numofObjects *= 0.5; //only create half objects
		int randPos = 0;
		for(int i = 0; i < numofObjects; i++) //randomly create objects
		{
			do
			{
				randPos = rand() % numofObjects;
			}while(PosX[randPos] == -1 && PosY[randPos] == -1);
			PosX[randPos] = -1;
			PosY[randPos] = -1;
			ObjectPickups.push_back(new heart(PosX[randPos],PosY[randPos],"heart","assets/heart.bmp",0,level,newhero));
		}
	}
	else //else normal difficulty
	{
		for (int i = 0; i < numofObjects; i++) //depending on number found
		{
				ObjectPickups.push_back(new heart(PosX[i], PosY[i], "heart", "assets/heart.bmp", 0, level, newhero));

		}																										//put into parent class vector
	}
	numofObjects = level->setGameObjects(PosX,PosY,'G'); //gold
	for(int i = 0; i < numofObjects; i++)
		ObjectPickups.push_back(new goldcoins(PosX[i],PosY[i],"Gold Coin","assets/goldcoins.bmp",0,level,newhero));

	numofObjects = level->setGameObjects(PosX,PosY,'l'); //life
	for(int i = 0; i < numofObjects; i++)
		ObjectPickups.push_back(new lives(PosX[i],PosY[i],"LIVES","assets/life.bmp",0,level,newhero));

	numofObjects = level->setGameObjects(PosX,PosY,'i'); //idols
	for(int i = 0; i < numofObjects; i++)
		ObjectPickups.push_back(new idol(PosX[i],PosY[i],"DAVE","assets/idol.bmp",0,level,newhero));

	numofObjects = level->setGameObjects(PosX,PosY,'K'); //keys
	for(int i = 0; i < numofObjects; i++)
		ObjectPickups.push_back(new keys(PosX[i],PosY[i],"key","assets/key.bmp",0,level,newhero));

	numofObjects = level->setGameObjects(PosX,PosY,'C'); //crushign blocks
	for(int i = 0; i < numofObjects;i++)
		platform_container.push_back(new crushingBlock(PosX[i],PosY[i],"Crushing Block","assets/crusher.bmp",0,level));

	numofObjects = level->setGameObjects(PosX,PosY,'V'); //movable blocks
	for(int i = 0; i < numofObjects;i++)
		platform_container.push_back(new movableBlock(PosX[i],PosY[i],"Movable Block","assets/movableBLock.bmp",0,level));

	numofObjects = level->setGameObjects(PosX,PosY,'D'); //crumbling platform
	for(int i = 0; i < numofObjects;i++)
		platform_container.push_back(new crumblingPlatform(PosX[i],PosY[i],"Crumbling_Platform","assets/platformspritesheet.bmp",0,level));

	numofObjects = level->setGameObjects(PosX,PosY,'d'); //door
	for(int i = 0; i < numofObjects;i++)
		levelobjects.push_back(new door(PosX[i],PosY[i],"Door","assets/door.bmp",0,level));

	numofObjects = level->setGameObjects(PosX,PosY,'M'); //monsters
	for(int i = 0;i < numofObjects;i++)
	{
		ss << i; //concatonating with string with int
		pointermonster.push_back(new monster(PosX[i],PosY[i],"Monster Number " + ss.str(),"assets/monstertrans.bmp",3,level,newhero));
		GameObjects.push_back(pointermonster[i]); //our main creature vector for drawing
		ss.str(""); //overwriting stringstream
	}
	numofObjects = level->setGameObjects(PosX,PosY,'s'); //shooting monsters
	for(int i = 0;i < numofObjects;i++)
	{
		pointermonster.push_back(new shooting_monster(PosX[i],PosY[i],"Shooting Monster","assets/monstertrans.bmp",1,level,newhero));
		GameObjects.push_back(pointermonster[pointermonster.size()-1+i]); //our main creature vector for drawing
	}
	////////////////////////////////////////////Currently one lever-opened door per level //////////////////////////
	numofObjects = level->setGameObjects(PosX,PosY,'O'); //do we have a  LEVER opened door? (one per level)
	if(numofObjects > 0)
	{
		door* lever_door = new door(PosX[0],PosY[0],"Door_open","assets/door.bmp",0,level); //create the appropriate door
		levelobjects.push_back(lever_door);

		numofObjects = level->setGameObjects(PosX,PosY,'L'); // create appropriate lever for the door
		for(int i = 0; i < numofObjects;i++)
			levelobjects.push_back(new lever(PosX[i],PosY[i],"lever","assets/lever.bmp",0,level,lever_door));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GameObjects.push_back(newhero); // push our hero into creature vector for drawing
	newparticles = new particleSystem(10000,level); //create a particle system for our main game loop

	/////////////////////////Moving Platforms/////////////////////////////////////////////////////////////////////////////////////////
	numofObjects = level->setGameObjects(PosX,PosY,'u'); //find platforms going UP
	for(int i = 0; i < numofObjects; i++)
		platform_container.push_back(new movingPlatform(PosX[i],PosY[i],"Vertical_Block","assets/platform.bmp",0,level,newhero,0));

	numofObjects = level->setGameObjects(PosX,PosY,'h'); //find platforms going horizontal
	for(int i = 0; i < numofObjects; i++)
		platform_container.push_back(new movingPlatform(PosX[i],PosY[i],"horizontal_Block","assets/platform.bmp",0,level,newhero,3));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	loading = al_load_bitmap("assets/loaded.bmp");
	al_draw_bitmap(loading,0.0,0.0,0);
	al_flip_display();
	//al_rest(0.0f); //loaded
		return true; // all created ? return true.
}

int game::playGame()
{

hud* create_player_hud = new hud(newhero,level,timer); //create our hud, pass in hero, arena and timer.

 for(;;) //infinite gameloop
{
	al_start_timer(timer); //start timer

	while(newhero->getlives() > 0)
	{
		if(newhero->getHealth() > 0) //Player controls are unlocked
			newhero->control->IsKeyPressed(&ev,event_queue, false); //all players input object and deal with key presses
		else
		{ // lock players controls for small countdown on kill.
			newhero->control->IsKeyPressed(&ev,event_queue, true);
			if(create_player_hud->countdownOnKill() == true) //countdown finished?
				{
					deAlloc();
					return 2;
				}
		}

		if(render_logic) //if true, logic
		{
			render_logic = false; //set false for next check
			bool mCollide = false;
/////////////////////////////////// Particle Logic Updates /////////////////////////////////////////////////
			newparticles->update();
			////////////////////////////////////// update misc objects
		for(int i = 0; i < levelobjects.size();i++) //loop our entire vector
		{
            if(levelobjects[i]->isOnScreen())
            {
                levelobjects[i]->update(newhero); //call each child objects update method using polymorphism
                if(levelobjects[i]->getName() == "lever")
                    levelobjects[i]->collidesWith(newhero);
                else
                {
                 int pushing_block = levelobjects[i]->collidesWithMovingBlock(newhero,(int)newhero->control->m_x_velocity,(int)newhero->control->m_y_velocity);
                 if(pushing_block != NULL) // have we collided at all?
                 {
                     if(levelobjects[i]->getName() == "end") //is the object the end trigger?
                     {
                         hiscoreInstance.writeFile(m_current_level, newhero->getlives()); //save game
                         hiscoreInstance.saveHiscores( newhero->getGold(),m_current_level);//save level hiscore
                         m_has_dave = newhero->m_has_dave;
                         delete create_player_hud; //deallocate hud
                         create_player_hud = NULL;
                         deAlloc(); //deallocate parent vectors and others.
                         return 1; //return 1 to move onto next level
                     }
                     newhero->control->keys_pressed[5] = false; //stops player from moving
                         if(pushing_block == 1)
                         {
                             if(newhero->m_has_key == true)
                             {
                                 levelobjects[i]->setActive(); //door is active, start moving it
                                 newhero->m_has_key = false; // set players key to false
                                //	levelobjects.erase(levelobjects.begin() + i);
                            //		break;
                             }
                             newhero->control->keys_pressed[3] = false; // stops movement
                             newhero->setX((levelobjects[i]->getX() - newhero->getWidth()) - 1); //moves player infront of door
                         }
                         else if(pushing_block == 2)
                         {
                             newhero->control->keys_pressed[4] = false;
                             newhero->setX((levelobjects[i]->getX() + newhero->getWidth() + 10));
                         }
    //				 newhero->control->setVelocityX(0.0f);
                     newhero->control->setVelocityY(0.0f); //stop players y velocity (if standing on door)
                     if(pushing_block < 0)
                     {
                         if(newhero->control->keys_pressed[6] == true)
                         {
                             newhero->control->platformLift(4);
                         }
                         else
                         {
                             newhero->setY(levelobjects[i]->getY() - (newhero->getHeight()- 10));
                             newhero->control->setVelocityY(0.0f);
                        //
                         }
                     }
                 }
                }
            }
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Pushable Block Logic ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
			bool collide_moving_block = false; // here we have a simple bool to see what object we've
			bool collide_pushing_block = false; // collided with
			bool collide_crumbling_block = false; //
		for(int i = 0; i < platform_container.size();i++)
		{
		if(platform_container[i]->isOnScreen())
		{
				if(platform_container[i]->getName() == "Crushing Block") //checking for big crushign blocks
				{
					if(platform_container[i]->collidesWithBot(newhero) == true) //does the bottom hit the player?
						{
							newhero->killPlayer(); //instantly kill the player
						}
				}

			if(platform_container[i]->getName() == "Crumbling_Platform") //is the platform a crumbling platform/
			{
				if(platform_container[i]->collidesWith(newhero) == true) //is it currently collided?
				{
					platform_container[i]->setActive(); //object is now active
					level->setGravity(0.0f); //stop gravity from pushing player down
					newhero->setY(platform_container[i]->getY() - newhero->getHeight()); //put player on top of object
				}
			}
			if(platform_container[i]->getName() == "Movable Block") //can player move this block?
			{
			 int pushing_block = platform_container[i]->collidesWithMovingBlock(newhero,(int)newhero->control->m_x_velocity + 1,(int)newhero->control->m_y_velocity);
			 platform_container[i]->push(pushing_block); //pushing block gets the direction of the push, we now
			 if(pushing_block > 0)
			 {
				 newhero->control->keys_pressed[5] = false; // stop player sprinting
					 if(pushing_block == 1) // right
					 {
						 newhero->control->keys_pressed[3] = false; //stop player moving right
						 newhero->setX((platform_container[i]->getX() - newhero->getWidth()) - 1);
					 }
					 else if(pushing_block == 2) // left
					 {
						 newhero->control->keys_pressed[4] = false;//stop player moving left
						 newhero->setX((platform_container[i]->getX() + newhero->getWidth())- 7);
					 }
				 newhero->control->setVelocityX(0.0f); // stop all velocity
				 newhero->control->setVelocityY(0.0f); //
			 }

			 if(pushing_block < 0) //else if player is approach from top
			 {
				 if(newhero->control->keys_pressed[6] == true) //allows player to jump from object
				 {
					 newhero->control->platformLift(4);
				 }
				 else //is player just standing ontop?
				 {
					 newhero->setY(platform_container[i]->getY() - (platform_container[i]->getHeight()+ 10)); //set them on top of object
					 newhero->control->setVelocityY(0.0f); //stop player from falling through
				//
				 }
			 }

			}
			 collide_moving_block = platform_container[i]->collidesWithMovingPlatform(newhero,(int)newhero->control->m_y_velocity);
			 collide_crumbling_block = platform_container[i]->collidesWithMovingBlock(newhero,(int)newhero->control->m_x_velocity,(int)newhero->control->m_y_velocity);
			 collide_pushing_block = platform_container[i]->collidesWithMovingBlock(newhero,(int)newhero->control->m_x_velocity,(int)newhero->control->m_y_velocity);
			 if(collide_pushing_block == true) // are we colliding with a block?
			 {
				level->setGravity(0.0f); //set all gravity to 0 for player.
		//		newhero->control->setVelocityX(0.0f);
			 }
			if(collide_moving_block == true || collide_pushing_block == true || collide_crumbling_block == true)
				{
					newhero->control->setVelocityY(0.0f);
					bool update_remover = platform_container[i]->update(newhero);
					if(update_remover == true)
					{
						delete platform_container[i];
						platform_container[i] = NULL;
						platform_container.erase(platform_container.begin() + i);
						break;
					}
					level->setGravity(0.0f);
				if(collide_moving_block == true)
				{
					switch(platform_container[i]->getDirection())
					{
					case 0: //up
						{
							newhero->setY(platform_container[i]->getY() - newhero->getHeight());
							if(newhero->control->platformLift(0) == true) //stops player from being pushed into wall
							{
								platform_container[i]->canMove(0);
							}
						}
					break;
					case 1: //down
						{
							newhero->setY(platform_container[i]->getY() - newhero->getHeight()); //keeps player on top of object
							newhero->control->platformLift(1);
							newhero->control->platformLift(1); //calls player to move with platform
						}
					break;
					case 2: //left
						{
							newhero->setY(platform_container[i]->getY() - newhero->getHeight());
							newhero->control->platformLift(2);
						}
					break;
					case 3: //right
						{
							newhero->setY(platform_container[i]->getY() - newhero->getHeight());
							newhero->control->platformLift(3);
						}
					}
				}
			}
			else
			{
				bool player_is_colliding = false; //check for collisions
				for(int c = 0; c < 2;c++)
				{
					if(collide_moving_block == true) //we set player to colliding
					{
						player_is_colliding = true;
						break;
					}
				}
					if(player_is_colliding == true) //if they are colliding
					{
						bool update_remover = platform_container[i]->update(newhero); //update all platforms
						if(update_remover == true) //platform called for deletion?
						{
							delete platform_container[i]; //deallocate and delete from vector
							platform_container[i] = NULL;
							platform_container.erase(platform_container.begin() + i);
							break;
						}
					}
					else
					{
						bool update_remover = platform_container[i]->update(newhero);
						if(update_remover == true)
						{
							delete platform_container[i];
							platform_container[i] = NULL;
							platform_container.erase(platform_container.begin() + i);
							break;
						}
						level->setGravity(0.1f);
					}
			}
		}
		}
////////////////////////////Deals with all collectable logic /////////////////////////////////////////////////////
if(ObjectPickups.size() > 0)
{
	for(int i = 0; i < ObjectPickups.size(); i++)
	{
		ObjectPickups[i]->update();
		if(ObjectPickups[i]->collidesWith(newhero) == true) //has the player collided with object
			{
				ObjectPickups[i]->applyEffect(newhero); //apply desired effect on hero, for whatever object current in vector
				delete ObjectPickups[i]; //deallocate the memory of the object in vector
				ObjectPickups[i] = NULL; //
				ObjectPickups.erase(ObjectPickups.begin() + i); //resize the vector to normal
					if(ObjectPickups.size() < 1) //keep one object in the game
					{
						ObjectPickups.push_back(new goldcoins(-50,-50,"Temp","assets/gold.bmp",0,level,newhero));
					}
			}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			int tempx = 0; //used to keep track of monsters current positions
			int tempy = 0;
			bool reseted = false;
			for(int i = 0;i < pointermonster.size();i++)
			{
							tempx = (pointermonster[i]->getX()); //first we store their current positions
							tempy = (pointermonster[i]->getY());
							pointermonster[i]->chase(newhero); //call their chase (includes movement)
				for(int q = 0; q < pointermonster.size();q++) //now loop through all monsters
				{
					mCollide = pointermonster[i]->collidesWith(pointermonster[q]); //are they colliding with each other?
						if(mCollide == true)
						{
							if(pointermonster[i]->collidesWithBot(pointermonster[q]) == false) //are they on top of each other?
							{
								if(pointermonster[i]->down(pointermonster[i]->getX(),pointermonster[i]->getY()) == true) //are they colliding
								{
									tempy = (pointermonster[i]->getY()); //reset (stops them from clipping on top of each other)
								}
							}
							pointermonster[i]->resetPos(&tempx,&tempy, pointermonster[q]); //reset back to previous position
							reseted = true;
							break;
						}
						else
						{
							mCollide = false; //reset variable
						}
				}
				if(reseted == true)
					break;
			}
		for(int i = 0;i < pointermonster.size();i++) //checks for player collided
			{
				loop = pointermonster[i]->collidesWith(newhero); //collided?
					if(loop == true)
					{
								newhero->setHealth(newhero->getHealth() - 1); //deplete player health by 1
						break;
					}
			}
	}
		if(render_frame) //are we currently rendering?
		{
		render_frame = false; //set back to default
			level->render(); //render the level first
			for(int i = 0; i < levelobjects.size();i++) //draws contents of all the vectors for our objects in game
				levelobjects[i]->draw(levelobjects[i]->getX(),levelobjects[i]->getY(),false,false); //now draw each
			for(int i = 0; i < GameObjects.size(); i ++)
			{
				if(GameObjects[i]->getName(GameObjects[i]) == true) //returns true if hero
				{
					GameObjects[i]->draw(GameObjects[i]->getX(),GameObjects[i]->getY(),false,true); //call draw with hero flag true
				}
				else
				{
					GameObjects[i]->draw(GameObjects[i]->getX(),GameObjects[i]->getY(),false,false);
				}
			}
			for(int i = 0; i < ObjectPickups.size();i ++) //draws contents of vector
			{
				ObjectPickups[i]->draw(ObjectPickups[i]->getX(),ObjectPickups[i]->getY(),false,false);
			}
			for(int i = 0; i < platform_container.size();i++)
				platform_container[i]->draw(platform_container[i]->getX(),platform_container[i]->getY(),false,false );
			newparticles->draw(); //update particles
			create_player_hud->update(); //draws hud last (so appears before all)
			al_flip_display();
		}
		//
		//chasing

}
	newhero->deductlives(); //player is dead? deduct lives
	if(newhero->getlives() < 0)
		{
			al_clear_to_color(al_map_rgb(0,0,0));
	    	al_draw_textf(gameover , al_map_rgb(255, 255, 255), 10, 200, 0, "Game over!"); //player has no lives? show this!
			al_flip_display(); //update so player can see
			al_rest(2); //wait so player reads game over
			al_clear_to_color(al_map_rgb(0,0,0));
			delete create_player_hud; //deallocate hud
			create_player_hud = NULL;
			deAlloc(); //deallocate
			return 0;
		}
	} //infinite loop ends here
} //end of method

void game::deAlloc() //deallocation for all objects in game
{
	delete newparticles;
	newparticles = NULL;
	for(int i = 0; i < ObjectPickups.size();i++) //loop through the vector
	{
		delete ObjectPickups[i]; //deallocate the vector
		ObjectPickups[i] = NULL; //rinse, repeat
	}

	for(int i = 0; i < pointermonster.size();i++)
	{
		delete pointermonster[i];
		pointermonster[i] = NULL;
	}

	for(int i = 0; i < platform_container.size();i++)
	{
		delete platform_container[i];
		platform_container[i] = NULL;
	}

	for(int i = 0; i < levelobjects.size();i++)
	{
		delete levelobjects[i];
		levelobjects[i] = NULL;
	}

	delete level; //deallocating level
	level = NULL;

	delete newhero;
	newhero = NULL;
	al_destroy_bitmap(loading);
	al_destroy_event_queue(event_queue); //destroying event queue and timer.
	al_destroy_timer(timer);
}

int initialise_allegro()
{
	ALLEGRO_EVENT ev;
	if(!al_init())
	{
		return -1;
	}
	if(!al_install_keyboard())
	{
		cout << "Keyboard initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		return -1;
	}
	if(!al_install_mouse())
	{
		cout << "Keyboard initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		return -1;
	}
	// initialise keypress array
	//display = al_create_display(WIDTH, HEIGHT); // create a display window of size 640 by 480

	if(!display)
	{
		cout << "Display initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		return -1;
	}
 //	al_set_window_position(display, 250, 50); // position display window on screen
	// need to create an event queue so allegro can respond to events, such as keypresses
	event_queue = al_create_event_queue();
	event_queue_two = al_create_event_queue();
	if(!event_queue)
	{
		cout << "Event queue creation failed" << endl;
		cout << "Press any key to exit" << endl;
		al_destroy_display(display);
		return -1;
	}
	al_init_image_addon();

    	al_init_font_addon(); // initialize the font addon
    	al_init_ttf_addon();  // initialize the True Type Font addon
	timer = al_create_timer(1.0 / 60);
	al_init_primitives_addon();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	gameover = al_load_ttf_font("assets/DejaVuSans.ttf", 20 ,0);
	bmp = al_load_bitmap("assets/a4_font.bmp");
	   int ranges[] = {
                  0x0020, 0x007F,  /* ASCII */
                  0x00A1, 0x00FF,  /* Latin 1 */
                  0x0100, 0x017F,  /* Extended-A */
                  0x20AC, 0x20AC}; /* Euro */
	 font1 = al_grab_font_from_bitmap(bmp,1,ranges);
	if (!gameover)
	{

		al_draw_textf(font1 , al_map_rgb(255, 255, 255), 10, 200, 0, "Font creation failed");
		al_flip_display();
			 do
			 al_wait_for_event(event_queue, &ev);
			 while (ev.type != ALLEGRO_EVENT_KEY_DOWN);
		al_destroy_display(display);
		exit(1);
	 }
	// register  display with the event queue, so display change events can be processed
	return 0;
}
