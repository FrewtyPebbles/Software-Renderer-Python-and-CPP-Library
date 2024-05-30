#include "Polygon.h"
#include "Tup.h"
#include "Camera.h"
#include "debug.h"
#include <math.h>
#include <array>
#include <iostream>
#include <tuple>
#include <mutex>
#include "threadpool.h"



float polygon::bary_get_z(int x, int y) {
    vec3 a = this->A;
    vec3 b = this->A;
    
    float apx = a.x/a.z;
    float bpx = b.x/b.z;
    float bpxdiffapx = (bpx-apx);
    float b_coord = (x - apx)/ (bpxdiffapx != 0 ? bpxdiffapx : 0.001);

    float z = fmaf(a.z, (1-b_coord), b.z*b_coord);
    return z;
}

void polygon::render(camera* camera, screen* screen) {
    // TODO: change color to texturemap color.
    uint8_t shade;
    tup<uint8_t, 3> color;
    //vector<pixel> local_buffer;
    tup<int, 2> hor_bounds;
    PROJECTIONS projections = this->project(camera, screen);
    tup<int, 2> vert_bounds = this->get_vertical_bounds(projections, camera, screen);
    float z;
    bool d_check;
    
    for (int y = vert_bounds[0]; y < vert_bounds[1]; y++) {
        hor_bounds = this->get_render_row_range(y, projections, camera, screen);
        for (int x = hor_bounds[0]; x < hor_bounds[1]; x++) {
            z = this->bary_get_z(x, y);
            render_mut.lock();
            d_check = camera->depth_buffer[x][y] > z;
            render_mut.unlock();
            if (d_check) {
                shade = std::max(0, (255 - static_cast<int>(z/130*255)));
                color = make_tup<uint8_t, 3>({ shade, shade, shade });
                render_mut.lock();
                camera->frame_buffer.push_back(pixel(x, y, color));
                camera->depth_buffer[x][y] = z;
                render_mut.unlock();
            }
        }
    }
    
}

tup2i polygon::get_vertical_bounds(PROJECTIONS projections, camera* camera, screen* screen) {
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
    return make_tup<int, 2>({c_max2((int)min, 0), c_min2((int)max, screen->height)});
}

tup<int, 2> polygon::get_render_row_range(int y, PROJECTIONS projections, camera* camera, screen* screen) {
    tup<float, 2> p1 = projections[0];
    tup<float, 2> p2 = projections[1];
    tup<float, 2> p3 = projections[2];
    float denom1 = (p2[0]-p1[0]);
    float denom2 = (p3[0]-p2[0]);
    float denom3 = (p1[0]-p3[0]);
    float a1 = denom1 != 0.0f ? (p2[1]-p1[1])/denom1 : 1.0f;
    float a2 = denom2 != 0.0f ? (p3[1]-p2[1])/denom2 : 1.0f;
    float a3 = denom3 != 0.0f ? (p1[1]-p3[1])/denom3 : 1.0f;
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
        y1 = c_min2(group[0], group[1]);
        y2 = c_max2(group[0], group[1]);
        a = group[2];
        b = group[3];
        if (floor(y1) < y && y <= floor(y2)){
            //std::cout << floor(y1) << " < " << y << " <= " << floor(y2) << ", a=" << a << ", b=" << b << ", (int)(y-b)/a=" << (int)((y-b)/a + 0.5f) << std::endl;
            // if (a != 0.0f) {
            //     xs.push_back(static_cast<int>((y-b)/a + 0.5f));
            // } else xs.push_back(1);
            xs.push_back((int)((y-b)/a));
        }
            
    }
    auto ret = xs[0] < xs[1] ? make_tup<int, 2>({xs[0], xs[1]}) : make_tup<int, 2>({xs[1], xs[0]});
    //std::cout << ret << "\n";
    return ret;
}

PROJECTIONS polygon::project(camera* camera, screen* screen) {
    tup<float, 2> p1 = this->A.project(camera, screen);
    tup<float, 2> p2 = this->B.project(camera, screen);
    tup<float, 2> p3 = this->C.project(camera, screen);
    return make_tup<tup<float, 2>, 3>({p1,p2,p3});
}

std::ostream& operator<<(std::ostream& os, const polygon& self) {
    os << "polygon<" << self.A << ", " << self.B << ", " << self.C << '>';
    return os;
}