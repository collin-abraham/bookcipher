/* nodeclasses.cpp
* 2022-06-30
* Collin Abraham
*
* Class definitions used throughout the program (from nodeclasses.h)
*/


#include "nodeclasses.h"

// -------------------------------------------------------------------------------------------
// CharNode 
// -------------------------------------------------------------------------------------------

/* 1 arg constructor to build a CharNode */
CharNode::CharNode (char c) { _top = c; }

/* resets the internal iterator to .begin() of the vector
* this was necessary to prevent the iterator pointing to the wrong spot in the container
* as the size increased this issue became more common
*/
void CharNode::reset_iter() { 
		_loc_iter = _locations.begin();
}

/* prevents the iterator from undefined behavior.. 
* if its not pointing to the end of the vector, increment.. otherwise reset it to .begin()
* to keep it running in a circle over the values 
*/
void CharNode::increment_iter() {
	if (_loc_iter != _locations.end())
		_loc_iter++;
	else
		_loc_iter = _locations.begin();
}

/* returns the main signifier of the struct */
char CharNode::get_char() { return _top; }

/* pushses a new location into the vector of a line and offset to match the char of _top */
void CharNode::add_loc(int line, int offset) { _locations.push_back(std::make_pair(line, offset)); }

/* shuffles the pairs of line/offset in the vector to produce some randomness in the end cipher file */
void CharNode::shuffle_loc() { std::random_shuffle(begin(_locations), end(_locations)); }

// -------------------------------------------------------------------------------------------
// LineNode 
// -------------------------------------------------------------------------------------------

/* 1 arg constructor to build a LineNode */
LineNode::LineNode(int c) { _top = c; }

/* returns the main signifier of the struct */
int LineNode::get_line_value() { return _top; }

/* pushses a new location into the vector of a char and offset to match the line of _top */
void LineNode::add_loc(char c, int offset) { _locations.push_back(std::make_pair(c, offset)); }

// -------------------------------------------------------------------------------------------
// Arguments 
// -------------------------------------------------------------------------------------------

/* 2-arg constructor, pushes the standard console char* argv [] into a vector num times 
* Recently listened to a talk from cppcon where Bjarne suggested passing raw pointers around the 
* program, and especially raw pointers to arrays is a recipe for disaster.. and is honestly pointless
* now that unique_ptr<T>/shared_ptr<T> have guaranteed RAII 
*/
Arguments::Arguments(char* argv[], int num) {
	for (int i = 0; i <= num; ++i)
		_args.push_back(argv[i]);
}


/* accesses the internal vector and calls .at() for index access similar to how you'd use argv[index] */
std::string Arguments::return_arg(int index) {
	if ((size_t)index <= _args.size())
		return _args.at(index);
	else
		return "null";
}
