#pragma once
#include <vector>
using std::vector;

template<typename T, size_t N>
class tup;
class vec3;
class polygon;

class mesh {
public:
    mesh() {}

    mesh(vector<vec3> vertexes, vector<tup<int, 3>> polygon_inds);

    vector<polygon> get_polygons(vector<vec3> vertexes);

    vector<vec3> vertexes;
    vector<tup<int, 3>> polygon_inds;
};