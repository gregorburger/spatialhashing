#include <QApplication>

#include "mainwindow.h"
#include <chrono>
#include <boost/random.hpp>
#include "spatialhash.h"

#define NOGUI

#define TIME_IT(f) { \
    auto before = std::chrono::high_resolution_clock::now(); \
    f; \
    auto after = std::chrono::high_resolution_clock::now(); \
    auto took = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> > >(after - before).count(); \
    std::cout << " took " << took << " seconds" << std::endl; \
    }

void gen_random(std::vector<vector> &points) {
    boost::random::mt19937 rng;
    boost::random::uniform_real_distribution<> gen(0, 1.0);
    srandom(time(NULL));

    std::generate(points.begin(), points.end(), [&]{
        return vector(gen(rng), gen(rng));
    });
}

int main(int argc, char **argv) {
    std::vector<vector> points(100000000);
    gen_random(points);

    SpatialHash<vector> hash(0.05);

    TIME_IT(hash.insert(points));

#ifndef NOGUI
    QApplication a(argc, argv);
    MainWindow w(&hash);
    w.show();
    return a.exec();
#endif
}

