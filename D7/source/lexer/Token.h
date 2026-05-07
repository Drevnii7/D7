/*
    The inspiration is "i582"
    His code was taken as a basis, but it was fine-tuned to suit my vision.

    https://github.com/i582/cpp-asm-compiler/blob/master/src/lexer/token/token.h
*/
#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

enum class UTokenType : uint8_t
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

    int32_t Line = 0;
    int32_t Row = 0;

    FToken() = default;
    explicit FToken(std::string lexeme, int32_t line, int32_t row)
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

    // call throw std::runtime_error on fail
    template <typename Container>
    static void Serialize(const Container& tokens, const std::string& filename)
    {
        std::ofstream file(filename, std::ios::binary | std::ios::trunc);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }

        size_t count = tokens.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& token : tokens)
        {
            file << token;
            if (!file.good())
            {
                throw std::runtime_error("Error writing token to file: " + filename);
            }
        }

        file.close();
    }

    

#if IS_CPP_20 == 0
    template<typename T, typename = void>
    struct has_reserve : std::false_type {};

    template<typename T>
    struct has_reserve<T, std::void_t<decltype(std::declval<T>().reserve(std::declval<size_t>()))>>
        : std::true_type {
    };
#endif

    // call throw std::runtime_error on fail
    template <typename Container>
    static Container Deserialize(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file for reading: " + filename);
        }

        size_t count = 0;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

        if (!file.good())
        {
            throw std::runtime_error("Error reading count from file: " + filename);
        }

        Container tokens;

    #if IS_CPP_20 == 1
        if constexpr (requires { tokens.reserve(count); }) 
        {
            tokens.reserve(count);
        }
    #else
        // Check container contains function reserve()
        // If false, not call
        if constexpr (has_reserve<Container>::value)
        {
            tokens.reserve(count);
        }
    #endif

        for (size_t i = 0; i < count; ++i)
        {
            FToken token;
            file >> token;

            if (!file.good())
            {
                throw std::runtime_error("Error reading token at index " + std::to_string(i) + " from file: " + filename);
            }

            tokens.emplace_back(std::move(token));
        }

        file.close();
        return tokens;
    }
    

    friend std::ostream& operator<<(std::ostream& os, const FToken& token);
    friend std::istream& operator>>(std::istream& is, FToken& token);
};
