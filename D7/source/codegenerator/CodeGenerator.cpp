#include "CodeGenerator.h"
#include "../notify/Notify.h"

bool ICodeGenerator::LoadAST(const std::string& filePath)
{
	return false;
}

bool ICodeGenerator::SaveProgram(const std::string& filePath)
{
	return false;
}

void ICodeGenerator::SetAST(const FASTNode&& AST)
{
	m_rootNode = std::move(AST);
}

bool ICodeGenerator::RunFullCycle()
{
	if (!LoadAST())
		return false;

	RunProcessing();

	if (!SaveProgram())
		return false;

	return true;
}

void ICodeGenerator::Fatal(const std::string& message) const
{
	FatalBase(UNotifyFrom::CODE_GENERATION, message);
}

void ICodeGenerator::Error(const std::string& message) const
{
	ErrorBase(UNotifyFrom::CODE_GENERATION, message);
}

void ICodeGenerator::Warning(const std::string& message) const
{
	WarningBase(UNotifyFrom::CODE_GENERATION, message);
}

void ICodeGenerator::Success(const std::string& message) const
{
	SuccessBase(UNotifyFrom::CODE_GENERATION, message);
}
