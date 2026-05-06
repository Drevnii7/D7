#include "Preprocessor.h"
#include "../Notify/Notify.h"
#include <fstream>
#include <iostream>

bool CPreprocessor::args(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "[0] input file, [1] output file, [2>=]  -dp/--debugPrint \n";
        return false;
    }

    std::string m_inputPath = argv[1];
    std::string m_outputPath = argv[2];
    bool m_enableDebugPrint = false;

    for (int i = 3; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-dp" || arg == "--debugPrint")
        {
            m_enableDebugPrint = true;
        }
        else
        {
            PreprocessorWarning("Unknown argument: " + arg);
        }
    }

    LoadTokens(m_inputPath);
    Run();
    SaveTokens(m_outputPath);

    if (m_enableDebugPrint)
    {
        DebugPrint();
    }

    return true;
}

void CPreprocessor::LoadTokens(const std::string& filePath)
{
    m_filePath = filePath;
    m_tokens = FToken::Deserialize<std::list<FToken>>(m_filePath);
}

void CPreprocessor::SaveTokens(const std::string& filePath)
{
    FToken::Serialize<std::list<FToken>>(m_tokens, filePath);
}

void CPreprocessor::SetTokens(const std::vector<FToken>& tokens)
{
    m_filePath = "CPreprocessor::SetTokens()";
    m_tokens = { tokens.begin(), tokens.end() };
}

void CPreprocessor::SetTokens(const std::list<FToken>& tokens)
{
    m_filePath = "CPreprocessor::SetTokens()";
    m_tokens = tokens;
}

void CPreprocessor::SetTokens(std::list<FToken>&& tokens)
{
    m_filePath = "CPreprocessor::SetTokens()";
    m_tokens = std::move(tokens);
}

void CPreprocessor::Run()
{
    // Placeholder
}

void CPreprocessor::DebugPrint() const
{
    PreprocessorWarning("DebugPrint()");
    for (const FToken& token : m_tokens)
    {
        std::cout << token.Debug() << '\n';
    }
}

const std::list<FToken>& CPreprocessor::GetTokens() const
{
    return m_tokens;
}

std::list<FToken> CPreprocessor::ExtractTokens()
{
	return std::exchange(m_tokens, {});
}

void CPreprocessor::Reset()
{
	m_tokens.clear();
}
