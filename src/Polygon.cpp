#include "Polygon.h"
#include "Tup.h"
#include "Camera.h"
#include <math.h>
#include <array>
#include <iostream>
#include <tuple>
#include <mutex>

polygon::polygon(vec3 A, vec3 B, vec3 C) : A(A),B(B),C(C) {};

std::mutex render_mutex;
void polygon::render(camera* camera, screen* screen) {
    
    // TODO: change color to texturemap color.
    tup<uint8_t, 3> color = make_tup<uint8_t, 3>({ 255, 255, 255 });
    std::vector<pixel> local_buffer;
    PROJECTIONS projections = this->project(*camera, *screen);
    tup<int, 2> vert_bounds = this->get_vertical_bounds(projections, *camera, *screen);
    for (int y = vert_bounds[0]; y < vert_bounds[1]; y++) {
        tup<int, 2> hor_bounds = this->get_render_row_range(y, projections, *camera, *screen);
        for (int x = hor_bounds[0]; x < hor_bounds[1]; x++) {
            local_buffer.push_back(pixel(x, y, color));
        }
    }

    render_mutex.lock();
    camera->frame_buffer.reserve(camera->frame_buffer.size() + distance(local_buffer.begin(),local_buffer.end()));
    camera->frame_buffer.insert(camera->frame_buffer.end(),local_buffer.begin(),local_buffer.end());
    render_mutex.unlock();
}

tup2i polygon::get_vertical_bounds(PROJECTIONS projections, camera& camera, screen& screen) {
    float min = projections[0][1];
    float max = projections[0][1];
    float curr;
    for (int i = 0; i < 3; i++){
        curr = projections[i][1];
        if (curr < min)
            min = curr;
        if (curr > max)
            max = curr;
    }
    return make_tup<int, 2>({clamp((int)min, 0, screen.height), clamp((int)max, 0, screen.height)});
}

tup<int, 2> polygon::get_render_row_range(int y, PROJECTIONS projections, camera& camera, screen& screen) {
    tup<float, 2> p1 = projections[0];
    tup<float, 2> p2 = projections[1];
    tup<float, 2> p3 = projections[2];
    float denom1 = (p2[0]-p1[0]);
    float denom2 = (p3[0]-p2[0]);
    float denom3 = (p1[0]-p3[0]);
    float a1 = denom1 != 0 ? (p2[1]-p1[1])/denom1 : 1.0f;
    float a2 = denom2 != 0 ? (p3[1]-p2[1])/denom2 : 1.0f;
    float a3 = denom3 != 0 ? (p1[1]-p3[1])/denom3 : 1.0f;
    float b1 = p1[1]-a1*p1[0];
    float b2 = p2[1]-a2*p2[0];
    float b3 = p3[1]-a3*p3[0];
    
    float y1,y2,a,b;
    vector<int> xs = {};
    y += 1;
    std::array<tup<float, 4>, 3> itterator{
        make_tup<float, 4>({p1[1], p2[1], a1, b1}),
        make_tup<float, 4>({p2[1], p3[1], a2, b2}),
        make_tup<float, 4>({p3[1], p1[1], a3, b3})
    };
    for (tup<float, 4> group : itterator) {
        y1 = group[0] < group[1] ? group[0] : group[1];
        y2 = group[0] > group[1] ? group[0] : group[1];
        a = group[2];
        b = group[3];
        if (floor(y1) < y && y <= floor(y2)){
            if (a != 0.0f) {
                xs.push_back(clamp((int)((y-b)/a), 0, screen.width));
            } else xs.push_back(1);
        }
            
    }

    return xs[0] < xs[1] ? make_tup<int, 2>({(int)xs[0], (int)xs[1]}) : make_tup<int, 2>({(int)xs[1], (int)xs[0]});
}

tup<tup<float, 2>, 3> polygon::project(camera& camera, screen& screen) {
    tup<float, 2> p1 = this->A.project(camera, screen);
    tup<float, 2> p2 = this->B.project(camera, screen);
    tup<float, 2> p3 = this->C.project(camera, screen);
    return make_tup<tup<float, 2>, 3>({p1,p2,p3});
}

std::ostream& operator<<(std::ostream& os, const polygon& self) {
    os << "polygon<" << self.A << ", " << self.B << ", " << self.C << '>';
    return os;
}