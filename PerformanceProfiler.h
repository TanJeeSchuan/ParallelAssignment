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

        std::forward<Func>(func)(std::forward<Args>(args)...);

        end = omp_get_wtime();
        return end - start;
    }
};