#include "Vec3.h"
#include "Camera.h"
#include "Tup.h"
#include <algorithm>
#include <iostream>

tup<float, 2> vec3::project(camera& camera, screen& screen) {
    float cam_w = (float)camera.view_width;
    float cam_h = (float)camera.view_height;
    float scr_w = (float)screen.width;
    float scr_h = (float)screen.height;
    float z_prime = (float)camera.view_distance;
    float y_prime = this->y*z_prime / this->z;
    float x_prime = this->x*z_prime / this->z;
    float _x = x_prime * cam_w / scr_w;
    float _y = y_prime * cam_h / scr_h;
    tup<float, 2> proj = make_tup<float, 2>({0.0f, _x + (cam_w/2.0f), cam_h, -_y + (cam_h/2.0f)});
    return proj;
}

vec3::vec3(){
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

vec3::vec3(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

