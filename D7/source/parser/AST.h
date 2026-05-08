#pragma once

#include "../lexer/Token.h"
#include <vector>
#include <iomanip>
#include <sstream>

enum class UNodeType : uint8_t
{
    None         = 0, // Invalid

    // Down level
    Block        = 1, // {}
    Literal      = 2, // int, float, sting, char
    Identifier   = 3, // main, std::cout, 

    // Up level
    VarDecl      = 4, // type name = value;
    FunctionDecl = 5, // type name() { ... }
    ReturnStmt   = 6, // return value;
    BinaryExpr   = 7, // a + b; x = 41
    UnaryExpr    = 8, // ++x; !flag, x--
};

struct FASTNode
{
    UNodeType Type = UNodeType::None;
	FToken Token;
	std::vector<std::unique_ptr<FASTNode>> ChildNodes;

    FASTNode() = default;
    FASTNode(const FASTNode&) = delete;
    FASTNode& operator=(const FASTNode&) = delete;

    FASTNode(FASTNode&&) noexcept = default;
    FASTNode& operator=(FASTNode&&) noexcept = default;

    bool IsValid() const
    {
        return Type != UNodeType::None;
    }

    void AddChild(std::unique_ptr<FASTNode> child)
    {
        if (child)
        {
            ChildNodes.push_back(std::move(child));
        }
    }

    std::string Dump(int indent = 0) const
    {
        std::string pad(indent * 4, ' ');
        std::string line = pad + "[" + std::string(internal::NodeTypeToString(Type)) + "] ";
        if (Type != UNodeType::Block)
        {
            line += Token.IsValid() ? Token.Dump() : "INVALID";
        }
        line += '\n';

        for (const auto& child : ChildNodes)
        {
            line += child->Dump(indent + 1);
        }
        return line;
    }

    struct internal
    {
        static std::string_view NodeTypeToString(UNodeType nodeType);
        static UNodeType StringToNodeType(std::string_view string);
    };

    static void Serialize(FASTNode* root, const std::string& filename);
    static FASTNode Deserialize(const std::string& filename);

    friend std::ostream& operator<<(std::ostream& os, const FASTNode& node);
    friend std::istream& operator>>(std::istream& is, FASTNode& node);
};
