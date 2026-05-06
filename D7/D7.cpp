#include <iostream>
#include "source/lexer/Token.h"
#include "source/lexer/Lexer.h"
#include "source/preprocessor/Preprocessor.h"

int main()
{
    std::string File_SourceCode = "test_code.d7";
    std::string File_TokensAfterLexer = "test_code.tokens";

    CLexer* Lexer = new CLexer();
    Lexer->LoadCode(File_SourceCode);
    Lexer->Run();
    Lexer->DebugPrint();
    Lexer->SaveTokens(File_TokensAfterLexer);

    CPreprocessor* Preprocessor = new CPreprocessor();
    Preprocessor->LoadTokens(File_TokensAfterLexer);
    Preprocessor->Run();
    Preprocessor->DebugPrint();
    Preprocessor->SaveTokens(File_TokensAfterLexer);
}
