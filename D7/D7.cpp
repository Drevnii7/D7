#include <iostream>

#include "source/utils/assert.hpp"
#include "source/utils/expected.hpp"

#include "source/lexer/ETokenType.hpp"
#include "source/lexer/FToken.hpp"

#include "source/tests/Test_ETokenType.hpp"

int main()
{
	std::cout << "Hello from first rewrite from zero!\n";

	Test_ETokenType();

	//assert(1 == 0, "test", "testtoken");
	return 0;
}
