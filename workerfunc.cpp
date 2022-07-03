/* workerfunc.cpp
* 2022-06-30
* Collin Abraham
* 
* Func definitions for workerfunc.h 
* These functions build the book files corresponding to encoding/decoding and to the process of outputting the end file 
*/

#include "workerfunc.h"


/* build a simple vector that contains all of the ASCII table 0-128, used by read_in_bookfile*/
void work::build_vector(std::vector<CharNode>& v) {
	for (int c = 0; c != 128; ++c)
		v.push_back(CharNode((char)c));
}

/* constructs a randomized book file that can act as a custom made key for future use */
int work::generate_bookfile(const std::unique_ptr<Arguments>& argv) {

	std::cout << "--- Writing new book file to: " << argv->return_arg(2) << std::endl;
	std::ofstream newBook(argv->return_arg(2));

	if (!newBook.good()) {
		std::cerr << argv->return_arg(2) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// pick a number of lines between 1000 and 5000 
	// pick a number of offsets between 30 and 150 
	const int MINLINE = 1000;
	const int MAXLINE = 5000;
	const int MINOFFSET = 30;
	const int MAXOFFSET = 150;
	const std::vector<char> charPossibilites = utility::return_accepted_chars_vector();

	std::default_random_engine gen;
	const std::uniform_int_distribution<int> newLineDistance(MINLINE, MAXLINE);
	const int numLines = newLineDistance(gen);

	const std::uniform_int_distribution<int> offsetDisance(MINOFFSET, MAXOFFSET);
	const int numOffset = offsetDisance(gen);

	const std::uniform_int_distribution<int> randomChar(0, (int)charPossibilites.size() - 1);


	// fill the file with the random number of lines/offsets.. each output is a 
	// a random index between 0 - size() is chosen and the char sitting there is pushed into the new file 
	for (int c = 0; c < numLines; ++c)
		for (int x = 0; x < numOffset; ++x) {
			int charIndex = randomChar(gen);
			auto pullChar = charPossibilites.at(charIndex);
			newBook << pullChar;
		}

	
	std::cout << "--- Successfully created new bookfile!"  << std::endl;

	newBook.close();
	return EXIT_SUCCESS;
}

/* reads in the bookfile and constructs the CharNode vector to add lines/offsets to each Char entry created through work::build_vector */
std::vector<CharNode> work::read_in_bookfile(const std::unique_ptr<Arguments>& argv, std::vector<CharNode>::iterator& vBookIter) {
	
	// create the book vector, fill it with all the ASCII chars 0-128 and create an iterator to access each character node 
	std::vector<CharNode> vBook;
	work::build_vector(vBook);

	std::cout << "--- Reading in BookFile: " << argv->return_arg(2) << std::endl;

	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv->return_arg(2));
	int line = 1;	//.txt files start at line 1, not 0
	int offset = 0;
	std::string strLine = "";
	std::string strLineTester = "";
	std::vector<char> checkVec = utility::return_accepted_chars_vector();

	while (std::getline(bookFile, strLine)) {

		// check that the string line has all valid characters 
		// a string which is not empty is formatted and returned with the problem char
		// in this case we prompt an error and exit the program by returning an empty bookfile vector
		strLineTester = utility::accepted_chars(strLine, checkVec);
		if (strLineTester != "") {
			utility::invalid_character(strLineTester, line, argv->return_arg(2));
			utility::show_valid_chars();
			vBook.clear();
			return vBook;
		}


		// iterate over the line string, find the char match, add the location to the match, increment offset counter 
		for (const auto& currentChar : strLine) {
			vBookIter = std::find_if(vBook.begin(), vBook.end(), [currentChar](CharNode& x) { return x.get_char() == currentChar; });
			vBookIter->add_loc(line, offset);
			vBookIter->reset_iter();	// ensure the internal iterator points to the beginning still 
			++offset;
		}

		// push a newline after the rest of the line is read, reset offset, increment line counter 
		vBookIter = std::find_if(vBook.begin(), vBook.end(), [](CharNode& x) { return x.get_char() == '\n'; });
		vBookIter->add_loc(line, offset);
		vBookIter->reset_iter();
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
std::vector<LineNode> work::read_in_bookfile(const std::unique_ptr<Arguments>& argv, std::vector<LineNode>::iterator& vBookIter) {
	
	// create the book vector,
	std::vector<LineNode> vBook;

	std::cout << "--- Reading in BookFile: " << argv->return_arg(2) << std::endl;

	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv->return_arg(2));
	int line = 1;	//txt files start at line 1, not index 0
	int offset = 0;
	std::string strLine = "";
	std::string strLineTester = "";
	std::vector<char> checkVec = utility::return_accepted_chars_vector();

	while (std::getline(bookFile, strLine)) {

		// check that the string line has all valid characters 
		// a string which is not empty is formatted and returned with the problem char
		// in this case we prompt an error and exit the program by returning an empty bookfile vector
		strLineTester = utility::accepted_chars(strLine, checkVec);
		if (strLineTester != "") {
			utility::invalid_character(strLineTester, line, argv->return_arg(2));
			vBook.clear();
			return vBook;
		}

		std::unique_ptr<LineNode> newNode = std::make_unique<LineNode>(line);

		// iterate over the line string, find the char match, add the location to the match, increment offset counter 
		for (const auto& currentChar : strLine) {
			newNode->add_loc(currentChar, offset);
			++offset;
		}

		// push a newline after the rest of the line is read, reset offset, increment line counter 
		newNode->add_loc('\n', offset);
		offset = 0;
		++line;

		// the line object with chars/offsets in the book vector 
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
int work::execute_encoding(const std::unique_ptr<Arguments>& argv) {

	// construct the bookfile's components 
	std::vector<CharNode>::iterator vBookIter;
	std::vector<CharNode> vBook = std::move(work::read_in_bookfile(argv, vBookIter));

	// if the vBook turns out to be empty, an invalid character was found
	// clears out the vector, returns it as empty and the program terminates 
	// error message is spawned from read_in_bookfile
	if (vBook.empty())
		return EXIT_FAILURE; 

	// prompt user and attempt to open file that is to be encoded, fail program if there's an issue opening 
	std::cout << "--- Opening file to be encoded: " << argv->return_arg(3) << std::endl;
	std::ifstream cipherIn(argv->return_arg(3));
	if (!cipherIn.good()) {
		std::cerr << argv->return_arg(3) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing ciper to: " << argv->return_arg(4) << std::endl;
	std::ofstream cipherOut(argv->return_arg(4));
	if (!cipherOut.good()) {
		std::cerr << argv->return_arg(4) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// shuffle the contents of the internal vectors to get some randomness
	for (CharNode& node : vBook)
		node.shuffle_loc();

	std::string strLine = "";
	std::string strLineTester = "";
	int line = 1; // only used in error checking for this part 
	std::vector<char> checkVec = utility::return_accepted_chars_vector();

	while (std::getline(cipherIn, strLine)) {

		// check that the string line has all valid characters 
		// a string which is not empty is formatted and returned with the problem char
		// in this case we prompt an error and exit the program by returning an empty bookfile vector
		strLineTester = utility::accepted_chars(strLine, checkVec);
		if (strLineTester != "") {
			utility::invalid_character(strLineTester, line, argv->return_arg(3));
			utility::show_valid_chars();
			utility::delete_partial_file(argv->return_arg(4));
			return EXIT_FAILURE;
		}

		// start looping through each char of each line 
		for (const auto& currentChar : strLine) {

			// point the book iterator to where it matches the current char 
			// set the internal iterator to point to the internal vector.begin() to start pulling locations 
			vBookIter = std::find_if(vBook.begin(), vBook.end(), [currentChar](CharNode& x) { return x.get_char() == currentChar; });
			cipherOut << vBookIter->_loc_iter->first << " " << vBookIter->_loc_iter->second << '\n';
			vBookIter->increment_iter();
		}

		// point iterator to where newline is stored, push each new line we encounter
		vBookIter = std::find_if(vBook.begin(), vBook.end(), [](CharNode& x) { return x.get_char() == '\n'; });
		cipherOut << vBookIter->_loc_iter->first << " " << vBookIter->_loc_iter->second << '\n';
		vBookIter->increment_iter();

		++line;
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
int work::execute_decoding(const std::unique_ptr<Arguments>& argv) {
	std::vector<LineNode>::iterator vBookIter;
	std::vector<LineNode>vBook = std::move(work::read_in_bookfile(argv, vBookIter));

	// if the vBook turns out to be empty, an invalid character was found during read_in_bookfile()..
	// clears out the vector, returns it as empty and the program terminates 
	// error message is spawned from read_in_bookfile()
	if (vBook.empty())
		return EXIT_FAILURE;

	// prompt user and attempt to open file that is already encoded, fail program if there's an issue opening 
	std::cout << "--- Opening encoded file: " << argv->return_arg(3) << std::endl;
	std::ifstream cipherIn(argv->return_arg(3));
	if (!cipherIn.good()) {
		std::cerr << argv->return_arg(3) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing decoded ciper to: " << argv->return_arg(4) << std::endl;
	std::ofstream cipherOut(argv->return_arg(4));
	if (!cipherOut.good()) {
		std::cerr << argv->return_arg(4) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}
	
	int lineNum = 0;
	int offsetNum = 0;	

	// check the encoded file to ensure it's only digits and spaces as expected
	std::string lineTester = utility::check_encoded_file(argv->return_arg(3));
	if (lineTester != "") {
		utility::invalid_character(lineTester, argv->return_arg(3));
		utility::show_valid_decoded_example();
		return EXIT_FAILURE;
	}
	
	// start pulling the line and offset information 
	while (cipherIn >> lineNum >> std::ws >> offsetNum) {
	
		// set the book iterator to the matching line 
		vBookIter = std::find_if(vBook.begin(), vBook.end(), [lineNum](LineNode& x) { return x.get_line_value() == lineNum; });
		
		// within that line object, set the internal iterator to match the offset
		vBookIter->_loc_iter = std::find_if(begin(vBookIter->_locations), end(vBookIter->_locations), 
				[offsetNum] (const std::pair<char,int>& currentNode) {
					return currentNode.second == offsetNum;
				}
		);

		// push the character that the internal iterator is pointing to 
		cipherOut << vBookIter->_loc_iter->first;

	}

	std::cout << "--- Successfully decoded ciper!" << std::endl;

	cipherIn.close();
	cipherOut.close();
	
	return EXIT_SUCCESS;
}

/* used in a predicate function throughout execute_decoding2 (experimental, not implemented) */
bool work::sort_condition(EncodedObj& lhs, EncodedObj& rhs) {
	
	if (lhs._line < rhs._line)
		return true;
	if (rhs._line < lhs._line)
		return false;
	if (lhs._line == rhs._line)
		if (lhs._offset < rhs._offset)
			return true;
		if (rhs._offset < lhs._offset)
			return false;
}

/* an experimental different approach to decoding.. showed to be slower as the other method. left the algorithm anyways */
int work::execute_decoding2(const std::unique_ptr<Arguments>& argv) {
	
	// prompt user and attempt to open file that is already encoded, fail program if there's an issue opening 
	std::cout << "--- Opening encoded file: " << argv->return_arg(3) << std::endl;
	std::ifstream cipherIn(argv->return_arg(3));
	if (!cipherIn.good()) {
		std::cerr << argv->return_arg(3) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}

	// check out our encoded file for issues 
	std::string lineTester = utility::check_encoded_file(argv->return_arg(3));
	if (lineTester != "") {
		utility::invalid_character(lineTester, argv->return_arg(3));
		utility::show_valid_decoded_example();
		return EXIT_FAILURE;
	}

	// push the encoded objects into a vector, saving their original position to use for sorting later 
	std::vector<EncodedObj> vEncodedObj;
	int lineNum = 0;
	int offsetNum = 0;
	int idxCounter = 0;
	while (cipherIn >> lineNum >> std::ws >> offsetNum) {
		vEncodedObj.emplace_back(EncodedObj(lineNum, offsetNum, idxCounter, 'x'));
		++idxCounter;
	}

	// ------------------------------------------------------------------------------------------------------

	std::cout << "--- Writing decoded ciper to: " << argv->return_arg(4) << std::endl;
	std::ofstream cipherOut(argv->return_arg(4));
	if (!cipherOut.good()) {
		std::cerr << argv->return_arg(4) << " could not be opened or accessed, terminating program.";
		return EXIT_FAILURE;
	}
	
	
	// sort encoded by line by offset 
	std::sort(begin(vEncodedObj), end(vEncodedObj), [](auto& lhs, auto& rhs) { return work::sort_condition(lhs, rhs); });

	// iterate through the bookfile and match line/offsets with chars it encounters
	//while we can read in lines from the bookfile
	std::ifstream bookFile(argv->return_arg(2));
	int line = 1;	//txt files start at line 1, not index 0
	int offset = 0;
	std::string strLine = "";
	std::string strLineTester = "";

	std::vector<LineOffsetChar> vBook;
	std::vector<char>checkVec = utility::return_accepted_chars_vector();

	// start pulling in lines from the bookfile
	// test the line for validity

	while (std::getline(bookFile, strLine)) {

		strLineTester = utility::accepted_chars(strLine, checkVec);
		if (strLineTester != "") {
			utility::invalid_character(strLineTester, line, argv->return_arg(2));
			utility::show_valid_decoded_example();
			return EXIT_FAILURE;
		}

		// push the line into an object that contains line, offset, character to store the entirety of the book encapsulated into individual objects (in a vector)
		for (const char& x : strLine) {
			vBook.emplace_back(LineOffsetChar(line, offset, x));
			++offset;
		}

		// push a new line 
		vBook.emplace_back(LineOffsetChar(line, offset, '\n'));
		offset = 0;
		++line;
	}
	bookFile.close();

	// loop through the vector of encoded objects
	// set an iterator to find (within the book) where both lines and both offsets match.. place the char found there in the book into the encoded object 
	for (EncodedObj& x : vEncodedObj) {
		auto it = std::find_if(begin(vBook), end(vBook), [x](const LineOffsetChar& loc) { return x._line == loc._line && x._offset == loc._offset; });
		x._ch = it->_ch;
	}

	// resort the vector based on the original index to get the correct order of characters as it appeared when read in originally
	std::sort(begin(vEncodedObj), end(vEncodedObj), [](auto& lhs, auto& rhs) { return lhs._idx < rhs._idx; });

	// push the now altered characters into the output file
	for (EncodedObj& x : vEncodedObj) 
		cipherOut << x._ch;
	
	std::cout << "--- Successfully wrote decoded cipher to: " << argv->return_arg(4) << '\n';

	cipherIn.close();
	cipherOut.close();

	return EXIT_SUCCESS;

}

