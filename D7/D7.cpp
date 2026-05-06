#include <iostream>
#include "source/lexer/Token.h"
#include "source/lexer/Lexer.h"

int main()
{
    CLexer* Lexer = new CLexer();

    Lexer->OpenFile("test_code.d7");
    Lexer->Run();
    Lexer->DebugPrint();
}
