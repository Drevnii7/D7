#include "CLexer.h"

#include "../utils/notify.hpp"

inline constexpr size_t LEXEME_DEFAULT_RESERVE = 128;

using expected = d7::expected;

expected d7::CLexer::Run()
{
    notify_callback("Run");

    assert(!m_code.empty(), "No code set - set code first");
    
    std::string lexeme = "";
    lexeme.reserve(LEXEME_DEFAULT_RESERVE);
    //uint8_t inBlockComment = 0;

    return expected::Fatal("Fatal");
}