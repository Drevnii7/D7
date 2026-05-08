#include "AST.h"

#include <unordered_map>

const std::unordered_map<std::string_view, UNodeType> gl_stringToTokenType
{
    { "None",         UNodeType::None },
    { "Block",        UNodeType::Block },
    { "Literal",      UNodeType::Literal },
    { "Identifier",   UNodeType::Identifier },
    { "VarDecl",      UNodeType::VarDecl },
    { "FunctionDecl", UNodeType::FunctionDecl },
    { "ReturnStmt",   UNodeType::ReturnStmt },
    { "BinaryExpr",   UNodeType::BinaryExpr },
    { "UnaryExpr",    UNodeType::UnaryExpr },
};

const std::unordered_map<UNodeType, std::string_view> gl_tokenTypeToString = []
    {
        std::unordered_map<UNodeType, std::string_view> result;
        for (const auto& Pair : gl_stringToTokenType)
            result[Pair.second] = Pair.first;

        return result;
    }();

std::string_view FASTNode::internal::NodeTypeToString(UNodeType nodeType)
{
    auto it = gl_tokenTypeToString.find(nodeType);
    if (it != gl_tokenTypeToString.end())
        return it->second; // Find

    // Not find
    static std::string invalid = "None";
    return invalid;
}

UNodeType FASTNode::internal::StringToNodeType(std::string_view string)
{
    auto it = gl_stringToTokenType.find(string);
    if (it != gl_stringToTokenType.end())
        return it->second; // Find

    return UNodeType::None;
}
