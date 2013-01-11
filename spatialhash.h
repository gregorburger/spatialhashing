#ifndef SPATIALHASH_H
#define SPATIALHASH_H

#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_vector.h>
#include <cmath>
#include <cassert>
#include <array>

struct vector {
    vector(double x, double y) : x(x), y(y) {}
    vector() = default;

    double length_sq() const {
        return x*x + y*y;
    }

    vector operator-(const vector &other) const {
        return vector(x-other.x, y-other.y);
    }

    double dist_sq(const vector &other) const {
        return (*this - other).length_sq();
    }
    double x, y;
};

template<class T>
class SpatialHash {
public:
    SpatialHash(double cell_size)
        : cell_size(cell_size) {}

    struct cell {
        cell(int x, int y) : x(x), y(y) {}

        cell(const T &v, double cell_size) {
            x = std::floor(v.x / cell_size);
            y = std::floor(v.y / cell_size);
        }

        cell() = default;
        int x, y;
    };

    struct VectorHashCompare {
        bool equal(const cell &j, const cell &k) const {
            return j.x == k.x && j.y == k.y;
        }

        size_t hash(const cell &v) const {
            return v.x * p1 ^ v.y*p2;
        }

        const int p1 = 73856093;
        const int p2 = 19349663;
    };

    typedef tbb::concurrent_vector<T> value_type;
    typedef tbb::concurrent_hash_map<cell, value_type *, VectorHashCompare> map_type;

    void insert(const std::vector<T> &points) {
#pragma omp parallel for
        for (size_t i = 0; i < points.size(); ++i) {
            const vector &v = points[i];
            cell c(v, cell_size);
            typename map_type::const_accessor result;
            if (map.find(result, c)) {
                result->second->push_back(v);
            } else {
                typename map_type::accessor mutable_result;
                if (map.insert(mutable_result, c)) {
                    value_type *value = new value_type();
                    mutable_result->second = value;
                    value->push_back(v);
                }
            }
        }
    }

    std::vector<T> query(const vector &v) const {
        std::vector<T> points;
        cell c(v, cell_size);
        std::array<cell, 9> cells{{
                    cell(c.x-1, c.y-1),
                    cell(c.x-1, c.y),
                    cell(c.x-1, c.y+1),
                    cell(c.x, c.y-1),
                    c,
                    cell(c.x, c.y+1),
                    cell(c.x+1, c.y-1),
                    cell(c.x+1, c.y),
                    cell(c.x+1, c.y+1)}};

        double range = cell_size*cell_size;

        for (cell ci: cells) {
            typename map_type::const_accessor result;
            if (!map.find(result, ci))
                continue;
            for (const vector &vi: *result->second) {
                if (v.dist_sq(vi) < range) {
                    points.push_back(vi);
                }
            }
        }
        return points;
    }

    double cell_size;
    map_type  map;
};

#endif // SPATIALHASH_H
