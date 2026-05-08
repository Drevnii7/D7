#pragma once

/*
	Representing Code 5
	https://craftinginterpreters.com/representing-code.html
	Parsing Expressions 6
	https://craftinginterpreters.com/parsing-expressions.html
	// Создание языка программирования с использованием LLVM. Часть 2: Реализация парсера и AST
	https://habr.com/ru/articles/120005/
*/

#include "../service/IBaseService.h"
#include "../BaseTypes.h"
#include "../lexer/Token.h"
#include "AST.h"

#include <list>

#if IS_CPP_20 == 1
	#include <span>
	#define HAS_STD_SPAN 1
#else
	#define HAS_STD_SPAN 0
#endif

class CParser : public IBaseService
{
public: // CBaseService

	// virtual bool Main(int argc, char* argv[]);

	virtual void RunProcessing() override;

	virtual bool RunFullCycle() override;

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

public: // CParser

	[[nodiscard]] bool LoadTokens(const std::string& filePath = "");
	[[nodiscard]] bool SaveTokens(const std::string& filePath = "");
	[[nodiscard]] bool SaveAST(const std::string& filePath = "");

	// Set tokens raw
	void SetTokens(const std::vector<FToken>& tokens);
	void SetTokens(const std::list<FToken>& tokens);
	void SetTokens(std::vector<FToken>&& tokens);



#if HAS_STD_SPAN == 1
	std::span<FToken> GetTokens();
#else
	const std::vector<FToken>& GetTokens();
#endif
	std::vector<FToken> ExtractTokens();

protected: // CParser

	std::vector<FToken> m_tokens;
	FASTNode m_rootNode;
};
