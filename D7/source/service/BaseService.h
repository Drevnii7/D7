#pragma once
#include <string>

class CBaseService
{
public:

	CBaseService() = default;
	virtual ~CBaseService() = default;

	// Example work as a tool with command line arguments
	virtual bool Main(int argc, char* argv[]) = 0;

	virtual void Run() = 0;

	virtual void DebugPrint() const = 0;

	virtual void Reset() = 0;
};