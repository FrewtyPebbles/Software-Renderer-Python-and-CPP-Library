#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include "Vec3.h"
using std::vector;

#define c_min2(a,b) (((a)<(b))?(a):(b))
#define c_max2(a,b) (((a)>(b))?(a):(b))


template<typename T, size_t N>
class tup;
class camera;
class screen;

typedef tup<tup<float, 2>, 3> PROJECTIONS;

class polygon {
public:
    polygon() = default;
    polygon(vec3 A, vec3 B, vec3 C) : A(A), B(B), C(C) {};
    vec3 A;
    vec3 B;
    vec3 C;
    void render(camera* camera, screen* screen);
    tup<int, 2> get_vertical_bounds(PROJECTIONS projections, camera* camera, screen* screen);
    tup<int, 2> get_render_row_range(int y, PROJECTIONS projections, camera* camera, screen* screen);
    tup<tup<float, 2>, 3> project(camera* camera, screen* screen);
    float bary_get_z(int x, int y);

    friend std::ostream& operator<<(std::ostream& os, const polygon& self);
};
 

template <typename T>
inline T clamp(const T& value, const T& low, const T& high) {
  return value < low ? low : (value > high ? high : value); 
}

template <typename T>
inline void vec_extend(vector<T>& v, vector<T>& v_prime) {
  v.reserve(v.size() + distance(v_prime.begin(),v_prime.end()));
  v.insert(v.end(),v_prime.begin(),v_prime.end());
}