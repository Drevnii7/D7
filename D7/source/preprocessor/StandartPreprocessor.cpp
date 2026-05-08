#include "StandartPreprocessor.h"

void CStandartPreprocessor::RunProcessing()
{
    DirectiveCollectDefine();
    DirectiveApplyDefine();
}

void CStandartPreprocessor::Reset()
{
	m_inputFilePath = "";
	m_outputFilePath = "";
	m_tokens.clear();

	m_defines.clear();
}

int CStandartPreprocessor::FindNextPreprocessorDirective()
{
	size_t index = -1;

	for (const FToken& token : m_tokens)
	{
		index++;

		if (token.Type == UTokenType::PREPROCESSOR_DIRECTIVE)
		{
			return index;
		}
	}

	return -1;
}

void CStandartPreprocessor::DirectiveCollectDefine()
{
    auto it = m_tokens.begin();

    while (it != m_tokens.end())
    {
        if (std::distance(it, m_tokens.end()) < 3)
            break;

        if (it->Type != UTokenType::PREPROCESSOR_DIRECTIVE || it->Lexeme != "#define")
        {
            ++it;
            continue;
        }

        std::string defineName = std::next(it, 1)->Lexeme;
        FToken defineValue = *std::next(it, 2);
        
        if (auto collectDefine = m_defines.find(defineName); collectDefine != m_defines.end() && collectDefine->second.IsValid())
        {
            if (defineValue.Lexeme != collectDefine->second.Lexeme || defineValue.Type != collectDefine->second.Type)
            {
                Fatal("Already determined define: \"" + defineValue.Debug() + "\" defined in \"" + collectDefine->second.Debug());
            }
        }

        m_defines[defineName] = defineValue;

        it = m_tokens.erase(it, std::next(it, 3));
    }
}

void CStandartPreprocessor::DirectiveApplyDefine()
{
    
    for (auto it = m_tokens.begin(); it != m_tokens.end(); ++it)
    {
        if (auto collectDefine = m_defines.find(it->Lexeme); collectDefine != m_defines.end() && collectDefine->second.IsValid())
        {
            it->Lexeme = collectDefine->second.Lexeme;
            it->Type = collectDefine->second.Type;
            it->Modifiers.IsTokenValueChanged = true;
            it->Modifiers.IsTokenTypeChanged = true;
        }
    }
}
