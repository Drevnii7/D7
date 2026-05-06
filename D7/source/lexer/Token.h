/*
    The inspiration is "i582"
    His code was taken as a basis, but it was fine-tuned to suit my vision.

    https://github.com/i582/cpp-asm-compiler/blob/master/src/lexer/token/token.h
*/
#pragma once

#include <string>
#include <string_view>

enum class UTokenType
{
    IDENTIFIER,


    INTEGER_CONST,
    DOUBLE_CONST,
    STRING_CONST,
    CHAR_CONST,

    // boolean constant
    TRUE,
    FALSE,




    CONST,


    // types
    UNDEFINED,

    INT,
    DOUBLE,
    BOOL,
    CHAR,
    VOID,
    AUTO,

    // cycles
    DO_WHILE,
    WHILE,
    FOR,
    // cycles addition
    BREAK,
    CONTINUE,


    SWITCH,
    CASE,
    DEFAULT,


    // conditions
    IF,
    ELSE,


    // relationship operators
    LESS,          // <
    GREATER,       // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL, // >=


    // equal operators
    EQUAL,     // ==
    NOT_EQUAL, // !=


    // logical operators
    AND,         // &&
    OR,          // ||
    EXCLAMATION, // !

    // math operators
    PLUS,  // +
    MINUS, // -
    STAR,  // *
    SLASH, // /
    INC,   // ++
    DEC,   // --

    // assign
    ASSIGN,     // =
    ADD_ASSIGN, // +=
    SUB_ASSIGN, // -=
    MUL_ASSIGN, // *=
    DIV_ASSIGN, // /=

    // brackets
    LBRA, // {
    RBRA, // }
    LPAR, // (
    RPAR, // )
    LSQR, // [
    RSQR, // ]


   


    // function
    FUNCTION,
    RETURN,




    // other symbols
    SEMICOLON, // ;
    COLON,     // :
    COMMA,     // ,
    POINT,     // .
    QUESTION,  // ?
    AMPERSAND, // &


    NEW,
    DELETE,


    PREPROCESSOR_DIRECTIVE,


    ACCESS_OPERATOR, // ::
};

struct FToken
{
    std::string Lexeme = "";
    UTokenType Type = UTokenType::UNDEFINED;

    size_t Line = 0;
    size_t Row = 0;

    FToken() = default;
    explicit FToken(std::string lexeme, size_t line, size_t row)
    {
        Lexeme = lexeme;
        Type = internal::StringToTokenType(lexeme);
        Line = line;
        Row = row;
    }

    std::string Debug() const;

    struct internal
    {
        static constexpr bool is_string(std::string_view lexeme);
        static constexpr bool is_char(std::string_view lexeme);
        static constexpr bool is_integer(std::string_view lexeme);
        // | 0.0 | 3.14f | 55.55 | 123. | .45 | 86.f |
        static constexpr bool is_double(std::string_view lexeme);
        static constexpr bool is_preprocessorDirective(std::string_view lexeme);
        static bool is_separator(std::string_view tokenType);
        static bool is_separatorType(UTokenType tokenType);

        static std::string_view TokenTypeToString(UTokenType tokenType);
        static UTokenType StringToTokenType(std::string_view lexeme);
    };  
};
