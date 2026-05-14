#include "CParser.h"
#include "../Notify/Notify.h"
#include <cctype>
#include <stack>

// Global static func
// Global var
void CParser::TryParseAs_Program()
{
	int l_current = 0;

	std::unique_ptr<FASTNode> ParsedNode = nullptr;
	while (l_current < m_tokens.size())
	{
		ParsedNode = nullptr;
		int l_currentSave = l_current;

		{
			l_current = l_currentSave;
			CParser::URetState StateRet = TryParseAs_Func(l_current, ParsedNode);

			if (StateRet == CParser::URetState::Yes)
			{
				m_rootNode.AddChild(std::move(ParsedNode));
				continue;
			}
			else if(StateRet == CParser::URetState::Fail)
			{
				Error(std::string("TryParseAs_Program:TryParseAs_Func"));
				return;
			}
		}

		{
			l_current = l_currentSave;
			CParser::URetState StateRet = TryParseAs_Var(l_current, ParsedNode);

			if (StateRet == CParser::URetState::Yes)
			{
				m_rootNode.AddChild(std::move(ParsedNode));
				continue;
			}
			else if (StateRet == CParser::URetState::Fail)
			{
				Error(std::string("TryParseAs_Program:TryParseAs_Var"));
				return;
			}
		}

		// Error
		// Can't parse
		return;
	}
}

// Height level

CParser::URetState CParser::TryParseAs_Func(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	m_tokens; // For debug

	/* func */
	if (!ConsumeToken(l_current, UTokenType::DEF_FUNC))
		return CParser::URetState::No; // This not function

	/* int32 */
	std::unique_ptr<FASTNode> Node_Type;
	if (!ParseAs_Type(l_current, Node_Type) || Node_Type == nullptr)
		return CParser::URetState::Fail;

	/* main */
	std::unique_ptr<FASTNode> Node_Name;
	if (!ParseAs_Name(l_current, Node_Name) || Node_Name == nullptr)
		return CParser::URetState::Fail;

	/* (int argc, char *argv[]) */

	std::unique_ptr<FASTNode> Node_Args;
	if (!ParseAs_BlockArgs(l_current, Node_Args) || Node_Args == nullptr)
		return CParser::URetState::Fail;

	// Define only, without implementation ;
	if (ConsumeToken(l_current, UTokenType::SEMICOLON /* ; */))
	{
		Node = std::make_unique<FASTNode>(UNodeType::Func);
		Node->AddChild(std::move(Node_Type));
		Node->AddChild(std::move(Node_Name));
		Node->AddChild(std::move(Node_Args));

		return CParser::URetState::Yes;
	}
	// Define with implementation {
	else if(PeekToken(l_current, UTokenType::LBRA /* { */))
	{
		std::unique_ptr<FASTNode> Node_Block;
		if (!ParseAs_BlockCode(l_current, Node_Block) || Node_Block == nullptr)
			return CParser::URetState::Fail;

		Node = std::make_unique<FASTNode>(UNodeType::Func);
		Node->AddChild(std::move(Node_Type));
		Node->AddChild(std::move(Node_Name));
		Node->AddChild(std::move(Node_Args));
		Node->AddChild(std::move(Node_Block));

		return CParser::URetState::Yes;
	}

	// 26: After void main(int argc, char *argv[])  must be ';' or \"{}\". Now end on: ")"
	Error(std::to_string(Node_Type->Token.Line) + ": After " + Node_Type->Token.Lexeme + ' ' + Node_Name->Token.Lexeme + ' ' +
		Node_Args->Token.Lexeme + " must be ';' or \"{}\". Now end on: \"" + m_tokens[l_current].Lexeme + '\"');

	return CParser::URetState::Fail;
}

CParser::URetState CParser::TryParseAs_Var(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	m_tokens; // For debug

	/* func */
	if (!ConsumeToken(l_current, UTokenType::DEF_VAR))
		return CParser::URetState::No; // This not variable

	/* int32 */
	std::unique_ptr<FASTNode> Node_Type;
	if (!ParseAs_Type(l_current, Node_Type) || Node_Type == nullptr)
		return CParser::URetState::Fail;

	/* main */
	std::unique_ptr<FASTNode> Node_Name;
	if (!ParseAs_Name(l_current, Node_Name) || Node_Name == nullptr)
		return CParser::URetState::Fail;

	// Define only, without implementation ;
	if (ConsumeToken(l_current, UTokenType::SEMICOLON /* ; */))
	{
		Node = std::make_unique<FASTNode>(UNodeType::Variable);
		Node->AddChild(std::move(Node_Type));
		Node->AddChild(std::move(Node_Name));

		return CParser::URetState::Yes;
	}
	// Define with implementation {
	else if (PeekToken(l_current, UTokenType::ASSIGN /* = */))
	{
		std::unique_ptr<FASTNode> Node_Math;
		if (!ParseAs_BlockMath(l_current, Node_Math) || Node_Math == nullptr)
			return CParser::URetState::Fail;
		
		Node = std::make_unique<FASTNode>(UNodeType::Variable);
		Node->AddChild(std::move(Node_Type));
		Node->AddChild(std::move(Node_Name));
		Node->AddChild(std::move(Node_Math));

		return CParser::URetState::Yes;
	}

	// 26: After var world must be ';' or "=". Now end on: ")"
	Error(std::to_string(Node_Type->Token.Line) + ": After " + Node_Type->Token.Lexeme + ' ' + Node_Name->Token.Lexeme + " must be ';' or \'=\'. Now end on: \"" + m_tokens[l_current].Lexeme + '\"');

	return CParser::URetState::Fail;
}

CParser::URetState CParser::TryParseAs_Return(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	m_tokens; // For debug

	/* func */
	if (!ConsumeToken(l_current, UTokenType::RETURN))
		return CParser::URetState::No; // This not return

	int indexFrom = l_current;

	while (!PeekToken(l_current, UTokenType::SEMICOLON /* ; */)) l_current++;

	std::vector<FToken> Tokens;
	if (!ParseAs_MathShuntingYard(indexFrom, l_current, Tokens))
		return CParser::URetState::Fail;

	Node = std::make_unique<FASTNode>(UNodeType::Return);
	std::unique_ptr<FASTNode> ParsedNode = nullptr;

	if (!MSY_To_BlockMath(std::move(Tokens), ParsedNode))
		return CParser::URetState::Fail;

	Node->AddChild(std::move(ParsedNode));
		
	l_current++; // Skip ;

	return CParser::URetState::Yes;
}

// Medium level

bool CParser::ParseAs_BlockCode(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	if (!ConsumeTokenThrow(l_current, UTokenType::LBRA /* { */, "ParseAs_BlockCode. Miss \'{\'"))
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::BlockCode);

	std::unique_ptr<FASTNode> ParsedNode = nullptr;

	int CountBra = 1;
	while (l_current < m_tokens.size())
	{
		ParsedNode = nullptr;
		int l_currentSave = l_current;

		if (PeekToken(l_current, UTokenType::LBRA /* { */))
		{
			CountBra++;
			l_current++;
			continue;
		}

		if (PeekToken(l_current, UTokenType::RBRA /* } */))
		{
			CountBra--;
			l_current++;
			continue;
		}

		if (CountBra == 0)
		{
			break;
		}

		{
			l_current = l_currentSave;
			CParser::URetState StateRet = TryParseAs_Var(l_current, ParsedNode);

			if (StateRet == CParser::URetState::Yes)
			{
				Node->AddChild(std::move(ParsedNode));
				continue;
			}
			else if (StateRet == CParser::URetState::Fail)
			{
				Error(std::string("ParseAs_BlockCode:TryParseAs_Var"));
				return false;
			}
		}
		
		{
			l_current = l_currentSave;
			CParser::URetState StateRet = TryParseAs_Return(l_current, ParsedNode);

			if (StateRet == CParser::URetState::Yes)
			{
				Node->AddChild(std::move(ParsedNode));
				continue;
			}
			else if (StateRet == CParser::URetState::Fail)
			{
				Error(std::string("ParseAs_BlockCode:TryParseAs_Return"));
				return false;
			}
		}

		Error(std::string("ParseAs_BlockCode: Unkown what: ") + m_tokens[l_current].Dump());


		break;
	}

	return true;
}

bool CParser::ParseAs_BlockArgs(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	if (!ConsumeTokenThrow(l_current, UTokenType::LPAR /* ( */, "ParseAs_BlockArgs. Miss \'(\'"))
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::BlockArgs);

	while (!ConsumeToken(l_current, UTokenType::RPAR /* ) */));

	return true;
}

bool CParser::ParseAs_BlockMath(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	if (!ConsumeTokenThrow(l_current, UTokenType::ASSIGN /* = */, "ParseAs_BlockArgs. Miss \'=\'"))
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::BlockMath);

	int indexFrom = l_current;
	
	while (!PeekToken(l_current, UTokenType::SEMICOLON /* ; */)) l_current++;

	std::vector<FToken> Tokens;
	if (!ParseAs_MathShuntingYard(indexFrom, l_current, Tokens))
		return false;

	if (!MSY_To_BlockMath(std::move(Tokens), Node))
		return false;

	l_current++; // Skip ;

	return true;
}

// Low level

bool CParser::ParseAs_Name(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	if (!PeekTokenThrow(l_current, UTokenType::IDENTIFIER, "ParseAs_Name fail, token index: " + std::to_string(l_current) + ". Dump: " + m_tokens[l_current].Dump()))
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::Name);
	Node->Token = m_tokens[l_current];
	l_current++;

	return true;
}

bool CParser::ParseAs_Type(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	if (!PeekTokenThrow(l_current, UTokenType::IDENTIFIER, "ParseAs_Type fail, token index: " + std::to_string(l_current) + ". Dump: " + m_tokens[l_current].Dump()))
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::Type);
	Node->Token = m_tokens[l_current];

	l_current++;
	return true;
}

bool CParser::ParseAs_Arg(int& l_current, std::unique_ptr<FASTNode>& Node)
{
	return true;
}

bool IsOperator(const FToken& Token)
{
	return
		Token.Type == UTokenType::ADD ||
		Token.Type == UTokenType::SUB ||
		Token.Type == UTokenType::MUL ||
		Token.Type == UTokenType::DIV ||
		Token.Type == UTokenType::MOD;
}
bool IsValue(const FToken& Token)
{
	return
		Token.Type == UTokenType::INT ||
		Token.Type == UTokenType::FLOAT ||
		Token.Type == UTokenType::STRING ||
		Token.Type == UTokenType::CHAR ||
		Token.Type == UTokenType::BOOLEAN ||
		Token.Type == UTokenType::IDENTIFIER;
}
int GetIntensity(const FToken& Token)
{
	switch (Token.Type)
	{
	case UTokenType::ADD: return 2;
	case UTokenType::SUB: return 2;
	case UTokenType::MUL: return 3;
	case UTokenType::DIV: return 3;
	case UTokenType::MOD: return 3;
	default: return 0;
	}
}

bool CParser::ParseAs_MathShuntingYard(int from, int to, std::vector<FToken>& TokensOutput)
{
	TokensOutput.empty();
	std::stack<FToken> TokensStack;

	for(int i = from; i < to; i++)
	{
		FToken& Token = m_tokens[i];

		Token.Dump();

		if (IsValue(Token))
		{
			TokensOutput.emplace_back(Token);
		}
		else if (IsOperator(Token))
		{
			while (!TokensStack.empty() && IsOperator(TokensStack.top()) &&
				GetIntensity(TokensStack.top()) >= GetIntensity(Token))
			{
				TokensOutput.push_back(TokensStack.top());
				TokensStack.pop();
			}

			TokensStack.push(Token);
		}
		else if (Token.Type == UTokenType::LPAR /* ( */)
		{
			TokensStack.push(Token);
		}
		else if (Token.Type == UTokenType::RPAR /* ) */)
		{
			while (!TokensStack.empty() && TokensStack.top().Type != UTokenType::LPAR /* ( */)
			{
				TokensOutput.push_back(TokensStack.top());
				TokensStack.pop();
			}
			if (TokensStack.empty()) return false;
			TokensStack.pop();
		}
		else
		{
			// ERR
			return false;
		}
	}

	while (!TokensStack.empty())
	{
		TokensOutput.push_back(TokensStack.top());
		TokensStack.pop();
	}

	std::cout << "Dump after ParseAs_MathShuntingYard\n";
	for (const FToken& t : TokensOutput)
	{
		std::cout << t.Dump() << '\n';
	}
	return true;
}

bool CParser::MSY_To_BlockMath(std::vector<FToken>&& Tokens, std::unique_ptr<FASTNode>& Node)
{
	std::stack<std::unique_ptr<FASTNode>> st;

	for (const FToken& token : Tokens)
	{
		if (IsValue(token))
		{
			auto leaf = std::make_unique<FASTNode>(UNodeType::Math);
			leaf->Token = token;
			st.push(std::move(leaf));
		}
		else if (IsOperator(token)) 
		{
			if (st.size() < 2)
			{
				Error("MSY_To_BlockMath: Not enough operands for operator " + token.Lexeme);
				return false;
			}

			auto right = std::move(st.top()); st.pop();
			auto left = std::move(st.top()); st.pop();

			auto opNode = std::make_unique<FASTNode>(UNodeType::Math);
			opNode->Token = token;
			opNode->AddChild(std::move(left));
			opNode->AddChild(std::move(right));

			st.push(std::move(opNode));
		}
		else
		{
			Error("MSY_To_BlockMath: Unexpected token type in RPN: " + token.Lexeme);
			return false;
		}
	}

	if (st.size() != 1)
	{
		Error("MSY_To_BlockMath: Invalid RPN, stack size = " + std::to_string(st.size()));
		return false;
	}

	Node = std::make_unique<FASTNode>(UNodeType::BlockMath);
	Node->AddChild(std::move(st.top()));

	return true;
}

// Raw level

bool CParser::PeekToken(int& l_current, UTokenType TokenType)
{
	return m_tokens[l_current].Type == TokenType;
}

bool CParser::PeekTokenThrow(int& l_current, UTokenType TokenType, std::string_view failMessage)
{
	if (m_tokens[l_current].Type == TokenType)
	{
		return true;
	}

	Error(std::to_string(m_tokens[l_current].Line) + ": " + std::string(failMessage));
	return false;
}

bool CParser::ConsumeToken(int& l_current, UTokenType TokenType)
{
	if (m_tokens[l_current].Type == TokenType)
	{
		l_current++;
		return true;
	}
	
	return false;
}

bool CParser::ConsumeTokenThrow(int& l_current, UTokenType TokenType, std::string_view failMessage)
{
	if (m_tokens[l_current].Type == TokenType)
	{
		l_current++;
		return true;
	}
	
	Error(std::to_string(m_tokens[l_current].Line) + ": " + std::string(failMessage));
	return false;
}
