#include "nodeclasses.h"

/* CharNode func definitions */

CharNode::CharNode (char c) { _top = c; }

void CharNode::resetIter() { 
	if(_loc_iter != _locations.begin())
		_loc_iter = _locations.begin();
}

void CharNode::incrementIter() {
	if (_loc_iter != _locations.end())
		_loc_iter++;
	else
		_loc_iter = _locations.begin();
}

char CharNode::getChar() { return _top; }

void CharNode::addLoc(int line, int offset) { _locations.push_back(std::make_pair(line, offset)); }

void CharNode::shuffleLoc() { std::random_shuffle(begin(_locations), end(_locations)); }

// -------------------------------------------------------------------------------------------


/* LineNode func definitions */

LineNode::LineNode(int c) { _top = c; }

int LineNode::getLineValue() { return _top; }

void LineNode::addLoc(char c, int offset) { _locations.push_back(std::make_pair(c, offset)); }
