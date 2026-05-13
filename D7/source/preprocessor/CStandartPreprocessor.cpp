#include "CStandartPreprocessor.h"

void CStandartPreprocessor::RunProcessing()
{
    DirectiveCollectUsing();
    DirectiveApplyUsing();
}

void CStandartPreprocessor::Reset()
{
	m_inputFilePath = "";
	m_outputFilePath = "";
	m_tokens.clear();

	m_usings.clear();
}

int CStandartPreprocessor::FindNextPreprocessorDirective()
{
    int index = -1;

	for (const FToken& token : m_tokens)
	{
		index++;

		if (token.Type == UTokenType::DIRECTIVE)
		{
			return index;
		}
	}

	return -1;
}

void CStandartPreprocessor::DirectiveCollectUsing()
{
    auto it = m_tokens.begin();

    while (it != m_tokens.end())
    {
        if (std::distance(it, m_tokens.end()) < 4)
            break;

        if (it->Lexeme != "#" || std::next(it, 1)->Lexeme != "using")
        {
            ++it;
            continue;
        }

        std::string usingName = std::next(it, 2)->Lexeme;
        FToken usingValue = *std::next(it, 3);

        if (auto collectUsing = m_usings.find(usingName);
            collectUsing != m_usings.end() && collectUsing->second.IsValid())
        {
            if (usingValue.Lexeme != collectUsing->second.Lexeme ||
                usingValue.Type != collectUsing->second.Type)
            {
                Fatal("Already determined USING: \"" + usingValue.Dump() +
                    "\" USING in \"" + collectUsing->second.Dump());
            } 
        }

        m_usings[usingName] = usingValue;

        it = m_tokens.erase(it, std::next(it, 4));
    }
}

void CStandartPreprocessor::DirectiveApplyUsing()
{
    for (auto it = m_tokens.begin(); it != m_tokens.end(); ++it)
    {
        if (auto collectUsing = m_usings.find(it->Lexeme);
            collectUsing != m_usings.end() && collectUsing->second.IsValid())
        {
            it->Lexeme = collectUsing->second.Lexeme;
            it->Type = collectUsing->second.Type;
            it->Modifiers.IsTokenValueChanged = true;
            it->Modifiers.IsTokenTypeChanged = true;
        }
    }
}