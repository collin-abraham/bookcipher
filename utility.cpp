#include "utility.h"

void utility::usagePrompt() {
	std::cout << "Encoding: cipher.exe -e bookfile.txt source.txt output.txt\n"
		<< "Decoding: cipher.exe -d bookfile.txt encoded.txt decoded.txt\n"
		<< "Generate Bookfile: cipher.exe -g bookfile.txt";
}