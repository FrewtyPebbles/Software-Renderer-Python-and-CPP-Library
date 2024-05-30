#include "Vec3.h"
#include "Camera.h"
#include "Tup.h"
#include <algorithm>
#include <iostream>
 
tup<float, 2> vec3::project(camera* camera, screen* screen) {
    int cam_w = camera->view_width;
    int cam_h = camera->view_height;
    int scr_w = screen->width;
    int scr_h = screen->height;
    int z_prime = camera->view_distance;
    float y_prime = this->y*z_prime / this->z;
    float x_prime = this->x*z_prime / this->z;
    float _x = x_prime * cam_w / scr_w;
    float _y = y_prime * cam_h / scr_h;
    auto ret = make_tup<float, 2>({-_x + (cam_w/2.0f), -_y + (cam_h/2.0f)});
    //std::cout << ret << " proj\n";
    return ret;
}

