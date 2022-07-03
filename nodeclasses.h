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

	void reset_iter();

	void increment_iter();

	char get_char();

	void add_loc(int line, int offset);

	void shuffle_loc();

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

	int get_line_value();

	void add_loc(char c, int offset);

};

/* simple class to circumvent passing a pointer to an array around the program, as suggested by Bjarne Stoustrup */
class Arguments {

public:
	std::vector<std::string> _args;

	Arguments(char* argv[], int num);

	std::string return_arg(int index);
};

//---------------------------------------------------------------
// EXPERIMENTAL CLASSES
//---------------------------------------------------------------

/* A class that was used to experiment with an alternative method of decoding, work::execute_decoding2 */
class EncodedObj {

public:
	int _line;
	int _offset;
	int _idx;
	char _ch;

	EncodedObj(int line, int offset, int index, char ch) {
		_line = line;
		_offset = offset;
		_idx = index;
		_ch = ch;
	}	
};

/* A class that was used to experiment with an alternative method of decoding, work::execute_decoding2 */
class LineOffsetChar {
public:
	int _line;
	int _offset;
	char _ch;

	LineOffsetChar(int line, int offset, char ch) {
		_line = line;
		_offset = offset;
		_ch = ch;
	}
};