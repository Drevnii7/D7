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
#include <vector>
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
    std::unique_ptr <FASTNode > parseProgram();


    [[nodiscard]] bool LoadTokens(const std::string& filePath = " ");
    [[nodiscard]] bool SaveTokens(const std::string& filePath = " ");
    [[nodiscard]] bool SaveAST(const std::string& filePath = " ");

    // Set tokens raw
    void SetTokens(const std::vector <FToken >& tokens);
    void SetTokens(const std::list <FToken >& tokens);
    void SetTokens(std::vector <FToken >&& tokens);
#if HAS_STD_SPAN == 1
    std::span<FToken> GetTokens();
#else
    const std::vector<FToken>& GetTokens();
#endif
    std::vector<FToken> ExtractTokens();
protected: // CParser
    std::vector<FToken> m_tokens;
    int m_current = 0;
    FASTNode m_rootNode;

    const FToken* peek() const
    {
        return isAtEnd() ? nullptr : &m_tokens[m_current];
    }

    const FToken* previous() const
    {
        return m_current == 0 ? nullptr : &m_tokens[m_current - 1];
    }

    const FToken* advance()
    {
        return isAtEnd() ? nullptr : &m_tokens[m_current++];
    }

    bool isAtEnd() const
    {
        return m_current >= m_tokens.size();
    }

    bool check(UTokenType type) const
    {
        return !isAtEnd() && peek()->Type == type;
    }

    bool match(UTokenType type)
    {
        if (check(type))
        {
            advance();
            return true;
        }
        return false;
    }

    const FToken& consume(UTokenType type, const std::string& message);

private:
    std::string GetErrorPos() const;
    bool isTypeKeyword(const FToken& tok) const;
    bool isDeclarationStart() const;

    std::unique_ptr<FASTNode> parseDeclaration();
    std::unique_ptr<FASTNode> parseVarDecl(const FToken& typeTok, const FToken& nameTok);
    std::unique_ptr<FASTNode> parseFuncDecl(const FToken& typeTok, const FToken& nameTok);

    std::unique_ptr<FASTNode> parseStatement();
    std::unique_ptr<FASTNode> parseBlock();
    std::unique_ptr<FASTNode> parseReturnStmt();
    std::unique_ptr<FASTNode> parseIfStmt();
    std::unique_ptr<FASTNode> parseWhileStmt();
    std::unique_ptr<FASTNode> parseForStmt();

    std::unique_ptr<FASTNode> parseExpression();
    std::unique_ptr<FASTNode> parseAssignment();
    std::unique_ptr<FASTNode> parseLogicalOr();
    std::unique_ptr<FASTNode> parseLogicalAnd();
    std::unique_ptr<FASTNode> parseEquality();
    std::unique_ptr<FASTNode> parseComparison();
    std::unique_ptr<FASTNode> parseTerm();
    std::unique_ptr<FASTNode> parseFactor();
    std::unique_ptr<FASTNode> parseUnary();
    std::unique_ptr<FASTNode> parsePrimary();
};