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
#include "util.h"
#include "Mesh.h"
#include <opencv2/opencv.hpp>

// TODO: ADD REPEATING TEXTURES
 
float polygon::bary_get_z(int x, int y, PROJECTIONS proj) {
    tup<float, 3> b_coords = barycentric_coords(proj[0][0], proj[0][1], proj[1][0], proj[1][1], proj[2][0], proj[2][1], x, y);
    return fmaf(A.z, b_coords[0], fmaf(B.z, b_coords[1], C.z*b_coords[2]));
}

tup<int, 2> polygon::get_texture_coordinates(int x, int y, PROJECTIONS proj, int width, int height) {
    tup<float, 3> b_coords_vec = barycentric_coords(proj[0][0], proj[0][1], proj[1][0], proj[1][1], proj[2][0], proj[2][1], x, y);

    auto tupl = make_tup<int, 2>({
        static_cast<int>(fmaf(A_tex.x*width, b_coords_vec[0], fmaf(B_tex.x*width, b_coords_vec[1], C_tex.x*width*b_coords_vec[2]))),
        static_cast<int>(fmaf(A_tex.y*height, b_coords_vec[0], fmaf(B_tex.y*height, b_coords_vec[1], C_tex.y*height*b_coords_vec[2])))
    });
    //std::cout << B_tex << std::endl;
    return tupl;
}
 
void polygon::render(camera* camera, screen* screen) {
    int shade;
    tup<uint8_t, 3> color;
    //vector<pixel> local_buffer;
    tup<int, 2> hor_bounds, tex_coords;
    PROJECTIONS projections = this->project(camera, screen);
    tup<int, 2> vert_bounds = this->get_vertical_bounds(projections, camera, screen);
    float z;
    auto texture = this->mesh_data->material_data->diffuse_texture;
    for (int y = vert_bounds[0]; y < vert_bounds[1]; y++) {
        hor_bounds = this->get_render_row_range(y, projections, camera, screen);
        for (int x = hor_bounds[0]; x < hor_bounds[1]; x++) {
            z = this->bary_get_z(x, y, projections);
            if (camera->depth_buffer[x][y] > z) {
                
                tex_coords = this->get_texture_coordinates(x,y,projections, texture.cols, texture.rows);
                auto cvcolor = texture.at<cv::Vec3b>(
                    clamp(tex_coords[1], 0, texture.rows-1),
                    clamp(tex_coords[0], 0, texture.cols-1)
                );
                //shade = std::max(0, (255 - static_cast<uint8_t>(z/130*255)));
                shade = static_cast<int>(z/160*255);
                color = make_tup<uint8_t, 3>({
                    (uint8_t)clamp((int)(cvcolor[2]), 0, 255),
                    (uint8_t)clamp((int)(cvcolor[1]), 0, 255),
                    (uint8_t)clamp((int)(cvcolor[0]), 0, 255)
                });
                //std::cout << color << "\n";
                camera->depth_buffer[x][y] = z;
                render_mut.lock();
                camera->frame_buffer.push_back(pixel(x, y, color));
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
    return make_tup<int, 2>({clamp((int)min, 0, screen->height), clamp((int)max, 0, screen->height)});
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
    float b1 = fmaf(-a1, p1[0], p1[1]);
    float b2 = fmaf(-a2, p2[0], p2[1]);
    float b3 = fmaf(-a3, p3[0], p3[1]);
    
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
            xs.push_back(clamp((int)((y-b)/a), 0, screen->width));
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