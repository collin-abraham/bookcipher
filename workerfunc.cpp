/* workerfunc.cpp
* 2022-06-30
* Collin Abraham
* 
* Func definitions for workerfunc.h 
* These functions build the book files corresponding to encoding/decoding and to the process of outputting the end file 
*/

#include "workerfunc.h"
#include <memory> //std::unique_ptr<T>
#include <random>

/* build a simple vector that contains all of the ASCII table 0-128, used by readInBookFile*/
void work::buildVector(std::vector<CharNode>& v) {
	for (int c = 0; c != 128; ++c)
		v.push_back(CharNode((char)c));
}

/* constructs a randomized book file that can act as a custom made key for future use */
int work::generateBookfile(const std::unique_ptr<Arguments>& argv) {

	std::cout << "--- Writing new book file to: " << argv->returnArg(2) << std::endl;
	std::ofstream newBook(argv->returnArg(2));

	if (!newBook.good()) {
		std::cerr << argv->returnArg(2) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// pick a number of lines between 1000 and 5000 pseudo_randomly
	// pick a number of offsets between 30 and 150 ^
	// characters between 34 and 128
	const int MINLINE = 1000;
	const int MAXLINE = 5000;
	const int MINOFFSET = 30;
	const int MAXOFFSET = 150;

	// these are all the reasonable ascii characters that would be found in a text file to be encoded 
	const std::vector<char> charPossibilites{
	'0','1','2','3','4','5','6','7','8','9',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'!','@','#','$','%','^','&','*','(',')','-','_','+','=','{','}','|','\\',';',':','\'','"',',','<','.','>','/','?',
	'\n'
	};

	std::default_random_engine gen;
	const std::uniform_int_distribution<int> newLineDistance(MINLINE, MAXLINE);
	const int numLines = newLineDistance(gen);

	const std::uniform_int_distribution<int> offsetDisance(MINOFFSET, MAXOFFSET);
	const int numOffset = offsetDisance(gen);

	const std::uniform_int_distribution<int> randomChar(0, (int)charPossibilites.size() - 1);


	//auto pullChar = charPossibilites.at(charIndex);

	// fill the file with the random number of lines/offsets.. each output is a 
	// a random index between 0 - size() is chosen and the char sitting there is pushed into the new file 
	for (int c = 0; c < numLines; ++c)
		for (int x = 0; x < numOffset; ++x) {
			int charIndex = randomChar(gen);
			auto pullChar = charPossibilites.at(charIndex);
			newBook << pullChar;
		}

	
	std::cout << "--- Successfully created new bookfile!"  << std::endl;

	return EXIT_SUCCESS;
}

/* reads in the bookfile and constructs the CharNode vector to add lines/offsets to each Char entry created through work::buildVector */
std::vector<CharNode> work::readInBookFile(const std::unique_ptr<Arguments>& argv, std::vector<CharNode>::iterator& vBookIter) {
	
	// create the book vector, fill it with all the ASCII chars 0-128 and create an iterator to access each character node 
	std::vector<CharNode> vBook;
	work::buildVector(vBook);

	std::cout << "--- Reading in BookFile: " << argv->returnArg(2) << std::endl;

	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv->returnArg(2));
	int line = 1;	//txt files start at line 1, not index 0
	int offset = 0;
	std::string strLine = "";
	std::string strLineTester = "";
	std::vector<char> checkVec = utility::returnAcceptedCharsVector();

	while (std::getline(bookFile, strLine)) {

		// check that the string line has all valid characters 
		// a string which is not empty is formatted and returned with the problem char
		// in this case we prompt an error and exit the program by returning an empty bookfile vector
		strLineTester = utility::acceptedChars(strLine, checkVec);
		if (strLineTester != "") {
			utility::invalidCharacter(strLineTester, line);
			utility::showValidChars();
			vBook.clear();
			return vBook;
		}


		// iterate over the line string, find the char match, add the location to the match, increment offset counter 
		for (const auto& currentChar : strLine) {
			vBookIter = std::find_if(vBook.begin(), vBook.end(), [currentChar](CharNode& x) { return x.getChar() == currentChar; });
			vBookIter->addLoc(line, offset);
			vBookIter->resetIter();	// ensure the internal iterator points to the beginning still 
			++offset;
		}

		// push a newline after the rest of the line is read, reset offset, increment line counter 
		vBookIter = std::find_if(vBook.begin(), vBook.end(), [](CharNode& x) { return x.getChar() == '\n'; });
		vBookIter->addLoc(line, offset);
		vBookIter->resetIter();
		offset = 0;
		++line;
	}

	std::cout << "--- Successfully read in BookFile!" << std::endl;
	bookFile.close();

	return vBook;
}

/* reads in the bookfile and constructs the LineNode vector to add chars/offsets to each line entry 
* each line entry is constructed by a unique_ptr here, locations are added as each line is read
* That line is then pushed into the vector of lines 
*/
std::vector<LineNode> work::readInBookFile(const std::unique_ptr<Arguments>& argv, std::vector<LineNode>::iterator& vBookIter) {
	// create the book vector, fill it with all the ASCII chars 0-128 and create an iterator to access each character node 
	std::vector<LineNode> vBook;

	std::cout << "--- Reading in BookFile: " << argv->returnArg(2) << std::endl;

	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv->returnArg(2));
	int line = 1;	//txt files start at line 1, not index 0
	int offset = 0;
	std::string strLine = "";
	std::string strLineTester = "";
	std::vector<char> checkVec = utility::returnAcceptedCharsVector();

	while (std::getline(bookFile, strLine)) {

		// check that the string line has all valid characters 
		// a string which is not empty is formatted and returned with the problem char
		// in this case we prompt an error and exit the program by returning an empty bookfile vector
		strLineTester = utility::acceptedChars(strLine, checkVec);
		if (strLineTester != "") {
			utility::invalidCharacter(strLineTester, line);
			vBook.clear();
			return vBook;
		}

		std::unique_ptr<LineNode> newNode = std::make_unique<LineNode>(line);

		// iterate over the line string, find the char match, add the location to the match, increment offset counter 
		for (const auto& currentChar : strLine) {
			newNode->addLoc(currentChar, offset);
			++offset;
		}

		// push a newline after the rest of the line is read, reset offset, increment line counter 
		newNode->addLoc('\n', offset);
		offset = 0;
		++line;

		vBook.emplace_back(*newNode);
	}

	std::cout << "--- Successfully read in BookFile!" << std::endl;
	bookFile.close();


	return vBook;
}

/* drives the encoding functionality of the program
* each character of the user provided file is matched to a corresponding character in the bookfile by calling find_if() on the vector
* locations are pushed into the cipher, after each line is read a newline line/offset is pushed, also using find_if() to find one within the vector
*/
int work::executeEncoding(const std::unique_ptr<Arguments>& argv) {

	// construct the bookfile's components 
	std::vector<CharNode>::iterator vBookIter;
	std::vector<CharNode> vBook = std::move(work::readInBookFile(argv, vBookIter));

	// if the vBook turns out to be empty, an invalid character was found
	// clears out the vector, returns it as empty and the program terminates 
	// error message is spawned from readInBookFile
	if (vBook.empty())
		return EXIT_FAILURE; 

	// prompt user and attempt to open file that is to be encoded, fail program if there's an issue opening 
	std::cout << "--- Opening file to be encoded: " << argv->returnArg(3) << std::endl;
	std::ifstream cipherIn(argv->returnArg(3));
	if (!cipherIn.good()) {
		std::cerr << argv->returnArg(3) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing ciper to: " << argv->returnArg(4) << std::endl;
	std::ofstream cipherOut(argv->returnArg(4));
	if (!cipherOut.good()) {
		std::cerr << argv->returnArg(4) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// shuffle the contents of the internal vectors to get some randomness
	for (CharNode& node : vBook)
		node.shuffleLoc();

	// read in a line that has two integers and a new line 

	std::string strLine = "";
	while (std::getline(cipherIn, strLine)) {
		for (const auto& currentChar : strLine) {

			// point the book iterator to where it matches the current char 
			// set the internal iterator to point to the internal vector.begin() to start pulling locations 
			vBookIter = std::find_if(vBook.begin(), vBook.end(), [currentChar](CharNode& x) { return x.getChar() == currentChar; });
			cipherOut << vBookIter->_loc_iter->first << " " << vBookIter->_loc_iter->second << '\n';
			vBookIter->incrementIter();
		}
		vBookIter = std::find_if(vBook.begin(), vBook.end(), [](CharNode& x) { return x.getChar() == '\n'; });
		//vBookIter->incrementIter();
		cipherOut << vBookIter->_loc_iter->first << " " << vBookIter->_loc_iter->second << '\n';
		vBookIter->incrementIter();
	}

	std::cout << "--- Successfully wrote encoded ciper!" << std::endl;

	cipherIn.close();
	cipherOut.close();

	return EXIT_SUCCESS;
}

/* drives the decoding functionality of the program
* each line of the user provided file is matched to a char/offset for that line.. by using find_if()
* characters that match the line/offset in the bookfile are pushed to the decoded file argv[4]
*/
int work::executeDecoding(const std::unique_ptr<Arguments>& argv) {
	std::vector<LineNode>::iterator vBookIter;
	std::vector<LineNode>vBook = std::move(work::readInBookFile(argv, vBookIter));

	// if the vBook turns out to be empty, an invalid character was found
	// clears out the vector, returns it as empty and the program terminates 
	// error message is spawned from readInBookFile
	if (vBook.empty())
		return EXIT_FAILURE;

	// prompt user and attempt to open file that is already encoded, fail program if there's an issue opening 
	std::cout << "--- Opening encoded file: " << argv->returnArg(3) << std::endl;
	std::ifstream cipherIn(argv->returnArg(3));
	if (!cipherIn.good()) {
		std::cerr << argv->returnArg(3) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing decoded ciper to: " << argv->returnArg(4) << std::endl;
	std::ofstream cipherOut(argv->returnArg(4));
	if (!cipherOut.good()) {
		std::cerr << argv->returnArg(4) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}
	
	int lineNum = 0;
	int offsetNum = 0;	

	while (cipherIn >> lineNum >> std::ws >> offsetNum){
		
		vBookIter = std::find_if(vBook.begin(), vBook.end(), [lineNum](LineNode& x) { return x.getLineValue() == lineNum; });
		
		vBookIter->_loc_iter = std::find_if(
			begin(vBookIter->_locations),
			end(vBookIter->_locations), 
				[offsetNum] (const std::pair<char,int>& currentNode) {
					return currentNode.second == offsetNum;
				}
		);

		cipherOut << vBookIter->_loc_iter->first;

	}

	std::cout << "--- Successfully decoded ciper!" << std::endl;

	cipherIn.close();
	cipherOut.close();
	
	return EXIT_SUCCESS;
}

