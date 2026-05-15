#pragma once

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Vex
{
    class Log final
    {
      public:
        static std::shared_ptr<spdlog::logger> GetCoreLogger() { return Get().m_CoreLogger; }
        static std::shared_ptr<spdlog::logger> GetClientLogger() { return Get().m_ClientLogger; }

        static void Init()
        {
            spdlog::set_pattern("%^[%T:%e] %n (%l) : %v%$");

            Get().m_CoreLogger = spdlog::stdout_color_mt("RANDOM");
            Get().m_CoreLogger->set_level(spdlog::level::trace);

            Get().m_ClientLogger = spdlog::stdout_color_mt("APP");
            Get().m_ClientLogger->set_level(spdlog::level::trace);
        }

      private:
        static Log& Get() { return s_Instance; }

      private:
        static Log s_Instance;

        std::shared_ptr<spdlog::logger> m_CoreLogger;
        std::shared_ptr<spdlog::logger> m_ClientLogger;
    };

} // namespace Vex

// clang-format off
#define VEX_CORE_TRACE(...) Vex::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VEX_CORE_INFO(...)  Vex::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VEX_CORE_WARN(...)  Vex::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VEX_CORE_ERROR(...) Vex::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VEX_CORE_FATAL(...) Vex::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define VEX_TRACE(...) Vex::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VEX_INFO(...)  Vex::Log::GetClientLogger()->info(__VA_ARGS__)
#define VEX_WARN(...)  Vex::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VEX_ERROR(...) Vex::Log::GetClientLogger()->error(__VA_ARGS__)
#define VEX_FATAL(...) Vex::Log::GetClientLogger()->fatal(__VA_ARGS__)
// clang-format on
