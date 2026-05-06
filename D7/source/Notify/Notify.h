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


void Error(UNotifyFrom errorType, const std::string& message);

#define LexerError(message) Error(UNotifyFrom::LEXER, message);
#define PreprocessorError(message) Error(UNotifyFrom::PREPROCESSOR, message);
#define ParserError(message) Error(UNotifyFrom::PARSER, message);
#define SemanticError(message) Error(UNotifyFrom::SEMANTIC, message);
#define CodeGenerationError(message) Error(UNotifyFrom::CODE_GENERATION, message);


void Warning(UNotifyFrom warningType, const std::string& message);

#define LexerWarning(message) Warning(UNotifyFrom::LEXER, message);
#define PreprocessorWarning(message) Warning(UNotifyFrom::PREPROCESSOR, message);
#define ParserWarning(message) Warning(UNotifyFrom::PARSER, message);
#define SemanticWarning(message) Warning(UNotifyFrom::SEMANTIC, message);
#define CodeGenerationWarning(message) Warning(UNotifyFrom::CODE_GENERATION, message);



void Succes(UNotifyFrom succesType, const std::string& message);

#define LexerSucces(message) Succes(UNotifyFrom::LEXER, message);
#define PreprocessorSucces(message) Succes(UNotifyFrom::PREPROCESSOR, message);
#define ParserSucces(message) Succes(UNotifyFrom::PARSER, message);
#define SemanticSucces(message) Succes(UNotifyFrom::SEMANTIC, message);
#define CodeGenerationSucces(message) Succes(UNotifyFrom::CODE_GENERATION, message);
