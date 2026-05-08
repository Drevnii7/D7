#include "CParser.h"
#include "../Notify/Notify.h"

void CParser::RunProcessing()
{

}

bool CParser::RunFullCycle()
{
	if (!LoadTokens())
		return false;

	RunProcessing();

	if (!SaveAST())
		return false;

	return true;
}

void CParser::DebugPrint() const
{
	Warning("DebugPrint()");
	std::cout << m_rootNode.Dump() << '\n';
}

void CParser::Reset()
{
	m_inputFilePath = "";
	m_outputFilePath = "";
	m_tokens.clear();

	m_rootNode.ChildNodes.clear();
	m_rootNode.Type = UNodeType::None;
	m_rootNode.Token.Lexeme = "";
	m_rootNode.Token.Type = UTokenType::NONE;
	m_tokens.clear();
}

bool CParser::LoadTokens(const std::string& filePath)
{
	if (!filePath.empty()) { m_inputFilePath = filePath; }

	try
	{
		m_tokens = FToken::Deserialize<std::vector<FToken>>(m_inputFilePath);
	}
	catch (const std::runtime_error& error)
	{
		Error("LoadTokens: " + std::string(error.what()));
		return false;
	}

	return true;
}

bool CParser::SaveTokens(const std::string& filePath)
{
	if (!filePath.empty()) { m_outputFilePath = filePath; }

	try
	{
		FToken::Serialize<std::vector<FToken>>(m_tokens, m_outputFilePath);
	}
	catch (const std::runtime_error& error)
	{
		Error("SaveTokens: " + std::string(error.what()));
		return false;
	}

	return true;
}

bool CParser::SaveAST(const std::string& filePath)
{
	if (!filePath.empty()) { m_outputFilePath = filePath; }

	try
	{
		FASTNode::Serialize(&m_rootNode, m_outputFilePath);
	}
	catch (const std::runtime_error& error)
	{
		Error("SaveAST: " + std::string(error.what()));
		return false;
	}

	return true;
}


void CParser::SetTokens(const std::vector<FToken>& tokens)
{
	Reset();

	m_inputFilePath = "CParser::SetCode()";
	m_tokens = tokens;
}

void CParser::SetTokens(const std::list<FToken>& tokens)
{
	Reset();

	m_inputFilePath = "CParser::SetTokens()";
	m_tokens.assign(tokens.begin(), tokens.end());
}

void CParser::SetTokens(std::vector<FToken>&& tokens)
{
	Reset();

	m_inputFilePath = "CParser::SetCode()";
	m_tokens = std::move(tokens);
}

void CParser::Fatal(const std::string& message) const
{
	FatalBase(UNotifyFrom::PARSER, message);
}

void CParser::Error(const std::string& message) const
{
	ErrorBase(UNotifyFrom::PARSER, message);
}

void CParser::Warning(const std::string& message) const
{
	WarningBase(UNotifyFrom::PARSER, message);
}

void CParser::Success(const std::string& message) const
{
	SuccessBase(UNotifyFrom::PARSER, message);
}
