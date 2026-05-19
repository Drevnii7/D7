#pragma once

#include "FToken.hpp"
#include "EASTNodeType.hpp"

namespace d7
{
	struct FASTNode
	{
		EASTNodeType type = EASTNodeType::None;
		FToken token;
		std::vector<std::unique_ptr<FASTNode>> childNodes;

        FASTNode() = default;
        FASTNode(EASTNodeType lType)
        {
            type = lType;
        }

        bool IsValid() const
        {
            return type != EASTNodeType::None;
        }

        void AddChild(std::unique_ptr<FASTNode> child)
        {
            assert(child != nullptr, "Try add child invalid FASTNode");
            
            childNodes.push_back(std::move(child));
        }

        std::string Dump(int indent = 0) const
        {
            std::string pad(indent * 4, ' ');
            std::string line = pad + "[" + std::string(d7::EASTNodeTypeUtils::EASTNodeTypeToString(type)) + "] ";

            if (token.IsValid())
            {
                line += token.Dump();
            }

            line += '\n';

            for (const auto& child : childNodes)
            {
                line += child->Dump(indent + 1);
            }
            return line;
        }

        static d7::expected Serialize(const FASTNode& Root, const std::string& FilePath);
        static d7::expected Deserialize(FASTNode& Root, const std::string& FilePath);

        friend std::ostream& operator<<(std::ostream& os, const FASTNode& node);
        friend std::istream& operator>>(std::istream& is, FASTNode& node);
	};
}

