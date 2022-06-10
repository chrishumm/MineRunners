#include "menu.h"
#include "game.h"

ALLEGRO_FONT* outputNames = NULL;	//  set allegro defaults to null
ALLEGRO_DISPLAY* firstDisplay = NULL;// for safety.
ALLEGRO_EVENT_QUEUE* menu_queue = NULL;//
ALLEGRO_EVENT current_event;			//

const int WIDTH = 900; // our screen resoultion for display
const int HEIGHT = 700; //
int menu::m_idol_count = 0; //how many idols player has collected

menu::menu()	//default constructor
{
	m_difficulty = 1; //default values
	m_current_level = 1;
	m_max_level = 2;
	m_lives = 3;
	allegro_init(); //creates allegro
	allegro_font_init(); //grabs and creates our font
	menuCreate(); //populate menu
	update();// start display loop
}

void menu::menuCreate()
{
	menuItemName[0] = "Start Game"; //store all our menu item names in an array of strctures
	menuItemName[1] = "Options";
	menuItemName[2] = "Load Game";
	menuItemName[3] = "Exit Game";
	menuItemName[4] = "Hiscores!";
	menuItemName[5] = "Game by Christopher Lee Humm";
	for(int i = 0; i < menuItems; i++) //loop through all menu items
	{
		if(i == 0)	//is it the first item?
		{
			menuItemPosX[i] =	WIDTH*0.5; //set position to centre of screen
			menuItemPosY[i] =	(HEIGHT*0.5) - 20;
		}
		else //if not..
		{
			menuItemPosX[i] = menuItemPosX[i-1]; //set position just below previous menu item
			menuItemPosY[i] = menuItemPosY[i-1] + 20;
		}
		menuItemWidth[i] = menuItemName[i].length() * 20; //generate box depending on length of string.
		//generate our primitive in set position, then draw text upon that box at that set position.
		al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeight,al_map_rgb(0,255,0));
		al_draw_textf(outputNames , al_map_rgb(45, 245, 10), menuItemPosX[i], menuItemPosY[i], 0,menuItemName[i].c_str());

		m_background = al_load_bitmap("assets/title_background.bmp"); //loading our background
		al_draw_bitmap_region(m_background, 0, 0, WIDTH+10, HEIGHT, -10, 20, 0);
	}
}
bool menu::allegro_font_init() //loading out font
{
	m_font_from_file = al_load_bitmap("assets/a4_font.bmp"); //grab our font file we want to use
	   int ranges[] = {				//store memory addresses for first 95 characters
                  0x0020, 0x007F, //ascii
                  0x00A1, 0x00FF,  //others
                  0x0100, 0x017F,
                  0x20AC, 0x20AC};
	 outputNames = al_grab_font_from_bitmap(m_font_from_file,1,ranges); //we only want ascii, so grab and store in our font file

	 return true;
}

bool menu::update() //called from menu, this is indefintely looped.
{
	al_rest(0.005);
	bool got_event = al_get_next_event(menu_queue,&current_event); //do we have any new events?
	if(got_event == false) return false;
	if(current_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		return false;
	else if(current_event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY)
		return false;
    else if(current_event.type == ALLEGRO_EVENT_KEY_DOWN) //if we've pressed a key
	{
		 switch(current_event.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE: //is it escape?
			exit(1);			//exit our program
			break;
		 }
	}
		ALLEGRO_MOUSE_STATE getMousePos; //here we constantly update our mouses current x and y positions
		al_get_mouse_state(&getMousePos); //
   if (current_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) //did we click on our mouse?
   {
	  for(int i = 0; i < menuItems;i++) //loop through all our menu items
	  {
			if (getMousePos.x >= menuItemPosX[i] && getMousePos.x < menuItemPosX[i]+menuItemWidth[i] && //was the click within
				getMousePos.y >= menuItemPosY[i] && getMousePos.y < menuItemPosY[i] + menuHeight) //box our that menu item?
			{
				if(i == 0) //if it was first
				{
					m_current_level = 1;
					startGame(); //start our game
				}
				if(i == 1)
				{
					launchOptions(); //launch options
				}
				if(i == 2)
				{
					fileio* getSavedLevel = new fileio(); //generated new io class,
					if(getSavedLevel->readFile(&m_current_level, &m_lives) != 0) //grab save file
					{
						delete getSavedLevel; //deallocate fileio class
						getSavedLevel= NULL;
						startGame(); //start game (values changed in filio via pointer)
					}
					else
					{
						delete getSavedLevel; //no save file? deallocate
						getSavedLevel = NULL; //do nothing
					}
				}
			    if(i == 3)
					exitGame(); //did we click exit? then exit.
			    if(i == 4)
				{ //similar to fileio, we create hiscore which overwrites current menu
					hiscore* retrieveHiscores = new hiscore(outputNames,firstDisplay,menu_queue);
					delete retrieveHiscores; //when finished navigating hiscores, delete and deallocate.
					retrieveHiscores = NULL;
				}
			}
	  }
   }

		for(int i = 0; i < menuItems;i++) //has no event occured? loop through menu items
		{
			if (getMousePos.x >= menuItemPosX[i] && getMousePos.x < menuItemPosX[i]+menuItemWidth[i] && //get mouse coorindates
				getMousePos.y >= menuItemPosY[i] && getMousePos.y < menuItemPosY[i] + menuHeight)
			{ //if hovering over a menu item, colour it in giving a mouse_over effect.
				al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeight,al_map_rgb(53,56,144));
			}
			else
			{ //if not, draw normal colour
				al_draw_filled_rectangle(menuItemPosX[i],menuItemPosY[i],menuItemPosX[i]+menuItemWidth[i],menuItemPosY[i]+menuHeight,al_map_rgb(255,0,0));
			}
			//output menu names
			al_draw_textf(outputNames , al_map_rgb(45, 245, 10), menuItemPosX[i], menuItemPosY[i], 0, menuItemName[i].c_str());
		}
		al_flip_display(); //update our display
		return false;
}

void menu::startGame() //called from menu
{
	fileio* getNumLevels = new fileio(); //generate dynamic fileio class
	int max_no_levels = getNumLevels->getNumLevels(); //grab how many levels we have in our assets and store.
	if(m_current_level > max_no_levels) //is current level more than max level?
		m_current_level = 1; //reset current back to 1 for safety.

	delete getNumLevels; //deallocate our fileio
	getNumLevels= NULL;
	bool next_level = false; //create a new bool, dealing with moving onto next level
for(;;) //start our main loop for our game and level creation
{
	for(;;)
	{//create a new game instance using values in in menu (can be changed in options, or loading a save!)
		game* newgame = new game(firstDisplay,m_current_level,m_difficulty, m_lives);
		newgame->createGameObjects(); //create our objects within our game.
		int game_code = newgame->playGame(); //start main loop, but capture any return values.

		switch(game_code) //start a switch statement deciding what to do with return value.
		{
			case 1: // player has got past the level, load next
				{
					if(newgame->m_has_dave == true) //did we get secret dave?
						m_idol_count++; //increment our static variable

					delete newgame; //deallocate our current level
					newgame = NULL;
					next_level = true; //set next_level flag to true
				}
			break;
			case 2: // player dead, restart level
				{
					delete newgame; //deallocate
					newgame = NULL;
					m_lives--; //deicrement lives within menu
				}
			break;
			case 0: //player have no lives? end the game and exit out.
				{
					delete newgame; //deallocate
					newgame = NULL;
					max_no_levels = m_current_level; //resetting values
					m_idol_count = -1; //resetting values
					next_level = true;
					break;
				}
		}
		if(next_level == true) //does player want to move onto next level?
		{
			next_level = false; //set variable to false again
			break; //break out of loop
		}
	}
	if(max_no_levels == m_current_level) //did we finish ALL levels?
	{
		al_clear_to_color(al_map_rgb(0,0,0)); //paint screen black
		al_flip_display(); //update display
		if(m_idol_count == max_no_levels) //did we get ALL daves?
		{
			game* newgame = new game(firstDisplay,0,m_difficulty); //create new game instance
			newgame->createGameObjects();						//loading hidden level
			int game_code = newgame->playGame();

			delete newgame; //deallocate
			newgame = NULL;
		}
		al_clear_to_color(al_map_rgb(0,0,0)); //paint screen
		al_flip_display();	//update,

		break; //break out of second for loop,
	}
	else //we still have more levels...
	{
		m_current_level++; //(creating a new game object now, but with new values and next level)
	}
}
	m_idol_count = 0;
	m_lives = 3;
	m_difficulty = 1;
}

void menu::exitGame() // player decided to exit
{
	outputNames = NULL;
	exit(1);
}

void menu::launchOptions() //when player has launched optioons..
{
	al_clear_to_color(al_map_rgb(0,0,0)); //paint screen black for next display
	options* createOptions = new options(&m_difficulty,outputNames,firstDisplay,menu_queue); //create new options and loop within
	delete createOptions; //player has exited options loop/menu, deallocate
	createOptions = NULL;
	if(m_difficulty == 1) //if current difficulty has changed. update lives to reflect
		m_lives = 10;
	else if(m_difficulty == 2)
		m_lives = 3;
	else if(m_difficulty == 3)
		m_lives = 1;
	else if(m_difficulty == 4) //infinite lives (but 999 is close enough!)
		m_lives = 999;
}
bool menu::allegro_init() //used for allegro init, exits our if cannot init
{
		if(!al_init()) //did allegro init?
	{
		cout << "Allegro initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		return -1; //exit..
	}
	//al_install_mouse();
	// tell allegro program needs keyboard input
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
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	firstDisplay = al_create_display(WIDTH, HEIGHT); // create a display window on our const ints

	if(!firstDisplay)
	{
		cout << "Display initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		return -1;
	}
 	al_set_window_position(firstDisplay, 250, 50); // position display window on screen
	// need to create an event queue so allegro can respond to events, such as keypresses
	menu_queue = al_create_event_queue(); //tells our program what event queue to use for allegro functions
	al_init_image_addon(); //allows us to load bitmaps

    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();  // initialize the True Type Font addon
	al_init_primitives_addon();

	al_register_event_source(menu_queue, al_get_display_event_source(firstDisplay)); //tells our event_queues what events to track
	al_register_event_source(menu_queue, al_get_keyboard_event_source());
	al_register_event_source(menu_queue, al_get_mouse_event_source());
	// register  display with the event queue, so display change events can be processed
	return 0;
}
