#include <iostream>

#include "source/lexer/Token.h"
#include "source/lexer/CLexer.h"

#include "source/preprocessor/IPreprocessor.h"
#include "source/preprocessor/CStandartPreprocessor.h"

#include "source/parser/AST.h"
#include "source/parser/CParser.h"

int main()
{
    std::string File_SourceCode = "test_code.d7";
    std::string File_TokensAfterLexer = "test_code.lexer";
    std::string File_TokensAfterPreprocessor = "test_code.tokens";
    std::string File_ASTAfterParser = "test_code.ast";

    CLexer* Lexer = new CLexer();
    Lexer->LoadCode(File_SourceCode);
    Lexer->RunProcessing();
    Lexer->DebugPrint();
    Lexer->SaveTokens(File_TokensAfterLexer);

    CStandartPreprocessor* Preprocessor = new CStandartPreprocessor();
    Preprocessor->LoadTokens(File_TokensAfterLexer);
    Preprocessor->RunProcessing();
    Preprocessor->DebugPrint();
    Preprocessor->SaveTokensAsCode(File_TokensAfterPreprocessor);

    CParser* Parser = new CParser();
    Parser->LoadTokens(File_TokensAfterLexer);
    Parser->RunProcessing();
    Parser->DebugPrint();
    Parser->SaveAST(File_TokensAfterPreprocessor);
}
