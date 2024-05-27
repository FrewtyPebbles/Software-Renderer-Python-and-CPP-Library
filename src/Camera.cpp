#include "Camera.h"
#include "Vec3.h"

screen::screen() {
    this->width = 500;
    this->height = 500;
}

screen::screen(int width, int height) {
    this->width = width;
    this->height = height;
}

camera::camera() {}


camera::camera(vec3& position, int view_width, int view_height, int view_distance) {
    this->position = &position;
    this->view_width = view_width;
    this->view_height = view_height;
    this->view_distance = view_distance;
    
    // create "empty" depth buffer

    vector<vector<float>> outer_vec;
    vector<float> inner_vec;
    for (int i = 0; i < view_width; i++){
        inner_vec.clear();
        for (int j = 0; j < view_height; j++){
            inner_vec.push_back(INFINITY);
        }
        outer_vec.push_back(inner_vec);
    }
    this->cleared_depth_buffer = outer_vec;
    this->depth_buffer = outer_vec;
}