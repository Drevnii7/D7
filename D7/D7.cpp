#include <iostream>

#include "source/lexer/Token.h"
#include "source/lexer/CLexer.h"

#include "source/preprocessor/IPreprocessor.h"
#include "source/preprocessor/CStandartPreprocessor.h"

#include "source/parser/AST.h"
#include "source/parser/CParser.h"

#include "source/codegenerator/CodeGenerator.h"

int main()
{
    std::string File_SourceCode = "test_code.d7";

    std::string File_AfterLexer = "test_code.lexer";
    std::string File_AfterPreprocessor = "test_code.tokens";
    std::string File_AfterParser = "test_code.ast";

    std::string File_Out = "test_code.exe";

    CLexer Lexer;
    Lexer.LoadCode(File_SourceCode);
    Lexer.RunProcessing();
    Lexer.DebugPrint();
    Lexer.SaveTokens(File_AfterLexer);

    CStandartPreprocessor Preprocessor;
    Preprocessor.LoadTokens(File_AfterLexer);
    Preprocessor.RunProcessing();
    Preprocessor.DebugPrint();
    Preprocessor.SaveTokensAsCode(File_AfterPreprocessor);

    CParser Parser;
    Parser.LoadTokens(File_AfterPreprocessor);
    Parser.RunProcessing();
    Parser.DebugPrint();
    Parser.SaveAST(File_AfterParser);

    /*
    ICodeGenerator CodeGenerator;
    CodeGenerator.LoadAST(File_AfterParser);
    CodeGenerator.RunProcessing();
    CodeGenerator.DebugPrint();
    CodeGenerator.SaveProgram(File_Out);
    */
}
