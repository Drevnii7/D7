#include "Notify.h"
#include <iostream>


void Error(UNotifyFrom errorType, const std::string& message)
{
    std::string errorMessage;

    switch (errorType)
    {
        case UNotifyFrom::LEXER:
        {
            errorMessage = "Lexical error! ";
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

     throw std::logic_error(errorMessage);
}

void Warning(UNotifyFrom warningType, const std::string& message)
{
    std::string warningMessage;

    switch (warningType)
    {
        case UNotifyFrom::LEXER:
        {
            warningMessage = "Lexical warning. ";
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

void Succes(UNotifyFrom succesType, const std::string& message)
{
    std::string succesMessage;

    switch (succesType)
    {
        case UNotifyFrom::LEXER:
        {
            succesMessage = "Lexical succes. ";
            break;
        }
        case UNotifyFrom::PARSER:
        {
            succesMessage = "Syntax succes. ";
            break;
        }
        case UNotifyFrom::SEMANTIC:
        {
            succesMessage = "Semantic succes. ";
            break;
        }
        case UNotifyFrom::CODE_GENERATION:
        {
            succesMessage = "Code generation succes. ";
            break;
        }
    }

    succesMessage += message + '\n';

    std::cout << succesMessage;
}
