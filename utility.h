/* utility.h
* Collin Abraham
* 2022-06-30
* 
* Top level utility namespace for combining headers and error messages 
* This header file also acts as a linker with the main std libraries used in the program 
*/

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric> //iota

namespace utility {

	void usagePrompt();

	void generatedBookfileWarning();

	void invalidCharacter(std::string errorStr, int line);

	void showValidChars();

	const std::vector<char> returnAcceptedCharsVector();

	std::string acceptedChars(const std::string& strLine, std::vector<char>& checkVec);
}