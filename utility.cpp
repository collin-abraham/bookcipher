/* utility.cpp
* 2022-06-30
* Collin Abraham
*
* Func definitions for declarations found in utility.h
*/


#include "utility.h"

void utility::usagePrompt() {
	std::cout << "Encoding: cipher.exe -e bookfile.txt source.txt output.txt\n"
		<< "Encoding (generated bookfile): cipher.exe -g bookfile.txt source.txt output.txt\n"
		<< "Decoding: cipher.exe -d bookfile.txt encoded.txt decoded.txt\n"
		<< "Generate Bookfile: cipher.exe -g bookfile.txt";
}

void utility::generatedBookfileWarning() {
	std::cout << "!!! WARNING !!! save a copy of your new bookfile!\n";
}

void utility::invalidCharacter(std::string errorStr, int line) {
	std::cerr << "ERROR: Invalid character found: " << errorStr << " on line: " << line;
}

void utility::showValidChars() {
	std::cout << "\n\nValid Characters to use are: \n";

	auto temp = utility::returnAcceptedCharsVector();
	std::vector<char>::iterator it = temp.begin();


	int offset = 10;
	while (it != temp.end()) {
		while (offset != 0 && it != temp.end()) {
			std::cout << *it << " ";
			--offset;
			++it;
		}
		std::cout << "\n";
		offset = 10;
	}

	std::cout << "Terminating...\n";

}

const std::vector<char> utility::returnAcceptedCharsVector() {

	std::vector<char> chars{
		'0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'!','@','#','$','%','^','&','*','(',')','-','_','+','=','{','}','|','\\',';',':','\'','"',',','<','.','>','/','?',
		'\n',' '
	};

	return chars;
}

/* loops through a string of character that is read in.. compares it to the vector of acceptable
characters.. if it is unable to find the character, it kills the program */
std::string utility::acceptedChars(const std::string& strLine, std::vector<char>& checkVec) {
	
	std::string returnString = "";
	std::vector<char>::iterator it;

	for (const char ch : strLine) {
		it = find_if(begin(checkVec), end(checkVec), [ch](const char c) { return ch == c; });

		if (it == checkVec.end()) {
			returnString += "\"";
			returnString += ch;
			returnString += "\"";

			return returnString;
		}
	}

	// an empty string should be returned unless there was an issue
	return returnString;
}