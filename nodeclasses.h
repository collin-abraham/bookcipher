#include "utility.h"

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

class LineNode {
public:
	int _top;
	std::vector<std::pair<char, int>> _locations;
	std::vector<std::pair<char, int>>::iterator _loc_iter;

	LineNode(int c);

	int getLineValue();

	void addLoc(char c, int offset);

};