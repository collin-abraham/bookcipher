#include "utility.h"

void utility::usagePrompt() {
	std::cout << "To encode: cipher.exe -e bookfile.txt source.txt output.txt\n"
		<< "To decode: cipher.exe -d bookfile.txt encoded.txt decoded.txt";
}