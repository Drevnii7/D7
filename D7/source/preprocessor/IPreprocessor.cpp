#include "IPreprocessor.h"
#include "../Notify/Notify.h"
#include <fstream>
#include <iostream>

bool IPreprocessor::LoadTokens(const std::string& filePath)
{
    if (!filePath.empty()) { m_inputFilePath = filePath; }

    try
    {
        m_tokens = FToken::Deserialize<std::list<FToken>>(m_inputFilePath);
    }
    catch (const std::runtime_error& error)
    {
        Error("LoadTokens: " + std::string(error.what()));
        return false;
    }

    return true;
}

bool IPreprocessor::SaveTokens(const std::string& filePath)
{
    if (!filePath.empty()) { m_outputFilePath = filePath; }

    try
    {
        FToken::Serialize<std::list<FToken>>(m_tokens, filePath);
    }
    catch (const std::runtime_error& error)
    {
        Error("SaveTokens: " + std::string(error.what()));
        return false;
    }

    return true;
}

void IPreprocessor::SetTokens(const std::vector<FToken>& tokens)
{
    m_inputFilePath = "IPreprocessor::SetTokens()";
    m_tokens = { tokens.begin(), tokens.end() };
}

void IPreprocessor::SetTokens(const std::list<FToken>& tokens)
{
    m_inputFilePath = "IPreprocessor::SetTokens()";
    m_tokens = tokens;
}

void IPreprocessor::SetTokens(std::list<FToken>&& tokens)
{
    m_inputFilePath = "IPreprocessor::SetTokens()";
    m_tokens = std::move(tokens);
}

bool IPreprocessor::RunFullCycle()
{
    if (!LoadTokens())
        return false;
    
    RunProcessing();

    if (!SaveTokens())
        return false;

    return true;
}

void IPreprocessor::DebugPrint() const
{
    Warning("DebugPrint()");
    for (const FToken& token : m_tokens)
    {
        std::cout << token.Debug() << '\n';
    }
}

const std::list<FToken>& IPreprocessor::GetTokens() const
{
    return m_tokens;
}

std::list<FToken> IPreprocessor::ExtractTokens()
{
	return std::exchange(m_tokens, {});
}

void IPreprocessor::Reset()
{
    m_inputFilePath = "";
    m_outputFilePath = "";
	m_tokens.clear();
}

void IPreprocessor::Fatal(const std::string& message) const
{
    FatalBase(UNotifyFrom::PREPROCESSOR, message);
}

void IPreprocessor::Error(const std::string& message) const
{
    ErrorBase(UNotifyFrom::PREPROCESSOR, message);
}

void IPreprocessor::Warning(const std::string& message) const
{
    WarningBase(UNotifyFrom::PREPROCESSOR, message);
}

void IPreprocessor::Success(const std::string& message) const
{
    SuccessBase(UNotifyFrom::PREPROCESSOR, message);
}
