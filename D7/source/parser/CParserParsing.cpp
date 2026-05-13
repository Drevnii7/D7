#include "CParser.h"
#include "../Notify/Notify.h"
#include <cctype>


// Global static func
// Global var
void CParser::TryParseAs_Program()
{
	int current = 0;

	while (current < m_tokens.size())
	{
		std::unique_ptr<FASTNode> ParsedNode = nullptr;


		{
			CParser::URetState StateRet = TryParseAs_Func(current, ParsedNode);

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
			CParser::URetState StateRet = TryParseAs_Var(current, ParsedNode);

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

CParser::URetState CParser::TryParseAs_Func(int& current, std::unique_ptr<FASTNode>& Node)
{
	int l_current = current;
	
	// func
	if (m_tokens[l_current].Type != UTokenType::DEF_FUNC)
	{
		return CParser::URetState::No;
	}
	l_current++;
		
	// THIS FUNCTUIN! Later all return error

	// int32
	if (m_tokens[l_current].Type != UTokenType::IDENTIFIER)
	{
		Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 1. Miss function return type");
		return CParser::URetState::Fail;
	}
	l_current++;

	// main
	if (m_tokens[l_current].Type != UTokenType::IDENTIFIER)
	{
		Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 2. Miss function name");
		return CParser::URetState::Fail;
	}
	l_current++;

	// (
	if (m_tokens[l_current].Type != UTokenType::LPAR)
	{
		Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 3. Miss (");
		return CParser::URetState::Fail;
	}
	l_current++;

	// int argc, char *argv[]
	std::unique_ptr<FASTNode> Node_Args = nullptr;
	if (!ParseAs_BlockArgs(l_current, Node_Args) || Node_Args == nullptr)
	{
		Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 4. Arguments fail parse");
		return CParser::URetState::Fail;
	}
	l_current++;

	// )
	if (m_tokens[l_current].Type != UTokenType::RPAR)
	{
		Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 5. Miss ) after args");
		return CParser::URetState::Fail;
	}
	l_current++;

	// Only define without implementation
	if (m_tokens[l_current].Type == UTokenType::SEMICOLON)
	{
		l_current++;
		current = l_current;
		
		{
			Node = std::make_unique<FASTNode>(UNodeType::BlockCode);
			Node->AddChild(std::move(Node_Args));
		} // Formitare return block

		return CParser::URetState::Yes;
	}
	// Define with implementation
	else if(m_tokens[l_current].Type == UTokenType::LBRA)
	{
		std::unique_ptr<FASTNode> Node_Block = nullptr;
		if (!ParseAs_BlockCode(l_current, Node_Block) || Node_Block == nullptr)
		{
			Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 6. Fail parse block {}");
			return CParser::URetState::Fail;
		}

		{ // Formitare return block
			Node = std::make_unique<FASTNode>(UNodeType::Func);
			Node->AddChild(std::move(Node_Args));
			Node->AddChild(std::move(Node_Block));
		}

		l_current++;
		current = l_current;
		return CParser::URetState::Yes;
	}
	// Next token not end define and not start implementation
	else
	{
		// Err
		Error(std::to_string(m_tokens[l_current].Line) + ": define function error. Step 6. Not end define and not start implementation");
		return CParser::URetState::Fail;
	}
}

CParser::URetState CParser::TryParseAs_Var(int& current, std::unique_ptr<FASTNode>& Node)
{
	return CParser::URetState::No;
}

bool CParser::ParseAs_BlockArgs(int& current, std::unique_ptr<FASTNode>& Node)
{
	Node = std::make_unique<FASTNode>(UNodeType::BlockArgs);

	return true;
}

bool CParser::ParseAs_BlockCode(int& current, std::unique_ptr<FASTNode>& Node)
{
	Node = std::make_unique<FASTNode>(UNodeType::BlockCode);

	return true;
}

bool CParser::ParseAs_Name(int& current, std::unique_ptr<FASTNode>& Node)
{
	if (m_tokens[current].Type != UTokenType::IDENTIFIER)
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::Name);
	Node->Token = m_tokens[current];

	return true;
}

bool CParser::ParseAs_Type(int& current, std::unique_ptr<FASTNode>& Node)
{
	if (m_tokens[current].Type != UTokenType::IDENTIFIER)
		return false;

	Node = std::make_unique<FASTNode>(UNodeType::Type);
	Node->Token = m_tokens[current];

	return true;
}
