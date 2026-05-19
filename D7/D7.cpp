#include <iostream>

#include "source/utils/notify.hpp"
#include "source/utils/assert.hpp"
#include "source/utils/expected.hpp"

#include "source/common/ETokenType.hpp"
#include "source/common/FToken.hpp"

#include "source/lexer/CLexer.hpp"

#include "source/preprocessor/IPreprocessor.hpp"
#include "source/preprocessor/CPreprocessor.hpp"

#include "source/service/CService.hpp"

std::string File_Source = "test_code.d7";
std::string File_Tokens = "test_code.tokens";

using expected = d7::expected;

#ifdef _WIN32
#include <windows.h>
#endif

static void enableANSI() 
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}



int main(int argc, char* argv[])
{
    enableANSI();

	d7::CService Service;

    int targc;
    char** targv;

    if (argc > 1)
    {
        targc = argc;
        targv = argv;
    }
    else
    {   
		#ifdef NDEBUG
            static const char* default_argv[] =
            {
                "program_name",     // 0
                "-h",               // 1
                nullptr             // 2
            };

            targc = 2;
            targv = const_cast<char**>(default_argv);
		#else
            static const char* default_argv[] =
            {
                "program_name",           // 0

                "-li",                     // 1
                    "code/test_code.d7",   // 2  .d7 - source code
                "-lo",                     // 3
                    "code/test_code.lt",   // 4  .lt - lexer token
                "-ppi",                    // 5
                    "code/test_code.lt",   // 6
                "-ppo",                    // 7
                    "code/test_code.ppt",  // 8  .ppt - pre processor token
                "-pi",                     // 9
                    "code/test_code.ppt",  // 10
                "-po",                     // 11
                    "code/test_code.past", // 12 .past - parser ast
                 "--notify-mask",          // 13
                    "1111",                // 14

                nullptr                   // 15
            };

            targc = 15;
            targv = const_cast<char**>(default_argv);
		#endif
    }
	
	if (expected Exp = Service.SetConfig(targc, const_cast<char**>(targv)); !Exp)
	{
		notify_warning(("Service.SetConfig(): " + Exp.ExtractFatalMessageOrFail()).c_str());
        //std::cin.get();
        return -1;
	}

	if (expected Exp = Service.Run(); !Exp)
	{
		notify_warning(("Service.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
        //std::cin.get();
        return -2;
	}

	return 0;
}
