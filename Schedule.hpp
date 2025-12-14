#pragma once
#include <thread>
#include <chrono>
#include <functional>

inline void scheduleTask(int delayMilliseconds, std::function<void()> task) {
    std::thread timerThread([=]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMilliseconds));
        task();
    });
    timerThread.detach();
}