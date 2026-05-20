#pragma once

#include "Vexel/Utils/STL.hpp"

namespace Vex
{
    namespace Profiler
    {
        struct Result final
        {
            std::string Name;
            long long Start, End;
            std::thread::id ThreadID;
        };

        class Timer final
        {
          public:
            Timer(const char* name) : m_Name(name), m_StartPoint(std::chrono::high_resolution_clock::now()) {}
            virtual ~Timer() = default;

            void Start() { m_StartPoint = std::chrono::high_resolution_clock::now(); }
            Result Stop()
            {
                auto endPoint = std::chrono::high_resolution_clock::now();

                long long startTime = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint)
                                          .time_since_epoch()
                                          .count();
                long long endTime = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint)
                                        .time_since_epoch()
                                        .count();

                Start();
                return {.Name = m_Name,
                    .Start = startTime,
                    .End = endTime,
                    .ThreadID = std::this_thread::get_id()};
            }

          private:
            const char* m_Name;
            std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
        };
    } // namespace Profiler
} // namespace Vex
