#include "fileio.h"
#include <fstream> 
#include <sstream>

stringstream ss_convert; // used for covnerting ints to strings, vica versa

fileio::fileio()
{
}

fileio::~fileio()
{
}

void fileio::writeFile(int m_current_level, int m_lives) //used for autosave after each level
{
	if(m_lives  <= 0 || m_current_level == 0) //are you dead? (do not save!) or are you bonus level?
		return;

	m_current_level += 1; //saves for the NEXT level

	int digit = m_current_level * 10 + m_lives; //converts two digits into one!

	ofstream binary_outfile("rawdata.bin", ios::binary); // open for binary output

    if(binary_outfile.is_open()) //have we opened our file?
    {
        binary_outfile.write((char*)&digit, sizeof(int)); //save stored digit
        binary_outfile.close(); // always close streams once finished
    }
    else
    {
        cout << "Error opening file for output" << endl; //failed? do not save
    }

	ofstream append_level("rawdata.bin", ios::binary | ios::app); // in case of additional information to add
    if(append_level.is_open())									 // we can append existing file in future
    {
        append_level.close(); // always close streams once finished 
    }
    else
    {
        cout << "Error opening file for output" << endl;
    }
}
unsigned int fileio::readFile(int* m_current_level, unsigned int* lives) //used for load game in menu
{
    int previous_fileio; // will use for reading!
	char* imagedata = NULL; // will use for binary reading!

    ifstream binary_infile("rawdata.bin", ios::binary);  //open our binary file

    if(binary_infile.is_open()) // have we opened it?
    {
        binary_infile.read((char*)&previous_fileio, sizeof(int)); //read and store as int

		binary_infile.seekg (0, binary_infile.end); // how many characters until the end of file?
		int length = binary_infile.tellg(); // store characters found in file
		binary_infile.seekg (0, binary_infile.beg);// find beginning of 
    }
    else
    {
        cout << "Error opening file for input" << endl; // if no file is found, this is outputted.
		return 0;
    }
	ss_convert << previous_fileio; //convert int to string
	string temp = ss_convert.str(); //store into a temp string
	char m_level = temp.front(); //first character is level number. Could use substring trickery if we have more than 9 (store are 01, etc)
	string m_lives = temp.substr(1,temp.length()); //last will be lives
	

	*m_current_level = atoi(&m_level); //convert our character to an integer, store in current level via pointer.
	*lives = atoi(m_lives.c_str());		// same as above, convert to c string to use with atoi

	return 1;
}

int fileio::getNumLevels()
{
	string number = "1"; //always will have 1 level
	int levels_found = 1; //same, always will find one
	ss_convert.str(""); //clear previous stringstream usage

	for(;;) //constantly loop logic
	{
		ifstream binary_infile("assets/level" + number + ".txt"); //load level file with value in number

		if(binary_infile.is_open()) // does it exist?
		{
			binary_infile.close(); //close our stream
			levels_found++; //we found a level, so increment

			ss_convert << levels_found; //convert to string to repeat above process
			number = ss_convert.str();
			ss_convert.str("");
		}
		else
		{
			break; //no more levels are found?break out of the loop
		}
	}
	return levels_found - 1; //return the amount, minus one.
}

int fileio::retrieveHiscores(int level_no) //used in hiscore class
{
	string level_number = "";		//here we retrieve our highscores
	string hiscore = "";			//we create temp strings to cycle through
	int hiscore_retrieved = 0;		//level numbers and retrieve highscores

	ss_convert << level_no;			//level_no parameter is converted to a string
	level_number = ss_convert.str();	//to use in ifstream
	ss_convert.str("");

	for(;;)
	{	//now we can cycle through, seeing if we have any highscores
		ifstream hiscore_file("assets/levelhiscore" + level_number + ".txt"); 

		if(hiscore_file.is_open())
		{
			getline(hiscore_file,hiscore);
			hiscore_retrieved = atoi(hiscore.c_str()); //we now convert that retrieve highscore
			hiscore_file.close();
			return hiscore_retrieved; //returning it
		}
		else
		{
			return 0; //else we didn't get a highscore for the level, so it's 0
		}
	}
	return hiscore_retrieved;

}

void fileio::saveHiscores(int hiscore, int current_level)
{
	if(hiscore < retrieveHiscores(current_level)) //here we check if we've scored higher before
		return;	//if this score is lower than old score, return

	string hiscore_string = "";			//like retrieving, we convert
	string level_name = "";				//our highscore and level names
										//into strings to write to a file
	ss_convert << hiscore;
	hiscore_string = ss_convert.str();
	ss_convert.str("");

	ss_convert << current_level;
	level_name = ss_convert.str();
	ss_convert.str("");
													//level name depends on current_level parameter
	ofstream savedLevelHiscore("assets/levelhiscore" + level_name + ".txt"); 

    if(savedLevelHiscore.is_open()) //is the file open?
    {
		savedLevelHiscore << hiscore_string; //write the file
		savedLevelHiscore.close(); //close it
		return;
    }
	else
	{
		return;
	}

}