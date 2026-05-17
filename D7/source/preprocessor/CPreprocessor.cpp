#include "CPreprocessor.hpp"

#include "../utils/notify.hpp"

using expected = d7::expected;

d7::CPreprocessor::CPreprocessor()
{
    Register_MacroSubstitutions();
}

expected d7::CPreprocessor::Run()
{
    notify_callback("Run");
    mb_runCall = true;

    auto it = m_tokens.begin();
    while (it != m_tokens.end())
    {
        size_t remaining = std::distance(it, m_tokens.end());
        if (remaining >= 3)
        {
            const bool isOpenMacro = (it->type == ETokenType::MACRO && it->lexeme == "$");
            const bool isOpenMacroS = (it->type == ETokenType::MACRO_S && it->lexeme == "@");

            if (isOpenMacro || isOpenMacroS)
            {
                auto nameIt = std::next(it, 1);
                auto closeIt = std::next(it, 2);

                if (nameIt->type != ETokenType::IDENTIFIER)
                {
                    return expected::Fatal(
                        "[" + std::to_string(nameIt->line) + ":" + std::to_string(nameIt->row) +
                        "] Expected macro name after '" + it->lexeme + "', got '" + nameIt->lexeme + "'"
                    );
                }

                if (closeIt->type != it->type || closeIt->lexeme != it->lexeme)
                {
                    return expected::Fatal(
                        "[" + std::to_string(closeIt->line) + ":" + std::to_string(closeIt->row) +
                        "] Expected '" + it->lexeme + "' to close macro, got '" + closeIt->lexeme + "'"
                    );
                }

                std::string macroName = nameIt->lexeme;
                auto macroIt = m_MacroSubstitutions.find(macroName);
                if (macroIt == m_MacroSubstitutions.end())
                {
                    return expected::Fatal(
                        "[" + std::to_string(nameIt->line) + ":" + std::to_string(nameIt->row) +
                        "] Unknown macro: '" + macroName + "'"
                    );
                }

                std::list<FToken> expanded = macroIt->second(*it, *nameIt, *closeIt);

                if (isOpenMacroS)
                {
                    for (auto& token : expanded)
                    {
                        token.type = ETokenType::STRING;
  
                        if (token.lexeme.empty() || token.lexeme.front() != '"')
                        {
                            token.lexeme = "\"" + token.lexeme + "\"";
                        }
                    }
                }

                {
                    if (expanded.size() == 1)
                    {
                        std::string msg = "[" + std::to_string(it->line) + ":" + std::to_string(it->row) + "] " +
                            it->lexeme + macroName + closeIt->lexeme +
                            " -> " + expanded.front().Dump();
                        notify_trace(msg.c_str());
                    }
                    else
                    {
                        std::string msg = "[" + std::to_string(it->line) + ":" + std::to_string(it->row) + "] " +
                            it->lexeme + macroName + closeIt->lexeme +
                            " -> " + std::to_string(expanded.size()) + " token(s)";
                        notify_trace(msg.c_str());
                    }
                }

                it = m_tokens.erase(it, std::next(it, 3));
                m_tokens.splice(it, std::move(expanded));
                continue;
            }
        }
        ++it;
    }

    return expected::Success();
}

void d7::CPreprocessor::Register_MacroSubstitutions()
{
    m_MacroSubstitutions["POS"] = &CPreprocessor::MS_Pos;
    m_MacroSubstitutions["LINE"] = &CPreprocessor::MS_Line;
    m_MacroSubstitutions["COLUMN"] = &CPreprocessor::MS_Column;
}

std::list<d7::FToken> d7::CPreprocessor::MS_Pos(const FToken& Before, const FToken& Now, const FToken& After)
{
	return std::list<FToken>
	{
		FToken(
			Now.line,
			Now.row,
			std::to_string(Before.line) + ":" + std::to_string(Before.row)
		)
	};
}

std::list<d7::FToken> d7::CPreprocessor::MS_Line(const FToken& Before, const FToken& Now, const FToken& After)
{
	return std::list<FToken>
	{ 
		FToken(
			Now.line, 
			Now.row, 
			std::to_string(Before.line)
		) 
	};
}

std::list<d7::FToken> d7::CPreprocessor::MS_Column(const FToken& Before, const FToken& Now, const FToken& After)
{
	return std::list<FToken>
	{
		FToken(
			Now.line,
			Now.row,
			std::to_string(Before.row)
		)
	};
}
