#pragma once
#include <string>

class CBaseService
{
public:

	CBaseService() = default;
	virtual ~CBaseService() = default;

	// Example work as a tool with command line arguments
	virtual bool Main(int argc, char* argv[]);

	virtual void RunProcessing() = 0;
	virtual bool RunFullCycle() = 0;

	virtual void DebugPrint() const {};

	virtual void Reset() = 0;

protected:

	virtual void Fatal(const std::string& message) const = 0;
	virtual void Error(const std::string& message) const = 0;
	virtual void Warning(const std::string& message) const = 0;
	virtual void Success(const std::string& message) const = 0;

	std::string m_inputFilePath = "";
	std::string m_outputFilePath = "";
};
