#pragma once
#include <iostream> // std::cerr
#include <cstdlib>  // std::abort

#include "consolecolors.hpp"

/*
    Custom assert
    Work in DEBUG and RELEASE


    Example work assert and unreachable:

    - assert(1 == 0);
    - assert(1 == 0, "test");
    - assert(1 == 0, "test", "testtoken");

    - unreachable();
    - unreachable("test");
    - unreachable("test", "testtoken");
*/

namespace d7
{
    namespace assert
    {
        inline void AssertImplementation
        (
            const char* expression,
            const char* file,
            int line,
            const char* function = nullptr,
            const char* message = nullptr,
            const char* dump_token = nullptr
        )
        {
            std::cerr << colors::REVERSE << "\n<=========ASSERTION FAILED==========\n" << colors::RESET;
            std::cerr << colors::BRIGHT_CYAN << "File: " << colors::RESET
                << colors::UNDERLINE << file << colors::RESET << "\n";

            std::cerr << colors::BRIGHT_BLUE << "Function: " << colors::RESET
                << colors::BOLD << function << colors::RESET << "\n";

            std::cerr << colors::BRIGHT_YELLOW << "Line: " << colors::RESET
                << line << "\n";

            if (strcmp(expression, "false") != 0)
            {
                std::cerr << colors::BRIGHT_GREEN << "Expression: " << colors::RESET
                    << expression << colors::RESET << "\n";
            }

            if (message != nullptr && message[0] != '\0')
            {
                std::cerr << colors::BRIGHT_MAGENTA << "Message: " << colors::RESET
                    << colors::BOLD << message << colors::RESET << "\n";
            }

            if (dump_token != nullptr && dump_token[0] != '\0')
            {
                std::cerr << colors::BRIGHT_RED << "Token dump: " << colors::RESET
                    << dump_token << "\n";
            }
            std::cerr << colors::REVERSE << "==========ASSERTION FAILED=========>\n" << colors::RESET;
        }
    }
}

#if defined(_MSC_VER)
    #define D7_FUNC_SIGNATURE __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
    #define D7_FUNC_SIGNATURE __PRETTY_FUNCTION__
#else
    #define D7_FUNC_SIGNATURE __FUNCTION__  // fallback
#endif

// Args: expression, message, token dump
#define assert(expr, ...) \
    do { \
        if (!(expr)) { \
            d7::assert::AssertImplementation(#expr, __FILE__, __LINE__, D7_FUNC_SIGNATURE, __VA_ARGS__); \
            std::abort(); \
        } \
    } while(0)

// Args: message, token dump
#define unreachable(...) assert(false, __VA_ARGS__)

#define assertSoft(expr, ...) \
    do { \
        if (!(expr)) { \
            d7::assert::AssertImplementation(#expr, __FILE__, __LINE__, D7_FUNC_SIGNATURE, __VA_ARGS__); \
        } \
    } while(0)


// Args: expression, message, token dump
// Especially for testing
#define assertTest(expr, ...) \
    do { \
        if (!(expr)) { \
            d7::assert::AssertImplementation(#expr, __FILE__, __LINE__, D7_FUNC_SIGNATURE, __VA_ARGS__); \
            l_fails++; \
        } \
    } while(0)
