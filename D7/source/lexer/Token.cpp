#include "Token.h"

#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <sstream>

const std::unordered_map<std::string_view, UTokenType> gl_stringToTokenType
{
    { "::", UTokenType::NAMESPACE },
    { "#",  UTokenType::DIRECTIVE },
    { "__", UTokenType::MACRO },
    { "##", UTokenType::MACRO_S },

    // math
    { "+",  UTokenType::ADD },
    { "-",  UTokenType::SUB },
    { "*",  UTokenType::MUL },
    { "/",  UTokenType::DIV },
    { "%",  UTokenType::MOD },
    { "**", UTokenType::POW },
    { "++", UTokenType::INC },
    { "--", UTokenType::DEC },

    // logical operators
    { "&&", UTokenType::AND },
    { "||", UTokenType::OR },
    { "!",  UTokenType::NOT },

    // equal operators
    { "==", UTokenType::EQUAL },
    { "!=", UTokenType::NOT_EQUAL },
    { "<",  UTokenType::LESS },
    { ">",  UTokenType::GREATER },
    { "<=", UTokenType::LESS_EQUAL },
    { ">=", UTokenType::GREATER_EQUAL },

    // assign
    { "=", UTokenType::ASSIGN },
    { "+=", UTokenType::ASSIGN_ADD },
    { "-=", UTokenType::ASSIGN_SUB },
    { "*=", UTokenType::ASSIGN_MUL },
    { "/=", UTokenType::ASSIGN_DIV },
    { "%=", UTokenType::ASSIGN_MOD },

    // symbols
    { ";", UTokenType::SEMICOLON },
    { ",", UTokenType::COMMA },
    { ".", UTokenType::POINT },
    { "?", UTokenType::QUESTION },
    { "&", UTokenType::AMPERSAND },

    // brackets
    { "{", UTokenType::LBRA },
    { "}", UTokenType::RBRA },
    { "(", UTokenType::LPAR },
    { ")", UTokenType::RPAR },
    { "[", UTokenType::LSQR },
    { "]", UTokenType::RSQR },

    // base func
    { ":",      UTokenType::LABEL },
    { "goto",   UTokenType::GOTO },
    { "new",    UTokenType::NEW },
    { "delete", UTokenType::DELETE },

    // control flow
    { "return", UTokenType::RETURN },
    { "if", UTokenType::IF },
    { "else", UTokenType::ELSE },
    { "elif", UTokenType::ELIF },
    { "do", UTokenType::DO_WHILE },
    { "while", UTokenType::WHILE },
    { "for", UTokenType::FOR },
    { "break", UTokenType::BREAK },
    { "continue", UTokenType::CONTINUE },
    { "switch", UTokenType::SWITCH },
    { "case", UTokenType::CASE },
    { "default", UTokenType::DEFAULT },
};

const std::unordered_map<UTokenType, std::string_view> gl_tokenTypeToString = []
{
    std::unordered_map<UTokenType, std::string_view> result;
    for (const auto& Pair : gl_stringToTokenType)
        result[Pair.second] = Pair.first;

    result.reserve(gl_stringToTokenType.size() + 6);

    /* 1 */ result.emplace(UTokenType::IDENTIFIER, "IDENTIFIER" );
    /* 2 */ result.emplace(UTokenType::INT,        "INT" );
    /* 3 */ result.emplace(UTokenType::FLOAT,      "FLOAT" );
    /* 4 */ result.emplace(UTokenType::STRING,     "STRING" );
    /* 5 */ result.emplace(UTokenType::CHAR,       "CHAR" );
    /* 6 */ result.emplace(UTokenType::BOOLEAN,    "BOOLEAN" );

    return result;
}();


const std::unordered_set<UTokenType> gl_tokensSeparator
{
    UTokenType::NONE,

    UTokenType::NAMESPACE, // ::
    UTokenType::DIRECTIVE, // #
    UTokenType::MACRO,     // __
    UTokenType::MACRO_S,   // ##

    // math operators
    UTokenType::ADD, // +
    UTokenType::SUB, // -
    UTokenType::MUL, // *
    UTokenType::DIV, // /
    UTokenType::MOD, // %
    UTokenType::POW, // **
    UTokenType::INC, // ++
    UTokenType::DEC, // --

    // logical
    UTokenType::AND,  // &&
    UTokenType::OR,   // ||
    UTokenType::NOT, // !

    // equal operators
    UTokenType::EQUAL,         // ==
    UTokenType::NOT_EQUAL,     // !=
    UTokenType::LESS,          // <
    UTokenType::GREATER,       // >
    UTokenType::LESS_EQUAL,    // <=
    UTokenType::GREATER_EQUAL, // >=

    // assign
    UTokenType::ASSIGN,     // =
    UTokenType::ASSIGN_ADD, // +=
    UTokenType::ASSIGN_SUB, // -=
    UTokenType::ASSIGN_MUL, // *=
    UTokenType::ASSIGN_DIV, // /=
    UTokenType::ASSIGN_MOD, // %=

    // other symbols
    UTokenType::SEMICOLON, // ;
    UTokenType::COMMA,     // ,
    UTokenType::POINT,     // .
    UTokenType::QUESTION,  // ?
    UTokenType::AMPERSAND, // &

    // brackets
    UTokenType::LBRA, // {
    UTokenType::RBRA, // }
    UTokenType::LPAR, // (
    UTokenType::RPAR, // )
    UTokenType::LSQR, // [
    UTokenType::RSQR, // ]

    // base func
    UTokenType::LABEL, // :
};

std::string FToken::Dump() const
{
    std::ostringstream result;
    std::string position = std::to_string(Line + 1) + ":" + std::to_string(Row + 1);

    result << std::left
        << std::setw(10) << position
        << std::setw(20) << Lexeme
        << internal::TokenTypeToString(Type);

    return result.str();
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
    return invalid;
}

UTokenType FToken::internal::StringToTokenType(std::string_view lexeme)
{
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

    os.write(reinterpret_cast<const char*>(&token.Modifiers.IsTokenGenerate), sizeof(token.Modifiers.IsTokenGenerate));
    os.write(reinterpret_cast<const char*>(&token.Modifiers.IsTokenValueChanged), sizeof(token.Modifiers.IsTokenValueChanged));
    os.write(reinterpret_cast<const char*>(&token.Modifiers.IsTokenTypeChanged), sizeof(token.Modifiers.IsTokenTypeChanged));
    os.write(reinterpret_cast<const char*>(&token.Modifiers.IsTokenCopyFromAnywhere), sizeof(token.Modifiers.IsTokenCopyFromAnywhere));

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

    is.read(reinterpret_cast<char*>(&token.Modifiers.IsTokenGenerate), sizeof(token.Modifiers.IsTokenGenerate));
    is.read(reinterpret_cast<char*>(&token.Modifiers.IsTokenValueChanged), sizeof(token.Modifiers.IsTokenValueChanged));
    is.read(reinterpret_cast<char*>(&token.Modifiers.IsTokenTypeChanged), sizeof(token.Modifiers.IsTokenTypeChanged));
    is.read(reinterpret_cast<char*>(&token.Modifiers.IsTokenCopyFromAnywhere), sizeof(token.Modifiers.IsTokenCopyFromAnywhere));

    return is;
}