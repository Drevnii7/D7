#pragma once

#include "../lexer/Token.h"
#include <vector>
#include <iomanip>
#include <sstream>

enum class UNodeType : uint8_t
{
    None      = 0, // Invalid

    Program   = 1,

    BlockCode = 2, // {}
    BlockMath = 3, // a + (b * c)
    BlockArgs = 4, // (int argc, char *argv[])

    Func      = 100, // func void main();
    Variable  = 101, // var float pi = 3.14f;
    Setter    = 102, // pi = 3.14f;

    Name      = 200, // name variable / function
    Type      = 201, // type variable / type returned function
    Math      = 202, // (8 * 5) + (4 / 2)
};

struct FASTNode
{
    UNodeType Type = UNodeType::None;
	FToken Token;
	std::vector<std::unique_ptr<FASTNode>> ChildNodes;

    FASTNode() = default;
    FASTNode(UNodeType lType)
    {
        Type = lType;
    }

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
        if (Type != UNodeType::BlockCode && Type != UNodeType::BlockMath && Type != UNodeType::BlockArgs)
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
