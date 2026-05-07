#include "Preprocessor.h"
#include "../Notify/Notify.h"
#include <fstream>
#include <iostream>

bool CPreprocessor::Main(int argc, char* argv[])
{
    std::vector<std::string> l_args;
    l_args.reserve(argc);

    for (int i = 0; i < argc; ++i)
    {
        l_args.emplace_back(argv[i]);
    }

    return Main(l_args);
}

bool CPreprocessor::Main(const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        std::cout << "[1] input file, [2] output file, [3+]  -dp/--debugPrint \n";
        return false;
    }

    std::string l_inputPath = args[0];
    std::string l_outputPath = args[1];
    bool l_enableDebugPrint = false;

    for (size_t i = 3; i < args.size(); ++i)
    {
        std::string arg = args[i];
        if (arg == "-dp" || arg == "--debugPrint")
        {
            l_enableDebugPrint = true;
        }
        else
        {
            LexerWarning("Unknown argument: " + arg);
        }
    }

    try
    {
        LoadTokens(l_inputPath);
        Run();
        SaveTokens(l_outputPath);
    }
    catch (const std::exception& e)
    {
        LexerError(std::string("Critical error during processing: ") + e.what());
        return false;
    }

    if (l_enableDebugPrint)
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
