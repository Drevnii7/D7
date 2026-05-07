#include "Notify.h"
#include <iostream>


void FatalBase(UNotifyFrom fatalType, const std::string& message)
{
    std::string fatalMessage;

    switch (fatalType)
    {
    case UNotifyFrom::LEXER:
    {
        fatalMessage = "Lexical FATAL! ";
        break;
    }
    case UNotifyFrom::PREPROCESSOR:
    {
        fatalMessage = "Preprocessing FATAL! ";
        break;
    }
    case UNotifyFrom::PARSER:
    {
        fatalMessage = "Syntax FATAL! ";
        break;
    }
    case UNotifyFrom::SEMANTIC:
    {
        fatalMessage = "Semantic FATAL! ";
        break;
    }
    case UNotifyFrom::CODE_GENERATION:
    {
        fatalMessage = "Code generation FATAL! ";
        break;
    }
    }

    fatalMessage += message + '\n';

    std::cout << fatalMessage;

    throw std::logic_error(fatalMessage);
}

void ErrorBase(UNotifyFrom errorType, const std::string& message)
{
    std::string errorMessage;

    switch (errorType)
    {
        case UNotifyFrom::LEXER:
        {
            errorMessage = "Lexical error! ";
            break;
        }
        case UNotifyFrom::PREPROCESSOR:
        {
            errorMessage = "Preprocessing error! ";
            break;
        }
        case UNotifyFrom::PARSER:
        {
            errorMessage = "Syntax error! ";
            break;
        }
        case UNotifyFrom::SEMANTIC:
        {
            errorMessage = "Semantic error! ";
            break;
        }
        case UNotifyFrom::CODE_GENERATION:
        {
            errorMessage = "Code generation error! ";
            break;
        }
    }

    errorMessage += message + '\n';

    std::cout << errorMessage;
}

void WarningBase(UNotifyFrom warningType, const std::string& message)
{
    std::string warningMessage;

    switch (warningType)
    {
        case UNotifyFrom::LEXER:
        {
            warningMessage = "Lexical warning. ";
            break;
        }
        case UNotifyFrom::PREPROCESSOR:
        {
            warningMessage = "Preprocessing warning. ";
            break;
        }
        case UNotifyFrom::PARSER:
        {
            warningMessage = "Syntax warning. ";
            break;
        }
        case UNotifyFrom::SEMANTIC:
        {
            warningMessage = "Semantic warning. ";
            break;
        }
        case UNotifyFrom::CODE_GENERATION:
        {
            warningMessage = "Code generation warning. ";
            break;
        }
    }

    warningMessage += message + '\n';

    std::cout << warningMessage;
}

void SuccessBase(UNotifyFrom succesType, const std::string& message)
{
    std::string successMessage;

    switch (succesType)
    {
        case UNotifyFrom::LEXER:
        {
            successMessage = "Lexical success. ";
            break;
        }
        case UNotifyFrom::PREPROCESSOR:
        {
            successMessage = "Preprocessing success. ";
            break;
        }
        case UNotifyFrom::PARSER:
        {
            successMessage = "Syntax success. ";
            break;
        }
        case UNotifyFrom::SEMANTIC:
        {
            successMessage = "Semantic success. ";
            break;
        }
        case UNotifyFrom::CODE_GENERATION:
        {
            successMessage = "Code generation success. ";
            break;
        }
    }

    successMessage += message + '\n';

    std::cout << successMessage;
}
