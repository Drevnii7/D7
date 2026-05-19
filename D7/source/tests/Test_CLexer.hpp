#pragma once
#ifndef DISABLE_TEST

#include "../lexer/CLexer.hpp"
#include "../utils/assert.hpp"

namespace d7
{
    namespace tests
    {
        bool Test_CLexer()
        {
            int l_fails = 0;

            {
                CLexer Lexer;

                std::vector<std::string> Code
                {
                    "var float Pi = 3.14f;",
                    "var int Answer = 42;",
                    "if (Pi > 3.0f) {",
                    "return Answer;",
                    "}"
                };

                Lexer.SetCode(std::move(Code));

                auto result = Lexer.Run();
                assertTest(result.IsSuccess(), "Lexer.Run() failed");

                std::vector<d7::FToken> Tokens = Lexer.ExtractTokens();

                assertTest(Tokens[0].type == ETokenType::DEF_VAR, Tokens[0].Dump().c_str());
                assertTest(Tokens[1].type == ETokenType::IDENTIFIER, Tokens[1].Dump().c_str());
                assertTest(Tokens[2].type == ETokenType::IDENTIFIER, Tokens[2].Dump().c_str());
                assertTest(Tokens[3].type == ETokenType::ASSIGN, Tokens[3].Dump().c_str());
                assertTest(Tokens[4].type == ETokenType::FLOAT, Tokens[4].Dump().c_str());
                assertTest(Tokens[5].type == ETokenType::SEMICOLON, Tokens[5].Dump().c_str());

                assertTest(Tokens[6].type == ETokenType::DEF_VAR, Tokens[6].Dump().c_str());
                assertTest(Tokens[7].type == ETokenType::IDENTIFIER, Tokens[7].Dump().c_str());
                assertTest(Tokens[8].type == ETokenType::IDENTIFIER, Tokens[8].Dump().c_str());
                assertTest(Tokens[9].type == ETokenType::ASSIGN, Tokens[9].Dump().c_str());
                assertTest(Tokens[10].type == ETokenType::INT, Tokens[10].Dump().c_str());
                assertTest(Tokens[11].type == ETokenType::SEMICOLON, Tokens[11].Dump().c_str());

                assertTest(Tokens[12].type == ETokenType::IF, Tokens[12].Dump().c_str());
                assertTest(Tokens[13].type == ETokenType::LPAR, Tokens[13].Dump().c_str());
                assertTest(Tokens[14].type == ETokenType::IDENTIFIER, Tokens[14].Dump().c_str());
                assertTest(Tokens[15].type == ETokenType::GREATER, Tokens[15].Dump().c_str());
                assertTest(Tokens[16].type == ETokenType::FLOAT, Tokens[16].Dump().c_str());
                assertTest(Tokens[17].type == ETokenType::RPAR, Tokens[17].Dump().c_str());
                assertTest(Tokens[18].type == ETokenType::LBRA, Tokens[18].Dump().c_str());

                assertTest(Tokens[19].type == ETokenType::RETURN, Tokens[19].Dump().c_str());
                assertTest(Tokens[20].type == ETokenType::IDENTIFIER, Tokens[20].Dump().c_str());
                assertTest(Tokens[21].type == ETokenType::SEMICOLON, Tokens[21].Dump().c_str());

                assertTest(Tokens[22].type == ETokenType::RBRA, Tokens[22].Dump().c_str());
            }

            if (l_fails == 0)
            {
                notify_warning("Test_CLexer: ALL TESTS PASSED");
            }
            else
            {
                unreachable(("Test_CLexer: FAILED with" + std::to_string(l_fails) + " errors").c_str());
            }

            return l_fails == 0;
        }
    }
}

#else

namespace d7
{
    namespace tests
    {
        bool Test_FToken()
        {
            return true;
        }
    }
}

#endif