#pragma once

#include "DataTypes.hpp"

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


struct SProfileResult {
    const char* Name;
    uint32 ThreadID;
    uint64 StartMicroseconds;
    uint64 EndMicroseconds;
};


class CProfilerManager {
public:
    static CProfilerManager& Get() {
        static CProfilerManager Instance;
        return Instance;
    }

    void BeginSession(const std::string& Filepath = "profiling.json");
    void EndSession();
    void SubmitResult(const SProfileResult& Result);

private:
    CProfilerManager();
    ~CProfilerManager();

    void WriteThreadLoop();

    std::vector<SProfileResult> Queue;
    std::mutex QueueMutex;
    std::condition_variable QueueCV;
    std::thread WriterThread;
    bool IsRunning;
    std::ofstream OutputStream;
    bool IsFirstEvent;
};


class CProfilerScopeTimer {
public:
    CProfilerScopeTimer(const char* InName);
    ~CProfilerScopeTimer();

private:
    const char* Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> StartTime;
};


#define PROFILER_CONCAT_IMPL(a, b) a##b
#define PROFILER_CONCAT(a, b) PROFILER_CONCAT_IMPL(a, b)

#ifndef NDEBUG
#define TRACE_SCOPE(Name) CProfilerScopeTimer PROFILER_CONCAT(Timer_, __LINE__)(Name)
#define TRACE_FUNCTION() TRACE_SCOPE(__FUNCTION__)
#else
#define TRACE_SCOPE(Name)
#define TRACE_FUNCTION()
#endif
