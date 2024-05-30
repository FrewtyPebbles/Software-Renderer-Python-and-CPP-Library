#include "Camera.h"
#include "Vec3.h"

screen::screen() {
    this->width = 500;
    this->height = 500;
    this->threadpool = new ThreadPool();
    this->threadpool->Start();
}

screen::screen(int width, int height) {
    this->width = width;
    this->height = height;
    this->threadpool = new ThreadPool();
    this->threadpool->Start();
}

screen::~screen() {
    this->threadpool->Stop();
    delete threadpool;
}

camera::camera() {}


camera::camera(vec3* position, int view_width, int view_height, int view_distance) {
    this->position = position;
    this->view_width = view_width;
    this->view_height = view_height;
    this->view_distance = view_distance;
    
    // create "empty" depth buffer
    vector<vector<tup<uint8_t, 3>>> frame_outer_vec;
    vector<tup<uint8_t, 3>> frame_inner_vec;
    vector<vector<float>> depth_outer_vec;
    vector<float> depth_inner_vec;
    for (int i = 0; i < view_width; i++){
        depth_inner_vec.clear();
        frame_inner_vec.clear();
        for (int j = 0; j < view_height; j++){
            depth_inner_vec.push_back(INFINITY);
            frame_inner_vec.push_back(make_tup<uint8_t, 3>({0,0,0}));
        }
        depth_outer_vec.push_back(depth_inner_vec);
        frame_outer_vec.push_back(frame_inner_vec);
    }
    this->cleared_depth_buffer = depth_outer_vec;
    this->depth_buffer = depth_outer_vec;
}  