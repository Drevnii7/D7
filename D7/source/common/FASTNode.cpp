#include "FASTNode.hpp"

#include <ostream>
#include <istream>

using expected = d7::expected;

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

expected d7::FASTNode::Serialize(const FASTNode& Root, const std::string& FilePath)
{
    std::ofstream file(FilePath, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        return expected::Fatal(("Cannot open file for writing: " + FilePath).c_str());
    }

    file << Root;

    file.close();

    return expected::Success();
}

expected d7::FASTNode::Deserialize(FASTNode& Root, const std::string& FilePath)
{
    std::ifstream file(FilePath, std::ios::binary);
    if (!file.is_open())
    {
        return expected::Fatal(("Cannot open file for reading: " + FilePath).c_str());
    }

    file >> Root;

    file.close();

    return expected::Success();
}