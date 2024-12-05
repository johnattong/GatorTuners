//
// Created by John Attong on 12/3/24.
//

#include "Stopwatch.h"
Stopwatch::Stopwatch() : is_running(false), elapsed_time(0) {}

void Stopwatch::start() {
    if (!is_running) {
        start_time = std::chrono::high_resolution_clock::now();
        is_running = true;
    }
}

void Stopwatch::pause() {
    if (is_running) {
        auto now = std::chrono::high_resolution_clock ::now();
        elapsed_time += std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
        is_running = false;
    }
}

void Stopwatch::reset() {
    is_running = false;
    elapsed_time = 0;
}

double Stopwatch::getElapsedTime() const {
    if (is_running) {
        auto now = std::chrono::high_resolution_clock::now();
        return static_cast<double>(elapsed_time + std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count());
    }
    return static_cast<double>(elapsed_time);
}

void Stopwatch::save() {
    if (is_running) {
        pause();
        start();
    }
    saved_times.push_back(static_cast<double>(elapsed_time));
}

