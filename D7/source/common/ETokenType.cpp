#include <map>
#include <set>

#include "ETokenType.hpp"
#include "../utils/assert.hpp"

const std::map<std::string_view, d7::ETokenType> gl_stringToTokenType
{
    /* NONE */

    /* IDENTIFIER */
    { "::", d7::ETokenType::NAMESPACE },
    { "#",  d7::ETokenType::DIRECTIVE },
    { "__", d7::ETokenType::MACRO },
    { "##", d7::ETokenType::MACRO_S },

    // math
    { "+",  d7::ETokenType::ADD },
    { "-",  d7::ETokenType::SUB },
    { "*",  d7::ETokenType::MUL },
    { "/",  d7::ETokenType::DIV },
    { "%",  d7::ETokenType::MOD },
    { "**", d7::ETokenType::POW },
    { "++", d7::ETokenType::INC },
    { "--", d7::ETokenType::DEC },

    // logical operators
    { "&&", d7::ETokenType::AND },
    { "||", d7::ETokenType::OR },
    { "!",  d7::ETokenType::NOT },

    // equal operators
    { "==", d7::ETokenType::EQUAL },
    { "!=", d7::ETokenType::NOT_EQUAL },
    { "<",  d7::ETokenType::LESS },
    { ">",  d7::ETokenType::GREATER },
    { "<=", d7::ETokenType::LESS_EQUAL },
    { ">=", d7::ETokenType::GREATER_EQUAL },

    // assign
    { "=",  d7::ETokenType::ASSIGN },
    { "+=", d7::ETokenType::ASSIGN_ADD },
    { "-=", d7::ETokenType::ASSIGN_SUB },
    { "*=", d7::ETokenType::ASSIGN_MUL },
    { "/=", d7::ETokenType::ASSIGN_DIV },
    { "%=", d7::ETokenType::ASSIGN_MOD },

    // symbols
    { ";", d7::ETokenType::SEMICOLON },
    { ":", d7::ETokenType::COLON },
    { ",", d7::ETokenType::COMMA },
    { ".", d7::ETokenType::POINT },
    { "?", d7::ETokenType::QUESTION },
    { "&", d7::ETokenType::AMPERSAND },

    // brackets
    { "{", d7::ETokenType::LBRA },
    { "}", d7::ETokenType::RBRA },
    { "(", d7::ETokenType::LPAR },
    { ")", d7::ETokenType::RPAR },
    { "[", d7::ETokenType::LSQR },
    { "]", d7::ETokenType::RSQR },

    // base func
    { "goto",   d7::ETokenType::GOTO },
    { "new",    d7::ETokenType::NEW },
    { "delete", d7::ETokenType::DELETE },

    // control flow
    { "return", d7::ETokenType::RETURN },
    { "if", d7::ETokenType::IF },
    { "else", d7::ETokenType::ELSE },
    { "elif", d7::ETokenType::ELIF },
    { "do", d7::ETokenType::DO_WHILE },
    { "while", d7::ETokenType::WHILE },
    { "for", d7::ETokenType::FOR },
    { "break", d7::ETokenType::BREAK },
    { "continue", d7::ETokenType::CONTINUE },
    { "switch", d7::ETokenType::SWITCH },
    { "case", d7::ETokenType::CASE },
    { "default", d7::ETokenType::DEFAULT },

    // define
    { "func", d7::ETokenType::DEF_FUNC },
    { "var",  d7::ETokenType::DEF_VAR },
    { "enum", d7::ETokenType::DEF_ENUM },
    { "struct", d7::ETokenType::DEF_STRUCT },
};

const std::map<d7::ETokenType, std::string_view> gl_tokenTypeToString = []
{
    std::map<d7::ETokenType, std::string_view> result;
    for (const auto& Pair : gl_stringToTokenType)
        result[Pair.second] = Pair.first;

    //result.reserve(gl_stringToTokenType.size() + 6);

    /* 1 */ result.emplace(d7::ETokenType::NONE,       "NONE");
    /* 1 */ result.emplace(d7::ETokenType::IDENTIFIER, "IDENTIFIER");
    /* 2 */ result.emplace(d7::ETokenType::INT,        "INT");
    /* 3 */ result.emplace(d7::ETokenType::FLOAT,      "FLOAT");
    /* 4 */ result.emplace(d7::ETokenType::STRING,     "STRING");
    /* 5 */ result.emplace(d7::ETokenType::CHAR,       "CHAR");
    /* 6 */ result.emplace(d7::ETokenType::BOOLEAN,    "BOOLEAN");

    return result;
}();

const std::set<d7::ETokenType> gl_tokensSeparator
{
    d7::ETokenType::NAMESPACE, // ::
    d7::ETokenType::DIRECTIVE, // #
    d7::ETokenType::MACRO,     // __
    d7::ETokenType::MACRO_S,   // ##

    // math operators
    d7::ETokenType::ADD, // +
    d7::ETokenType::SUB, // -
    d7::ETokenType::MUL, // *
    d7::ETokenType::DIV, // /
    d7::ETokenType::MOD, // %
    d7::ETokenType::POW, // **
    d7::ETokenType::INC, // ++
    d7::ETokenType::DEC, // --

    // logical
    d7::ETokenType::AND,  // &&
    d7::ETokenType::OR,   // ||
    d7::ETokenType::NOT,  // !

    // equal operators
    d7::ETokenType::EQUAL,         // ==
    d7::ETokenType::NOT_EQUAL,     // !=
    d7::ETokenType::LESS,          // <
    d7::ETokenType::GREATER,       // >
    d7::ETokenType::LESS_EQUAL,    // <=
    d7::ETokenType::GREATER_EQUAL, // >=

    // assign
    d7::ETokenType::ASSIGN,     // =
    d7::ETokenType::ASSIGN_ADD, // +=
    d7::ETokenType::ASSIGN_SUB, // -=
    d7::ETokenType::ASSIGN_MUL, // *=
    d7::ETokenType::ASSIGN_DIV, // /=
    d7::ETokenType::ASSIGN_MOD, // %=

    // other symbols
    d7::ETokenType::SEMICOLON, // ;
    d7::ETokenType::COLON,     // :
    d7::ETokenType::COMMA,     // ,
    d7::ETokenType::POINT,     // .
    d7::ETokenType::QUESTION,  // ?
    d7::ETokenType::AMPERSAND, // &

    // brackets
    d7::ETokenType::LBRA, // {
    d7::ETokenType::RBRA, // }
    d7::ETokenType::LPAR, // (
    d7::ETokenType::RPAR, // )
    d7::ETokenType::LSQR, // [
    d7::ETokenType::RSQR, // ]
};


std::string_view d7::ETokenTypeUtils::ETokenTypeToString(d7::ETokenType Type)
{
    auto it = gl_tokenTypeToString.find(Type);
    if (it != gl_tokenTypeToString.end())
        return it->second; // Find

    // Not find
    return "NONE";
}

d7::ETokenType d7::ETokenTypeUtils::ETokenTypeFromString(std::string_view String)
{
    if (IsBoolean(String))
        return d7::ETokenType::BOOLEAN;

    if (IsInteger(String))
        return d7::ETokenType::INT;

    if (IsFloat(String))
        return d7::ETokenType::FLOAT;

    if (IsString(String))
        return d7::ETokenType::STRING;

    if (IsChar(String))
        return d7::ETokenType::CHAR;

    auto it = gl_stringToTokenType.find(String);
    if (it != gl_stringToTokenType.end())
        return it->second; // Find

    return d7::ETokenType::IDENTIFIER;
}

bool d7::ETokenTypeUtils::IsSeparator(ETokenType TokenType)
{
    return gl_tokensSeparator.find(TokenType) != gl_tokensSeparator.end();
}

bool d7::ETokenTypeUtils::IsConst(d7::ETokenType TokenType)
{
    return TokenType >= d7::ETokenType::INT
        && TokenType <= d7::ETokenType::BOOLEAN;
}

bool d7::ETokenTypeUtils::IsMath(d7::ETokenType TokenType)
{
    return TokenType >= d7::ETokenType::ADD
        && TokenType <= d7::ETokenType::DEC;
}

bool d7::ETokenTypeUtils::IsLogical(d7::ETokenType TokenType)
{
    return TokenType >= d7::ETokenType::AND
        && TokenType <= d7::ETokenType::NOT;
}

bool d7::ETokenTypeUtils::IsEqual(d7::ETokenType TokenType)
{
    return TokenType >= d7::ETokenType::EQUAL
        && TokenType <= d7::ETokenType::GREATER_EQUAL;
}

bool d7::ETokenTypeUtils::IsAssign(d7::ETokenType TokenType)
{
    return TokenType >= d7::ETokenType::ASSIGN
        && TokenType <= d7::ETokenType::ASSIGN_MOD;
}

bool d7::ETokenTypeUtils::IsBoolean(std::string_view Lexeme)
{
    bool RetValue = false;
    return TryToBoolean(Lexeme, RetValue);
}

bool d7::ETokenTypeUtils::IsInteger(std::string_view Lexeme)
{
    int RetValue = 0;
    return TryToInteger(Lexeme, RetValue);
}

bool d7::ETokenTypeUtils::IsFloat(std::string_view Lexeme)
{
    float RetValue = 0.0f;
    return TryToFloat(Lexeme, RetValue);
}

bool d7::ETokenTypeUtils::IsString(std::string_view Lexeme)
{
    std::string RetValue = "";
    return TryToString(Lexeme, RetValue);
}

bool d7::ETokenTypeUtils::IsChar(std::string_view Lexeme)
{
    char RetValue = ' ';
    return TryToChar(Lexeme, RetValue);
}

bool d7::ETokenTypeUtils::TryToBoolean(std::string_view Lexeme, bool& Out)
{
    if (Lexeme == "true") 
    {
        Out = true;
        return true;
    }
    else if (Lexeme == "false") 
    {
        Out = false;
        return true;
    }
    return false;
}

bool d7::ETokenTypeUtils::TryToInteger(std::string_view Lexeme, int& Out)
{
    if (Lexeme.empty()) return false;

    std::string_view trimmed = Lexeme;
    int base = 10;
    bool negative = false;

    if (!trimmed.empty() && trimmed[0] == '-') {
        negative = true;
        trimmed.remove_prefix(1);
    }

    if (trimmed.empty()) return false;

    if (trimmed.size() >= 2 && trimmed[0] == '0') {
        char second = trimmed[1];
        if (second == 'x' || second == 'X') {
            base = 16;
            trimmed.remove_prefix(2);
        }
        else if (second == 'b' || second == 'B') {
            base = 2;
            trimmed.remove_prefix(2);
        }
        else if (second == 'd' || second == 'D') {
            base = 10;
            trimmed.remove_prefix(2);
        }
        else {
            if (trimmed.size() == 1) {
                Out = 0;
                return true;
            }
            return false;
        }
    }

    if (trimmed.empty()) return false;

    long long value = 0;
    for (size_t i = 0; i < trimmed.size(); ++i) {
        char c = trimmed[i];
        int digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        }
        else if (base == 16 && ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
            digit = tolower(c) - 'a' + 10;
        }
        else {
            return false;
        }

        if (digit >= base) return false;

        if (value > (LLONG_MAX - digit) / base) return false;
        value = value * base + digit;
    }

    if (negative) {
        value = -value;
    }

    if (value < INT_MIN || value > INT_MAX) return false;

    Out = static_cast<int>(value);
    return true;
}

bool d7::ETokenTypeUtils::TryToFloat(std::string_view Lexeme, float& Out)
{
    if (Lexeme.empty()) return false;

    size_t dotPos = Lexeme.find('.');
    if (dotPos != std::string_view::npos) 
    {
        if (dotPos == 0) return false;

        if (dotPos + 1 >= Lexeme.size()) return false;

        char nextChar = Lexeme[dotPos + 1];
        if (!(nextChar >= '0' && nextChar <= '9')) return false;
    }
    else
    {
        bool hasExponent = false;
        for (char c : Lexeme) {
            if (c == 'e' || c == 'E') 
            {
                hasExponent = true;
                break;
            }
        }
        if (!hasExponent) return false;
    }

    std::string str(Lexeme);
    char* endptr = nullptr;
    float value = strtof(str.c_str(), &endptr);

    if (endptr == str.c_str()) return false;

    while (*endptr && (isspace(static_cast<unsigned char>(*endptr)) || *endptr == 'f' || *endptr == 'F')) 
    {
        ++endptr;
    }

    if (*endptr != '\0') return false;

    Out = value;
    return true;
}

bool d7::ETokenTypeUtils::TryToString(std::string_view Lexeme, std::string& Out)
{
    if (Lexeme.size() < 2) return false;

    if (Lexeme.front() != '"' || Lexeme.back() != '"') return false;

    std::string_view content = Lexeme.substr(1, Lexeme.size() - 2);

    std::string result;
    result.reserve(content.size());

    for (size_t i = 0; i < content.size(); ++i)
    {
        if (content[i] == '\\' && i + 1 < content.size()) 
        {
            ++i;
            switch (content[i]) {
            case 'n': result += '\n'; break;
            case 't': result += '\t'; break;
            case 'r': result += '\r'; break;
            case '\\': result += '\\'; break;
            case '"': result += '"'; break;
            default: return false;
            }
        }
        else {
            result += content[i];
        }
    }

    Out = std::move(result);
    return true;
}

bool d7::ETokenTypeUtils::TryToChar(std::string_view Lexeme, char& Out)
{
    if (Lexeme.size() < 3) return false;

    if (Lexeme.front() != '\'' || Lexeme.back() != '\'') return false;

    std::string_view content = Lexeme.substr(1, Lexeme.size() - 2);

    if (content.empty()) return false;

    if (content.size() == 1) 
    {
        Out = content[0];
        return true;
    }

    if (content.size() == 2 && content[0] == '\\') 
    {
        switch (content[1]) {
        case 'n': Out = '\n'; break;
        case 't': Out = '\t'; break;
        case 'r': Out = '\r'; break;
        case '\\': Out = '\\'; break;
        case '\'': Out = '\''; break;
        default: return false;
        }
        return true;
    }

    return false;
}
