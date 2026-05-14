#include <iostream>
#include "source/utils/assert.hpp"

int main()
{
	std::cout << "Hello from first rewrite from zero!\n";

	assert(1 == 0, "test", "testtoken");
}
