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

void utility::invalidCharacter(std::string errorStr, int line, const std::string& fileName) {
	std::cerr << "ERROR: Invalid character found in " << fileName << ": " << errorStr << " on line: " << line;
}

void utility::invalidCharacter(std::string errorStr, const std::string& fileName) {
	std::cerr << "ERROR: Invalid character found in " << fileName << ": " << errorStr;
}



void utility::deletePartialFile(const std::string& fileName) {
	try {
		remove(fileName);
	}
	catch (const filesystem_error& e) {
		std::cout << "filesystem error: " << e.what() << '\n';
	}

	std::cout << "--- Removed partial file: " << fileName << "\n";
}

void utility::showValidChars() {
	std::cout << "\n\nValid Characters to use are: \n";

	auto temp = utility::returnAcceptedCharsVector();
	std::vector<char>::iterator it = temp.begin();


	int offset = 10; 	// 10 offsets allowed per line
	while (it != temp.end()) {	// loop until the iterator doesn't reach the end of the file 
		while (offset != 0 && it != temp.end()) {	// loop one single line
			std::cout << *it << " ";	// display the contents at the iterator
			--offset;	// move the offset
			++it;	// increment the iterator
		}
		std::cout << "\n"; // once the line is done, push a newline character 
		offset = 10;	// reset the offset counter for a new line 
	}

	std::cout << "Terminating...\n";

}

void utility::showValidDecodedExample() {
	std::cout << "\n\nA valid codified file can only contain whole numbers and spaces resembling: \n"
		<< "4325 6\n12432 45\n671 51\n1042 53\netc...\n\n";

	std::cout << "Terminating...\n";
}

const std::vector<char> utility::returnAcceptedCharsVector() {

	std::vector<char> chars{
		'0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'!','@','#','$','%','^','&','*','(',')','-','_','+','=','[',']','{','}','|','\\',';',':','\'','"',',','<','.','>','/','?',
		'\n',' '
	};

	return chars;
}

/* loops through a string of character that is read in.. compares it to the vector of acceptable
* characters.. if it is unable to find the character:
* it returns the string with the unaccepted char and in turn it kills the program */
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

std::string utility::checkEncodedFile(const std::string& argv) {
	
	std::ifstream inFile(argv);

	std::string temp = "";
	std::string returnString = "";
	int lineNum = 1;
	
	while (std::getline(inFile, temp)) {
		for (const char& x : temp) {
			if (!isdigit(x) && x != ' ') {		// the encoded file should ONLY contain digits and spaces 
				returnString += "\"";
				returnString += x;
				returnString += "\" on line: ";
				returnString += std::to_string(lineNum);
				return returnString;
			}
		}
		lineNum++;
	}

	inFile.close();
	return returnString;
}