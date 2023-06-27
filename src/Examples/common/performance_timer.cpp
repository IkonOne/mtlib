#include "performance_timer.h"

using namespace std;

void performance_timer::start() {
    _start = _clock.now();
}

void performance_timer::stop() {
    _stop = _clock.now();
}

void performance_timer::report(const string& msg) {
    const auto elapsed = chrono::duration_cast<chrono::milliseconds>(_stop - _start);
    cout << msg << ": " << elapsed.count() << "ms\n";
}