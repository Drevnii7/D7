#pragma once

#include "../service/BaseService.h"
#include "../lexer/Token.h"

#include <vector>
#include <span>
#include <forward_list>
#include <list>

class CPreprocessor : public CBaseService
{
public: // CBaseService

	virtual bool Main(int argc, char* argv[]) override;

	// Start preprocessing
	virtual void Run() override;

	// std::cout all tokens
	virtual void DebugPrint() const override;

	// Reset state
	virtual void Reset() override;

public: // CPreprocessor

	void LoadTokens(const std::string& filePath);
	void SaveTokens(const std::string& filePath);

	// Set tokens raw
	void SetTokens(const std::vector<FToken>& tokens);
	void SetTokens(const std::list<FToken>& tokens);
	void SetTokens(std::list<FToken>&& tokens);

	

	const std::list<FToken>& GetTokens() const;
	std::list<FToken> ExtractTokens();

private:
	std::string m_filePath = "";
	std::list<FToken> m_tokens;
};

