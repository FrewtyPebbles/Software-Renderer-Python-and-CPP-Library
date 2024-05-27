#include "Mesh.h"
#include "Polygon.h"
#include "Tup.h"
#include "Vec3.h"

mesh::mesh(vector<vec3> vertexes, vector<tup<int, 3>> polygon_inds) : vertexes(vertexes), polygon_inds(polygon_inds) {}

vector<polygon> mesh::get_polygons(vector<vec3> vertexes) {
    vector<polygon> polygons;
    vector<int> v_inds;
    vector<vec3> poly_buffer;
    int buf_len, s_ind = 0;
    
    for (tup3i v_inds : this->polygon_inds) {
        buf_len = 0;
        for (int i : v_inds.data) {
            if (buf_len == 2) {
                poly_buffer.push_back(vertexes[i]);
                buf_len += 1;
                polygons.push_back(polygon(poly_buffer[s_ind + 0], poly_buffer[s_ind + 1], poly_buffer[s_ind + 2]));
                s_ind += 3;
            } else {
                poly_buffer.push_back(vertexes[i]);
                buf_len += 1;
            }
        }
        if (buf_len == 2) {
            poly_buffer.push_back(vertexes[v_inds.data[sizeof(v_inds.data)-1]]);
            polygons.push_back(polygon(poly_buffer[s_ind + 0], poly_buffer[s_ind + 1], poly_buffer[s_ind + 2]));
        }
    }
    return polygons;
}