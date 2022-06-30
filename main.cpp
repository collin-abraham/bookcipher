#include "utility.h"
#include "workerfunc.h"

int main(int argc, char* argv []) {
	int returnStatus = 0;

	// correct number of args 
	if (argc == 5) {

		// string analysis on argv[1]
		// determine encoding/decoding version of program
		std::string flagArgs = argv[1];
		if (flagArgs == "-e" || flagArgs == "-E") {

			returnStatus = work::executeEncoding(argv);
			if (returnStatus == 1)
				return EXIT_FAILURE;
			else
				return EXIT_SUCCESS;
		}

		else if (flagArgs == "-d" || flagArgs == "-D") {
			returnStatus = work::executeDecoding(argv);
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
	
	// 2 args givenw with -g flag, generate a new book file 
	else if (argc == 3) {
		std::string flagArgs = argv[1];
		if (flagArgs == "-g" || flagArgs == "-G")
			returnStatus = work::generateBookfile(argv);
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


