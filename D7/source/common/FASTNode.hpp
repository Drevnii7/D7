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
            if (child)
            {
                childNodes.push_back(std::move(child));
            }
        }

        std::string Dump(int indent = 0) const
        {
            std::string pad(indent * 4, ' ');
            std::string line = pad + "[" + std::string(internal::NodeTypeToString(type)) + "] ";

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

        struct internal
        {
            static std::string_view NodeTypeToString(EASTNodeType nodeType);
            static EASTNodeType StringToNodeType(std::string_view string);
        };

        static void Serialize(FASTNode* root, const std::string& filename);
        static FASTNode Deserialize(const std::string& filename);

        friend std::ostream& operator<<(std::ostream& os, const FASTNode& node);
        friend std::istream& operator>>(std::istream& is, FASTNode& node);
	};
}

