#include "utility.h"
#include "workerfunc.h"

int main(int argc, char* argv []) {

	// string analysis on argv[1]
	// determine encoding/decoding version of program
	std::string flagArgs = argv[1];
	if (flagArgs == "-e" || flagArgs == "-E") {

		int returnStatus = work::executeEncoding(argv);
		if (returnStatus == 1)
			return EXIT_FAILURE;
		else
			return EXIT_SUCCESS;
	}

	else if (flagArgs == "-d" || flagArgs == "-D")
		work::executeDecoding(argv);
	else {
		std::cout << "ERROR: Invald argument " << flagArgs << "\n";
		utility::usagePrompt();

	}
}


