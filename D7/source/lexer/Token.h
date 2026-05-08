/*
    The inspiration is "i582"
    His code was taken as a basis, but it was fine-tuned to suit my vision.

    https://github.com/i582/cpp-asm-compiler/blob/master/src/lexer/token/token.h
*/
#pragma once

#include "../BaseTypes.h"

#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

enum UTokenType
{
    NONE       = 0, // invalid, none
    
    IDENTIFIER = 1, // func name, var name, var type
    NAMESPACE  = 2, // ::
    DIRECTIVE  = 3, // #  call preprocessor directive                #include
    MACRO      = 4, // __ return preprocessor property how tokens    __LINE__
    MACRO_S    = 5, // ## return preprocessor property as string     ##LINE##

    // const 
    // 100 - 199
    GROUP_CONST = 100,
    INT     = 101, // 12345
    FLOAT   = 102, // 3.14f
    STRING  = 103, // "Hello world"
    CHAR    = 104, // ' '
    BOOLEAN = 105, // true, false

    // math
    // 200 - 219
    GROUP_MATH = 200,
    ADD = 201, // +
    SUB = 202, // -
    MUL = 203, // *
    DIV = 204, // /
    MOD = 205, // %
    POW = 206, // **
    INC = 207, // ++
    DEC = 208, // --

    // logical
    // 220 - 239
    GROUP_LOGICAL = 220,
    AND = 221, // &&
    OR  = 222, // ||
    NOT = 223, // !

    // equal operators
    // 240 - 259
    GROUP_EQUAL = 240,
    EQUAL         = 241, // ==
    NOT_EQUAL     = 242, // !=
    LESS          = 243, // <
    GREATER       = 244, // >
    LESS_EQUAL    = 245, // <=
    GREATER_EQUAL = 246, // >=

    // assign
    // 260 - 278
    GROUP_ASSIGN = 260,
    ASSIGN     = 261, // =
    ASSIGN_ADD = 262, // +=
    ASSIGN_SUB = 263, // -=
    ASSIGN_MUL = 264, // *=
    ASSIGN_DIV = 265, // /=
    ASSIGN_MOD = 266, // %=





    // symbols
    // 300 - 319
    GROUP_SYMBOLS = 300,
    SEMICOLON = 301, // ;
    COMMA     = 303, // ,
    POINT     = 304, // .
    QUESTION  = 305, // ?
    AMPERSAND = 306, // &


    // brackets
    // 320 - 339
    GROUP_BRACKETS = 320,
    LBRA = 321, // {
    RBRA = 322, // }
    LPAR = 323, // (
    RPAR = 324, // )
    LSQR = 325, // [
    RSQR = 326, // ]

    // base func
    // 400 - 499
    GROUP_FUNC = 400,
    LABEL  = 401, // ":"
    GOTO   = 402,
    NEW    = 403,
    DELETE = 404,

    // control flow
    // 500 - 599
    GROUP_CONTROL = 500,
    RETURN   = 501,
    IF       = 502,
    ELSE     = 503,
    ELIF     = 504,
    DO_WHILE = 505,
    WHILE    = 506,
    FOR      = 507,
    BREAK    = 508,
    CONTINUE = 509,
    SWITCH   = 510,
    CASE     = 511,
    DEFAULT  = 512,
};

// All modifiers are based on the source code.
struct FTokenModifiers
{
    // Such tokens did not originally exist
    bool IsTokenGenerate = false;

    // If modified this token's value
    bool IsTokenValueChanged = false;

    // If modified this token's type
    bool IsTokenTypeChanged = false;

    // True if this token was copied/inserted from another file
    bool IsTokenCopyFromAnywhere = false;
};

struct FToken
{
    std::string Lexeme = "";
    UTokenType Type = UTokenType::NONE;

    size_t Line = 0;
    size_t Row = 0;
    FTokenModifiers Modifiers;

    FToken() = default;
    explicit FToken(std::string lexeme, size_t line, size_t row)
    {
        Lexeme = lexeme;
        Type = internal::StringToTokenType(lexeme);
        Line = line;
        Row = row;
    }

    std::string Dump() const;

    bool IsValid() const
    {
        return Type != UTokenType::NONE;
    }

    struct internal
    {
        static constexpr bool is_string(std::string_view lexeme);
        static constexpr bool is_char(std::string_view lexeme);
        static constexpr bool is_integer(std::string_view lexeme);
        // | 0.0 | 3.14f | 55.55 | 123. | .45 | 86.f |
        static constexpr bool is_double(std::string_view lexeme);
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

    template <typename Container>
    static void SerializeAsCode(const Container& tokens, const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }

        int indentLevel = 0;
        const int indentSpaces = 4;

        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            const auto& token = *it;
            auto next = std::next(it);

            if (token.Type == UTokenType::RBRA)
            {
                indentLevel--;
                file << std::string(indentLevel * indentSpaces, ' ') << token.Lexeme << '\n';
                if (next != tokens.end() && next->Type != UTokenType::RBRA)
                {
                    file << '\n';
                }
                continue;
            }

            if (token.Type == UTokenType::LBRA)
            {
                file << '\n' << token.Lexeme << '\n';
                indentLevel++;
                continue;
            }

            if (token.Type == UTokenType::SEMICOLON)
            {
                file << token.Lexeme << '\n';
                continue;
            }

            static bool needIndent = true;
            if (needIndent)
            {
                file << std::string(indentLevel * indentSpaces, ' ');
                needIndent = false;
            }

            file << token.Lexeme;

            bool putSpace = true;
            if (next != tokens.end())
            {
                UTokenType nt = next->Type;

                if (nt == UTokenType::SEMICOLON || nt == UTokenType::COMMA ||
                    nt == UTokenType::RPAR || nt == UTokenType::RBRA ||
                    nt == UTokenType::INC || nt == UTokenType::DEC ||
                    token.Type == UTokenType::LPAR || token.Type == UTokenType::INC ||
                    token.Type == UTokenType::DEC)
                {
                    putSpace = false;
                }
            }

            if (putSpace && next != tokens.end())
            {
                file << ' ';
            }

            if (next != tokens.end())
            {
                UTokenType nt = next->Type;
                if (nt == UTokenType::SEMICOLON || nt == UTokenType::LBRA || nt == UTokenType::RBRA)
                {
                    needIndent = true;
                }
            }
            else
            {
                needIndent = true;
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
