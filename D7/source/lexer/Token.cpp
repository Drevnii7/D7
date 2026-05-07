#include "Token.h"

#include <unordered_map>
#include <unordered_set>

const std::unordered_map<std::string_view, UTokenType> gl_stringToTokenType
{
    // { "", UTokenType::IDENTIFIER }, // NONE BASE LEXEME

    // { "", UTokenType::INTEGER_CONST }, // NONE BASE LEXEME
    // { "", UTokenType::DOUBLE_CONST }, // NONE BASE LEXEME
    // { "", UTokenType::STRING_CONST }, // NONE BASE LEXEME
    // { "", UTokenType::CHAR_CONST }, // NONE BASE LEXEME

    // boolean constant
    { "true", UTokenType::TRUE },
    { "false", UTokenType::FALSE },

    { "const", UTokenType::CONST },

    // types
    // { "", UTokenType::UNDEFINED }, // NONE BASE LEXEME

    { "int", UTokenType::INT },
    { "double", UTokenType::DOUBLE },
    { "bool", UTokenType::BOOL },
    { "char", UTokenType::CHAR },
    { "void", UTokenType::VOID },
    { "auto", UTokenType::AUTO },

    // cycles
    { "do", UTokenType::DO_WHILE },
    { "while", UTokenType::WHILE },
    { "for", UTokenType::FOR },

    // cycles addition
    { "break", UTokenType::BREAK },
    { "continue", UTokenType::CONTINUE },

    { "switch", UTokenType::SWITCH },
    { "case", UTokenType::CASE },
    { "default", UTokenType::DEFAULT },

    // conditions
    { "if", UTokenType::IF },
    { "else", UTokenType::ELSE },

    // relationship operators
    { "<", UTokenType::LESS },
    { ">", UTokenType::GREATER },
    { "<=", UTokenType::LESS_EQUAL },
    { ">=", UTokenType::GREATER_EQUAL },

    // equal operators
    { "==", UTokenType::EQUAL },
    { "!=", UTokenType::NOT_EQUAL },

    // logical operators
    { "&&", UTokenType::AND },
    { "||", UTokenType::OR },
    { "!",  UTokenType::EXCLAMATION },

    // math operators
    { "+", UTokenType::PLUS },
    { "-", UTokenType::MINUS },
    { "*", UTokenType::STAR },
    { "/", UTokenType::SLASH },
    { "++", UTokenType::INC },
    { "--", UTokenType::DEC },

    // assign
    { "=", UTokenType::ASSIGN },
    { "+=", UTokenType::ADD_ASSIGN },
    { "-=", UTokenType::SUB_ASSIGN },
    { "*=", UTokenType::MUL_ASSIGN },
    { "/=", UTokenType::DIV_ASSIGN },

    // brackets
    { "{", UTokenType::LBRA },
    { "}", UTokenType::RBRA },
    { "(", UTokenType::LPAR },
    { ")", UTokenType::RPAR },
    { "[", UTokenType::LSQR },
    { "]", UTokenType::RSQR },

    // function
    // { "", UTokenType::FUNCTION }, // NONE BASE LEXEME
    { "return", UTokenType::RETURN },

    // other symbols
    { ";", UTokenType::SEMICOLON },
    { ":", UTokenType::COLON },
    { ",", UTokenType::COMMA },
    { ".", UTokenType::POINT },
    { "?", UTokenType::QUESTION },
    { "&", UTokenType::AMPERSAND },

    { "new", UTokenType::NEW },
    { "delete", UTokenType::DELETE },


    // { "", UTokenType::PREPROCESSOR_DIRECTIVE }, // NONE BASE LEXEME

    { "::", UTokenType::ACCESS_OPERATOR },

};

const std::unordered_map<UTokenType, std::string_view> gl_tokenTypeToString = []
{
    std::unordered_map<UTokenType, std::string_view> result;
    for (const auto& Pair : gl_stringToTokenType)
        result[Pair.second] = Pair.first;

    result.reserve(gl_stringToTokenType.size() + 8);

    /* 1 */ result.emplace(UTokenType::IDENTIFIER,             "IDENTIFIER" );
    /* 2 */ result.emplace(UTokenType::INTEGER_CONST,          "INTEGER_CONST" );
    /* 3 */ result.emplace(UTokenType::DOUBLE_CONST,           "DOUBLE_CONST" );
    /* 4 */ result.emplace(UTokenType::STRING_CONST,           "STRING_CONST" );
    /* 5 */ result.emplace(UTokenType::CHAR_CONST,             "CHAR_CONST" );
    /* 6 */ result.emplace(UTokenType::UNDEFINED,              "UNDEFINED" );
    /* 7 */ result.emplace(UTokenType::FUNCTION,               "FUNCTION" );
    /* 8 */ result.emplace(UTokenType::PREPROCESSOR_DIRECTIVE, "PREPROCESSOR_DIRECTIVE" );

    return result;
}();


const std::unordered_set<UTokenType> gl_tokensSeparator
{
    // relationship operators
    UTokenType::LESS,          // <
    UTokenType::GREATER,       // >
    UTokenType::LESS_EQUAL,    // <=
    UTokenType::GREATER_EQUAL, // >=

    // equal operators
    UTokenType::EQUAL,     // ==
    UTokenType::NOT_EQUAL, // !=

    // logical operators
    UTokenType::AND,         // &&
    UTokenType::OR,          // ||
    UTokenType::EXCLAMATION, // !

    // math operators
    UTokenType::PLUS,  // +
    UTokenType::MINUS, // -
    UTokenType::STAR,  // *
    UTokenType::SLASH, // /
    UTokenType::INC,   // ++
    UTokenType::DEC,   // --

    // assign
    UTokenType::ASSIGN,     // =
    UTokenType::ADD_ASSIGN, // +=
    UTokenType::SUB_ASSIGN, // -=
    UTokenType::MUL_ASSIGN, // *=
    UTokenType::DIV_ASSIGN, // /=

    // brackets
    UTokenType::LBRA, // {
    UTokenType::RBRA, // }
    UTokenType::LPAR, // (
    UTokenType::RPAR, // )
    UTokenType::LSQR, // [
    UTokenType::RSQR, // ]

    // other symbols
    UTokenType::SEMICOLON, // ;
    UTokenType::COLON,     // :
    UTokenType::COMMA,     // ,
    UTokenType::POINT,     // .
    UTokenType::QUESTION,  // ?
    UTokenType::AMPERSAND, // &

    UTokenType::ACCESS_OPERATOR, // ::
};

std::string FToken::Debug() const 
{
    std::string result = "Lexeme: \"";
    result += Lexeme;
    result += "\", type: \"";
    result += internal::TokenTypeToString(Type);
    result += "\", Line: ";
    result += std::to_string(Line+1);
    result += ", Row: ";
    result += std::to_string(Row+1);
    return result;
}

constexpr bool FToken::internal::is_string(std::string_view lexeme)
{
    return lexeme.size() >= 2 && lexeme.front() == '"' && lexeme.back() == '"';
}

constexpr bool FToken::internal::is_char(std::string_view lexeme)
{
    return lexeme.size() >= 2 && lexeme.front() == '\'' && lexeme.back() == '\'';
}

constexpr bool FToken::internal::is_integer(std::string_view lexeme)
{
    if (lexeme.empty()) return false;

    size_t indexBegin = 0;

    if (lexeme.front() == '-' || lexeme.front() == '+')
        indexBegin++;

    for (size_t i = indexBegin; i < lexeme.size(); ++i)
        if (lexeme[i] < '0' || lexeme[i] > '9')
            return false;
    return true;
}

constexpr bool FToken::internal::is_double(std::string_view lexeme)
{
    if (lexeme.empty()) return false;

    bool hasDigitBeforePoint = false;
    bool hasPoint = false;
    bool hasDigitAfterPoint = false;

    size_t indexBegin = 0;
    size_t indexEnd = lexeme.size();

    if (lexeme.front() == '+' || lexeme.front() == '-')
        indexBegin++;

    if (indexEnd > indexBegin && (lexeme.back() == 'f' || lexeme.back() == 'F'))
        indexEnd--;

    for (size_t i = indexBegin; i < indexEnd; ++i)
    {
        char c = lexeme[i];
        if (c >= '0' && c <= '9')
        {
            if (!hasPoint) hasDigitBeforePoint = true;
            else hasDigitAfterPoint = true;
        }
        else if (c == '.' && !hasPoint)
        {
            hasPoint = true;
        }
        else
        {
            return false;
        }
    }

    return hasPoint && (hasDigitBeforePoint || hasDigitAfterPoint);
}

constexpr bool FToken::internal::is_preprocessorDirective(std::string_view lexeme)
{
    return lexeme.front() == '#';
}

bool FToken::internal::is_separator(std::string_view tokenType)
{
    return is_separatorType(StringToTokenType(tokenType));
}

bool FToken::internal::is_separatorType(UTokenType tokenType)
{
    return gl_tokensSeparator.find(tokenType) != gl_tokensSeparator.end();
}


std::string_view FToken::internal::TokenTypeToString(UTokenType tokenType)
{
    auto it = gl_tokenTypeToString.find(tokenType);
    if (it != gl_tokenTypeToString.end())
        return it->second; // Find

    // Not find
    static std::string invalid = "INVALID";
    return "invalid";
}

UTokenType FToken::internal::StringToTokenType(std::string_view lexeme)
{
    if (is_integer(lexeme))
        return UTokenType::INTEGER_CONST;

    if (is_double(lexeme))
        return UTokenType::DOUBLE_CONST;

    if (is_string(lexeme))
        return UTokenType::STRING_CONST;

    if (is_char(lexeme))
        return UTokenType::CHAR_CONST;

    if (is_preprocessorDirective(lexeme))
        return UTokenType::PREPROCESSOR_DIRECTIVE;

    auto it = gl_stringToTokenType.find(lexeme);
    if (it != gl_stringToTokenType.end())
        return it->second; // Find

    return UTokenType::IDENTIFIER;
}

std::ostream& operator<<(std::ostream& os, const FToken& token)
{
    size_t len = token.Lexeme.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(token.Lexeme.c_str(), len);

    os.write(reinterpret_cast<const char*>(&token.Type), sizeof(token.Type));
    os.write(reinterpret_cast<const char*>(&token.Line), sizeof(token.Line));
    os.write(reinterpret_cast<const char*>(&token.Row), sizeof(token.Row));

    return os;
}

std::istream& operator>>(std::istream& is, FToken& token)
{
    size_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    token.Lexeme.resize(len);
    is.read(&token.Lexeme[0], len);

    is.read(reinterpret_cast<char*>(&token.Type), sizeof(token.Type));
    is.read(reinterpret_cast<char*>(&token.Line), sizeof(token.Line));
    is.read(reinterpret_cast<char*>(&token.Row), sizeof(token.Row));

    return is;
}