#include "FToken.hpp"

std::string d7::FToken::Dump() const
{
    std::string Ret;
    Ret.reserve(10);

    Ret += '[';
    Ret += std::to_string(line);
    Ret += ':';
    Ret += std::to_string(row);
    Ret += "] ";
    Ret += lexeme;
    Ret += " (";
    Ret += d7::ETokenTypeUtils::ETokenTypeToString(type);
    Ret += ')';

    return Ret;
}

std::ostream& d7::operator<<(std::ostream& os, const d7::FToken& token)
{
    size_t len = token.lexeme.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (len > 0) {
        os.write(token.lexeme.c_str(), len);
    }

    os.write(reinterpret_cast<const char*>(&token.type), sizeof(token.type));
    os.write(reinterpret_cast<const char*>(&token.line), sizeof(token.line));
    os.write(reinterpret_cast<const char*>(&token.row), sizeof(token.row));

    return os;
}

std::istream& d7::operator>>(std::istream& is, d7::FToken& token)
{
    size_t len = 0;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));

    if (len > 0) {
        token.lexeme.resize(len);
        is.read(&token.lexeme[0], len);
    }
    else {
        token.lexeme.clear();
    }

    is.read(reinterpret_cast<char*>(&token.type), sizeof(token.type));
    is.read(reinterpret_cast<char*>(&token.line), sizeof(token.line));
    is.read(reinterpret_cast<char*>(&token.row), sizeof(token.row));

    return is;
}