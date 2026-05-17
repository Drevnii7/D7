#pragma once

#include <cstdint>
#include <string_view>

namespace d7
{
	enum class EASTNodeType : uint16_t
	{
        None = 0, // Invalid

        Program   = 1,
        Namespace = 2, // namespace {}

        BlockCode       = 100, // func {}
        BlockScope      = 101, // {}
        BlockMath       = 102, // 42 + (4 * VarX)
        BlockArgs       = 103, // (int argc, char *argv[])
        BlockAgreements = 104, // [ ]

        Func     = 201, // func void main();
        Variable = 202, // var float pi = 3.14f;
        Setter   = 203, // pi = 3.14f;
        Call     = 204, // parse();

        Name = 301, // name variable / function
        Type = 302, // type variable / type returned function
        Math = 303, // (8 * 5) + (4 / 2)

        Return = 400 // return 42;
	};

    namespace EASTNodeTypeUtils
    {
        std::string_view EASTNodeTypeToString(EASTNodeType Type);
        EASTNodeType EASTNodeTypeFromString(std::string_view String);
    }
}
