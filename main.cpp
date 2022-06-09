#include <iostream>
#include "menu.h"

int main()
{
	srand(time(NULL)); //seeding for future random events!
	menu newgame; //creating our new menu, our centre point of the game
	while(newgame.update() == false); //update
}

// I have left little code within main as it serves little purpose to the overall design of my game.
// All logic is handled within the menu -- popping new game instances, changing options, everything.

//So I have decided to replace it with an update method, a way of constantly displaying what is on the 
//screen to the player. This would logically be within a menu class, so including code from other 
//objects in main would be detriment to overall design and readability. This allows for higher levels
// of encapulation 