#pragma once

#include "Token.h"

#include <vector>
#include <span>

class CLexer
{
public:

	CLexer() = default;
	CLexer(int argc, char* argv[]) { args(argc, argv); }

	bool args(int argc, char* argv[]);

	void LoadCode(const std::string& filePath);
	void SaveTokens(const std::string& filePath);

	// Set code raw
	void SetCode(const std::vector<std::string>& code);
	void SetCode(std::vector<std::string>&& code);
	void SetCode(const std::string& code);

	// Start lexical analyse
	void Run();

	// std::cout all tokens
	void DebugPrint() const;

	std::span<FToken> GetTokens();
	std::vector<FToken> ExtractTokens();

	// Reset state
	void Reset();

private:
	std::string m_filePath = "";
	std::vector <std::string> m_code;
	std::vector<FToken> m_tokens;

	size_t currentTokenIndex = 0;
};