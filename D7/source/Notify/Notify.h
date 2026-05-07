#pragma once
#include <string>

enum class UNotifyFrom
{
    LEXER,
    PREPROCESSOR,
    PARSER,
    SEMANTIC,
    CODE_GENERATION
};

void FatalBase(UNotifyFrom fatalType, const std::string& message);

//#define ParserFatal(message) FatalBase(UNotifyFrom::PARSER, message)
//#define SemanticFatal(message) FatalBase(UNotifyFrom::SEMANTIC, message)
//#define CodeGenerationFatal(message) FatalBase(UNotifyFrom::CODE_GENERATION, message)


void ErrorBase(UNotifyFrom errorType, const std::string& message);

//#define ParserError(message) ErrorBase(UNotifyFrom::PARSER, message)
//#define SemanticError(message) ErrorBase(UNotifyFrom::SEMANTIC, message)
//#define CodeGenerationError(message) ErrorBase(UNotifyFrom::CODE_GENERATION, message)


void WarningBase(UNotifyFrom warningType, const std::string& message);

//#define ParserWarning(message) WarningBase(UNotifyFrom::PARSER, message)
//#define SemanticWarning(message) WarningBase(UNotifyFrom::SEMANTIC, message)
//#define CodeGenerationWarning(message) WarningBase(UNotifyFrom::CODE_GENERATION, message)



void SuccessBase(UNotifyFrom succesType, const std::string& message);

//#define ParserSuccess(message) SuccessBase(UNotifyFrom::PARSER, message)
//#define SemanticSuccess(message) SuccessBase(UNotifyFrom::SEMANTIC, message)
//#define CodeGenerationSuccess(message) SuccessBase(UNotifyFrom::CODE_GENERATION, message)
