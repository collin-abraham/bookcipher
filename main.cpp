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
	

	// correct number of args for encoding or decoding
	if (argc == 5) {
		const std::unique_ptr<Arguments> mainArgv = std::make_unique<Arguments>(argv, 4);

		// string analysis on argv[1]
		// determine encoding/decoding version of program
		// also possible to generate a new book file with -g then process the encoding 
		std::string flagArgs = argv[1];
		if (flagArgs == "-e" || flagArgs == "-E") {

			returnStatus = work::executeEncoding(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;
		}

		else if (flagArgs == "-d" || flagArgs == "-D") {

			returnStatus = work::executeDecoding(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;
		}
		else if (flagArgs == "-g" || flagArgs == "-G") {
			work::generateBookfile(mainArgv);	// build a generated bookfile
			utility::generatedBookfileWarning(); // warn the user that this bookfile must be saved or encoding will be impossible 
			
			returnStatus = work::executeEncoding(mainArgv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;

		}
		else {
			std::cerr << "ERROR: Invald flag argument " << flagArgs << "\n";
			utility::usagePrompt();

		}
	}
	
	// 2 args givenw with -g flag, generate a new book file only 
	else if (argc == 3) {
		const std::unique_ptr<Arguments> mainArgv = std::make_unique<Arguments>(argv, 2);

		std::string flagArgs = mainArgv->returnArg(2);
		if (flagArgs == "-g" || flagArgs == "-G")
			returnStatus = work::generateBookfile(mainArgv);
		else {
			std::cerr << "ERROR: Invalid flag given for number of arguments supplied!\n";
			utility::usagePrompt();
		}

	}
	else {
		std::cerr << "ERROR: Invalid number of arguments given! \n";
		utility::usagePrompt();
	}

}


