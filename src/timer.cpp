#include "timer.hpp"

void Timer::start() {
    start_time = clock();
}

double Timer::stop() {
    return (clock() - start_time) / (CLOCKS_PER_SEC / 1000); // in ms
}