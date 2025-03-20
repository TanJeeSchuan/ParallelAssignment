//#pragma once
//#include <functional>
//#include <omp.h>
//#include <string>
//#include <sstream>
//#include <iomanip>
//#include <limits>
//
//class PerformanceProfiler
//{
//    struct ProfilingResult {
//        int iterations;
//        double totalTime;   // Sum of all individual runs (seconds)
//        double averageTime; // Average time per iteration (seconds)
//        double minTime;     // Minimum time across iterations (seconds)
//        double maxTime;     // Maximum time across iterations (seconds)
//
//        // Convert struct data to a formatted string
//        std::string toString() const {
//            std::stringstream ss;
//            ss << "Iterations: " << iterations
//                << ", Total Time: " << std::fixed << std::setprecision(6) << totalTime << "s"
//                << ", Average Time: " << std::fixed << std::setprecision(6) << averageTime << "s"
//                << ", Min Time: " << std::fixed << std::setprecision(6) << minTime << "s"
//                << ", Max Time: " << std::fixed << std::setprecision(6) << maxTime << "s";
//            return ss.str();
//        }
//    };
//
//public:
//    template <typename Func, typename... Args>
//    double profileFunction(Func&& func, Args&&... args)
//    {
//        double start = omp_get_wtime(), end = 0;
//
//        if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
//            func(std::forward<Args>(args)...);
//            end = omp_get_wtime();
//            return end - start;
//        }
//        else {
//            auto result = func(std::forward<Args>(args)...);
//            end = omp_get_wtime();
//
//            return end - start;
//        }
//    }
//
//    template <typename Func, typename... Args>
//    ProfilingResult profile(int iterationCount, Func&& func, Args&&... args)
//    {
//        ProfilingResult result = { 0 };
//        result.iterations = iterationCount;
//
//        double duration = 0;
//        result.minTime = std::numeric_limits<double>::max();
//        for (int i = 0; i < iterationCount; i++)
//        {
//            duration = profileFunction(std::forward<Func>(func), std::forward<Args>(args)...);
//            result.totalTime += duration;
//
//            if (duration > result.maxTime)
//                result.maxTime = duration;
//            if (duration < result.minTime)
//                result.minTime = duration;
//        }
//
//        result.averageTime = result.totalTime / result.iterations;
//
//        return result;
//    }
//};