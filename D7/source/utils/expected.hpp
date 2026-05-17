#pragma once

#include "assert.hpp"

#include <string>
#include <memory>
#include <cstdint>

namespace d7
{
	class expected
	{
	public:

		static expected Success() 
		{
			expected e;
			e.Status = EStatus::Success;
			return e;
		}

		static expected Fail() 
		{
			expected e;
			e.Status = EStatus::Fail;
			return e;
		}

		static expected Fatal(const std::string& msg) 
		{
			expected e;
			e.Status = EStatus::Fatal;
			e.FatalMessagePtr = std::make_unique<std::string>(msg);
			return e;
		}

		enum class EStatus : uint8_t
		{
			Fail    = 0,
			Success = 1,
			Fatal   = 2
		};

		bool operator!() const noexcept {
			return !IsSuccess();
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

		std::string ExtractFatalMessageOrFail()
		{
			assert(!IsSuccess(), "Try extract fatal message from \"expected\" or fail, but his type == succes");

			if (IsFatal())
			{
				assert(FatalMessagePtr != nullptr, "Fatal status set but message pointer is null :\\");

				return ExtractFatalMessage();
			}
			else if (IsFail())
			{
				return "Fail";
			}

			return "This string never print";
		}
	private:
		EStatus Status = EStatus::Fail;
		std::unique_ptr<std::string> FatalMessagePtr;

		expected() = default;
	};
}