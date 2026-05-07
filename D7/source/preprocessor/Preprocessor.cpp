#include "Preprocessor.h"
#include "../Notify/Notify.h"
#include <fstream>
#include <iostream>

bool CPreprocessor::LoadTokens(const std::string& filePath)
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

bool CPreprocessor::SaveTokens(const std::string& filePath)
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

void CPreprocessor::SetTokens(const std::vector<FToken>& tokens)
{
    m_inputFilePath = "CPreprocessor::SetTokens()";
    m_tokens = { tokens.begin(), tokens.end() };
}

void CPreprocessor::SetTokens(const std::list<FToken>& tokens)
{
    m_inputFilePath = "CPreprocessor::SetTokens()";
    m_tokens = tokens;
}

void CPreprocessor::SetTokens(std::list<FToken>&& tokens)
{
    m_inputFilePath = "CPreprocessor::SetTokens()";
    m_tokens = std::move(tokens);
}

void CPreprocessor::RunProcessing()
{
    // Placeholder
}

bool CPreprocessor::RunFullCycle()
{
    if (!LoadTokens())
        return false;
    
    RunProcessing();

    if (!SaveTokens())
        return false;

    return true;
}

void CPreprocessor::DebugPrint() const
{
    Warning("DebugPrint()");
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
    m_inputFilePath = "";
    m_outputFilePath = "";
	m_tokens.clear();
}

void CPreprocessor::Fatal(const std::string& message) const
{
    FatalBase(UNotifyFrom::PREPROCESSOR, message);
}

void CPreprocessor::Error(const std::string& message) const
{
    ErrorBase(UNotifyFrom::PREPROCESSOR, message);
}

void CPreprocessor::Warning(const std::string& message) const
{
    WarningBase(UNotifyFrom::PREPROCESSOR, message);
}

void CPreprocessor::Success(const std::string& message) const
{
    SuccessBase(UNotifyFrom::PREPROCESSOR, message);
}
