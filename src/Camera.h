#pragma once
#include <vector>
#include "Tup.h"

using std::vector;
class vec3;

struct pixel {
    pixel(): x(0), y(0), color(make_tup<uint8_t, 3>({0,0,0})){}
    pixel(int x, int y, tup3ui8 color)
        : x(x), y(y), color(color) {}
    int x;
    int y;
    tup3ui8 color;
};

class camera {
public:
    camera();
    camera(vec3& position, int view_width, int view_height, int view_distance);
    int view_width, view_height, view_distance;
    vec3* position;
    vector<vector<float>> depth_buffer;
    vector<vector<float>> cleared_depth_buffer;
    vector<pixel> frame_buffer;
};

class screen {
public:
    screen();
    screen(int width, int height);
    int width, height;
};