#pragma once

#include <string>

#include "ETokenType.hpp"

namespace d7
{
	struct Token
	{
		std::string lexeme = "";
		d7::ETokenType type = d7::ETokenType::NONE;
	};
}
