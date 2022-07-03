/* main.cpp
* 2022-06-30
* Collin Abraham
* 
* Main function to drive the program, checks the user inputted flags and some basic run-time error checking 
* Should be noted that argv is passed to the worker functions through a custom class <Arguments> declaration in nodeclasses.h
*/

#include "utility.h"
#include "workerfunc.h"
#include <array>



int main(int argc, char* argv []) {
	int returnStatus = 0;

	// pass argv around in an object container, rather than just sending a pointer to the C-style array 
	const std::unique_ptr<Arguments> mainArgv = std::make_unique<Arguments>(argv, 4);

	// correct number of args for encoding or decoding
	if (argc == 5) {

		// string analysis on argv[1]
		// determine encoding/decoding version of program
		// also possible to generate a new book file with -g then process the encoding 
		std::string flagArgs = argv[1];

		// user wants to encode 
		if (flagArgs == "-e" || flagArgs == "-E") {

			returnStatus = work::execute_encoding(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;
		}

		// user wants to decode 
		else if (flagArgs == "-d" || flagArgs == "-D") {

			returnStatus = work::execute_decoding(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;
		}

		// user wants to generate a book file and encode
		else if (flagArgs == "-g" || flagArgs == "-G") {
			work::generate_bookfile(mainArgv);	// build a generated bookfile
			utility::generate_bookfile_warning(); // warn the user that this bookfile must be saved or encoding will be impossible 
			
			returnStatus = work::execute_encoding(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;

		}
		else {
			std::cerr << "ERROR: Invald flag argument " << flagArgs << "\n";
			utility::usage_prompt();

		}
	}
	
	// user wants to generate a new book file only 
	else if (argc == 3) {

		std::string flagArgs = mainArgv->return_arg(1);
		if (flagArgs == "-g" || flagArgs == "-G") {

			returnStatus = work::generate_bookfile(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;
		}
		
		else {
			std::cerr << "ERROR: Invalid flag given for number of arguments supplied!\n";
			utility::usage_prompt();
			return EXIT_FAILURE;
		}

	}
	else {
		std::cerr << "ERROR: Invalid number of arguments given! \n";
		utility::usage_prompt();
		return EXIT_FAILURE;
	}

}


