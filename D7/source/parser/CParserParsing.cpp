#include "CParser.hpp"

#include "../utils/notify.hpp"

#include <stack>

using expected = d7::expected;

d7::expected d7::CParser::Run()
{
	notify_callback("Run");

	m_rootNode.type = EASTNodeType::Program;
	
	if (expected Exp = Parse_Program(); !Exp)
	{
		return expected::Fatal(("Parse_Program(): " + Exp.ExtractFatalMessageOrFail()).c_str());
	}

	if (true)
	{
		notify_trace(("Dump AST:\n "+ m_rootNode.Dump(0)).c_str());
	}

	return d7::expected::Success();
}

#define PARSE_PROGRAM_TRY(func)\
{\
	size_t m_currentLast = m_current; \
	std::unique_ptr<FASTNode> outNode;\
	if (expected Exp = func(outNode); Exp.IsFatal())\
	{\
		return expected::Fatal((Exp.ExtractFatalMessageOrFail()).c_str());\
	}\
	else if (Exp.IsSuccess())\
	{\
		m_rootNode.AddChild(std::move(outNode));\
		continue;\
	}\
	m_current = m_currentLast;\
}

d7::expected d7::CParser::Parse_Program()
{
	notify_callback("Parse_Program");

	while (LastTokens() > 0)
	{
		PARSE_PROGRAM_TRY(TryParseAs_Var)

		return expected::Fatal(("Unkown construction start with" + PeekToken().Dump()).c_str());
	}

	return d7::expected::Success();
}

d7::expected d7::CParser::TryParseAs_Var(std::unique_ptr<FASTNode>& outNode)
{
	// 1) var 2) type 3) name 4) ;
	if(LastTokens() <= 3)
		return d7::expected::Fail();

	// This not var
	if (PeekToken().type != ETokenType::DEF_VAR)
		return d7::expected::Fail();
	SkipToken();
	
	std::unique_ptr<FASTNode> Type;
	if (expected Exp = ParseAs_Type(Type); !Exp)
	{
		return d7::expected::Fatal("TryParseAs_Var(): after \"var\" " + Exp.ExtractFatalMessageOrFail());
	}

	FToken Name;
	if (expected Exp = ParseAs_Name(Name); !Exp)
	{
		return d7::expected::Fatal("TryParseAs_Var(): after var type " + Exp.ExtractFatalMessageOrFail());
	}

	if (PeekToken().type == ETokenType::SEMICOLON /* ; */)
	{
		SkipToken(); // ;

		outNode = std::make_unique<FASTNode>(EASTNodeType::Variable);
		outNode->token = std::move(Name);
		outNode->AddChild(std::move(Type));

		return d7::expected::Success();
	}
	else if(ETokenTypeUtils::IsAssign(PeekToken().type) /* = += -= */)
	{
		std::unique_ptr<FASTNode> MathBlock;
		if (expected Exp = ParseAs_BlockMath(MathBlock, ETokenType::ASSIGN /* = += -= */, ETokenType::SEMICOLON /* ; */); !Exp)
		{
			return d7::expected::Fatal("TryParseAs_Var(): " + Exp.ExtractFatalMessageOrFail());
		}

		outNode = std::make_unique<FASTNode>(EASTNodeType::Variable);
		outNode->token = std::move(Name);
		outNode->AddChild(std::move(Type));
		outNode->AddChild(std::move(MathBlock));

		return d7::expected::Success();
	}
	else
	{
		return d7::expected::Fatal(("TryParseAs_Var(): after var name was expecting a ';' or '=' token, but came across a " + PeekToken().Dump()).c_str());
	}
}

d7::expected d7::CParser::ParseAs_BlockMath(std::unique_ptr<FASTNode>& outNode, ETokenType begin, ETokenType end)
{
	if (LastTokens() <= 3)
		return d7::expected::Fail();

	if (PeekToken().type != begin)
	{
		return d7::expected::Fatal("ParseAs_BlockMath(): ");
	}
	SkipToken();

	size_t BeginShuntingYard = m_current;
	size_t EndShuntingYard  = FindSymbol(ETokenType::SEMICOLON /* ; */);
	m_current = EndShuntingYard + 1;

	if (EndShuntingYard == -1)
	{
		return d7::expected::Fatal(("ParseAs_BlockMath(): nor find closed ';' after math block. " + PeekToken().Dump()).c_str());
	}

	std::vector<FToken> Tokens;
	if (expected Exp = ParseAs_ShuntingYard(BeginShuntingYard, EndShuntingYard, Tokens); !Exp)
	{
		return d7::expected::Fatal("ParseAs_BlockMath(): " + Exp.ExtractFatalMessageOrFail());
	}

	return SY_To_BlockMath(std::move(Tokens), outNode);
}

d7::expected d7::CParser::ParseAs_Name(FToken& outToken)
{
	if (PeekToken().type != ETokenType::IDENTIFIER)
	{
		return d7::expected::Fatal(("was expecting a IDENTIFIER token, but came across a " + PeekToken().Dump()).c_str());
	}

	outToken = ConsumeToken();

	return d7::expected::Success();
}

d7::expected d7::CParser::ParseAs_Type(std::unique_ptr<FASTNode>& outNode)
{
	if (PeekToken().type != ETokenType::IDENTIFIER)
	{
		return d7::expected::Fatal(("was expecting a IDENTIFIER token, but came across a " + PeekToken().Dump()).c_str());
	}

	outNode = std::make_unique<FASTNode>(EASTNodeType::Type);
	outNode->token = ConsumeToken();

	return d7::expected::Success();
}

d7::expected d7::CParser::ParseAs_ShuntingYard(size_t from, size_t to, std::vector<FToken>& tokensOutput)
{
	tokensOutput.clear();
	std::stack<FToken> tokensStack;

	for (size_t i = from; i < to; i++)
	{
		FToken& token = m_tokens[i];

		token.Dump();

		if (ETokenTypeUtils::IsValue(token.type))
		{
			tokensOutput.emplace_back(token);
		}
		else if (ETokenTypeUtils::IsOperator(token.type))
		{
			while (!tokensStack.empty() && ETokenTypeUtils::IsOperator(tokensStack.top().type) &&
				ETokenTypeUtils::GetIntensity(tokensStack.top().type) >= ETokenTypeUtils::GetIntensity(token.type))
			{
				tokensOutput.push_back(tokensStack.top());
				tokensStack.pop();
			}

			tokensStack.push(token);
		}
		else if (token.type == ETokenType::LPAR /* ( */)
		{
			tokensStack.push(token);
		}
		else if (token.type == ETokenType::RPAR /* ) */)
		{
			while (!tokensStack.empty() && tokensStack.top().type != ETokenType::LPAR /* ( */)
			{
				tokensOutput.push_back(tokensStack.top());
				tokensStack.pop();
			}
			if (tokensStack.empty())
				return d7::expected::Fatal("ParseAs_ShuntingYard(): Mismatched parentheses");
			tokensStack.pop();
		}
		else
		{
			return d7::expected::Fatal(("ParseAs_ShuntingYard(): Unexpected token " + token.Dump()).c_str());
		}
	}

	while (!tokensStack.empty())
	{
		tokensOutput.push_back(tokensStack.top());
		tokensStack.pop();
	}

	std::cout << "Dump after ParseAs_MathShuntingYard\n";
	for (const FToken& t : tokensOutput)
	{
		std::cout << t.Dump() << '\n';
	}
	return d7::expected::Success();
}

d7::expected d7::CParser::SY_To_BlockMath(std::vector<FToken>&& tokens, std::unique_ptr<FASTNode>& node)
{
	std::stack<std::unique_ptr<FASTNode>> st;

	for (const FToken& token : tokens)
	{
		if (ETokenTypeUtils::IsValue(token.type))
		{
			auto leaf = std::make_unique<FASTNode>(EASTNodeType::Math);
			leaf->token = token;
			st.push(std::move(leaf));
		}
		else if (ETokenTypeUtils::IsOperator(token.type))
		{
			if (st.size() < 2)
			{
				return d7::expected::Fatal(("SY_To_BlockMath: Not enough operands for operator " + token.Dump()).c_str());
			}

			auto right = std::move(st.top()); st.pop();
			auto left = std::move(st.top()); st.pop();

			auto opNode = std::make_unique<FASTNode>(EASTNodeType::Math);
			opNode->token = token;
			opNode->AddChild(std::move(left));
			opNode->AddChild(std::move(right));

			st.push(std::move(opNode));
		}
		else
		{
			return d7::expected::Fatal(("SY_To_BlockMath: Unexpected token type in RPN: " + token.Dump()).c_str());
		}
	}

	if (st.size() != 1)
	{
		return d7::expected::Fatal("SY_To_BlockMath: Invalid RPN, stack size = " + std::to_string(st.size()));
	}

	node = std::make_unique<FASTNode>(EASTNodeType::BlockMath);
	node->AddChild(std::move(st.top()));

	return d7::expected::Success();
}

size_t d7::CParser::FindClosedBracket(ETokenType begin, ETokenType end)
{
	size_t counter = 1;

	for (size_t i = m_current; m_current < m_tokens.size(); i++)
	{
		if (m_tokens[i].type == begin)
		{
			++counter;

		}
		else if (m_tokens[i].type == end)
		{
			--counter;

			if (counter <= 0)
			{
				return i;
			}
		}
	}

	return -1;
}

size_t d7::CParser::FindSymbol(ETokenType type)
{
	for (size_t i = m_current; m_current < m_tokens.size(); i++)
	{
		if (m_tokens[i].type == type)
		{
			return i;
		}
	}

	return -1;
}
