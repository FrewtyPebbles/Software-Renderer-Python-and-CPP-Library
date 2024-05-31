#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Tup.h"
#include <opencv2/opencv.hpp>

using std::vector;
using std::string;

enum class illum_model {
    CONSTANT_COLOR,
    DIFFUSE,
    DIFFUSE_AND_SPECULAR
};

struct material {
    material(){};
    tup<float, 3> ambient, diffuse, specular, emissive_coeficient;
    float specular_exponent, optical_density, transparency;
    illum_model illumination_model;
    string ambient_tex_file, diffuse_tex_file, specular_highlight_file;
    cv::Mat ambient_texture, diffuse_texture, specular_highlight_texture;
};

class vec3;
class polygon;

struct face {
    face()=default;
    face(
        tup<int, 3> vertex_indicies,
        tup<int, 3> vertex_tex_coord_indices,
        tup<int, 3> normal_indicies
    ) :
        vertex_indicies(vertex_indicies),
        vertex_tex_coord_indices(vertex_tex_coord_indices),
        normal_indicies(normal_indicies)
    {}
    tup<int, 3> vertex_indicies, vertex_tex_coord_indices, normal_indicies;
};

class mesh {
public:
    mesh() {}

    mesh(vector<vec3> vertexes, vector<face> faces);
    mesh(const mesh& rhs) : vertexes(rhs.vertexes), faces(rhs.faces) {}

    vector<polygon> get_polygons(vector<vec3> vertexes);

    static mesh from_obj(string file_path);

    vector<vec3> vertexes;
    vector<vec3> texture_vertexes;
    vector<vec3> vertex_normals;
    vector<face> faces;
    vector<material> materials;

private:
    static vector<tup<int, 3>> parse_face(vector<string> tokens);

    static vector<material> get_material(string file_path);
};

inline string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

inline std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                 std::istream_iterator<std::string>());
    return ret;
}

template<typename T>
void print_vec(vector<T> in);

inline size_t sub_str_ind(const std::string& haystack, const std::string& needle, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while( cnt != nth )
    {
        pos+=1;
        pos = haystack.find(needle, pos);
        if ( pos == std::string::npos )
            return -1;
        cnt++;
    }
    return pos;
}