#pragma once
#include "../service/IBaseService.h"
#include "../parser/AST.h"

class ICodeGenerator : public IBaseService
{
public: // CBaseService

	// virtual bool Main(int argc, char* argv[]);

	virtual void RunProcessing() override = 0;

	virtual bool RunFullCycle() override;

	virtual void DebugPrint() const override = 0;

	// Reset state
	virtual void Reset() override = 0;

protected: // CBaseService

	virtual void Fatal(const std::string& message) const override;
	virtual void Error(const std::string& message) const override;
	virtual void Warning(const std::string& message) const override;
	virtual void Success(const std::string& message) const override;

	// std::string m_inputFilePath = "";
	// std::string m_outputFilePath = "";

public: // ICodeGenerator

	[[nodiscard]] bool LoadAST(const std::string& filePath = "");
	[[nodiscard]] virtual bool SaveProgram(const std::string& filePath = "") = 0;

	// Set tokens raw
	void SetAST(const FASTNode&& AST);

protected: // ICodeGenerator
	FASTNode m_rootNode;
};

