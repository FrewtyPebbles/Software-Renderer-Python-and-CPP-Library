#pragma once
#include <vector>

using std::vector;
class vec3;

class camera {
public:
    camera();
    camera(vec3& position, int view_width, int view_height, int view_distance);
    int view_width, view_height, view_distance;
    vec3* position;
    vector<vector<float>> depth_buffer;
    vector<vector<float>> cleared_depth_buffer;
};

class screen {
public:
    screen();
    screen(int width, int height);
    int width, height;
};