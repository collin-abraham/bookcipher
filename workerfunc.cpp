#include "workerfunc.h"
#include <memory> //std::unique_ptr<T>


void work::buildVector(std::vector<CharNode>& v) {
	for (int c = 0; c != 128; ++c)
		v.push_back(CharNode((char)c));
}

std::vector<CharNode> work::readInBookFile(char* argv[], std::vector<CharNode>::iterator& vBookIter) {
	// create the book vector, fill it with all the ASCII chars 0-128 and create an iterator to access each character node 
	std::vector<CharNode> vBook;
	work::buildVector(vBook);

	std::cout << "--- Reading in BookFile: " << argv[2] << std::endl;

	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv[2]);
	int line = 1;	//txt files start at line 1, not index 0
	int offset = 0;
	std::string strLine = "";
	while (std::getline(bookFile, strLine)) {

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

std::vector<LineNode> work::readInBookFile(char* argv[], std::vector<LineNode>::iterator& vBookIter) {
	// create the book vector, fill it with all the ASCII chars 0-128 and create an iterator to access each character node 
	std::vector<LineNode> vBook;

	std::cout << "--- Reading in BookFile: " << argv[2] << std::endl;

	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv[2]);
	int line = 1;	//txt files start at line 1, not index 0
	int offset = 0;
	std::string strLine = "";
	while (std::getline(bookFile, strLine)) {

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

int work::executeEncoding(char* argv[]) {

	// construct the bookfile's components 
	std::vector<CharNode>::iterator vBookIter;
	std::vector<CharNode>vBook = std::move(work::readInBookFile(argv, vBookIter));

	// prompt user and attempt to open file that is to be encoded, fail program if there's an issue opening 
	std::cout << "--- Opening file to be encoded: " << argv[3] << std::endl;
	std::ifstream cipherIn(argv[3]);
	if (!cipherIn.good()) {
		std::cerr << argv[3] << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing ciper to: " << argv[4] << std::endl;
	std::ofstream cipherOut(argv[4]);
	if (!cipherOut.good()) {
		std::cerr << argv[4] << " could not be opened or accessed, terminating program.";
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

int work::executeDecoding(char* argv[]) {
	std::vector<LineNode>::iterator vBookIter;
	std::vector<LineNode>vBook = std::move(work::readInBookFile(argv, vBookIter));

	// prompt user and attempt to open file that is already encoded, fail program if there's an issue opening 
	std::cout << "--- Opening encoded file: " << argv[3] << std::endl;
	std::ifstream cipherIn(argv[3]);
	if (!cipherIn.good()) {
		std::cerr << argv[3] << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing decoded ciper to: " << argv[4] << std::endl;
	std::ofstream cipherOut(argv[4]);
	if (!cipherOut.good()) {
		std::cerr << argv[4] << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}
	
	int lineNum = 0;
	int offsetNum = 0;
	//char eatNewline = '\n';
	

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

	std::cout << "--- Successfully wrote encoded ciper!" << std::endl;

	cipherIn.close();
	cipherOut.close();
	
	return EXIT_SUCCESS;
}

