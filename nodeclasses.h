/* nodeclasses.h
* 2022-06-30
* Collin Abraham
* 
* Class declarations used throughout the program 
*/

#include "utility.h"

/* A class which uses a single unique character in a vector to generate a vector 
* of lines and offsets to explain where that char is found in the bookfile
*/
class CharNode {

public:
	char _top;
	std::vector <std::pair<int, int>> _locations;
	std::vector<std::pair<int, int>>::iterator _loc_iter;

	CharNode(char c);

	void resetIter();

	void incrementIter();

	char getChar();

	void addLoc(int line, int offset);

	void shuffleLoc ();

};

/* A class which uses a line number to generate a vector
* of chars and offsets to explain where on that line you may find the corresponding pairs 
*/
class LineNode {

public:
	int _top;
	std::vector<std::pair<char, int>> _locations;
	std::vector<std::pair<char, int>>::iterator _loc_iter;

	LineNode(int c);

	int getLineValue();

	void addLoc(char c, int offset);

};

/* simple class to prevent passing pointer to arrays around the program, as suggested by Bjarne Stoustrup */
class Arguments {

public:
	std::vector<std::string> _args;

	Arguments(char* argv[], int num);

	std::string returnArg(int index);
};

class encodedObj {

public:
	int _line;
	int _offset;
	int _idx;
	char _ch;

	encodedObj(int line, int offset, int index, char ch) {
		_line = line;
		_offset = offset;
		_idx = index;
		_ch = ch;
	}	
};