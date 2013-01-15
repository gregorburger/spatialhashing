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

void query_test(std::vector<vector> &points, SpatialHash<vector> &hash) {
#pragma omp parallel for
    for (size_t i = 0; i < points.size() / 3; i++) {
        std::vector<vector> results = hash.query(points[i]);
        //qDebug() << "queried" << results.size() << "results";
    }
}

#ifdef NOGUI
#define LEN 10000000
#else
#define LEN 1000
#endif

int main(int argc, char **argv) {
    std::vector<vector> points(LEN);
    gen_random(points);

    SpatialHash<vector> hash(10000.0f/LEN);
    TIME_IT(hash.insert(points));
#ifdef NOGUI
    TIME_IT(query_test(points, hash));
#else
    QApplication a(argc, argv);
    MainWindow w(&hash);
    w.show();
    return a.exec();
#endif
}

