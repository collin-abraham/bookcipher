#include "nodeclasses.h"
#include <iomanip> // std::ws

namespace work {
	void buildVector(std::vector<CharNode>& v);

	int generateBookfile(char* argv[]);

	int executeEncoding(char* argv[]);

	int executeDecoding(char* argv[]);

	std::vector<CharNode> readInBookFile(char* argv[], std::vector<CharNode>::iterator& vBookIter);

	std::vector<LineNode> readInBookFile(char* argv[], std::vector<LineNode>::iterator& vBookIter);
}
