#pragma once
#include <iostream> // std::cerr
#include <cstdlib>  // std::abort

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
            const char* message = nullptr,
            const char* dump_token = nullptr
        )
        {
            std::cerr << "\n<=========ASSERTION FAILED==========\n";
            std::cerr << "File: " << file << "\n";
            std::cerr << "Line: " << line << "\n";
            // Hide expression if call from unreachable
            if (!strcmp(expression, "false"))
            {
                std::cerr << "Expression: " << expression << "\n";
            }
            if (message != nullptr)
            {
                std::cerr << "Message: " << message << "\n";
            }
            if (dump_token != nullptr)
            {
                std::cerr << "Token: " << dump_token << "\n";
            }
            std::cerr << "==========ASSERTION FAILED=========>\n";
        }
    }
}

// Args: expression, message, token dump
#define assert(expr, ...) \
    do { \
        if (!(expr)) { \
            d7::assert::AssertImplementation(#expr, __FILE__, __LINE__, __VA_ARGS__); \
            std::abort(); \
        } \
    } while(0)

// Args: message, token dump
#define unreachable(...) assert(false, __VA_ARGS__)

#define assertFlow(expr, ...) \
    do { \
        if (!(expr)) { \
            d7::assert::AssertImplementation(#expr, __FILE__, __LINE__, __VA_ARGS__); \
        } \
    } while(0)


// Args: expression, message, token dump
// Especially for testing
#define assertTest(expr, ...) \
    do { \
        if (!(expr)) { \
            d7::assert::AssertImplementation(#expr, __FILE__, __LINE__, __VA_ARGS__); \
            l_fails++; \
        } \
    } while(0)
