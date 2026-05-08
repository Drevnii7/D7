#pragma once

#include "IPreprocessor.h"
#include <map>

class CStandartPreprocessor : public IPreprocessor
{
public: // CBaseService

	virtual void RunProcessing() override;

	virtual void Reset() override;

protected: // IPreprocessor

	// std::list<FToken> m_tokens;

public:

	int FindNextPreprocessorDirective();

	void DirectiveCollectDefine();
	void DirectiveApplyDefine();
protected:

	std::map<std::string, FToken> m_defines;
};

