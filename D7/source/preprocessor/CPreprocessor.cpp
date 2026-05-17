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

    while (IsContainsMacro())
    {
        auto it = m_tokens.begin();
        while (it != m_tokens.end())
        {
            if (TryParseDefineDirective(it))
            {
                continue;
            }

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
                    std::list<FToken> expanded;

                    auto builtinIt = m_MacroSubstitutions.find(macroName);
                    if (builtinIt != m_MacroSubstitutions.end())
                    {
                        expanded = builtinIt->second(*it, *nameIt, *closeIt);
                    }
                    else if (auto userIt = m_UserMacros.find(macroName); userIt != m_UserMacros.end())
                    {
                        expanded = userIt->second;
                    }
                    else
                    {
                        return expected::Fatal(
                            "[" + std::to_string(nameIt->line) + ":" + std::to_string(nameIt->row) +
                            "] Unknown macro: '" + macroName + "'"
                        );
                    }

                    if (isOpenMacroS)
                    {
                        std::string concatenated;
                        for (const auto& token : expanded)
                        {
                            concatenated += token.lexeme;
                        }

                        expanded.clear();
                        FToken stringToken(nameIt->line, nameIt->row, "\"" + concatenated + "\"");
                        stringToken.type = ETokenType::STRING;
                        expanded.push_back(std::move(stringToken));
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
    }

    if (true)
    {
        notify_trace("Dump tokens:");
        for (const FToken& token : m_tokens)
        {
            notify_trace(token.Dump().c_str());
        }
    }

    return expected::Success();
}

bool d7::CPreprocessor::IsContainsMacro()
{
    for (const FToken& Token : m_tokens)
    {
        if (Token.type == ETokenType::DIRECTIVE || Token.type == ETokenType::MACRO || Token.type == ETokenType::MACRO_S)
        {
            return true;
        }
    }
    return false;
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

bool d7::CPreprocessor::TryParseDefineDirective(std::list<FToken>::iterator& it)
{
    if (it == m_tokens.end())
        return false;

    auto dist = std::distance(it, m_tokens.end());
    if (dist < 4)
        return false;

    auto hashIt = it;
    auto usingIt = std::next(it);
    if (usingIt == m_tokens.end()) return false;

    auto nameIt = std::next(usingIt);
    if (nameIt == m_tokens.end()) return false;

    auto valueIt = std::next(nameIt);
    if (valueIt == m_tokens.end()) return false;

    try
    {

        if (hashIt->type != ETokenType::DIRECTIVE || hashIt->lexeme != "#")
            return false;

        if (usingIt->type != ETokenType::IDENTIFIER ||
            (usingIt->lexeme != "define"))
            return false;

        if (nameIt->type != ETokenType::IDENTIFIER)
        {
            notify_warning(("Expected macro name after '#define', got '" + nameIt->lexeme + "'").c_str());
            return false;
        }

        std::list<FToken> valueTokens;
        auto current = valueIt;
        size_t targetLine = nameIt->line;

        while (current != m_tokens.end() && current->line == targetLine)
        {
            valueTokens.push_back(*current);
            current = std::next(current);
        }

        if (valueTokens.empty())
        {
            notify_warning(("Macro '" + nameIt->lexeme + "' has no value").c_str());
            return false;
        }

        std::string macroName = nameIt->lexeme;
        m_UserMacros[macroName] = std::move(valueTokens);

        auto endIt = current;
        it = m_tokens.erase(hashIt, endIt);

        notify_callback(("Registered macro: " + macroName).c_str());
        return true;
    }
    catch (const std::exception& e)
    {
        notify_warning(("Exception in TryParseDefineDirective: " + std::string(e.what())).c_str());
        return false;
    }
}

void d7::CPreprocessor::RegisterDefineMacro(const std::string& name, std::list<FToken>&& tokens)
{
    m_UserMacros[name] = std::move(tokens);
}
