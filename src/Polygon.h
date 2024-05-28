#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include "Vec3.h"
using std::vector;


template<typename T, size_t N>
class tup;
class camera;
class screen;

typedef tup<tup<float, 2>, 3> PROJECTIONS;

class polygon {
public:
    polygon() = default;
    polygon(vec3 A, vec3 B, vec3 C);
    vec3 A;
    vec3 B;
    vec3 C;
    // TODO: void rasterize(camera* camera);
    tup<int, 2> get_vertical_bounds(PROJECTIONS projections, camera& camera);
    tup<int, 2> get_render_row_range(int y, PROJECTIONS projections);
    tup<tup<float, 2>, 3> project(camera& camera, screen& screen);

    friend std::ostream& operator<<(std::ostream& os, const polygon& self);
};

