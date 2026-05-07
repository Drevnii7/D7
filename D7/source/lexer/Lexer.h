#pragma once

#include "../service/BaseService.h"
#include "../BaseTypes.h"
#include "Token.h"

#include <vector>

#if IS_CPP_20 == 1
	#include <span>
	#define HAS_STD_SPAN 1
#else
	#define HAS_STD_SPAN 0
#endif


class CLexer : public IBaseService
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

	#if HAS_STD_SPAN == 1
		std::span<FToken> GetTokens();
	#else
		const std::vector<FToken>& GetTokens();
	#endif
	
	std::vector<FToken> ExtractTokens();

protected:

	std::vector <std::string> m_code;
	std::vector<FToken> m_tokens;
};