#pragma once
#ifndef DISABLE_TEST

#include "../common/ETokenType.hpp"
#include "../utils/assert.hpp"


#include <filesystem>

namespace d7
{
    namespace tests
    {
        bool Test_FToken()
        {
            int l_fails = 0;


            {
                d7::FToken original(10, 25, "test_identifier");
                original.type = d7::ETokenType::IDENTIFIER;

                std::stringstream buffer(std::ios::binary | std::ios::in | std::ios::out);

                d7::operator<<(buffer, original);

                d7::FToken deserialized;
                d7::operator>>(buffer, deserialized);

                assertTest(original.lexeme == deserialized.lexeme,
                    (std::string("Lexeme mismatch: ") + original.lexeme + " vs " + deserialized.lexeme).c_str());
                assertTest(original.type == deserialized.type, "Type mismatch");
                assertTest(original.line == deserialized.line, "Line mismatch");
                assertTest(original.row == deserialized.row, "Row mismatch");
            }

            {
                d7::FToken original(5, 10, "");
                original.type = d7::ETokenType::NONE;

                std::stringstream buffer(std::ios::binary | std::ios::in | std::ios::out);
                d7::operator<<(buffer, original);

                d7::FToken deserialized;
                d7::operator>>(buffer, deserialized);

                assertTest(deserialized.lexeme.empty(), "Empty lexeme should stay empty");
                assertTest(deserialized.type == d7::ETokenType::NONE, "Type should be NONE");
            }

            {
                d7::FToken original(1, 1, "Hello\nWorld\t\"Test\"");
                original.type = d7::ETokenType::STRING;

                std::stringstream buffer(std::ios::binary | std::ios::in | std::ios::out);
                d7::operator<<(buffer, original);
                d7::operator>>(buffer, original);

                assertTest(original.lexeme == "Hello\nWorld\t\"Test\"", "String with escapes corrupted");
            }

            {
                std::string longLexeme(5000, 'A');
                d7::FToken original(100, 200, longLexeme);

                std::stringstream buffer(std::ios::binary | std::ios::in | std::ios::out);
                d7::operator<<(buffer, original);
                d7::operator>>(buffer, original);

                assertTest(original.lexeme == longLexeme,
                    (std::string("Long string mismatch: size ") +
                        std::to_string(original.lexeme.size()) +
                        " vs " + std::to_string(longLexeme.size())).c_str());
            }

            {
                std::vector<d7::ETokenType> types = {
                    d7::ETokenType::INT,
                    d7::ETokenType::FLOAT,
                    d7::ETokenType::STRING,
                    d7::ETokenType::IF,
                    d7::ETokenType::WHILE,
                    d7::ETokenType::RETURN
                };

                for (auto type : types) {
                    d7::FToken original(42, 42, "test");
                    original.type = type;

                    std::stringstream buffer(std::ios::binary | std::ios::in | std::ios::out);
                    d7::operator<<(buffer, original);
                    d7::operator>>(buffer, original);

                    assertTest(original.type == type,
                        (std::string("Type mismatch for ") +
                            std::string(d7::ETokenTypeUtils::ETokenTypeToString(type))).c_str());
                }
            }

            {
                std::string testFile = "temp_test_777d777_test_temp.tokens";

                std::vector<d7::FToken> originalTokens;
                originalTokens.emplace_back(1, 1, "var");
                originalTokens.emplace_back(1, 5, "x");
                originalTokens.emplace_back(1, 7, "=");
                originalTokens.emplace_back(1, 9, "42");

                originalTokens[0].type = d7::ETokenType::DEF_VAR;
                originalTokens[1].type = d7::ETokenType::IDENTIFIER;
                originalTokens[2].type = d7::ETokenType::ASSIGN;
                originalTokens[3].type = d7::ETokenType::INT;

                {
                    auto result = d7::FToken::Serialize(originalTokens, testFile);
                    assertTest(result.IsSuccess(), "Serialization to file failed");
                }

                {
                    std::vector<d7::FToken> deserializedTokens;
                    auto result = d7::FToken::Deserialize(deserializedTokens, testFile);

                    assertTest(result.IsSuccess(), "Deserialization from file failed");
                    assertTest(originalTokens.size() == deserializedTokens.size(),
                        "Token count mismatch");

                    for (size_t i = 0; i < originalTokens.size() && i < deserializedTokens.size(); ++i) {
                        assertTest(originalTokens[i].lexeme == deserializedTokens[i].lexeme,
                            (std::string("Token ") + std::to_string(i) + " lexeme mismatch").c_str());
                        assertTest(originalTokens[i].type == deserializedTokens[i].type,
                            (std::string("Token ") + std::to_string(i) + " type mismatch").c_str());
                    }
                }

                // Cleanup
                std::filesystem::remove(testFile);
            }


            if (l_fails == 0)
            {
                notify_warning("Test_FToken: ALL TESTS PASSED");
            }
            else
            {
                unreachable(("Test_FToken: FAILED with" + std::to_string(l_fails) + " errors").c_str());
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
