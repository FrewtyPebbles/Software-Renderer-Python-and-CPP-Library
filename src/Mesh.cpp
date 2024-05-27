#include "Mesh.h"
#include "Tup.h"
#include "Vec3.h"
#include "Camera.h"
#include <math.h>
#include <array>
#include <iostream>


polygon::polygon(vec3& A, vec3& B, vec3& C) : A(&A),B(&B),C(&C) {};

tup2i polygon::get_vertical_bounds(PROJECTIONS projections, camera& camera) {
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
    return make_tup<int, 2>({(int)min, (int)max});
}

tup<int, 2> polygon::get_render_row_range(int y, PROJECTIONS projections) {
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
            if (a != 0) {
                xs.push_back((int)((y-b)/a));
            } else xs.push_back(1);
        }
            
    }
    return xs[0] < xs[1] ? make_tup<int, 2>({(int)xs[0], (int)xs[1]}) : make_tup<int, 2>({(int)xs[1], (int)xs[0]});
}

tup<tup<float, 2>, 3> polygon::project(camera* camera, screen* screen) {
    tup<float, 2> p1 = this->A->project(camera, screen);
    tup<float, 2> p2 = this->B->project(camera, screen);
    tup<float, 2> p3 = this->C->project(camera, screen);
    return make_tup<tup<float, 2>, 3>({p1,p2,p3});
}

// int main() {
//     auto v = vec3(0.0f,0.0f,0.0f);
//     auto c = camera(v, 500, 500, 200);
//     auto s = screen(500, 500);
//     auto v1 = vec3(1.0f,65.0f,0.54f);
//     auto v2 = vec3(2.0f,5.0f,3.0f);
//     auto v3 = vec3(20.0f,4.0f,7.0f);
//     auto p = polygon(v1,v2,v3);

//     auto tupl = p.project(&c, &s);

//     auto proj = v1.project(&c, &s);

//     std::cout << tupl << '\n' << proj << '\n';

//     return 0;
// }