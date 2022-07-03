/* workerfunc.h
* 2022-06-30
* Collin Abraham
*
* Func declarations for workerfunc.cpp
*/


#include "nodeclasses.h"

namespace work {
	void build_vector(std::vector<CharNode>& v);

	int generate_bookfile(const std::unique_ptr<Arguments>& argv);

	int execute_encoding(const std::unique_ptr<Arguments>& argv);

	int execute_decoding(const std::unique_ptr<Arguments>& argv);

	bool sort_condition(EncodedObj& lhs, EncodedObj& rhs);

	int execute_decoding2(const std::unique_ptr<Arguments>& argv);

	std::vector<CharNode> read_in_bookfile(const std::unique_ptr<Arguments>& argv, std::vector<CharNode>::iterator& vBookIter);

	std::vector<LineNode> read_in_bookfile(const std::unique_ptr<Arguments>& argv, std::vector<LineNode>::iterator& vBookIter);
}
