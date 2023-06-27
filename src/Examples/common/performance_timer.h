#pragma once
#ifndef _COMMON_PERFORMANCE_TIMER_H_
#define _COMMON_PERFORMANCE_TIMER_H_

#include <chrono>
#include <iostream>
#include <string>

class performance_timer {
    public:
        performance_timer() = default;
        ~performance_timer() = default;
        performance_timer(const performance_timer&) = delete;
    
        void start();
        void stop();
        void report(const std::string& msg = "elapsed");
    
    private:
        std::chrono::high_resolution_clock _clock;
        std::chrono::high_resolution_clock::time_point _start;
        std::chrono::high_resolution_clock::time_point _stop;
};

#endif