#include "Lexer.h"
#include <fstream>
#include <iostream>

void CLexer::OpenFile(const std::string& filePath)
{
    Reset();
    m_filePath = filePath;

    std::ifstream opennedFile(m_filePath, std::ios::binary);

    if (!opennedFile.is_open())
    {
        LexerError("Fail open file: \"" + m_filePath + "\"");
        return;
    }

    LexerSucces("Succes open file: \"" + m_filePath + "\"");

    std::string line;
    while (std::getline(opennedFile, line))
    {
        m_code.push_back(line);
    }
}

void CLexer::SetCode(const std::vector<std::string>& code)
{
	Reset();

	m_filePath = "CLexer::SetCode()";
	m_code = code;
}

void CLexer::SetCode(const std::string& code)
{
    m_code.clear();

    size_t start = 0;
    size_t end = code.find('\n');

    while (end != std::string::npos) {
        m_code.push_back(code.substr(start, end - start));
        start = end + 1;
        end = code.find('\n', start);
    }

    if (start < code.length()) 
    {
        m_code.push_back(code.substr(start));
    }
}

void CLexer::Run()
{
    if (m_code.empty())
    {
        LexerWarning("Empty code");
        return;
    }

    m_tokens.clear();
    currentTokenIndex = 0;
    std::string lexeme;
    bool inBlockComment = false;

    auto FlushLexeme = [&](size_t line, size_t rowEnd) 
    {
        if (!lexeme.empty())
        {
            m_tokens.emplace_back(lexeme, line, rowEnd - lexeme.size());
            lexeme.clear();
        }
    };

    for (size_t lineIdx = 0; lineIdx < m_code.size(); ++lineIdx)
    {
        const std::string& lineStr = m_code[lineIdx];
        size_t len = lineStr.size();
        bool inLineComment = false;

        for (size_t i = 0; i < len; ++i)
        {
            char c = lineStr[i];

            // Comments
            if (inBlockComment)
            {
                if (c == '*' && i + 1 < len && lineStr[i + 1] == '/')
                {
                    inBlockComment = false;
                    ++i;
                }
                continue;
            }
            if (inLineComment) continue;

            if (!inLineComment && !inBlockComment)
            {
                // Comments
                if (i + 1 < len)
                {
                    std::string_view two(&lineStr[i], 2);
                    if (two == "//") { FlushLexeme(lineIdx, i); inLineComment = true; ++i; continue; }
                    if (two == "/*") { FlushLexeme(lineIdx, i); inBlockComment = true; ++i; continue; }
                }
            }

            // Strings and chars
            if (c == '"' || c == '\'')
            {
                FlushLexeme(lineIdx, i);
                std::string literal(1, c);
                ++i;
                while (i < len)
                {
                    char ch = lineStr[i];
                    literal += ch;
                    if (ch == '\\') { if (i + 1 < len) { literal += lineStr[i + 1]; ++i; } }
                    else if (ch == c) { ++i; break; }
                    ++i;
                }
                m_tokens.emplace_back(std::move(literal), lineIdx, i - 1);
                continue;
            }

            // Spaces \n \r \r\n
            if (std::isspace(static_cast<unsigned char>(c)))
            {
                FlushLexeme(lineIdx, i);
                continue;
            }

            // Check 2-symbol separator
            bool isSep = false;
            if (i + 1 < len)
            {
                std::string_view two(&lineStr[i], 2);
                if (FToken::internal::is_separator(two))
                {
                    FlushLexeme(lineIdx, i);
                    m_tokens.emplace_back(std::string(two), lineIdx, i);
                    ++i;
                    isSep = true;
                }
            }

            // Check 1-symbol separator
            if (!isSep)
            {
                std::string_view one(&c, 1);
                if (FToken::internal::is_separator(one))
                {
                    FlushLexeme(lineIdx, i);
                    m_tokens.emplace_back(std::string(one), lineIdx, i);
                    isSep = true;
                }
            }

            if (isSep) continue;

            lexeme += c;
        }

        FlushLexeme(lineIdx, len);
    }
}


void CLexer::DebugPrint()
{
    for (const FToken& token : m_tokens)
    {
        std::cout << token.Debug() << '\n';
    }
}

std::span<FToken> CLexer::GetTokens()
{
    return { m_tokens };
}

std::vector<FToken> CLexer::ExtractTokens()
{
    return std::exchange(m_tokens, {});
}

void CLexer::Reset()
{
	m_filePath = "";
	m_code.clear();
	m_tokens.clear();
	currentTokenIndex = 0;
}
