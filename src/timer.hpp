#include <ctime>

struct Timer {
    clock_t start_time;

    void start();
    double stop();
};