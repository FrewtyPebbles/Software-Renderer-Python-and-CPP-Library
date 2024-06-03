#include "Vec3.h"
#include "Camera.h"
#include "Tup.h"
#include <algorithm>
#include <iostream>
#include <math.h>

 
tup<float, 2> vec3::project(camera* camera, screen* screen) {
    int cam_w = camera->view_width;
    int cam_h = camera->view_height;
    int scr_w = screen->width;
    int scr_h = screen->height;
    int z_prime = camera->focal_length;
    float y_prime = this->y*z_prime / this->z;
    float x_prime = this->x*z_prime / this->z;
    float _x = x_prime * cam_w / scr_w;
    float _y = y_prime * cam_h / scr_h;
    auto ret = make_tup<float, 2>({-_x + (cam_w/2.0f), -_y + (cam_h/2.0f)});
    return ret;
}

bool vec3::not_clipping(camera* camera, screen* screen) {
    return this->z > 0;
}

float vec3::clip_near_plane(camera* camera, screen* screen) {
    return this->dot(vec3(0.0f,0.0f,1.0f)) + camera->focal_length;
}

float vec3::clip_right_plane(camera* camera, screen* screen) {
    float fov = camera->fov/2;
    return this->dot(vec3(-std::sin(fov),0.0f,std::sin(fov)));
}

float vec3::clip_left_plane(camera* camera, screen* screen) {
    float fov = camera->fov/2;
    return this->dot(vec3(std::sin(fov),0.0f,std::sin(fov)));
}

float vec3::clip_bottom_plane(camera* camera, screen* screen) {
    float fov = camera->fov/2;
    return this->dot(vec3(0.0f, std::sin(fov), std::sin(fov)));
}

float vec3::clip_top_plane(camera* camera, screen* screen) {
    float fov = camera->fov/2;
    return this->dot(vec3(0.0f, -std::sin(fov), std::sin(fov)));
}

float vec3::distance_from_view_plane(camera* camera, screen* screen) {
    return this->dot(*camera->position) + camera->focal_length;
}
