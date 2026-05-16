#include "CLexer.h"

#include "../utils/notify.hpp"

#include <fstream>

using expected = d7::expected;

expected d7::CLexer::LoadCode(std::string_view FilePath)
{
    notify_callback("LoadCode");

    std::ifstream file(std::string(FilePath), std::ios::in | std::ios::binary);

    if (!file.is_open()) 
    {
        return ::expected::Fatal("Failed to open the file");
    }
    
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();

    file.seekg(0, std::ios::beg);

    m_code.resize(size);

    if (!file.read(&m_code[0], size)) 
    {
        return ::expected::Fatal("Error reading the file");
    }

    if (m_code.empty())
    {
        return ::expected::Fatal("File succes read, byt his empty");
    }

    return ::expected::Success();
}

expected d7::CLexer::SaveTokens(std::string_view FilePath)
{
    notify_callback("SaveTokens");

    assert(!m_tokens.empty(), "No tokens generated - call Run() first");

	return d7::FToken::Deserialize(m_tokens, std::string(FilePath));
}

void d7::CLexer::SetCode(std::string&& Code)
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

    m_code = "";
	m_tokens.clear();
}
