#pragma once

#include "../common/ETokenType.hpp"
#include "../utils/assert.hpp"

#define TEST_INTEGER(instring, invalue) assertTest(d7::ETokenTypeUtils::TryToInteger(instring, value) && value == invalue, std::to_string(value).c_str());
#define TEST_FLOAT(instring, invalue) assertTest(d7::ETokenTypeUtils::TryToFloat(instring, value) && value == invalue, std::to_string(value).c_str());
#define TEST_STRING(instring, invalue) assertTest(d7::ETokenTypeUtils::TryToString(instring, value) && value == invalue, value.c_str());
#define TEST_CHAR(instring, invalue) assertTest(d7::ETokenTypeUtils::TryToChar(instring, value) && value == invalue, std::to_string(value).c_str());

namespace d7
{
    namespace tests
    {

        bool Test_ETokenType()
        {
            int l_fails = 0;

            {
                assertTest(d7::ETokenTypeUtils::ETokenTypeFromString("42") == d7::ETokenType::INT);
                assertTest(d7::ETokenTypeUtils::ETokenTypeFromString("\"Hello world!\"") == d7::ETokenType::STRING);
                assertTest(d7::ETokenTypeUtils::ETokenTypeFromString("\"\'Hello world!\'\"") == d7::ETokenType::STRING);
                assertTest(d7::ETokenTypeUtils::ETokenTypeFromString("\'\n\'") == d7::ETokenType::CHAR);
                assertTest(d7::ETokenTypeUtils::ETokenTypeFromString("\'a\'") == d7::ETokenType::CHAR);
            }

            {
                assertTest(d7::ETokenTypeUtils::ETokenTypeToString(d7::ETokenType::INT) == "INT");
            }

            {
                assertTest(d7::ETokenTypeUtils::IsSeparator(d7::ETokenType::COMMA) == true);
            }

            {
                assertTest(d7::ETokenTypeUtils::IsConst(d7::ETokenType::INT) == true);
                assertTest(d7::ETokenTypeUtils::IsMath(d7::ETokenType::MUL) == true);
                assertTest(d7::ETokenTypeUtils::IsLogical(d7::ETokenType::OR) == true);
                assertTest(d7::ETokenTypeUtils::IsEqual(d7::ETokenType::LESS) == true);
                assertTest(d7::ETokenTypeUtils::IsAssign(d7::ETokenType::ASSIGN_SUB) == true);
            }

            {
                assertTest(d7::ETokenTypeUtils::IsBoolean("true") == true);
                assertTest(d7::ETokenTypeUtils::IsInteger("42") == true);
                assertTest(d7::ETokenTypeUtils::IsFloat("3.14f") == true);
                assertTest(d7::ETokenTypeUtils::IsString("\"Hello world\"") == true);
                assertTest(d7::ETokenTypeUtils::IsChar("'w'") == true);
            }

            {
                bool value = false;

                assertTest(d7::ETokenTypeUtils::TryToBoolean("true", value) && value == true, std::to_string(value).c_str());
                assertTest(d7::ETokenTypeUtils::TryToBoolean("false", value) && value == false, std::to_string(value).c_str());

                assertTest(!d7::ETokenTypeUtils::TryToBoolean("hello", value));
                assertTest(!d7::ETokenTypeUtils::TryToBoolean("TRUE", value));
                assertTest(!d7::ETokenTypeUtils::TryToBoolean("", value));
            }

            {
                int value = 0;

                TEST_INTEGER("0", 0);
                TEST_INTEGER("-0", 0);
                TEST_INTEGER("42", 42);
                TEST_INTEGER("-42", -42);
                TEST_INTEGER("0d42", 42);
                TEST_INTEGER("-0d42", -42);

                TEST_INTEGER("0x0", 0);
                TEST_INTEGER("-0x0", 0);
                TEST_INTEGER("0x2A", 42);
                TEST_INTEGER("-0x2A", -42);

                TEST_INTEGER("0b0", 0);
                TEST_INTEGER("-0b0", 0);
                TEST_INTEGER("0b00101010", 42);
                TEST_INTEGER("-0b00101010", -42);

                assertTest(!d7::ETokenTypeUtils::TryToInteger("", value));
                assertTest(!d7::ETokenTypeUtils::TryToInteger("42abc", value));
                assertTest(!d7::ETokenTypeUtils::TryToInteger("0xGH", value));
                assertTest(!d7::ETokenTypeUtils::TryToInteger("0b12", value));
            }

            {
                float value = 0.0f;

                assertTest(d7::ETokenTypeUtils::TryToFloat("3.14", value) && std::abs(value - 3.14f) < 0.0001f, std::to_string(value).c_str());
                assertTest(d7::ETokenTypeUtils::TryToFloat("3.14f", value) && std::abs(value - 3.14f) < 0.0001f, std::to_string(value).c_str());

                TEST_FLOAT("0.0", 0.0f);
                TEST_FLOAT("42.0", 42.0f);
                TEST_FLOAT("42.2", 42.2f);
                TEST_FLOAT("42.23", 42.23f);
                TEST_FLOAT("42.234", 42.234f);
                TEST_FLOAT("-42.0", -42.0f);
                TEST_FLOAT("-42.23", -42.23f);
                TEST_FLOAT("-42.234", -42.234f);

                assertTest(!d7::ETokenTypeUtils::TryToFloat("", value));
                assertTest(!d7::ETokenTypeUtils::TryToFloat(".42", value));
                assertTest(!d7::ETokenTypeUtils::TryToFloat("42.", value));
                assertTest(!d7::ETokenTypeUtils::TryToFloat("abc", value));
            }

            {
                std::string value = "";

                TEST_STRING("\"Hello world!\"", "Hello world!");
                TEST_STRING("\"\"", "");
                TEST_STRING("\"Hello\\nWorld\"", "Hello\nWorld");
                TEST_STRING("\"Tab\\tHere\"", "Tab\tHere");
                TEST_STRING("\"Quote: \\\"\"", "Quote: \"");
                TEST_STRING("\"Backslash: \\\\\"", "Backslash: \\");

                assertTest(!d7::ETokenTypeUtils::TryToString("", value));
                assertTest(!d7::ETokenTypeUtils::TryToString("Hello", value));
                assertTest(!d7::ETokenTypeUtils::TryToString("\"Unclosed", value));
                assertTest(!d7::ETokenTypeUtils::TryToString("Unclosed\"", value));
                assertTest(!d7::ETokenTypeUtils::TryToString("\"Invalid\\x escape\"", value));
            }

            {
                char value = '\0';

                TEST_CHAR("'!'", '!');
                TEST_CHAR("'a'", 'a');
                TEST_CHAR("'\\n'", '\n');
                TEST_CHAR("'\\t'", '\t');
                TEST_CHAR("'\\\\'", '\\');
                TEST_CHAR("'\\''", '\'');

                assertTest(!d7::ETokenTypeUtils::TryToChar("", value));
                assertTest(!d7::ETokenTypeUtils::TryToChar("'ab'", value));
                assertTest(!d7::ETokenTypeUtils::TryToChar("'", value));
                assertTest(!d7::ETokenTypeUtils::TryToChar("a", value));
            }

            if (l_fails == 0)
            {
                notify_warning("Test_ETokenType: ALL TESTS PASSED");
            }
            else
            {
                unreachable(("Test_ETokenType: FAILED with" + std::to_string(l_fails) + " errors").c_str());
            }

            return l_fails == 0;
        }
    }
}