#include "CLexer.hpp"

#include "../utils/notify.hpp"

#include <fstream>

using expected = d7::expected;

expected d7::CLexer::LoadCode(std::string_view FilePath)
{
    notify_callback(("LoadCode. FilePath: " + colors::UNDERLINE + std::string(FilePath) + colors::RESET).c_str());

    std::ifstream file(std::string(FilePath), std::ios::binary);

    if (!file.is_open()) 
    {
        return ::expected::Fatal("Failed to open the file");
    }
    
    notify_trace("Dump lines:");

    std::string line;
    while (std::getline(file, line))
    {
        m_code.push_back(line);

        notify_trace(line.c_str());
    }

    if (m_code.empty())
    {
        return ::expected::Fatal("File succes read, byt his empty");
    }

    return ::expected::Success();
}

expected d7::CLexer::SaveTokens(std::string_view FilePath)
{
    notify_callback(("SaveTokens. FilePath: " + colors::UNDERLINE + std::string(FilePath) + colors::RESET).c_str());

    assert(!m_tokens.empty(), "No tokens generated - call Run() first");

    return d7::FToken::Serialize(m_tokens, std::string(FilePath));
}

void d7::CLexer::SetCode(std::vector<std::string>&& Code)
{
    notify_callback("SetCode");

    m_code = std::move(Code);
    m_tokens.clear();
}

std::vector<d7::FToken> d7::CLexer::ExtractTokens()
{
    notify_callback("ExtractTokens");

    assert(!m_code.empty(), "No code set - set code first");
    assert(!m_tokens.empty(), "No tokens generated - call Run() first");

    return std::exchange(m_tokens, {});
}

void d7::CLexer::Reset()
{
    notify_callback("Reset");

    m_code.clear();
	m_tokens.clear();
}
