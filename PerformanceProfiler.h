#pragma once
#include <functional>
#include <omp.h>
class PerformanceProfiler
{
    struct ProfilingResult {
        int iterations;
        double totalTime;   // Sum of all individual runs (seconds)
        double averageTime; // Average time per iteration (seconds)
        double minTime;     // Minimum time across iterations (seconds)
        double maxTime;     // Maximum time across iterations (seconds)
    };

public:
    template <typename Func, typename... Args>
    double profileFunction(Func&& func, Args&&... args)
    {
        double start = omp_get_wtime(), end = 0;

        if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
            func(std::forward<Args>(args)...);
            end = omp_get_wtime();
            return end - start;
        }
        else {
            auto result = func(std::forward<Args>(args)...);
            end = omp_get_wtime();
            return end - start;
        }

        //std::forward<Func>(func)(std::forward<Args>(args)...);

        //end = omp_get_wtime();
        //return end - start;
    }
};