#pragma once

#include "../common/FASTNode.hpp"
#include "../utils/expected.hpp"

#include <vector>
#include <string_view>
#include <list>

namespace d7
{
	class CParser
	{
	public:

		CParser() = default;

		// Specify together with the file type
		// Return only "Succes" and "Fatal"
		d7::expected LoadTokens(std::string_view FilePath);
		// Specify without the file extension
		// Return only "Succes" and "Fatal"
		d7::expected SaveAST(std::string_view FilePath);

		void SetTokens(std::list<d7::FToken>&& Tokens);
		void SetTokens(std::vector<d7::FToken>&& Tokens);

		d7::expected Run();

		void Reset();

	protected:

		std::vector<d7::FToken> m_tokens;
		FASTNode m_rootNode;
		size_t m_current = 0;


		d7::expected Parse_Program();

		// Height level


		d7::expected TryParseAs_Var(std::unique_ptr<FASTNode>& outNode);

		// Medium level

		d7::expected ParseAs_BlockMath(std::unique_ptr<FASTNode>& outNode, ETokenType begin, ETokenType end);

		// Low level

		d7::expected ParseAs_Name(FToken& outToken);
		d7::expected ParseAs_Type(std::unique_ptr<FASTNode>& outNode);


		d7::expected ParseAs_ShuntingYard(size_t from, size_t to, std::vector<FToken>& tokensOutput);
		d7::expected SY_To_BlockMath(std::vector<FToken>&& tokens, std::unique_ptr<FASTNode>& node);

		size_t FindClosedBracket(ETokenType begin, ETokenType end);
		size_t FindSymbol(ETokenType type);

		// return m_current
		const d7::FToken& PeekToken();
		// return m_current++
		const d7::FToken& ConsumeToken();
		// return m_tokens.size() - m_current
		size_t LastTokens();
		// m_current++
		bool SkipToken();

		
	};
}
