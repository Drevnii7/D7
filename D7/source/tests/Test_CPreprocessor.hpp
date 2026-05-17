#pragma once

#include "../preprocessor/CPreprocessor.hpp"
#include "../utils/assert.hpp"

namespace d7
{
    namespace tests
    {
        bool Test_CPreprocessor()
        {
            int l_fails = 0;

            {
                CPreprocessor Preprocessor;

                std::list<d7::FToken> TokensBP
                {
                    // #define M_PI_1 3.14f
                    FToken(0, 0, "#"),           // ETokenType::DIRECTIVE
                    FToken(0, 1, "define"),      // ETokenType::IDENTIFIER
                    FToken(0, 2, "M_PI_1"),      // ETokenType::IDENTIFIER
                    FToken(0, 3, "3.14f"),       // ETokenType::FLOAT

                    // #define M_PI_2 $M_PI_1$
                    FToken(1, 0, "#"),           // ETokenType::DIRECTIVE
                    FToken(1, 1, "define"),      // ETokenType::IDENTIFIER
                    FToken(1, 2, "M_PI_2"),      // ETokenType::IDENTIFIER
                    FToken(1, 3, "$"),           // ETokenType::MACRO
                    FToken(1, 4, "M_PI_1"),      // ETokenType::IDENTIFIER
                    FToken(1, 5, "$"),           // ETokenType::MACRO

                    // #define M_PI_3 $M_PI_2$
                    FToken(2, 0, "#"),           // ETokenType::DIRECTIVE
                    FToken(2, 1, "define"),      // ETokenType::IDENTIFIER
                    FToken(2, 2, "M_PI_3"),      // ETokenType::IDENTIFIER
                    FToken(2, 3, "$"),           // ETokenType::MACRO
                    FToken(2, 4, "M_PI_2"),      // ETokenType::IDENTIFIER
                    FToken(2, 5, "$"),           // ETokenType::MACRO

                    // #define M_PI_4 $M_PI_3$
                    FToken(3, 0, "#"),           // ETokenType::DIRECTIVE
                    FToken(3, 1, "define"),      // ETokenType::IDENTIFIER
                    FToken(3, 2, "M_PI_4"),      // ETokenType::IDENTIFIER
                    FToken(3, 3, "$"),           // ETokenType::MACRO
                    FToken(3, 4, "M_PI_3"),      // ETokenType::IDENTIFIER
                    FToken(3, 5, "$"),           // ETokenType::MACRO

                    // var float Pi = $M_PI_4$;
                    FToken(4, 0, "var"),         // ETokenType::DEF_VAR
                    FToken(4, 4, "float"),       // ETokenType::IDENTIFIER
                    FToken(4, 10, "Pi"),         // ETokenType::IDENTIFIER
                    FToken(4, 13, "="),          // ETokenType::ASSIGN
                    FToken(4, 15, "$"),          // ETokenType::MACRO
                    FToken(4, 16, "M_PI_4"),     // ETokenType::IDENTIFIER
                    FToken(4, 22, "$"),          // ETokenType::MACRO
                    FToken(4, 23, ";"),          // ETokenType::SEMICOLON
                };

                Preprocessor.SetTokens(std::move(TokensBP));

                auto result = Preprocessor.Run();
                assertTest(result.IsSuccess(), "Preprocessor.Run() failed");

                std::list<d7::FToken> TokensAP = std::move(Preprocessor.ExtractTokens());
                auto it = TokensAP.begin();

                std::advance(it,  4);
                //                |
                // 0   1     2  3 4<-  5
                // var float Pi = 3.14f;
                assert(it->lexeme == "3.14f" && it->type == ETokenType::FLOAT, " ", it->Dump().c_str());
            }

            if (l_fails == 0)
            {
                notify_warning("Test_CPreprocessor: ALL TESTS PASSED");
            }
            else
            {
                unreachable(("Test_CPreprocessor: FAILED with" + std::to_string(l_fails) + " errors").c_str());
            }

            return l_fails == 0;
        }
    }
}