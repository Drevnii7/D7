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

    enum class URetState : uint8_t
    {
        Fail, // Type valid, but parse fail. Critycal error
        Yes,  // Type valid, parse valid
        No    // Type invalid, parse fail
    };

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
    FASTNode m_rootNode;

    void TryParseAs_Program();

    // Height level

    URetState TryParseAs_Func(int& l_current, std::unique_ptr<FASTNode>& Node);
    URetState TryParseAs_Var(int& l_current, std::unique_ptr<FASTNode>& Node);

    // Medium level

    bool ParseAs_BlockCode(int& l_current, std::unique_ptr<FASTNode>& Node);
    bool ParseAs_BlockArgs(int& l_current, std::unique_ptr<FASTNode>& Node);
    bool ParseAs_BlockMath(int& l_current, std::unique_ptr<FASTNode>& Node);

    // Low level

    bool ParseAs_Name(int& l_current, std::unique_ptr<FASTNode>& Node);
    bool ParseAs_Type(int& l_current, std::unique_ptr<FASTNode>& Node);
    bool ParseAs_Arg(int& l_current, std::unique_ptr<FASTNode>& Node);


    // Raw level.

    // Сверить тип токена
    bool PeekToken(int& l_current, UTokenType TokenType);
    // Сверить тип токена, и если false вызвать Fatal
    bool PeekTokenThrow(int& l_current, UTokenType TokenType, std::string_view failMessage);
    
    // Сверить тип токена, и если m_current++
    bool ConsumeToken(int& l_current, UTokenType TokenType);
    // Сверить тип токена, и если m_current++. Если false вызвать Fatal
    bool ConsumeTokenThrow(int& l_current, UTokenType TokenType, std::string_view failMessage);
};
