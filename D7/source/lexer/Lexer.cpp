#include "Lexer.h"
#include "../Notify/Notify.h"
#include <fstream>
#include <iostream>

inline constexpr size_t LEXEME_DEFAULT_RESERVE = 256;

bool CLexer::LoadCode(const std::string& filePath)
{
    Reset();
    if (!filePath.empty()) { m_inputFilePath = filePath; }

    std::ifstream openedFile(m_inputFilePath, std::ios::binary);

    if (!openedFile.is_open())
    {
        Error("Fail open file: \"" + m_inputFilePath + "\"");
        return false;
    }

    Success("Succes open file: \"" + m_inputFilePath + "\"");

    std::string line;
    while (std::getline(openedFile, line))
    {
        m_code.push_back(line);
    }

    return true;
}

bool CLexer::SaveTokens(const std::string& filePath)
{
    if (!filePath.empty()) { m_outputFilePath = filePath; }

    try
    {
        FToken::Serialize<std::vector<FToken>>(m_tokens, m_outputFilePath);
    }
    catch (const std::runtime_error& error)
    {
        Error("SaveTokens: " + std::string(error.what()));
        return false;
    }

    return true;
}

void CLexer::SetCode(const std::vector<std::string>& code)
{
	Reset();

    m_inputFilePath = "CLexer::SetCode()";
	m_code = code;
}

void CLexer::SetCode(std::vector<std::string>&& code)
{
    Reset();

    m_inputFilePath = "CLexer::SetCode()";
    m_code = std::move(code);
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

void CLexer::RunProcessing()
{
    if (m_code.empty())
    {
        Warning("Empty code");
        return;
    }

    m_tokens.clear();
    std::string lexeme;
    lexeme.reserve(LEXEME_DEFAULT_RESERVE);
    uint8_t inBlockComment = 0;

    auto FlushLexeme = [&](size_t line, size_t startPos, size_t endPos)
        {
            if (!lexeme.empty())
            {
                m_tokens.emplace_back(lexeme, line, startPos);
                lexeme.clear();
            }
        };

    for (size_t lineIdx = 0; lineIdx < m_code.size(); ++lineIdx)
    {
        const std::string& lineStr = m_code[lineIdx];
        size_t len = lineStr.size();
        bool inLineComment = false;
        size_t lexemeStart = 0;

        for (size_t i = 0; i < len; ++i)
        {
            char c = lineStr[i];

            // Comments
            if (inBlockComment > 0)
            {
                if (c == '/' && i + 1 < len && lineStr[i + 1] == '*') {
                    inBlockComment++;
                    ++i;
                }
                else if (c == '*' && i + 1 < len && lineStr[i + 1] == '/') {
                    if (inBlockComment > 0) inBlockComment--;
                    ++i;
                }
                continue;
            }
            if (inLineComment) continue;

            if (!inLineComment && inBlockComment == 0)
            {
                if (i + 1 < len)
                {
                    std::string_view two(&lineStr[i], 2);
                    if (two == "//") {
                        FlushLexeme(lineIdx, lexemeStart, i - 1);
                        inLineComment = true;
                        ++i;
                        continue;
                    }
                    if (two == "/*") {
                        FlushLexeme(lineIdx, lexemeStart, i - 1);
                        inBlockComment++;
                        ++i;
                        continue;
                    }
                }
            }

            // Strings and chars
            if (c == '"' || c == '\'')
            {
                FlushLexeme(lineIdx, lexemeStart, i - 1);
                lexemeStart = i;
                std::string literal(1, c);
                ++i;
                while (i < len)
                {
                    char ch = lineStr[i];
                    literal += ch;
                    if (ch == '\\') {
                        if (i + 1 < len) {
                            literal += lineStr[i + 1];
                            ++i;
                        }
                    }
                    else if (ch == c) {
                        ++i;
                        break;
                    }
                    ++i;
                }
                m_tokens.emplace_back(std::move(literal), lineIdx, lexemeStart);
                lexemeStart = i;
                continue;
            }

            // Spaces
            if (std::isspace(static_cast<unsigned char>(c)))
            {
                FlushLexeme(lineIdx, lexemeStart, i - 1);
                lexemeStart = i + 1;
                continue;
            }

            // Check 2-symbol separator
            bool isSep = false;
            if (i + 1 < len)
            {
                std::string_view two(&lineStr[i], 2);
                if (FToken::internal::is_separator(two))
                {
                    FlushLexeme(lineIdx, lexemeStart, i - 1);
                    m_tokens.emplace_back(std::string(two), lineIdx, i);
                    ++i;
                    isSep = true;
                    lexemeStart = i + 1;
                }
            }

            // Check 1-symbol separator
            if (!isSep)
            {
                std::string_view one(&c, 1);
                if (FToken::internal::is_separator(one))
                {
                    if (c == '.' && !lexeme.empty() && std::isdigit(static_cast<unsigned char>(lexeme.back())) &&
                        i + 1 < len && std::isdigit(static_cast<unsigned char>(lineStr[i + 1])))
                    {
                        lexeme += c;
                        continue;
                    }

                    FlushLexeme(lineIdx, lexemeStart, i - 1);
                    m_tokens.emplace_back(std::string(one), lineIdx, i);
                    isSep = true;
                    lexemeStart = i + 1;
                }
            }

            if (isSep) continue;

            if (lexeme.empty())
            {
                lexemeStart = i;
            }
            lexeme += c;
        }

        FlushLexeme(lineIdx, lexemeStart, len - 1);
        lexemeStart = len;
    }
}

bool CLexer::RunFullCycle()
{
    if (!LoadCode())
        return false;

    RunProcessing();

    if (!SaveTokens())
        return false;

    return true;
}

void CLexer::DebugPrint() const
{
    Warning("DebugPrint()");
    for (const FToken& token : m_tokens)
    {
        std::cout << token.Debug() << '\n';
    }
}

#if HAS_STD_SPAN == 1
    std::span<FToken> CLexer::GetTokens()
    {
        return { m_tokens };
    }
#else
    const std::vector<FToken>& CLexer::GetTokens()
    {
        return m_tokens;
    }
#endif



std::vector<FToken> CLexer::ExtractTokens()
{
    return std::exchange(m_tokens, {});
}

void CLexer::Reset()
{
    m_inputFilePath = "";
    m_outputFilePath = "";
	m_code.clear();
	m_tokens.clear();
}

void CLexer::Fatal(const std::string& message) const
{
    FatalBase(UNotifyFrom::LEXER, message);
}

void CLexer::Error(const std::string& message) const
{
    ErrorBase(UNotifyFrom::LEXER, message);
}

void CLexer::Warning(const std::string& message) const
{
    WarningBase(UNotifyFrom::LEXER, message);
}

void CLexer::Success(const std::string& message) const
{
    SuccessBase(UNotifyFrom::LEXER, message);
}