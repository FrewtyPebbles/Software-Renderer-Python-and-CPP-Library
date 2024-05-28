#pragma once
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

template<typename T, size_t N>
class tup;
class vec3;
class polygon;

class mesh {
public:
    mesh() {}

    mesh(vector<vec3> vertexes, vector<tup<int, 3>> polygon_inds);

    vector<polygon> get_polygons(vector<vec3> vertexes);

    static mesh from_obj(string file_path);

    vector<vec3> vertexes;
    vector<tup<int, 3>> polygon_inds;
    vector<polygon> polygons;

private:
    static vector<int> parse_face(vector<string> tokens);
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