#pragma once

#include <string>

#include "ETokenType.hpp"

struct Token
{
	std::string Lexeme = "";
	ETokenType Type = ETokenType::None;
};
