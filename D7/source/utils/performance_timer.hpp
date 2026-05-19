#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <cstdint>

#include "consolecolors.hpp"
#include "assert.hpp"
#include "notify.hpp"

namespace d7
{
    class performance_timer
    {
    public:

        performance_timer(const std::string& name = "Timer", bool auto_output = true)
            : m_name(name)
            , m_auto_output(auto_output)
            , m_is_running(false)
        {
        }

        ~performance_timer() = default;

        void Start()
        {
            assert(!m_is_running, "Timer already running! Call Stop() before Start()");

            m_start_time = std::chrono::high_resolution_clock::now();
            m_is_running = true;

            if (m_auto_output)
            {
                notify_info(("[" + m_name + "] Started...").c_str());
            }
        }

        void Stop()
        {
            assert(m_is_running, "Timer not running! Call Start() before Stop()");

            m_end_time = std::chrono::high_resolution_clock::now();
            m_is_running = false;

            if (m_auto_output)
            {
                PrintElapsed();
            }
        }

        double GetElapsedMilliseconds() const
        {
            if (m_is_running)
            {
                auto now = std::chrono::high_resolution_clock::now();
                return std::chrono::duration<double, std::milli>(now - m_start_time).count();
            }
            else
            {
                return std::chrono::duration<double, std::milli>(m_end_time - m_start_time).count();
            }
        }

        double GetElapsedMicroseconds() const
        {
            return GetElapsedMilliseconds() * 1000.0;
        }

        double GetElapsedNanoseconds() const
        {
            return GetElapsedMilliseconds() * 1'000'000.0;
        }

        double GetElapsedSeconds() const
        {
            return GetElapsedMilliseconds() / 1000.0;
        }

        void PrintElapsed() const
        {
            double ms = GetElapsedMilliseconds();

            std::cout << colors::BRIGHT_CYAN << "[" << m_name << "] " << colors::RESET;
            std::cout << colors::BOLD << "Elapsed time: " << colors::RESET;

            if (ms < 1.0)
            {
                std::cout << colors::BRIGHT_GREEN << GetElapsedMicroseconds() << " us" << colors::RESET;
            }
            else if (ms < 1000.0)
            {
                std::cout << colors::BRIGHT_GREEN << ms << " ms" << colors::RESET;
            }
            else
            {
                std::cout << colors::BRIGHT_GREEN << GetElapsedSeconds() << " s" << colors::RESET;
            }
            std::cout << std::endl;
        }

        void Reset()
        {
            m_is_running = false;
            if (m_auto_output)
            {
                notify_trace(("[" + m_name + "] Reset").c_str());
            }
        }

        bool IsRunning() const noexcept
        {
            return m_is_running;
        }

        void SetName(const std::string& name)
        {
            m_name = name;
        }

        const std::string& GetName() const noexcept
        {
            return m_name;
        }

        performance_timer(const performance_timer&) = delete;
        performance_timer& operator=(const performance_timer&) = delete;

        performance_timer(performance_timer&& other) noexcept
            : m_name(std::move(other.m_name))
            , m_start_time(other.m_start_time)
            , m_end_time(other.m_end_time)
            , m_auto_output(other.m_auto_output)
            , m_is_running(other.m_is_running)
        {
            other.m_is_running = false;
        }

        performance_timer& operator=(performance_timer&& other) noexcept
        {
            if (this != &other)
            {
                m_name = std::move(other.m_name);
                m_start_time = other.m_start_time;
                m_end_time = other.m_end_time;
                m_auto_output = other.m_auto_output;
                m_is_running = other.m_is_running;
                other.m_is_running = false;
            }
            return *this;
        }

    private:
        std::string m_name;
        std::chrono::high_resolution_clock::time_point m_start_time;
        std::chrono::high_resolution_clock::time_point m_end_time;
        bool m_auto_output;
        bool m_is_running;
    };

    class scoped_timer
    {
    public:
        scoped_timer(const std::string& name = "ScopedTimer", bool auto_output = true)
            : m_timer(name, auto_output)
        {
            m_timer.Start();
            notify_callback(("Scoped timer [" + m_timer.GetName() + "] started").c_str());
        }

        ~scoped_timer()
        {
            if (m_timer.IsRunning())
            {
                m_timer.Stop();
            }
        }

        double Stop()
        {
            m_timer.Stop();
            return m_timer.GetElapsedMilliseconds();
        }

        double GetCurrentElapsed() const
        {
            return m_timer.GetElapsedMilliseconds();
        }

        bool IsActive() const noexcept
        {
            return m_timer.IsRunning();
        }

        scoped_timer(const scoped_timer&) = delete;
        scoped_timer& operator=(const scoped_timer&) = delete;

        scoped_timer(scoped_timer&& other) noexcept = default;
        scoped_timer& operator=(scoped_timer&& other) noexcept = default;

    private:
        performance_timer m_timer;
    };

    template<typename Func>
    double measure_execution_time(Func&& func, const std::string& name = "Measure")
    {
        performance_timer timer(name, false);
        timer.Start();
        func();
        timer.Stop();
        timer.PrintElapsed();
        return timer.GetElapsedMilliseconds();
    }

    template<typename Func, typename... Args>
    auto measure_and_run(Func&& func, const std::string& name, Args&&... args)
        -> decltype(func(std::forward<Args>(args)...))
    {
        performance_timer timer(name, true);
        timer.Start();
        auto result = func(std::forward<Args>(args)...);
        timer.Stop();
        return result;
    }
}