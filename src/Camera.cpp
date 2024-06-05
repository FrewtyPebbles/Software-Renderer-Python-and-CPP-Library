#include "Camera.h"
#include "Vec3.h"
#include "Object.h"
#include <algorithm>

void camera::render(vector<object*> objects, screen& screen) {
    this->py_frame_buffer.clear();
    std::fill(this->frame_buffer, this->frame_buffer + this->view_width*this->view_height, 0x000000FF);
    this->depth_buffer = this->cleared_depth_buffer;
    std::sort(objects.begin(), objects.end(), [](object* a, object* b){
        return (a->position.z) < (b->position.z);
    });
    for (object * obj : objects) {
        obj->render(*this, screen);
    }
}

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

camera::~camera() {
    delete this->frame_buffer;
}


camera::camera(vec3* position, int view_width, int view_height, int focal_length, float fov) {
    this->position = position;
    this->view_width = view_width;
    this->view_height = view_height;
    this->focal_length = focal_length;
    this->fov = fov;
    this->frame_buffer = new uint32_t[this->view_width*this->view_height];
    
    // create "empty" depth buffer
    vector<vector<float>> depth_outer_vec;
    vector<float> depth_inner_vec;
    int i = 0;
    for (int x = 0; x < view_width; x++){
        depth_inner_vec.clear();
        for (int y = 0; y < view_height; y++){
            depth_inner_vec.push_back(INFINITY);
            this->frame_buffer[i] = 0x000000FF;
            i++;
        }
        depth_outer_vec.push_back(depth_inner_vec);
    }
    this->cleared_depth_buffer = depth_outer_vec;
    this->depth_buffer = depth_outer_vec;

}  