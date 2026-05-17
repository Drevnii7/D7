#include "EASTNodeType.hpp"

#include <map>
#include <string>

const std::map<std::string_view, d7::EASTNodeType> gl_stringToTokenType
{
    { "None",       d7::EASTNodeType::None },

    { "Program",    d7::EASTNodeType::Program },
    { "Namespace",  d7::EASTNodeType::Namespace },

    { "BlockCode",        d7::EASTNodeType::BlockCode },
    { "BlockMath",        d7::EASTNodeType::BlockMath },
    { "BlockArgs",        d7::EASTNodeType::BlockArgs },
    { "BlockAgreements",  d7::EASTNodeType::BlockAgreements },

    { "Func",       d7::EASTNodeType::Func },
    { "Variable",   d7::EASTNodeType::Variable },
    { "Setter",     d7::EASTNodeType::Setter },
    { "Call",       d7::EASTNodeType::Call },

    { "Name",       d7::EASTNodeType::Name },
    { "Type",       d7::EASTNodeType::Type },
    { "Math",       d7::EASTNodeType::Math },

    { "Return",     d7::EASTNodeType::Return },
};

const std::map<d7::EASTNodeType, std::string_view> gl_tokenTypeToString = []
    {
        std::map<d7::EASTNodeType, std::string_view> result;
        for (const auto& Pair : gl_stringToTokenType)
            result[Pair.second] = Pair.first;

        return result;
    }();

std::string_view d7::EASTNodeTypeUtils::EASTNodeTypeToString(d7::EASTNodeType Type)
{
    auto it = gl_tokenTypeToString.find(Type);
    if (it != gl_tokenTypeToString.end())
        return it->second; // Find

    // Not find
    static std::string invalid = "None";
    return invalid;
}

d7::EASTNodeType d7::EASTNodeTypeUtils::EASTNodeTypeFromString(std::string_view String)
{
    auto it = gl_stringToTokenType.find(String);
    if (it != gl_stringToTokenType.end())
        return it->second; // Find

    return d7::EASTNodeType::None;
}
