#include "IPreprocessor.hpp"

#include "../utils/notify.hpp"

using expected = d7::expected;

expected d7::IPreprocessor::LoadTokens(std::string_view FilePath)
{
    notify_callback(("LoadTokens. FilePath: " + colors::UNDERLINE + std::string(FilePath) + colors::RESET).c_str());

    return d7::FToken::Deserialize(m_tokens, std::string(FilePath));
}

expected d7::IPreprocessor::SaveTokens(std::string_view FilePath)
{
    notify_callback(("SaveTokens. FilePath: " + colors::UNDERLINE + std::string(FilePath) + colors::RESET).c_str());
    
    assert(mb_runCall, "You not call Run() - call Run() first. (if you call Run(), check description function Run() in IPreprocessor.hpp)");

    return d7::FToken::Serialize(m_tokens, std::string(FilePath));
}

void d7::IPreprocessor::SetTokens(std::list<FToken>&& Tokens)
{
    notify_callback("SetTokens");

    m_tokens = std::move(Tokens);
}

void d7::IPreprocessor::SetTokens(std::vector<FToken>&& Tokens)
{
    notify_callback("SetTokens (vector)");

    m_tokens.clear();

    if (Tokens.empty())
    {
        notify_trace("SetTokens: empty vector, nothing to move");
        return;
    }

    m_tokens.insert(
        m_tokens.end(),
        std::make_move_iterator(Tokens.begin()),
        std::make_move_iterator(Tokens.end())
    );

    Tokens.clear();
}

std::list<d7::FToken> d7::IPreprocessor::ExtractTokens()
{
    notify_callback("ExtractTokens");

    assert(!m_tokens.empty(), "No tokens set - set tokens first");
    assert(mb_runCall, "You not call Run() - call Run() first. (if you call Run(), check description function Run() in IPreprocessor.hpp)");

    return std::exchange(m_tokens, {});
}

void d7::IPreprocessor::Reset()
{
    notify_callback("Reset");

    m_tokens.clear();
    mb_runCall = false;
}
