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

	// virtual bool Main(int argc, char* argv[]);

	// Start preprocessing
	virtual void RunProcessing() override;

	virtual bool RunFullCycle() override;

	// std::cout all tokens
	virtual void DebugPrint() const override;

	// Reset state
	virtual void Reset() override;

protected: // CBaseService

	virtual void Fatal(const std::string& message) const override;
	virtual void Error(const std::string& message) const override;
	virtual void Warning(const std::string& message) const override;
	virtual void Success(const std::string& message) const override;

	// std::string m_inputFilePath = "";
	// std::string m_outputFilePath = "";

public: // CPreprocessor

	[[nodiscard]] bool LoadTokens(const std::string& filePath = "");
	[[nodiscard]] bool SaveTokens(const std::string& filePath = "");

	// Set tokens raw
	void SetTokens(const std::vector<FToken>& tokens);
	void SetTokens(const std::list<FToken>& tokens);
	void SetTokens(std::list<FToken>&& tokens);

	

	const std::list<FToken>& GetTokens() const;
	std::list<FToken> ExtractTokens();

protected:

	std::list<FToken> m_tokens;
};

