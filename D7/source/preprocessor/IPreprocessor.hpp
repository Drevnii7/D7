#pragma once

#include "../common/ETokenType.hpp"
#include "../common/FToken.hpp"

#include "../utils/assert.hpp"
#include "../utils/expected.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <list>

namespace d7
{
	class IPreprocessor
	{
	public:

		IPreprocessor() = default;
		virtual ~IPreprocessor() = default;

		// Specify together with the file type
		// Return only "Succes" and "Fatal"
		d7::expected LoadTokens(std::string_view FilePath);
		// Specify without the file extension
		// Return only "Succes" and "Fatal"
		d7::expected SaveTokens(std::string_view FilePath);

		void SetTokens(std::list<d7::FToken>&& Tokens);
		void SetTokens(std::vector<d7::FToken>&& Tokens);
		// Before call ExtractTokens() you must call Run()
		std::list<d7::FToken> ExtractTokens();

		// You MUST call add to YOUR code:
		// notify_callback("Run");
		// mb_runCall = true;
		virtual d7::expected Run() = 0;

		virtual void Reset();

	protected:

		/*
			The preprocessor runs in multiple passes. I once wrote a macro so tangled
			that it took the poor preprocessor 23 steps to expand it. Shifting the entire
			code 23 times would have killed the machine, but std::list saves the day.
		*/
		std::list<d7::FToken> m_tokens;
		bool mb_runCall = false;
	};
}
