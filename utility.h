/* utility.h
* Collin Abraham
* 2022-06-30
* 
* Top level utility namespace for combining headers and error messages 
* This header file also acts as a linker with the main std libraries used in the program 
*/

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory> //std::unique_ptr<T>
#include <random>

// I could have used CPP 17 and avoided this, but it broke std::random_shuffle for some unknown reason...
// so I kept it on CPP 14 and suppressed the warnings, used the experimental library
#include <experimental/filesystem>
using std::experimental::filesystem::remove;
using std::experimental::filesystem::filesystem_error;

namespace utility {

	std::string check_encoded_file(const std::string& argv);

	void delete_partial_file(const std::string& fileName);

	void usage_prompt();

	void generate_bookfile_warning();

	void invalid_character(std::string errorStr, const std::string& fileName);

	void invalid_character(std::string errorStr, int line, const std::string& fileName);

	void show_valid_chars();

	void show_valid_decoded_example();

	const std::vector<char> return_accepted_chars_vector();

	std::string accepted_chars(const std::string& strLine, std::vector<char>& checkVec);
}