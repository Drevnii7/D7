#include <iostream>

#include "source/utils/notify.hpp"
#include "source/utils/assert.hpp"
#include "source/utils/expected.hpp"

#include "source/common/ETokenType.hpp"
#include "source/common/FToken.hpp"

#include "source/lexer/CLexer.hpp"

#include "source/tests/Test_AllInOne.hpp"
#include "source/tests/Test_ETokenType.hpp"

std::string File_Source = "test_code.d7";
std::string File_Tokens = "test_code.tokens";

using expected = d7::expected;

int main()
{
	d7::tests::Test_AllInOne();
	
	d7::CLexer Lexer;

	if (expected Exp = Lexer.LoadCode(File_Source); !Exp)
	{
		notify_warning(("Lexer.LoadCode(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		return -1;
	}

	if (expected Exp = Lexer.Run(); !Exp)
	{
		notify_warning(("Lexer.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		return -2;
	}

	if (expected Exp = Lexer.SaveTokens(File_Tokens); !Exp)
	{
		notify_warning(("Lexer.SaveTokens(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		return -3;
	}

	return 0;
}
