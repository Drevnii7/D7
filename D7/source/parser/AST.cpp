#include "AST.h"

#include <unordered_map>

const std::unordered_map<std::string_view, UNodeType> gl_stringToTokenType
{
    { "None",         UNodeType::None },

    { "BlockCode",    UNodeType::BlockCode },
    { "BlockMath",    UNodeType::BlockMath },
    { "BlockArgs",    UNodeType::BlockArgs },

    { "Func",         UNodeType::Func },
    { "Variable",     UNodeType::Variable },
    { "Setter",     UNodeType::Setter },

    { "Name",         UNodeType::Name },
    { "Type",         UNodeType::Type },
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


std::ostream& operator<<(std::ostream& os, const FASTNode& node)
{
    os.write(reinterpret_cast<const char*>(&node.Type), sizeof(node.Type));
    if (!os.good()) throw std::runtime_error("Error writing node type");

    os << node.Token;
    if (!os.good()) throw std::runtime_error("Error writing node token");

    size_t childCount = node.ChildNodes.size();
    os.write(reinterpret_cast<const char*>(&childCount), sizeof(childCount));
    if (!os.good()) throw std::runtime_error("Error writing child count");

    for (const auto& child : node.ChildNodes)
    {
        if (child)
        {
            os << *child;
        }
    }

    return os;
}

std::istream& operator>>(std::istream& is, FASTNode& node)
{
    is.read(reinterpret_cast<char*>(&node.Type), sizeof(node.Type));
    if (!is.good()) throw std::runtime_error("Error reading node type");

    is >> node.Token;
    if (!is.good()) throw std::runtime_error("Error reading node token");

    size_t childCount = 0;
    is.read(reinterpret_cast<char*>(&childCount), sizeof(childCount));
    if (!is.good()) throw std::runtime_error("Error reading child count");

    node.ChildNodes.clear();
    node.ChildNodes.reserve(childCount);
    for (size_t i = 0; i < childCount; ++i)
    {
        auto child = std::make_unique<FASTNode>();
        is >> *child;
        node.ChildNodes.push_back(std::move(child));
    }

    return is;
}

void FASTNode::Serialize(FASTNode* root, const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    if (root)
    {
        file << *root;
    }
    else
    {
        throw std::runtime_error("Cannot serialize null root node");
    }

    file.close();
}

FASTNode FASTNode::Deserialize(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    FASTNode root;
    file >> root;

    file.close();
    return std::exchange(root, {});
}