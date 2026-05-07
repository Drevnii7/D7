#pragma once

#include "../lexer/Token.h"

#include <vector>
#include <span>
#include <forward_list>
#include <list>

class CPreprocessor
{
public:

	CPreprocessor() = default;
	CPreprocessor(int argc, char* argv[]) { Main(argc, argv); }
	CPreprocessor(const std::vector<std::string>& args) { Main(args); }

	virtual bool Main(int argc, char* argv[]);
	virtual bool Main(const std::vector<std::string>& args);

	void LoadTokens(const std::string& filePath);
	void SaveTokens(const std::string& filePath);

	// Set tokens raw
	void SetTokens(const std::vector<FToken>& tokens);
	void SetTokens(const std::list<FToken>& tokens);
	void SetTokens(std::list<FToken>&& tokens);

	// Start preprocessing
	virtual void Run();

	// std::cout all tokens
	virtual void DebugPrint() const;

	const std::list<FToken>& GetTokens() const;
	std::list<FToken> ExtractTokens();

	// Reset state
	void Reset();

private:
	std::string m_filePath = "";
	std::list<FToken> m_tokens;
};

