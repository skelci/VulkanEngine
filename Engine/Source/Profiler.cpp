#include "Profiler.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>


CProfilerManager::CProfilerManager() : IsRunning(false), IsFirstEvent(true) {}

CProfilerManager::~CProfilerManager() { EndSession(); }

void CProfilerManager::BeginSession(const std::string& Filepath) {
    std::lock_guard<std::mutex> Lock(QueueMutex);
    if (IsRunning) return;

    const std::string FullFilepath = "Saved/Profiling/" + Filepath;
    std::filesystem::create_directories(std::filesystem::path(FullFilepath).parent_path());

    OutputStream.open(FullFilepath);
    if (OutputStream.is_open()) {
        OutputStream << "{\"otherData\": {},\"traceEvents\":[\n";
        OutputStream.flush();
    } else {
        std::cerr << "ProfilerManager: Failed to open " << FullFilepath << " for writing profile data.\n";
    }

    IsRunning = true;
    IsFirstEvent = true;
    WriterThread = std::thread(&CProfilerManager::WriteThreadLoop, this);
}

void CProfilerManager::EndSession() {
    {
        std::lock_guard<std::mutex> Lock(QueueMutex);
        if (!IsRunning) return;
        IsRunning = false;
        QueueCV.notify_one();
    }

    if (WriterThread.joinable()) {
        WriterThread.join();
    }

    if (OutputStream.is_open()) {
        OutputStream << "\n]}";
        OutputStream.close();
    }
}

void CProfilerManager::SubmitResult(const SProfileResult& Result) {
    std::lock_guard<std::mutex> Lock(QueueMutex);
    if (IsRunning) {
        Queue.push_back(Result);
        QueueCV.notify_one();
    }
}

void CProfilerManager::WriteThreadLoop() {
    std::vector<SProfileResult> LocalQueue;

    while (true) {
        {
            std::unique_lock<std::mutex> Lock(QueueMutex);
            QueueCV.wait(Lock, [this]() { return !IsRunning || !Queue.empty(); });

            if (!IsRunning && Queue.empty()) {
                break;
            }

            std::swap(LocalQueue, Queue);
        }

        if (OutputStream.is_open()) {
            for (const auto& Result : LocalQueue) {
                if (!IsFirstEvent) {
                    OutputStream << ",\n";
                }
                IsFirstEvent = false;

                // standard Chrome tracing json format
                OutputStream << "{"
                             << "\"cat\":\"function\","
                             << "\"dur\":" << (Result.EndMicroseconds - Result.StartMicroseconds) << ","
                             << "\"name\":\"" << Result.Name << "\","
                             << "\"ph\":\"X\"," // X denotes a Complete Event (with duration)
                             << "\"pid\":0,"
                             << "\"tid\":" << Result.ThreadID << ","
                             << "\"ts\":" << Result.StartMicroseconds << "}";
            }
            OutputStream.flush();
        }

        LocalQueue.clear();
    }
}

// -----------------------------------------------------------------------------
// TIMER IMPLEMENTATION
// -----------------------------------------------------------------------------

CProfilerScopeTimer::CProfilerScopeTimer(const char* InName) : Name(InName) {
    StartTime = std::chrono::high_resolution_clock::now();
}

CProfilerScopeTimer::~CProfilerScopeTimer() {
    auto EndTime = std::chrono::high_resolution_clock::now();

    uint64 Start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTime).time_since_epoch().count();
    uint64 End = std::chrono::time_point_cast<std::chrono::microseconds>(EndTime).time_since_epoch().count();

    // Quick and safe way to get a numeric thread ID
    uint32 ThreadID = static_cast<uint32>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

    CProfilerManager::Get().SubmitResult({Name, ThreadID, Start, End});
}
