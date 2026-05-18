#include "CParser.hpp"

#include "../utils/notify.hpp"

using expected = d7::expected;

expected d7::CParser::LoadTokens(std::string_view FilePath)
{
    notify_callback(("LoadTokens. FilePath: " + colors::UNDERLINE + std::string(FilePath) + colors::RESET).c_str());

    return d7::FToken::Deserialize(m_tokens, std::string(FilePath));
}

expected d7::CParser::SaveAST(std::string_view FilePath)
{
    notify_callback(("SaveAST. FilePath: " + colors::UNDERLINE + std::string(FilePath) + colors::RESET).c_str());
    
    assert(m_rootNode.IsValid(), "No node generated - call Run() first");

    return d7::FASTNode::Serialize(m_rootNode, std::string(FilePath));
}

void d7::CParser::SetTokens(std::list<d7::FToken>&& Tokens)
{
    notify_callback("SetTokens");

    m_tokens = std::vector<d7::FToken>(
        std::make_move_iterator(Tokens.begin()),
        std::make_move_iterator(Tokens.end())
    );
}

void d7::CParser::SetTokens(std::vector<d7::FToken>&& Tokens)
{
    notify_callback("SetTokens");

    m_tokens = std::move(Tokens);
}

void d7::CParser::Reset()
{
    notify_callback("Reset");

    m_tokens.clear();
    m_rootNode = FASTNode();
    m_current = 0;
}

const d7::FToken& d7::CParser::PeekToken()
{
    assert(m_current < m_tokens.size(), "Out of range m_tokens");

    return m_tokens[m_current];
}

const d7::FToken& d7::CParser::ConsumeToken()
{
    assert(m_current < m_tokens.size(), "Out of range m_tokens");

    return m_tokens[m_current++];
}

size_t d7::CParser::LastTokens()
{
    return m_tokens.size() - m_current;
}

bool d7::CParser::SkipToken()
{
    return ++m_current < m_tokens.size();
}
