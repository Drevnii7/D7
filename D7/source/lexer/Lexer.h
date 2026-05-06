#pragma once

#include "../Notify/Notify.h"
#include "Token.h"

#include <vector>
#include <span>

class CLexer
{
public:

	CLexer() = default;

	// Read file and set code
	void OpenFile(const std::string& filePath);

	// Set code raw
	void SetCode(const std::vector<std::string>& code);
	void SetCode(const std::string& code);

	// Start lexical analyse
	void Run();

	// std::cout all m_tokens
	void DebugPrint();

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