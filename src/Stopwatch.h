//
// Created by John Attong on 12/3/24.
//
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

#ifndef GATORTUNERS_STOPWATCH_H
#define GATORTUNERS_STOPWATCH_H

using namespace std;

class Stopwatch {
public:
    Stopwatch();

    void start();

    void pause();

    void reset();

    void save();

    double getElapsedTime() const;

private:
    bool is_running;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    double elapsed_time;
    std::vector<double> saved_times;
};



#endif //GATORTUNERS_STOPWATCH_H
