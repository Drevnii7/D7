#pragma once

#include "../service/BaseService.h"
#include "Token.h"

#include <vector>
#include <span>

class CLexer : public CBaseService
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

public: // CLexer

	[[nodiscard]] bool LoadCode(const std::string& filePath = "");
	[[nodiscard]] bool SaveTokens(const std::string& filePath = "");

	// Set code raw
	void SetCode(const std::vector<std::string>& code);
	void SetCode(std::vector<std::string>&& code);
	void SetCode(const std::string& code);

	std::span<FToken> GetTokens();
	std::vector<FToken> ExtractTokens();

protected:

	std::vector <std::string> m_code;
	std::vector<FToken> m_tokens;
};