#pragma once
#include <string>

namespace d7
{
    enum ETokenType
    {
        NONE = 0, // invalid, none

        IDENTIFIER = 1, // func name, var name, var type
        NAMESPACE  = 2, // ::
        DIRECTIVE  = 3, // #  call preprocessor directive                #include
        MACRO      = 4, // __ return preprocessor property how tokens    __LINE__
        MACRO_S    = 5, // ## return preprocessor property as string     ##LINE##

        // const 
        // 100 - 199
        INT     = 100, // 12345
        FLOAT   = 101, // 3.14f
        STRING  = 102, // "Hello world"
        CHAR    = 103, // ' '
        BOOLEAN = 104, // true, false

        // math
        // 200 - 219
        ADD = 200, // +
        SUB = 201, // -
        MUL = 202, // *
        DIV = 203, // /
        MOD = 204, // %
        POW = 205, // **
        INC = 206, // ++
        DEC = 207, // --

        // logical
        // 220 - 239
        AND = 220, // &&
        OR  = 221, // ||
        NOT = 222, // !

        // equal operators
        // 240 - 259
        EQUAL         = 240, // ==
        NOT_EQUAL     = 241, // !=
        LESS          = 242, // <
        GREATER       = 243, // >
        LESS_EQUAL    = 244, // <=
        GREATER_EQUAL = 245, // >=

        // assign
        // 260 - 278
        ASSIGN     = 260, // =
        ASSIGN_ADD = 261, // +=
        ASSIGN_SUB = 262, // -=
        ASSIGN_MUL = 263, // *=
        ASSIGN_DIV = 264, // /=
        ASSIGN_MOD = 265, // %=

        // symbols
        // 300 - 319
        SEMICOLON = 300, // ;
        COLON     = 301, // :
        COMMA     = 302, // ,
        POINT     = 303, // .
        QUESTION  = 304, // ?
        AMPERSAND = 305, // &

        // brackets
        // 320 - 339
        LBRA = 320, // {
        RBRA = 321, // }
        LPAR = 322, // (
        RPAR = 323, // )
        LSQR = 324, // [
        RSQR = 325, // ]

        // base func
        // 400 - 499
        GOTO   = 400,
        NEW    = 401,
        DELETE = 402,

        // control flow
        // 500 - 599
        RETURN   = 500,
        IF       = 501,
        ELSE     = 502,
        ELIF     = 503,
        DO_WHILE = 504,
        WHILE    = 505,
        FOR      = 506,
        BREAK    = 507,
        CONTINUE = 508,
        SWITCH   = 509,
        CASE     = 510,
        DEFAULT  = 511,

        // define 
        // 600 - 699
        DEF_FUNC   = 600,
        DEF_VAR    = 601,
        DEF_ENUM   = 602,
        DEF_STRUCT = 603,

        // define struct
        // 700 - 799
        // DEF_CONSTRUCTOR = 700,
        // DEF_DESTRUCTOR  = 701,
        // DEF_CAST        = 702,

        // define struct
        // 800 - 899
        // DEF_CONSTRUCTOR = 800,
        // DEF_DESTRUCTOR  = 801,
        // DEF_CAST        = 202,

        // agrements func
        // 900 - 999
        // AGREMENT_FUNC_ = 900,
        // AGREMENT_FUNC_ = 901,
        // AGREMENT_FUNC_ = 902,

        // agrements var
        // 900 - 999
        // AGREMENT_VAR_ = 1000,
        // AGREMENT_VAR_ = 1001,
        // AGREMENT_VAR_ = 1002,
    };
    
    namespace ETokenTypeUtils
    {
        std::string_view ETokenTypeToString(ETokenType Type);
        ETokenType ETokenTypeFromString(std::string_view String);

        bool IsSeparator(ETokenType TokenType);

        bool IsConst(ETokenType TokenType);
        bool IsMath(ETokenType TokenType);
        bool IsLogical(ETokenType TokenType);
        bool IsEqual(ETokenType TokenType);
        bool IsAssign(ETokenType TokenType);

        bool IsBoolean(std::string_view Lexeme);
        bool IsInteger(std::string_view Lexeme);
        bool IsFloat(std::string_view Lexeme);
        bool IsString(std::string_view Lexeme);
        bool IsChar(std::string_view Lexeme);

        // true | false
        bool TryToBoolean(std::string_view Lexeme, bool& Out);
        // 42| 0d42 | 0x2A | 0b00101010
        bool TryToInteger(std::string_view Lexeme, int& Out);
        // 3.14f | 3.14
        bool TryToFloat(std::string_view Lexeme, float& Out);
        // "Hello world!"
        bool TryToString(std::string_view Lexeme, std::string& Out);
        // '!'
        bool TryToChar(std::string_view Lexeme, char& Out);
    }
}

