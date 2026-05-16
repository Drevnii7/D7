#include <iostream>

#include "source/utils/assert.hpp"
#include "source/utils/expected.hpp"

#include "source/common/ETokenType.hpp"
#include "source/common/FToken.hpp"

#include "source/tests/Test_AllInOne.hpp"
#include "source/tests/Test_ETokenType.hpp"

int main()
{
	std::cout << "Hello from first rewrite from zero!\n";
	
	d7::tests::Test_AllInOne();
	d7::FToken Token(8, 4, "42");
	
	std::cout << Token.Dump() << std::endl;
	return 0;
}
