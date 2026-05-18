#pragma once

#include <string>
#include <array>

#include "../utils/expected.hpp"

namespace d7
{
	class CService
	{
	public:
		struct CServiceConfig
		{
			std::string FilePath_Lexer_In = "";
			std::string FilePath_Lexer_Out = "";

			std::string FilePath_Preprocessor_In = "";
			std::string FilePath_Preprocessor_Out = "";

			std::string FilePath_Parser_In = "";
			std::string FilePath_Parser_Out = "";

			// 0) Trace, 1) Callback, 2) Info, 3) Warning
			std::array<bool, 4> MaskNotify = { 1, 1, 1, 1 };
			// 0) Lexer, 2) Preprocessor, 2) Parser
			std::array<bool, 3> MaskWork = { 0, 0, 0};

			bool RunTests = false;

			//bool KeepTempFiles = false;
		};

		void SetConfig(const CServiceConfig& NewConfig);
		void SetConfig(CServiceConfig&& NewConfig);
		d7::expected SetConfig(int argc, char* argv[]);

		d7::expected Run();

	private:
		CServiceConfig m_config;
	};
}
