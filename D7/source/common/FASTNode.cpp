#include "FASTNode.hpp"

#include <ostream>
#include <istream>

std::ostream& d7::operator<<(std::ostream& os, const d7::FASTNode& node)
{
    os.write(reinterpret_cast<const char*>(&node.type), sizeof(node.type));
    if (!os.good()) return os;

    os << node.token;
    if (!os.good()) return os;

    size_t childCount = node.childNodes.size();
    os.write(reinterpret_cast<const char*>(&childCount), sizeof(childCount));
    if (!os.good()) return os;

    for (const auto& child : node.childNodes)
    {
        if (child)
        {
            os << *child;
        }
    }

    return os;
}

std::istream& d7::operator>>(std::istream& is, d7::FASTNode& node)
{
    is.read(reinterpret_cast<char*>(&node.type), sizeof(node.type));
    if (!is.good()) return is;

    is >> node.token;
    if (!is.good()) return is;

    size_t childCount = 0;
    is.read(reinterpret_cast<char*>(&childCount), sizeof(childCount));
    if (!is.good()) return is;

    node.childNodes.clear();
    node.childNodes.reserve(childCount);
    for (size_t i = 0; i < childCount; ++i)
    {
        auto child = std::make_unique<d7::FASTNode>();
        is >> *child;
        node.childNodes.push_back(std::move(child));
    }

    return is;
}

void d7::FASTNode::Serialize(d7::FASTNode* root, const std::string& filename)
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

d7::FASTNode d7::FASTNode::Deserialize(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    d7::FASTNode root;
    file >> root;

    file.close();
    return std::exchange(root, {});
}