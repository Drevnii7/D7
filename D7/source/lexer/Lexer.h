#pragma once

#include "../service/BaseService.h"
#include "Token.h"

#include <vector>
#include <span>

class CLexer : public CBaseService
{
public: // CBaseService

	virtual bool Main(int argc, char* argv[]) override;

	// Start preprocessing
	virtual void Run() override;

	// std::cout all tokens
	virtual void DebugPrint() const override;

	// Reset state
	virtual void Reset() override;

public: // CLexer

	void LoadCode(const std::string& filePath);
	void SaveTokens(const std::string& filePath);

	// Set code raw
	void SetCode(const std::vector<std::string>& code);
	void SetCode(std::vector<std::string>&& code);
	void SetCode(const std::string& code);

	std::span<FToken> GetTokens();
	std::vector<FToken> ExtractTokens();

private:
	std::string m_filePath = "";
	std::vector <std::string> m_code;
	std::vector<FToken> m_tokens;
};