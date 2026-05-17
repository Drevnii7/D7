#pragma once

#include <string>
#include <iostream>
#include <array>

#include "consolecolors.hpp"

namespace d7
{
	namespace notify
	{
        enum class ENotifyLevel
        {
            Trace      = 0, // load token + token.Dump()
            Callback   = 1, // CLexer::LoadCode
            Info       = 2, // Load file
            Warning    = 3, // Failed to open the file
        };

        inline std::array<bool, 4> MaskNotifyLevel = {1, 1, 1, 1};

        inline void SelectColorAndSet(ENotifyLevel level)
        {
            switch (level)
            {
            case ENotifyLevel::Trace:
                std::cout << d7::colors::BRIGHT_BLACK;
                return;

            case ENotifyLevel::Callback:
                std::cout << d7::colors::BRIGHT_BLUE;
                return;

            case ENotifyLevel::Info:
                std::cout << d7::colors::BRIGHT_WHITE;
                return;

            case ENotifyLevel::Warning:
                std::cout << d7::colors::YELLOW;
                return;

            default:
                std::cout << d7::colors::RESET;
                return;
            }
        }
        

        namespace ENotifyLevelUtils
        {
            const std::string Trace_String    = "Trace";
            const std::string Callback_String = "Callback";
            const std::string Info_String     = "Info";
            const std::string Warning_String  = "Warning";

            inline std::string_view ENotifyLevelToString(ENotifyLevel Type)
            {
                switch (Type)
                {
                case ENotifyLevel::Trace:
                    return Trace_String;

                case ENotifyLevel::Callback:
                    return Callback_String;

                case ENotifyLevel::Info:
                    return Info_String;

                case ENotifyLevel::Warning:
                    return Warning_String;

                default:
                    return "none";
                }
            }

            inline ENotifyLevel ENotifyLevelFromString(std::string_view String)
            {
                if (String == Trace_String)
                {
                    return ENotifyLevel::Trace;
                }
                else if (String == Callback_String)
                {
                    return ENotifyLevel::Callback;
                }
                else if (String == Info_String)
                {
                    return ENotifyLevel::Info;
                }
                else if (String == Warning_String)
                {
                    return ENotifyLevel::Warning;
                }
                else
                {
                    return ENotifyLevel::Trace;
                }
            }
        }
        
        inline void NotifyImplementation
        (
            int line,
            const char* function,
            ENotifyLevel level,
            const char* message
        )
        {
            if (MaskNotifyLevel[int(level)] == false)
                return;

            SelectColorAndSet(level);
            std::cout << "[" << function << ":" << line << "] "
                << "[" << ENotifyLevelUtils::ENotifyLevelToString(level) << "] " << d7::colors::RESET << d7::colors::BOLD
                << message << d7::colors::RESET << "\n";
        }
	}
}

#define notify_trace(message) \
    do { \
        d7::notify::NotifyImplementation(__LINE__, __FUNCTION__,d7::notify::ENotifyLevel::Trace, message); \
    } while(0)

#define notify_callback(message) \
    do { \
        d7::notify::NotifyImplementation(__LINE__, __FUNCTION__,d7::notify::ENotifyLevel::Callback, message); \
    } while(0)

#define notify_info(message) \
    do { \
        d7::notify::NotifyImplementation(__LINE__, __FUNCTION__,d7::notify::ENotifyLevel::Info, message); \
    } while(0)

#define notify_warning(message) \
    do { \
        d7::notify::NotifyImplementation(__LINE__, __FUNCTION__,d7::notify::ENotifyLevel::Warning, message); \
    } while(0)
