#pragma once
#include <iostream>

using namespace std;

class fileio //here is our file for creative saves, hiscores and reading from them
{
public:
	fileio();
	~fileio();

	void writeFile(int previous_fileio,int m_lives); //writing our saves (autosaves mind you)
	unsigned int readFile(int* m_current_level, unsigned int* m_lives); //reading from our saves
	int getNumLevels(); //retrieving how many levels we currently have
	int retrieveHiscores(int); //retrieving highscores for said levels
	void saveHiscores(int, int); //saving our highscores when we finish a level
private:
	unsigned int current_level; //used to check what current level is when we want to save/read 
};