/* workerfunc.h
* 2022-06-30
* Collin Abraham
*
* Func declarations for workerfunc.cpp
*/


#include "nodeclasses.h"

namespace work {
	void buildVector(std::vector<CharNode>& v);

	int generateBookfile(const std::unique_ptr<Arguments>& argv);

	int executeEncoding(const std::unique_ptr<Arguments>& argv);

	int executeDecoding(const std::unique_ptr<Arguments>& argv);

	std::vector<CharNode> readInBookFile(const std::unique_ptr<Arguments>& argv, std::vector<CharNode>::iterator& vBookIter);

	std::vector<LineNode> readInBookFile(const std::unique_ptr<Arguments>& argv, std::vector<LineNode>::iterator& vBookIter);
}
