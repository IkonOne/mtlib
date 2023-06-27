#include <MTLib/mtlib.h>

#include "common/performance_timer.h"

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace mtlib;

int main(int argc, char* argv[]) {
    int N = 100;
    if (argc > 1) {
        N = atoi(argv[1]);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution dis(10.0f, 390.0f);
    const auto rng = [&]() { return dis(gen); };

    vector<vec2f> points;
    points.reserve(N);

    svg::begin(400, 400);
    for (size_t i = 0; i < N; ++i) {
        points.push_back({ rng(), rng() });
    }

    vector<vec2f> hull;
    hull.reserve(points.size());

    performance_timer timer;
    timer.start();
    chull_graham_2d(points.begin(), points.end(), std::back_inserter(hull));
    timer.stop();
    timer.report();
    
    svg::stroke_color = "black";
    svg::polygon(hull.begin(), hull.end());
    svg::points(points.begin(), points.end());

    svg::end("chull.svg");

    return 0;
}