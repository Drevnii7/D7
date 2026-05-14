#include <iostream>

#include "source/utils/assert.hpp"
#include "source/utils/expected.hpp"

#include "source/lexer/ETokenType.hpp"
#include "source/lexer/FToken.hpp"

int main()
{
	std::cout << "Hello from first rewrite from zero!\n";


	assert(1 == 0, "test", "testtoken");
}
