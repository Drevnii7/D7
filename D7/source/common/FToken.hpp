#pragma once

#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>

#include "ETokenType.hpp"
#include "../utils/expected.hpp"

namespace d7
{
	struct FToken
	{
		std::string lexeme = "";
		d7::ETokenType type = d7::ETokenType::NONE;

		size_t line = 0;
		size_t row = 0;

		FToken() = default;
		FToken(size_t Line, size_t Row, std::string_view Lexeme)
		{
			line = Line;
			row = Row;
			type = d7::ETokenTypeUtils::ETokenTypeFromString(Lexeme);
			lexeme = Lexeme;
		}

		std::string Dump() const;

		bool IsValid() const
		{
			return type != d7::ETokenType::NONE;
		}

		friend std::ostream& operator<<(std::ostream& os, const FToken& token);
		friend std::istream& operator>>(std::istream& is, FToken& token);

		template <typename Container>
		static d7::expected Serialize(const Container& Tokens, const std::string& FilePath)
		{
			std::ofstream file(FilePath, std::ios::binary | std::ios::trunc);
			if (!file.is_open())
			{
				return d7::expected::Fatal("Error opening file for writing");
			}

			size_t count = Tokens.size();
			file.write(reinterpret_cast<const char*>(&count), sizeof(count));

			for (const auto& token : Tokens)
			{
				d7::operator<<(file, token);
				if (!file.good())
				{
					return d7::expected::Fatal("Error writing token to file");
				}
			}

			return d7::expected::Success();
		}

		template <typename Container>
		static d7::expected Deserialize(Container& Tokens, const std::string& FilePath)
		{
			std::ifstream file(FilePath, std::ios::binary);
			if (!file.is_open())
				return d7::expected::Fatal("Error opening file for reading");

			size_t count = 0;
			file.read(reinterpret_cast<char*>(&count), sizeof(count));
			if (!file.good() || count > 1000000) 
				return d7::expected::Fatal("Invalid token count");

			Tokens.clear();

			if constexpr (requires { Tokens.reserve(count); }) {
				Tokens.reserve(count);
			}

			for (size_t i = 0; i < count; ++i)
			{
				FToken token;

				d7::operator>>(file, token);

				if (!file.good())
				{
					return d7::expected::Fatal("Error reading token at index " + std::to_string(i));
				}

				Tokens.push_back(std::move(token));
			}

			return d7::expected::Success();
		}
	};

	std::ostream& operator<<(std::ostream& os, const FToken& token);
	std::istream& operator>>(std::istream& is, FToken& token);
}
