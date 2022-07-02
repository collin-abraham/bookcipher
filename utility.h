/* utility.h
* Collin Abraham
* 2022-06-30
* 
* Top level utility namespace for combining headers and error messages 
* This header file also acts as a linker with the main std libraries used in the program 
*/

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory> //std::unique_ptr<T>
#include <random>

// I could have used CPP 17 and avoided this, but it broke std::random_shuffle for some reason...
// so I kept it on CPP 14 and suppressed the warnings, used the experimental library
#include <experimental/filesystem>
using std::experimental::filesystem::remove;
using std::experimental::filesystem::filesystem_error;

namespace utility {

	std::string checkEncodedFile(const std::string& argv);

	void usagePrompt();

	void generatedBookfileWarning();

	void invalidCharacter(std::string errorStr, const std::string& fileName);

	void invalidCharacter(std::string errorStr, int line, const std::string& fileName);

	void deletePartialFile(const std::string& fileName);

	void showValidChars();

	void showValidDecodedExample();

	const std::vector<char> returnAcceptedCharsVector();

	std::string acceptedChars(const std::string& strLine, std::vector<char>& checkVec);
}