#pragma once

#include "assert.hpp"

#include <string>
#include <memory>
#include <cstdint>

namespace d7
{
	namespace expected
	{
		enum class EStatus : uint8_t
		{
			Fail    = 0,
			Success = 1,
			Fatal   = 2
		};

		class expected
		{
		private:
			EStatus Status = EStatus::Fail;
			std::unique_ptr<std::string> FatalMessagePtr;

		public:
			// Default: Success
			expected() noexcept
				: Status(EStatus::Success)
			{
			}

			// Bool constructor
			explicit expected(bool Success) noexcept
				: Status(Success ? EStatus::Success : EStatus::Fail)
			{
			}

			// Fatal from rvalue
			expected(std::string&& ErrorMessage)
				: Status(EStatus::Fatal)
				, FatalMessagePtr(std::make_unique<std::string>(std::move(ErrorMessage)))
			{
			}

			// Fatal from lvalue
			expected(const std::string& ErrorMessage)
				: Status(EStatus::Fatal)
				, FatalMessagePtr(std::make_unique<std::string>(ErrorMessage))
			{
			}

			// Copy constructor
			expected(const expected& Other)
				: Status(Other.Status)
				, FatalMessagePtr(Other.FatalMessagePtr ? std::make_unique<std::string>(*Other.FatalMessagePtr) : nullptr)
			{
			}

			// Move constructor
			expected(expected&& Other) noexcept
				: Status(Other.Status)
				, FatalMessagePtr(std::move(Other.FatalMessagePtr))
			{
				Other.Status = EStatus::Fail;
			}

			// Destructor
			~expected() = default;

			// Copy assignment
			expected& operator=(const expected& Other)
			{
				if (this != &Other)
				{
					Status = Other.Status;
					if (Other.FatalMessagePtr)
					{
						FatalMessagePtr = std::make_unique<std::string>(*Other.FatalMessagePtr);
					}
					else
					{
						FatalMessagePtr = nullptr;
					}
				}
				return *this;
			}

			// Move assignment
			expected& operator=(expected&& Other) noexcept
			{
				if (this != &Other)
				{
					Status = Other.Status;
					FatalMessagePtr = std::move(Other.FatalMessagePtr);
					Other.Status = EStatus::Fail;
				}
				return *this;
			}

			bool IsSuccess() const noexcept
			{
				return Status == EStatus::Success;
			}

			bool IsFail() const noexcept
			{
				return Status == EStatus::Fail;
			}

			bool IsFatal() const noexcept
			{
				return Status == EStatus::Fatal;
			}

			const std::string& FatalMessage() const
			{
				assert(IsFatal(), "Try get fatal message from \"expected\", but his type != fatal. Check your code. Add IsFatal() befor call FatalMessage().");
				assert(FatalMessagePtr != nullptr, "Fatal status set but message pointer is null :\\");
				return *FatalMessagePtr;
			}

			std::string ExtractFatalMessage()
			{
				assert(IsFatal(), "Try extract fatal message from \"expected\", but his type != fatal. Check your code. Add IsFatal() befor call FatalMessage().");
				assert(FatalMessagePtr != nullptr, "Fatal status set but message pointer is null :\\");

				std::string Msg = std::move(*FatalMessagePtr);

				FatalMessagePtr = nullptr;
				Status = EStatus::Fail;

				return Msg;
			}
		};
	}
}