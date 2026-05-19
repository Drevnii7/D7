#include "CLexer.hpp"

#include "../utils/notify.hpp"

inline constexpr size_t LEXEME_DEFAULT_RESERVE = 128;

using expected = d7::expected;

expected d7::CLexer::Run()
{
    notify_callback("Run");

    assert(!m_code.empty(), "No code set - set code first");

    m_tokens.clear();
    std::string lexeme;
    lexeme.reserve(LEXEME_DEFAULT_RESERVE);
    m_tokens.reserve(m_code.size() * 4);

    auto FlushLexeme = [&](size_t line, size_t startPos)
        {
            if (!lexeme.empty())
            {
                m_tokens.emplace_back(FToken(line, startPos, lexeme));
                lexeme.clear();
            }
        };

    int isInMultilineComment = 0;      // In /*  */

    for (size_t line = 0; line < size_t(m_code.size()); ++line)
    {
        const std::string& lineStr = m_code[line];
        size_t len = lineStr.size();
        bool inLineComment = false;
        size_t lexemeStart = 0;

        for (size_t i = 0; i < len; ++i)
        {
            char c = lineStr[i];
            size_t remaining = len - i;

            if (isInMultilineComment > 0)
            {
                // Begin /*
                if (remaining >= 2 && c == '/' && lineStr[i + 1] == '*')
                {
                    isInMultilineComment++;
                    ++i; // Skip /
                    continue; // Skip *
                }
                // End */
                if (remaining >= 2 && c == '*' && lineStr[i + 1] == '/')
                {
                    isInMultilineComment--;
                    ++i; // Skip *
                    continue; // Skip /
                }
                continue;
            }

            if (inLineComment)
                continue;

            if (remaining >= 2)
            {
                // Singleline //
                if (c == '/' && lineStr[i + 1] == '/')
                {
                    FlushLexeme(line, lexemeStart);
                    inLineComment = true;
                    ++i; // Skip /
                    continue; // Skip /
                }

                // Multiline /*
                if (c == '/' && lineStr[i + 1] == '*')
                {
                    FlushLexeme(line, lexemeStart);
                    isInMultilineComment++;
                    ++i; // Skip /
                    continue; // Skip *
                }
            }

            // Strings and chars
            if (c == '"' || c == '\'')
            {
                FlushLexeme(line, lexemeStart);
                lexemeStart = i;
                std::string literal;
                literal += c;
                ++i;

                while (i < len)
                {
                    char ch = lineStr[i];
                    literal += ch;

                    if (ch == '\\')
                    {
                        if (i + 1 < len)
                        {
                            literal += lineStr[i + 1];
                            ++i;
                        }
                    }
                    else if (ch == c)
                    {
                        ++i;
                        break;
                    }
                    ++i;
                }
                i--;

                m_tokens.emplace_back(FToken(line, lexemeStart, std::move(literal)));
                lexemeStart = i;
                continue;
            }

            // Skip spaces
            if (std::isspace(c))
            {
                FlushLexeme(line, lexemeStart);
                lexemeStart = i + 1;
                continue;
            }

            bool isSeparator = false;

            if (remaining >= 2)
            {
                std::string_view two(&lineStr[i], 2);
                if (d7::ETokenTypeUtils::IsSeparator(d7::ETokenTypeUtils::ETokenTypeFromString(two)))
                {
                    FlushLexeme(line, lexemeStart);
                    m_tokens.emplace_back(FToken(line, i, std::string(two)));
                    ++i; // Skip second char
                    isSeparator = true;
                    lexemeStart = i + 1;
                }
            }

            if (!isSeparator)
            {
                std::string_view one(&c, 1);
                if (d7::ETokenTypeUtils::IsSeparator(d7::ETokenTypeUtils::ETokenTypeFromString(one)))
                {
                    if (c == '.' && !lexeme.empty() &&
                        std::isdigit(lexeme.back()) &&
                        i + 1 < len &&
                        std::isdigit(lineStr[i + 1]))
                    {
                        lexeme += c;
                        continue;
                    }

                    FlushLexeme(line, lexemeStart);
                    m_tokens.emplace_back(FToken(line, i, std::string(one)));
                    isSeparator = true;
                    lexemeStart = i + 1;
                }
            }

            if (isSeparator)
                continue;

            if (lexeme.empty())
            {
                lexemeStart = i;
            }
            lexeme += c;
        }

        FlushLexeme(line, lexemeStart);
    }

    if (true)
    {
        notify_trace("Dump tokens:");

        for (const FToken& token : m_tokens)
        {
            notify_trace(token.Dump().c_str());
        }
    }

    return expected::Success();
}