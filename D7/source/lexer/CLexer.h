#pragma once

#include "../common/ETokenType.hpp"
#include "../common/FToken.hpp"

#include "../utils/assert.hpp"
#include "../utils/expected.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace d7
{
	class CLexer
	{
	public:
		
		CLexer() = default;
		~CLexer() = default;

		// Specify together with the file type
		// Return only "Succes" and "Fatal"
		d7::expected::expected LoadCode(std::string_view FilePath);
		// Specify without the file extension
		// Return only "Succes" and "Fatal"
		d7::expected::expected SaveTokens(std::string_view FilePath);

		void SetCode(std::string&& Code);
		// Before call ExtractTokens() you must call Run()
		std::vector<FToken> ExtractTokens();

		void Run();

		void Reset();

	private:
		std::string m_code;
		std::vector<FToken> m_tokens;
	};
}
